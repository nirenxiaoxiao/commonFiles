/*
 * ���������ļ���Helper
 *
*/

#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <tchar.h>

#include "URL.h"
#include "curl/curl.h"

using namespace std;

typedef function<void(string & response)>SuccessFinishedCallBack;//����ɹ�,�����ݷ���

class ICurlWriter
{
public:
    static size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        ICurlWriter* pThis = reinterpret_cast<ICurlWriter*>(userdata);
        return pThis->HandleWriteData(ptr, size, nmemb);
    }
    virtual size_t HandleWriteData(char *ptr, size_t size, size_t nmemb) = 0;
};

class CURLSList
{
public:
    CURLSList()
        : m_pList(NULL)
    {
    }
    ~CURLSList()
    {
        if (m_pList)
        {
            curl_slist_free_all(m_pList);
        }
    }
    bool Append(const char * data)
    {
        curl_slist *pNew = curl_slist_append(m_pList, data);
        if (!pNew)
        {
            return false;
        }
        else
        {
            m_pList = pNew;
            return true;
        }
    }
    curl_slist* Get() const
    {
        return m_pList;
    }
    std::string GetData()
    {
        std::string data;
        curl_slist *pHead = m_pList;
        while (pHead)
        {
            data.append(pHead->data);
            data.append(" ");
            pHead = pHead->next;
        }
        return std::move(data);
    }
private:
    curl_slist *m_pList;
};
class HttpResponse
{
public:
    HttpResponse()
    {}
    HttpResponse(const std::string& header)
    {
        FromHeader(header);
    }
    void FromHeader(const std::string& header)
    {
        m_rawHeader = header;
        std::unique_ptr<char[]> strBuf(new char[header.length() + 1]());
        strcpy_s(strBuf.get(), header.length() + 1, header.c_str());

        char *token = strtok(strBuf.get(), "\r\n");

        // ����http�汾��״̬�롢����
        std::string httpStatus = token;
        m_httpVersion = httpStatus.substr(0, httpStatus.find_first_of(' '));
        httpStatus = httpStatus.substr(httpStatus.find_first_of(' ') + 1, httpStatus.length());
        m_statusCode = httpStatus.substr(0, httpStatus.find_first_of(' '));
        httpStatus = httpStatus.substr(httpStatus.find_first_of(' ') + 1, httpStatus.length());
        m_statusText = httpStatus;

        token = strtok(NULL, "\r\n");

        // ����http�ֶ�
        while (token)
        {
            std::string field = token;
            std::string fieldType = field.substr(0, field.find_first_of(": "));
            std::transform(fieldType.begin(), fieldType.end(), fieldType.begin(), tolower);
            std::string fieldValue = field.substr(field.find_first_of(": ") + sizeof(": ") - 1, field.length());
            m_headerFields[fieldType] = fieldValue;
            token = strtok(NULL, "\r\n");
        }
    }
    std::string GetRawHeader() const
    {
        return m_rawHeader;
    }
    std::string GetHttpVersion() const
    {
        return m_httpVersion;
    }
    std::string GetStatusCode() const
    {
        return m_statusCode;
    }
    std::string GetStatusText() const
    {
        return m_statusText;
    }
    std::string GetFieldValue(std::string fieldType) const
    {
        std::transform(fieldType.begin(), fieldType.end(), fieldType.begin(), tolower);
        std::string value;
        try
        {
            value = m_headerFields.at(fieldType);
        }
        catch (std::out_of_range&) {
            return "";
        }
        return value;
    }
    bool IsEmpty()
    {
        return m_rawHeader.empty();
    }
    bool IsHeaderOK()
    {
        return !m_statusCode.empty() && m_statusCode[0] == '2';
    }
private:
    std::string m_rawHeader;
    std::string m_httpVersion;
    std::string m_statusCode;
    std::string m_statusText;
    std::map<std::string, std::string> m_headerFields;

};

typedef std::function<void(const HttpResponse&)> HttpHeaderCallback;
typedef std::function<void(curl_off_t, curl_off_t, curl_off_t, curl_off_t)> DownloadProgressCallback;

