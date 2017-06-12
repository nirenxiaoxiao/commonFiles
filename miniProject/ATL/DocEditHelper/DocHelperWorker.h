/*
 * ActiveX控件的后台工作线程
*/
#pragma once

#include <typeinfo>
#include <thread>
#include <mutex>
#include <string>
#include <vector>

#include "DocHelperWorkerDef.h"

using namespace std;

class CDocumentEditHelper;

struct LocalDocumentInfo;
class DocumentMonitorList;

class DocHelperWorker
{
public:
    DocHelperWorker(CDocumentEditHelper* pOwnerControl);
    ~DocHelperWorker();

    std::wstring LoadDocument(const wchar_t* documentURL, const wchar_t* postURL);

    bool IsDocumentAlreadyLoaded(const wchar_t* documentURL);

private:
    // 工作线程循环
    void RunWork();
    // 监视临时目录的更改的线程
    void DoMonitorDirChange(const wchar_t* dirPath);
    // 检查打开文档的程序是否正在运行
    void CheckProcessRunning();

    void HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void NotifyOwner(const WorkerNotifyHeader* pNotify);

private:
    // 载入一个文档：把文件下载下来，然后监视该文件的变更以及打开该文件的程序的状态
    bool DoLoadDocument(AddDocFileInfo* pInfo);
    // 从URL下载文档
    bool DownloadDocument(AddDocFileInfo* pInfo, const wchar_t* saveFilePath);
    // 控件被删除时，尝试关掉所有打开文档的进程，然后删除所有正在监视的本地文档
    void CloseAllLocalDocuments();

    // 创建工作目录
    bool CreateWorkDirectory();

    bool PostMessageToThread(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    CDocumentEditHelper* m_pOwnerControl;
    std::wstring m_workDir;// 工作目录

    std::unique_ptr<DocumentMonitorList> m_monitoringDocuments;// 正在监视的文档列表

private:
    std::thread m_workerThread;
    std::thread m_monitorDirChangeThread;
    HANDLE m_hThreadMsgEvent;
    HANDLE m_hWorkerThreadReadyEvent; // 
    bool m_bRunning;

private:
    DocHelperWorker(const DocHelperWorker&);
    DocHelperWorker& operator=(const DocHelperWorker&);
};

