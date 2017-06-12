#include "stdafx.h"
#include "DocHelperWorker.h"
#include "DocumentEditHelper.h"

#include "FileDownloader.h"

#include <curl/curl.h>
#include <algorithm>


struct LocalDocumentInfo
{
    LocalDocumentInfo()
        : hProcess(INVALID_HANDLE_VALUE)
    {
    }
    ~LocalDocumentInfo()
    {
        if (hProcess != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hProcess);
        }
    }

    std::wstring documentId;
    std::wstring documentURL;
	std::wstring postURL;
    std::wstring documentLocalPath;

    HANDLE hProcess; // 打开这个文档的程序
};

class DocumentMonitorList
{
public:
    typedef std::function<void(std::unique_ptr<LocalDocumentInfo>&)> IterateFunc;

    DocumentMonitorList()
    {
    }

    void SetInitTid(std::thread::id id)
    {
        assert(!m_pInitTID);
        m_pInitTID = std::make_unique<std::thread::id>(id);
    }

    bool IsDocumentLoaded(const wchar_t* documentURL)
    {
        assert(m_pInitTID && std::this_thread::get_id() == *m_pInitTID);
        auto iter = std::find_if(m_monitoringDocuments.cbegin(), m_monitoringDocuments.cend(),
            [documentURL](const std::pair<const std::wstring, std::unique_ptr<LocalDocumentInfo>>& data)
        {
            return data.second->documentURL == documentURL;
        });
        return iter != m_monitoringDocuments.cend();
    }
    void AddDocumentToMonitorList(std::unique_ptr<LocalDocumentInfo>&& newMonitorInfo)
    {
        assert(m_pInitTID && std::this_thread::get_id() == *m_pInitTID);
        m_monitoringDocuments[newMonitorInfo->documentId] = std::move(newMonitorInfo);
    }

    void RemoveDocumentFromList(const wchar_t* identifier)
    {
        assert(m_pInitTID && std::this_thread::get_id() == *m_pInitTID);
        m_monitoringDocuments.erase(identifier);
    }

    void IterateAll(IterateFunc func)
    {
        assert(m_pInitTID && std::this_thread::get_id() == *m_pInitTID);
        for (auto iter = m_monitoringDocuments.begin(); iter != m_monitoringDocuments.end(); iter++)
        {
            func(iter->second);
        }
    }

    const LocalDocumentInfo& GetDocumentInfo(const wchar_t* identifier) throw (std::out_of_range)
    {
        assert(m_pInitTID && std::this_thread::get_id() == *m_pInitTID);
        return *m_monitoringDocuments.at(identifier);
    }

private:
    std::map<std::wstring, std::unique_ptr<LocalDocumentInfo>> m_monitoringDocuments;
    std::unique_ptr<std::thread::id> m_pInitTID;


private:
    DocumentMonitorList(const DocumentMonitorList&);
    DocumentMonitorList& operator=(const DocumentMonitorList&);
};

DocHelperWorker::DocHelperWorker(CDocumentEditHelper* pOwnerControl)
    : m_pOwnerControl(pOwnerControl)
    , m_bRunning(true)
    , m_hThreadMsgEvent(INVALID_HANDLE_VALUE)
    , m_monitoringDocuments(std::make_unique<DocumentMonitorList>())
{
    ATLASSERT(m_pOwnerControl);
    m_hThreadMsgEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    ATLASSERT(m_hThreadMsgEvent);
    m_hWorkerThreadReadyEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    ATLASSERT(m_hWorkerThreadReadyEvent);
    CreateWorkDirectory();
    m_workerThread = std::thread(std::bind(&DocHelperWorker::RunWork, this));
    m_monitorDirChangeThread = std::thread(std::bind(&DocHelperWorker::DoMonitorDirChange, this, m_workDir.c_str())); //TOSEARCH
}