class FileDownloader
{
public:
    static std::wstring FormatErrCode(const int errorCode)
    {
        return ANSIToUTF16(curl_easy_strerror((CURLcode)errorCode));
    }
public:
    FileDownloader(std::wstring url);
    FileDownloader(std::wstring scheme, 
        std::wstring serverName,
        std::wstring path = _T(""),
        int port = 80, 
        std::wstring query = _T(""), 
        std::wstring fragment = _T(""), 
        std::wstring username = _T(""), 
        std::wstring password = _T(""));

    ~FileDownloader();

    std::wstring GetCurrentUrl() const;   // ��õ�ǰ����ʹ�õ�����URL(���ܴ���302�ض���ı�����)
    std::wstring GetScheme() const;
    std::wstring GetServerName() const;
    int GetPort() const;
    std::wstring GetServerPart() const;   // ���URL��ǰ�벿�֣���"http://www.example.com:8080"
    std::wstring GetObjectString() const; // ���URL��վ������ַ�������"/update/update.zip?key=value#fragment"
    std::wstring GetDirectory() const;    // "/update"
    std::wstring GetFileName() const;     // "update.zip"
    std::wstring GetUsername() const;
    std::wstring GetPassword() const;

    // HTTP 302 Found
    bool IsHandleRedirect();
    void SetHandleRedirect(bool bHandle);
    // ����HEAD���󣬴ӷ������ϻ�ȡ�ļ��Ļ�����Ϣ
    int FetchFileInfoFromServer(_Out_opt_ HttpResponse* result = nullptr);
    int GetURLContent(__in const ICurlWriter& writer, _Out_opt_ HttpResponse* result = nullptr);
    // �����ļ�ʱ��������¼�ļ���Ϣ���ļ���ֻ���ļ���������
    int SimpleSaveAsFile(std::wstring saveFilePath, _Out_opt_ HttpResponse* result = nullptr);

    void SetHttpHeaderCallback(HttpHeaderCallback func);
    void SetRecvContentStartCallback(HttpHeaderCallback func);
    void SetDownloadProgressCallback(DownloadProgressCallback func);
    void StopDownload();

	//debug
	static int onDebug(CURL * curl, curl_infotype itype, char * pData, size_t size, void * lpvoid);
	int  PostHttpRequest(const string & strUrl, const string & filePath, map<string, string>params, SuccessFinishedCallBack callback);
	//���ص����ݴ���
	static size_t write_data(void * buffer, size_t size, size_t nmenb, void * lpvoid);
	//����
	static int updateProgress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow);
	static FileDownloader* getInstance();
private:
    bool IsDownloadRunning();

    // callback for libcurl
    static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata)
    {
        FileDownloader* pThis = reinterpret_cast<FileDownloader*>(userdata);
        return pThis->HandleHttpHeaderData(buffer, size, nitems);
    }
    static int ProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
    {
        FileDownloader* pThis = reinterpret_cast<FileDownloader*>(clientp);
        return pThis->HandleProgress(dltotal, dlnow, ultotal, ulnow);
    }
    size_t HandleHttpHeaderData(char *buffer, size_t size, size_t nitems);
    int HandleProgress(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

private:
    FileDownloader(const FileDownloader&);
    const FileDownloader& operator=(const FileDownloader&);
private:
    CURL* m_curl;
private:
    // download file
    bool m_bHandleRedirect;
    bool m_bDownloadRunning;
    std::string m_recvHeaderBuf;
    std::vector<HttpResponse> m_recvHeaders;
    std::string m_fileMD5;// binary format
    // ������һ������httpͷ֮����õ�callback
    HttpHeaderCallback m_httpHeaderCallback;
    // ֪ͨ���ؿ�ʼ��callback
    HttpHeaderCallback m_recvContentStartCallback;
    // ��ȡ���ؽ��ȵ�callback
    DownloadProgressCallback m_progressCallback;
private:
    Url m_url;

	static FileDownloader * __instance;
};

