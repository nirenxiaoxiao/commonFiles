// DocumentEditHelper.h : CDocumentEditHelper ������
#pragma once
#include "resource.h"       // ������
#include <atlctl.h>
#include "DocEditHelper_i.h"
#include "_IDocumentEditHelperEvents_CP.H"

#include "DocHelperWorkerDef.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;

#define WM_HELPERMSG WM_APP+1

class DocHelperWorker;

// CDocumentEditHelper
class ATL_NO_VTABLE CDocumentEditHelper :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IDocumentEditHelper, &IID_IDocumentEditHelper, &LIBID_DocEditHelperLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IOleControlImpl<CDocumentEditHelper>,
    public IOleObjectImpl<CDocumentEditHelper>,
    public IOleInPlaceActiveObjectImpl<CDocumentEditHelper>,
    public IViewObjectExImpl<CDocumentEditHelper>,
    public IOleInPlaceObjectWindowlessImpl<CDocumentEditHelper>,
    public ISupportErrorInfo,
    public IConnectionPointContainerImpl<CDocumentEditHelper>,
    public IObjectWithSiteImpl<CDocumentEditHelper>,
    public IQuickActivateImpl<CDocumentEditHelper>,
#ifndef _WIN32_WCE
    public IDataObjectImpl<CDocumentEditHelper>,
#endif
    public IProvideClassInfo2Impl<&CLSID_DocumentEditHelper, &__uuidof(_IDocumentEditHelperEvents), &LIBID_DocEditHelperLib>,
    public IObjectSafetyImpl<CDocumentEditHelper, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
    public CComCoClass<CDocumentEditHelper, &CLSID_DocumentEditHelper>,
    public CComControl<CDocumentEditHelper>,
    public CProxy_IDocumentEditHelperEvents<CDocumentEditHelper>
{
public:


    CDocumentEditHelper();
    ~CDocumentEditHelper();

    DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
    OLEMISC_ACTSLIKELABEL |
        /*OLEMISC_INVISIBLEATRUNTIME |*/
        OLEMISC_CANTLINKINSIDE |
        OLEMISC_INSIDEOUT |
        OLEMISC_ACTIVATEWHENVISIBLE |
        OLEMISC_SETCLIENTSITEFIRST
        )

        DECLARE_REGISTRY_RESOURCEID(IDR_DOCUMENTEDITHELPER)


    BEGIN_COM_MAP(CDocumentEditHelper)
        COM_INTERFACE_ENTRY(IDocumentEditHelper)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IViewObjectEx)
        COM_INTERFACE_ENTRY(IViewObject2)
        COM_INTERFACE_ENTRY(IViewObject)
        COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
        COM_INTERFACE_ENTRY(IOleInPlaceObject)
        COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
        COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
        COM_INTERFACE_ENTRY(IOleControl)
        COM_INTERFACE_ENTRY(IOleObject)
        COM_INTERFACE_ENTRY(ISupportErrorInfo)
        COM_INTERFACE_ENTRY(IConnectionPointContainer)
        COM_INTERFACE_ENTRY(IQuickActivate)
#ifndef _WIN32_WCE
        COM_INTERFACE_ENTRY(IDataObject)
#endif
        COM_INTERFACE_ENTRY(IProvideClassInfo)
        COM_INTERFACE_ENTRY(IProvideClassInfo2)
        COM_INTERFACE_ENTRY(IObjectWithSite)
        COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
    END_COM_MAP()

    BEGIN_PROP_MAP(CDocumentEditHelper)
        PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
        PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
        // ʾ����
        // PROP_ENTRY_TYPE("������", dispid, clsid, vtType)
        // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()

    BEGIN_CONNECTION_POINT_MAP(CDocumentEditHelper)
        CONNECTION_POINT_ENTRY(__uuidof(_IDocumentEditHelperEvents))
    END_CONNECTION_POINT_MAP()

    BEGIN_MSG_MAP(CDocumentEditHelper)
        CHAIN_MSG_MAP(CComControl<CDocumentEditHelper>)
        MESSAGE_HANDLER(MSG_DOCHELPERWORKNOTIFY, CDocumentEditHelper::OnWorkerNotify)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    // �������ԭ��: 
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    // ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        static const IID* const arr[] =
        {
            &IID_IDocumentEditHelper,
        };

        for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        {
            if (InlineIsEqualGUID(*arr[i], riid))
                return S_OK;
        }
        return S_FALSE;
    }

    // IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

    // IDocumentEditHelper
public:
    HRESULT OnDraw(ATL_DRAWINFO& di)
    {

        return S_OK;
    }


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

    bool IsLoadDuplicateURLDisabled() const
    {
        return m_bDisableLoadDuplicateDoc;
    }

    STDMETHOD(LoadDocument)(BSTR documentURL, BSTR postURL, BSTR* docIdentifier);
    STDMETHOD(get_DisableLoadDuplicateURL)(VARIANT_BOOL* pVal);
    STDMETHOD(put_DisableLoadDuplicateURL)(VARIANT_BOOL newVal);
    // _IDocumentEditHelperEvents Methods
public:

private:
    LRESULT OnWorkerNotify(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    std::unique_ptr<DocHelperWorker> m_pWorker;
    bool m_bDisableLoadDuplicateDoc;
public:
    STDMETHOD(IsDocumentLoaded)(BSTR documentURL, VARIANT_BOOL* bLoaded);
};

OBJECT_ENTRY_AUTO(__uuidof(DocumentEditHelper), CDocumentEditHelper)