DocHelperWorker::~DocHelperWorker()
{
    m_bRunning = false;
    SetEvent(m_hThreadMsgEvent);
    m_monitorDirChangeThread.join();
    m_workerThread.join();
    CloseHandle(m_hWorkerThreadReadyEvent);
    CloseHandle(m_hThreadMsgEvent);
}

std::wstring DocHelperWorker::LoadDocument(const wchar_t* documentURL, const wchar_t* postURL)
{
    std::wstring id = CreateGUID();
    AddDocFileInfo* pInfo = new AddDocFileInfo();
    pInfo->documentURL = documentURL;
    pInfo->docIdentifier = id;
	pInfo->postURL = postURL;
    int tid = GetThreadId(m_workerThread.native_handle());
    PostMessageToThread(MSG_DOCHELPER, (WPARAM)pInfo->GetOpCode(), (LPARAM)pInfo);
    return id;
}

bool DocHelperWorker::IsDocumentAlreadyLoaded(const wchar_t* documentURL)
{
    HANDLE hCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    std::wstring result;
    
    DebugLog("====================================Posting query request to worker thread. documentURL = \"%s\"", UTF16ToUTF8(documentURL).c_str());

    QueryDocumentStatusInfo *pInfo = new QueryDocumentStatusInfo();
    pInfo->documentURL = documentURL;
    pInfo->hCompleteEvent = hCompleteEvent;
    pInfo->pMsgResult = &result;

    PostMessageToThread(MSG_DOCHELPER, (WPARAM)pInfo->GetOpCode(), (LPARAM)pInfo);

    // 等待工作线程返回结果
    WaitForSingleObject(hCompleteEvent, INFINITE);

    CloseHandle(hCompleteEvent);

    return result == L"Opening";
}

void DocHelperWorker::RunWork()
{
    m_monitoringDocuments->SetInitTid(std::this_thread::get_id());

    auto InitMessageQueue = [this]()
    {
        MSG msg;
        PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
        SetEvent(m_hWorkerThreadReadyEvent);
    };
    InitMessageQueue();

    while (m_bRunning)
    {
        // 检查打开文档的程序是否仍然在运行
        CheckProcessRunning();
        // 接收消息
        MSG msg;
        switch (MsgWaitForMultipleObjectsEx(1, &m_hThreadMsgEvent, 1000, QS_ALLINPUT, 0))
        {
        case WAIT_OBJECT_0 + 1:
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                HandleMessage(msg.message, msg.wParam, msg.lParam);
            }
            break;
        default:
            break;
        }
    }

    CloseAllLocalDocuments();

    DebugLog("ActiveX worker thread quit.");
}

