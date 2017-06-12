#pragma once

#include <winuser.h>

enum DocHelperMsgID
{
    MSG_DOCHELPER = WM_APP + 120, // �����̷߳���Ϣ(wParam = WorkerOperationCode / lParam = [new]WorkerOperationInfoBase*)
    MSG_DOCHELPERWORKNOTIFY, // �����̴߳����ؼ������֪ͨ��ͨ��SendMessage����(wParam = WorkerNotifyCode / lParam = WorkerNotifyHeader*)
    MSG_FILECHANGED,// ����Ŀ¼�仯���߳�֪ͨ�����̣߳����ļ��Ѿ��ı�(wParam = [new]std::wstring* / lParam = 0)
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
    HANDLE hCompleteEvent; // �ȴ�������ɵ�event������Ϣ�ķ��ͷ����𴴽��͹ر�
    std::wstring* pMsgResult;// ��ѯ���
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