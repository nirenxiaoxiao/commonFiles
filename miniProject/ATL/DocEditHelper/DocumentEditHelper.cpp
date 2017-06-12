// DocumentEditHelper.cpp : CDocumentEditHelper µÄÊµÏÖ
#include "stdafx.h"
#include "DocumentEditHelper.h"

#include "DocHelperWorker.h"

// CDocumentEditHelper

CDocumentEditHelper::CDocumentEditHelper()
    : m_pWorker(std::make_unique<DocHelperWorker>(this))
    , m_bDisableLoadDuplicateDoc(true)
{
    OutputDebugStringA("CDocumentEditHelper::CDocumentEditHelper()\n");
    m_bWindowOnly = true;
}

CDocumentEditHelper::~CDocumentEditHelper()
{
    OutputDebugStringA("CDocumentEditHelper::~CDocumentEditHelper()\n");
}

STDMETHODIMP CDocumentEditHelper::LoadDocument(BSTR documentURL, BSTR postURL, BSTR* docIdentifier)
{
    std::wstring url = OLE2W(documentURL);
	std::wstring postUrl = OLE2W(postURL);

    DebugLog("CDocumentEditHelper::LoadDocument - documentURL0 = \"%s\"", UTF16ToUTF8(url).c_str());

    std::wstring id = m_pWorker->LoadDocument(url.c_str(),postUrl.c_str());

	DebugLog("CDocumentEditHelper::LoadDocument - documentURL1 = \"%s\"", UTF16ToUTF8(url).c_str());
    *docIdentifier = ::SysAllocString(id.c_str());
	DebugLog("CDocumentEditHelper::LoadDocument - documentURL2 = \"%s\"", UTF16ToUTF8(url).c_str());
    return S_OK;
}

STDMETHODIMP CDocumentEditHelper::get_DisableLoadDuplicateURL(VARIANT_BOOL* pVal)
{
    *pVal = m_bDisableLoadDuplicateDoc ? VARIANT_BOOL(0xFFFF) : 0;
    return S_OK;
}


STDMETHODIMP CDocumentEditHelper::put_DisableLoadDuplicateURL(VARIANT_BOOL newVal)
{
    m_bDisableLoadDuplicateDoc = (newVal == VARIANT_BOOL(0xFFFF) ? true : false);

    DebugLog("CDocumentEditHelper.DisableLoadDuplicateURL = %s", m_bDisableLoadDuplicateDoc ? "true" : "false");

    return S_OK;
}


STDMETHODIMP CDocumentEditHelper::IsDocumentLoaded(BSTR documentURL, VARIANT_BOOL* bLoaded)
{
    std::wstring url = OLE2W(documentURL);
    DebugLog("CDocumentEditHelper::IsDocumentLoaded - documentURL = \"%s\"", UTF16ToUTF8(url).c_str());

    *bLoaded = m_pWorker->IsDocumentAlreadyLoaded(url.c_str()) ? VARIANT_BOOL(0xFFFF) : 0;
    return S_OK;
}

LRESULT CDocumentEditHelper::OnWorkerNotify(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;

    WorkerNotifyCode code = WorkerNotifyCode(wParam);
    WorkerNotifyHeader* pHeader = (WorkerNotifyHeader*)(lParam);

    switch (code)
    {
    case WorkerNotifyCode::DocumentOpened:
    {
        DocumentEventInfo* pInfo = (DocumentEventInfo*)pHeader;
        auto urlBuf = StringToBuffer(pInfo->documentURL);
        auto idBuf = StringToBuffer(pInfo->docIdentifier);
        auto errMsgBuf = StringToBuffer(pInfo->errorMsg);
        Fire_OnDocumentOpen(W2OLE(urlBuf.get()), W2OLE(idBuf.get()), pInfo->retCode, W2OLE(errMsgBuf.get()));
    }
    break;
    case WorkerNotifyCode::DocumentClosed:
    {
        DocumentEventInfo* pInfo = (DocumentEventInfo*)pHeader;
        auto urlBuf = StringToBuffer(pInfo->documentURL);
        auto idBuf = StringToBuffer(pInfo->docIdentifier);
        Fire_OnDocumentClosed(W2OLE(urlBuf.get()), W2OLE(idBuf.get()));
    }
    break;
    case WorkerNotifyCode::DocumentModified:
    {
        DocumentEventInfo* pInfo = (DocumentEventInfo*)pHeader;
        auto urlBuf = StringToBuffer(pInfo->documentURL);
        auto idBuf = StringToBuffer(pInfo->docIdentifier);
        Fire_OnDocumentModified(W2OLE(urlBuf.get()), W2OLE(idBuf.get()));
    }
    break;
    case WorkerNotifyCode::DocDownloadProgress:
    {
        DownloadProgressInfo* pInfo = (DownloadProgressInfo*)pHeader;
        auto urlBuf = StringToBuffer(pInfo->documentURL);
        auto idBuf = StringToBuffer(pInfo->docIdentifier);
        VARIANT_BOOL bContinue = pInfo->bContinue ? 0xFFFF : 0;
        Fire_OnDownloadProgress(W2OLE(urlBuf.get()), W2OLE(idBuf.get()), pInfo->downloadBytes, pInfo->totalBytes);
        pInfo->bContinue = (bContinue == VARIANT_BOOL(0xFFFF) ? true : false);
    }
    break;
    case WorkerNotifyCode::DocBeginDownload:
    {
        DocumentEventInfo* pInfo = (DocumentEventInfo*)pHeader;
        auto urlBuf = StringToBuffer(pInfo->documentURL);
        auto idBuf = StringToBuffer(pInfo->docIdentifier);
        Fire_OnDownloadBegin(W2OLE(urlBuf.get()), W2OLE(idBuf.get()));
    }
    break;
    case WorkerNotifyCode::DocDownloadResult:
    {
        DocumentEventInfo* pInfo = (DocumentEventInfo*)pHeader;
        auto urlBuf = StringToBuffer(pInfo->documentURL);
        auto errorMsgBuf = StringToBuffer(pInfo->errorMsg);
        auto idBuf = StringToBuffer(pInfo->docIdentifier);
        Fire_OnDownloadResult(W2OLE(urlBuf.get()), W2OLE(idBuf.get()), pInfo->retCode,  W2OLE(errorMsgBuf.get()));
    }
    break;
    default:
        break;
    }

    return 0;
}