void DocHelperWorker::DoMonitorDirChange(const wchar_t* dirPath)
{
    std::wstring dir(dirPath);

    DebugLog("Monitoring directory %s", UTF16ToANSI(dirPath).c_str());

    struct DirectoryInfo {
        HANDLE hDir;
        std::wstring dirName;
        std::unique_ptr<BYTE[]> dirChangeBuffer;
        size_t bufLength;
        OVERLAPPED pollingOverlap;
    };
    DirectoryInfo dirInfo;

    DWORD nRet;
    BOOL result = TRUE;
    dirInfo.hDir = CreateFileW(dir.c_str(), GENERIC_READ | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL);

    if (dirInfo.hDir == INVALID_HANDLE_VALUE)
    {
        DebugLog("Cannot open directory %s", UTF16ToANSI(dirPath).c_str());
        return; //cannot open folder
    }

    dirInfo.dirName = dir;

    dirInfo.bufLength = 4096;
    dirInfo.dirChangeBuffer = std::make_unique<BYTE[]>(dirInfo.bufLength);

    dirInfo.pollingOverlap.OffsetHigh = 0;
    dirInfo.pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    while (result && m_bRunning)
    {
        result = ReadDirectoryChangesW(
            dirInfo.hDir,// handle to the directory to be watched
            dirInfo.dirChangeBuffer.get(),// pointer to the buffer to receive the read results
            dirInfo.bufLength,// length of lpBuffer
            TRUE,// flag for monitoring directory or directory tree
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_SIZE,
            //FILE_NOTIFY_CHANGE_LAST_WRITE |
            //FILE_NOTIFY_CHANGE_LAST_ACCESS |
            //FILE_NOTIFY_CHANGE_CREATION,
            &nRet,// number of bytes returned
            &dirInfo.pollingOverlap,// pointer to structure needed for overlapped I/O
            NULL);

        bool bStop = false;
        while (WaitForSingleObject(dirInfo.pollingOverlap.hEvent, 200) != WAIT_OBJECT_0)
        {
            if (!m_bRunning)
            {
                bStop = true;
                break;
            }
        }
        if (bStop) break;

        FILE_NOTIFY_INFORMATION* pNotify;
        int offset = 0;

        auto NotifyControlFileModified = [this](const std::wstring& filePath)
        {
            std::wstring* fullFilePath = new std::wstring(filePath);
            PostMessageToThread(MSG_FILECHANGED, (WPARAM)fullFilePath, 0);
        };

        do
        {
            pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(dirInfo.dirChangeBuffer.get() + offset);

            std::wstring wFilename(pNotify->FileName, pNotify->FileNameLength);
            std::string filename(UTF16ToANSI(wFilename));

            switch (pNotify->Action)
            {
            case FILE_ACTION_MODIFIED:
            {
                DebugLog("The file is modified: [%s]", filename.c_str());
                // 通知工作线程，有文件已经更改
                NotifyControlFileModified(dir + L"\\" + wFilename);
            }
                break;
            case FILE_ACTION_ADDED:
                DebugLog("The file is added to the directory: [%s]", filename.c_str());
                break;
            case FILE_ACTION_REMOVED:
                DebugLog("The file is removed from the directory: [%s]", filename.c_str());
                break;
            case FILE_ACTION_RENAMED_OLD_NAME:
                DebugLog("The file was renamed and this is the old name: [%s]", filename.c_str());
                break;
            case FILE_ACTION_RENAMED_NEW_NAME:
                DebugLog("The file was renamed and this is the new name: [%s]", filename.c_str());
                NotifyControlFileModified(dir + L"\\" + wFilename);
                break;
            default:
                DebugLog("Default error.");
                break;
            }

            offset += pNotify->NextEntryOffset;

        } while (pNotify->NextEntryOffset); //(offset != 0);
    }

    CloseHandle(dirInfo.pollingOverlap.hEvent);
    CloseHandle(dirInfo.hDir);


    DebugLog("Directory Change monitor thread quit.");
}

void DocHelperWorker::CheckProcessRunning()
{
    std::vector<std::wstring> closedDocuments;

    m_monitoringDocuments->IterateAll(
        [&](std::unique_ptr<LocalDocumentInfo>& docInfo)
    {
        DWORD exitCode = 0;
        GetExitCodeProcess(docInfo->hProcess, &exitCode);
        if (exitCode != STILL_ACTIVE)
        {
            closedDocuments.push_back(docInfo->documentId);
        }
    });

    // 发送文档已经关闭的事件，尝试删除该文档文件，从监视列表中移除该文档
    for (size_t i = 0; i < closedDocuments.size(); i++)
    {
        const std::wstring& id = closedDocuments[i];

        const LocalDocumentInfo& docInfo = m_monitoringDocuments->GetDocumentInfo(id.c_str());

        DebugLog("Document closed: \"%s\" delete it from local storage.", UTF16ToANSI(docInfo.documentLocalPath).c_str());

        DocumentEventInfo notifyInfo;
        notifyInfo.notifyCode = WorkerNotifyCode::DocumentClosed;
        notifyInfo.docIdentifier = id;
        notifyInfo.documentURL = docInfo.documentURL;
        NotifyOwner(&notifyInfo);

        int ret = _wremove(docInfo.documentLocalPath.c_str());
        int error = errno;
        if (ret != 0)
        {
            DebugLog("Deletion failed, errno=%d: \"%s\" ", error, UTF16ToANSI(docInfo.documentLocalPath).c_str());
        }

        m_monitoringDocuments->RemoveDocumentFromList(id.c_str());
    }
}

