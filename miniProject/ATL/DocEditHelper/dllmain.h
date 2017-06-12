// dllmain.h: 模块类的声明。

class CDocEditHelperModule : public ATL::CAtlDllModuleT< CDocEditHelperModule >
{
public :
	DECLARE_LIBID(LIBID_DocEditHelperLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DOCEDITHELPER, "{CE5D400A-C312-4EFA-9F01-D06225451770}")
};

extern class CDocEditHelperModule _AtlModule;
