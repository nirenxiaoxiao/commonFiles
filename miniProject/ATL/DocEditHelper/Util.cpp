#include "stdafx.h"
#include "Util.h"

#include <stdio.h>
#include <stdarg.h>


int DebugLog(const char* format, ...)
{
    static const int logBufSize = 1024;
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(logBufSize);

    va_list argList;
    va_start(argList, format);
    int count = vsprintf_s(buf.get(), logBufSize, format, argList);
    va_end(argList);

    std::unique_ptr<char[]> logBuf = std::make_unique<char[]>(logBufSize);

    sprintf_s(logBuf.get(), logBufSize, "[DocEditHelper][%d]: %s\n", GetCurrentThreadId(), buf.get());

    fprintf_s(stdout, logBuf.get());

#ifdef _DEBUG
    OutputDebugStringA(logBuf.get());
#endif // _DEBUG

    return count;
}

std::wstring ANSIToUTF16(const std::string& in)
{
    int nLengthResult = MultiByteToWideChar(CP_ACP, 0, in.c_str(), in.length() + 1, NULL, 0);
    std::unique_ptr<wchar_t[]> stringBuf(new wchar_t[nLengthResult]());
    MultiByteToWideChar(CP_ACP, 0, in.c_str(), in.length(), stringBuf.get(), nLengthResult);
    return std::wstring(stringBuf.get());
}

std::string UTF16ToANSI(const std::wstring& in)
{
    int nLengthResult = WideCharToMultiByte(CP_ACP, 0, in.c_str(), in.length() + 1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> stringBuf(new char[nLengthResult]());
    WideCharToMultiByte(CP_ACP, 0, in.c_str(), in.length(), stringBuf.get(), nLengthResult, NULL, NULL);
    return std::string(stringBuf.get());
}

std::wstring UTF8ToUTF16(const std::string& in)
{
    int nLengthResult = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.length() + 1, NULL, 0);
    std::unique_ptr<wchar_t[]> stringBuf(new wchar_t[nLengthResult]());
    MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.length(), stringBuf.get(), nLengthResult);
    return std::wstring(stringBuf.get());
}

std::string UTF16ToUTF8(const std::wstring& in)
{
    int nLengthResult = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.length() + 1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> stringBuf(new char[nLengthResult]());
    WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.length(), stringBuf.get(), nLengthResult, NULL, NULL);
    return std::string(stringBuf.get());
}

std::unique_ptr<wchar_t[]> StringToBuffer(const std::wstring& in)
{
    auto buf = std::make_unique<wchar_t[]>(in.length() + 1);
    wcscpy_s(buf.get(), in.length() + 1, in.c_str());
    return std::move(buf);
}

std::wstring CreateGUID()
{
    GUID guid;
    CoCreateGuid(&guid);

    std::unique_ptr<wchar_t[]> strGuid = std::make_unique<wchar_t[]>(128);
    swprintf_s(strGuid.get(), 128, _T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"),
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1],
        guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);

    return strGuid.get();
}