void DocHelperWorker::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    DebugLog("msgId:%d wp:0x%08x lp:0x%08x", msg, wParam, lParam);
    if (msg == MSG_DOCHELPER)
    {
        WorkerOperationCode opcode = static_cast<WorkerOperationCode>(wParam);
        std::unique_ptr<WorkerOperationInfoBase> pInfoBase((WorkerOperationInfoBase*)lParam);

        switch (opcode)
        {
        case WorkerOperationCode::AddDocFile:
        {
            auto pInfo = static_cast<AddDocFileInfo*>(pInfoBase.get());
			DebugLog("======================================1\"%s\" ", UTF16ToUTF8(pInfo->documentURL).c_str());
            DoLoadDocument(pInfo);
			DebugLog("======================================\"%s\" ", UTF16ToUTF8(pInfo->documentURL).c_str());
        }
        break;
        case WorkerOperationCode::QueryDocStatus:
        {
            auto pInfo = static_cast<QueryDocumentStatusInfo*>(pInfoBase.get());
            bool bLoaded = m_monitoringDocuments->IsDocumentLoaded(pInfo->documentURL.c_str());

            *(pInfo->pMsgResult) = bLoaded ? L"Opening" : L"Closed";

            DebugLog("DocumentURL=\"%s\" status=%s", UTF16ToANSI(pInfo->documentURL).c_str(), (bLoaded ? "Opening" : "Closed"));

            SetEvent(pInfo->hCompleteEvent);
        }
        break;
        default:
            break;
        }
    }
    if (msg == MSG_FILECHANGED)
    {
        std::unique_ptr<std::wstring> dir((std::wstring*)wParam);
        DebugLog("Directory Change Notification Triggered");

        LocalDocumentInfo* pInfo = nullptr;

        m_monitoringDocuments->IterateAll(
            [&dir, &pInfo](std::unique_ptr<LocalDocumentInfo>& info)
        {
            if (wcscmp(info->documentLocalPath.c_str(), dir->c_str()) == 0)
            {
                pInfo = info.get();
            }
        });

        if (pInfo)
        {
          map<string,string> params;
		  params["aaaa"] = "aaaa";
		  FileDownloader::getInstance()->PostHttpRequest(UTF16ToANSI(pInfo->postURL).c_str(), UTF16ToANSI(pInfo->documentLocalPath).c_str(), params, [](string &response){DebugLog("%s", response.c_str()); });
          
            DocumentEventInfo info;
            info.notifyCode = WorkerNotifyCode::DocumentModified;
            info.documentURL = pInfo->documentURL;
            info.docIdentifier = pInfo->documentId;
            NotifyOwner(&info);
        }

    }
}

void DocHelperWorker::NotifyOwner(const WorkerNotifyHeader* pNotify)
{
    ATLASSERT(::IsWindow(m_pOwnerControl->m_hWnd));
    SendMessageW(m_pOwnerControl->m_hWnd, MSG_DOCHELPERWORKNOTIFY, (WPARAM)pNotify->notifyCode, (LPARAM)pNotify);
}

