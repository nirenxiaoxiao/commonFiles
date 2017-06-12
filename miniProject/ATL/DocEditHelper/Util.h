#pragma once
#include <string>

int DebugLog(const char* format, ...);

std::wstring ANSIToUTF16(const std::string& in);
std::string UTF16ToANSI(const std::wstring& in);
std::wstring UTF8ToUTF16(const std::string& in);
std::string UTF16ToUTF8(const std::wstring& in);

std::unique_ptr<wchar_t[]> StringToBuffer(const std::wstring& in);

std::wstring CreateGUID();
