/*
 * ActiveX�ؼ��ĺ�̨�����߳�
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
    // �����߳�ѭ��
    void RunWork();
    // ������ʱĿ¼�ĸ��ĵ��߳�
    void DoMonitorDirChange(const wchar_t* dirPath);
    // �����ĵ��ĳ����Ƿ���������
    void CheckProcessRunning();

    void HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void NotifyOwner(const WorkerNotifyHeader* pNotify);

private:
    // ����һ���ĵ������ļ�����������Ȼ����Ӹ��ļ��ı���Լ��򿪸��ļ��ĳ����״̬
    bool DoLoadDocument(AddDocFileInfo* pInfo);
    // ��URL�����ĵ�
    bool DownloadDocument(AddDocFileInfo* pInfo, const wchar_t* saveFilePath);
    // �ؼ���ɾ��ʱ�����Թص����д��ĵ��Ľ��̣�Ȼ��ɾ���������ڼ��ӵı����ĵ�
    void CloseAllLocalDocuments();

    // ��������Ŀ¼
    bool CreateWorkDirectory();

    bool PostMessageToThread(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    CDocumentEditHelper* m_pOwnerControl;
    std::wstring m_workDir;// ����Ŀ¼

    std::unique_ptr<DocumentMonitorList> m_monitoringDocuments;// ���ڼ��ӵ��ĵ��б�

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

