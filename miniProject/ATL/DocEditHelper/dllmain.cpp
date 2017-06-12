// dllmain.cpp: DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "DocEditHelper_i.h"
#include "dllmain.h"
#include "xdlldata.h"


#include <curl/curl.h>

class ScopedCurlGlobalContext
{
public:
    ScopedCurlGlobalContext()
    {
        CURLcode error = curl_global_init(CURL_GLOBAL_ALL);
        ATLASSERT(!error);
    }
    ~ScopedCurlGlobalContext()
    {
        curl_global_cleanup();
    }

private:

};

ScopedCurlGlobalContext g_curlContext;


CDocEditHelperModule _AtlModule;

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
