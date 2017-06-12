#pragma once

#include <winuser.h>

enum DocHelperMsgID
{
    MSG_DOCHELPER = WM_APP + 120, // 向工作线程发消息(wParam = WorkerOperationCode / lParam = [new]WorkerOperationInfoBase*)
    MSG_DOCHELPERWORKNOTIFY, // 工作线程传给控件本身的通知，通过SendMessage发送(wParam = WorkerNotifyCode / lParam = WorkerNotifyHeader*)
    MSG_FILECHANGED,// 监视目录变化的线程通知工作线程，有文件已经改变(wParam = [new]std::wstring* / lParam = 0)
};

enum class WorkerOperationCode
{
    AddDocFile,
    QueryDocStatus,
    
};

struct WorkerOperationInfoBase
{
    WorkerOperationInfoBase(WorkerOperationCode opCode)
        : m_code(opCode)
    {

    }
    virtual ~WorkerOperationInfoBase()
    {
    }
    WorkerOperationCode GetOpCode() const { return m_code; }
private:
    WorkerOperationCode m_code;

};

struct AddDocFileInfo : WorkerOperationInfoBase
{
    AddDocFileInfo()
        : WorkerOperationInfoBase(WorkerOperationCode::AddDocFile)
    {
    }
    std::wstring documentURL;
	std::wstring postURL;
    std::wstring docIdentifier;
};

struct QueryDocumentStatusInfo : WorkerOperationInfoBase
{
    QueryDocumentStatusInfo()
        : WorkerOperationInfoBase(WorkerOperationCode::QueryDocStatus)
        , hCompleteEvent(INVALID_HANDLE_VALUE)
        , pMsgResult(NULL)
    {

    }
    std::wstring documentURL;
    HANDLE hCompleteEvent; // 等待操作完成的event，由消息的发送方负责创建和关闭
    std::wstring* pMsgResult;// 查询结果
};

enum class WorkerNotifyCode
{
    DocumentOpened, 
    DocumentClosed, 
    DocumentModified,
    DocDownloadProgress,
    DocBeginDownload,
    DocDownloadResult,

};

struct WorkerNotifyHeader
{
    WorkerNotifyCode notifyCode;
};

struct DownloadProgressInfo : WorkerNotifyHeader
{
    DownloadProgressInfo()
    {
        notifyCode = WorkerNotifyCode::DocDownloadProgress;
        bContinue = true;
    }
    std::wstring documentURL;
    std::wstring docIdentifier;
    size_t downloadBytes;
    size_t totalBytes;
    bool bContinue;
};

struct DocumentEventInfo : WorkerNotifyHeader
{
    DocumentEventInfo()
        : retCode(0)
    {
    }
    std::wstring documentURL;
    std::wstring docIdentifier;
    int retCode;
    std::wstring errorMsg;
};