bool DocHelperWorker::DoLoadDocument(AddDocFileInfo* pInfo)
{
    if (m_pOwnerControl->IsLoadDuplicateURLDisabled())
    {
        if (m_monitoringDocuments->IsDocumentLoaded(pInfo->documentURL.c_str()))
        {
            DebugLog("Document is already loaded, URL: \"%s\"", UTF16ToANSI(pInfo->documentURL).c_str());

            DocumentEventInfo info;
            info.notifyCode = WorkerNotifyCode::DocumentOpened;
            info.documentURL = pInfo->documentURL;
            info.docIdentifier = L"";
            info.retCode = 5;
            info.errorMsg = L"文档已经被打开。";
            NotifyOwner(&info);

            return false;
        }
    }

    Url url(pInfo->documentURL);
    std::wstring id = pInfo->docIdentifier;

    auto SplitFileName = [](const std::wstring& filename, __out std::wstring& name, __out std::wstring& ext)
    {
        int separator = filename.find_last_of(L'.');
        name = filename.substr(0, separator);
        ext = filename.substr(separator + 1, filename.length());
    };

    auto BuildSaveFilename = [SplitFileName](const std::wstring& filename, const std::wstring& identifier)
    {
        std::wstring name, ext;
        SplitFileName(filename, name, ext);
        std::wstring saveFilename = name + L"_" + identifier + L"." + ext;
        return std::move(saveFilename);
    };

    // TODO: 只允许下载指定的文件类型

    std::wstring saveFilePath = m_workDir + L"\\" + BuildSaveFilename(url.GetFileName(), id);

    DebugLog("Begin download Document, URL=\"%s\"\n id=\"%s\"\n saveFilePath=\"%s\"",
        UTF16ToANSI(pInfo->documentURL).c_str(), UTF16ToANSI(id).c_str(), UTF16ToANSI(saveFilePath).c_str());

    // 下载文档到本地
    if (!DownloadDocument(pInfo, saveFilePath.c_str()))
    {
        return false;
    }

    // 尝试用本地程序打开这个文档，如果打开成功则把文档添加到监视列表

    DebugLog("》》》》》》》》》》》》》》》》Opening Document \"%s\"", UTF16ToANSI(saveFilePath).c_str());

    // 文档监视的信息
    std::unique_ptr<LocalDocumentInfo> newInfo = std::make_unique<LocalDocumentInfo>();
    newInfo->documentId = id;
	newInfo->postURL = pInfo->postURL;
    newInfo->documentURL = pInfo->documentURL;
    newInfo->documentLocalPath = saveFilePath;

    // 尝试打开该文档
    SHELLEXECUTEINFOW executeInfo = { 0 };
    executeInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
    executeInfo.fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS;
    executeInfo.lpFile = saveFilePath.c_str();
    executeInfo.nShow = SW_SHOW;
    
    BOOL ret = ShellExecuteExW(&executeInfo);

    auto GetExecuteErrorMsg = [](int execError)
    {
        if (execError > SE_ERR_NOASSOC)
        {
            return L"操作成功完成";
        }
        switch (execError)
        {
        case SE_ERR_FNF:
            return L"找不到对应的文件。";
        case SE_ERR_ASSOCINCOMPLETE:
            return L"文件关联有问题。";
        case SE_ERR_NOASSOC:
            return L"找不到文件类型关联的程序。";
        default:
            return L"打开文件失败。";
        }
    };

    ULONG errorCode = (int(executeInfo.hInstApp) > SE_ERR_NOASSOC) ? 0 : int(executeInfo.hInstApp);
    std::wstring errorMsg = GetExecuteErrorMsg(int(executeInfo.hInstApp));

    DebugLog("Open Result: %d", errorCode);

    if (ret)
    {
        newInfo->hProcess = executeInfo.hProcess;
        m_monitoringDocuments->AddDocumentToMonitorList(std::move(newInfo));
    }
    else
    {
        // 打开文件失败，删除下载下来的文档
        _wremove(saveFilePath.c_str());
    }

    // 通知打开文档的结果
    DocumentEventInfo info;
    info.notifyCode = WorkerNotifyCode::DocumentOpened;
    info.documentURL = pInfo->documentURL;
    info.docIdentifier = id;
    info.retCode = errorCode;
    info.errorMsg = errorMsg;

    NotifyOwner(&info);

    return true;
}

bool DocHelperWorker::DownloadDocument(AddDocFileInfo* pInfo, const wchar_t* saveFilePath)
{
    FileDownloader downloader(pInfo->documentURL);

    HttpResponse response;

    downloader.SetHttpHeaderCallback(
        [](const HttpResponse& response)
    {
        DebugLog("////////////////////////////////////////////////%s", response.GetRawHeader().c_str());
    });

    downloader.SetDownloadProgressCallback(
        [this, pInfo, &downloader](curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
    {
        // 通知下载进度
        DownloadProgressInfo info;
        info.documentURL = pInfo->documentURL;
        info.docIdentifier = pInfo->docIdentifier;
        info.downloadBytes = dlnow;
        info.totalBytes = dltotal;

        NotifyOwner(&info);

        if (!info.bContinue)
        {
            downloader.StopDownload();
        }
    });
    downloader.SetRecvContentStartCallback(
        [this, pInfo](const HttpResponse& response)
    {
        // 通知下载即将开始
        DocumentEventInfo info;
        info.notifyCode = WorkerNotifyCode::DocBeginDownload;
        info.docIdentifier = pInfo->docIdentifier;
        info.documentURL = pInfo->documentURL;
        NotifyOwner(&info);
    }
    );
    CURLcode result = (CURLcode)downloader.SimpleSaveAsFile(saveFilePath, &response);
    std::string errorMsg = UTF16ToANSI(FileDownloader::FormatErrCode(result));

    if (result != CURLE_OK)
    {
        DebugLog("Download \"%s\" failed. error code = %d msg: %s", UTF16ToANSI(pInfo->documentURL).c_str(), result, errorMsg.c_str());
        // 删掉下载失败的文件
        _wremove(saveFilePath);
    }

    // 通知下载结果
    auto NotifyDownloadResult = [=]()
    {
        DocumentEventInfo info;
        info.notifyCode = WorkerNotifyCode::DocDownloadResult;
        info.documentURL = pInfo->documentURL;
        info.docIdentifier = pInfo->docIdentifier;
        info.retCode = result;
        info.errorMsg = FileDownloader::FormatErrCode(result);
        NotifyOwner(&info);
    };
    NotifyDownloadResult();
    
    return result == CURLE_OK;
}

void DocHelperWorker::CloseAllLocalDocuments()
{
    DebugLog("Control Deleted, Terminating all opening local documents and delete them...");
    m_monitoringDocuments->IterateAll(
        [&](std::unique_ptr<LocalDocumentInfo>& docInfo)
    {
        TerminateProcess(docInfo->hProcess, 0);
        WaitForSingleObject(docInfo->hProcess, 5000);
        _wremove(docInfo->documentLocalPath.c_str());
    });
}

bool DocHelperWorker::CreateWorkDirectory()
{
    static const int maxChar = 1000;
    std::unique_ptr<wchar_t[]> tmpPathBuf(new wchar_t[maxChar]());
    std::unique_ptr<wchar_t[]> longTmpPathBuf(new wchar_t[maxChar]());
    GetEnvironmentVariableW(L"temp", tmpPathBuf.get(), maxChar);
    GetLongPathNameW(tmpPathBuf.get(), longTmpPathBuf.get(), maxChar);
    std::wstring tmpPath = longTmpPathBuf.get();
    tmpPath.append(_T("\\DocEditHelperTemp"));

    DebugLog("Create Work Directory: \"%s\"", UTF16ToANSI(tmpPath).c_str());

    m_workDir = tmpPath;

    BOOL ret = CreateDirectoryW(tmpPath.c_str(), nullptr);

    return ret || (GetLastError() == ERROR_ALREADY_EXISTS);
}

bool DocHelperWorker::PostMessageToThread(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // fixme: 这里需要修改，工作线程处于繁忙状态时（例如下载文件），
    // 通过PostThreadMessage发过来的消息会被丢弃无法处理，导致内存泄露等问题。
    if (WaitForSingleObject(m_hWorkerThreadReadyEvent, 100) != WAIT_OBJECT_0)
    {
        return false;
    }
	DebugLog("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    return PostThreadMessageW(GetThreadId(m_workerThread.native_handle()), msg, wParam, lParam) == TRUE;

}
