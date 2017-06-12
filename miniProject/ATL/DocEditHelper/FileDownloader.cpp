#include "StdAfx.h"
#include "FileDownloader.h"

#include <cassert>
#include <regex>

FileDownloader::FileDownloader(std::wstring url)
    : m_bHandleRedirect(true)
    , m_bDownloadRunning(true)
    , m_url(url)
    , m_curl(curl_easy_init())
    , m_recvContentStartCallback(nullptr)
    , m_progressCallback(nullptr)
{
    assert(m_curl);
}

FileDownloader::FileDownloader(std::wstring scheme, std::wstring serverName, std::wstring path, int port,
    std::wstring query, std::wstring fragment, std::wstring username, std::wstring password)
    : m_bHandleRedirect(true)
    , m_bDownloadRunning(true)
    , m_url(scheme, serverName, path, port, query, fragment, username, password)
    , m_curl(curl_easy_init())
    , m_recvContentStartCallback(nullptr)
    , m_progressCallback(nullptr)
{
}

FileDownloader::~FileDownloader()
{
    curl_easy_cleanup(m_curl);
}

std::wstring FileDownloader::GetCurrentUrl() const
{
    return m_url.GetURL();
}

std::wstring FileDownloader::GetScheme() const
{
    return m_url.GetScheme();
}

std::wstring FileDownloader::GetServerName() const
{
    return m_url.GetServerName();
}

int FileDownloader::GetPort() const
{
    return m_url.GetPort();
}

std::wstring FileDownloader::GetServerPart() const
{
    return m_url.GetServerPart();
}

std::wstring FileDownloader::GetObjectString() const
{
    return m_url.GetObjectString();
}

std::wstring FileDownloader::GetDirectory() const
{
    return m_url.GetDirectory();
}

std::wstring FileDownloader::GetFileName() const
{
    return m_url.GetFileName();
}

std::wstring FileDownloader::GetUsername() const
{
    return m_url.GetUsername();
}

std::wstring FileDownloader::GetPassword() const
{
    return m_url.GetPassword();
}


bool FileDownloader::IsHandleRedirect()
{
    return m_bHandleRedirect;
}

void FileDownloader::SetHandleRedirect(bool bHandle)
{
    m_bHandleRedirect = bHandle;
}

size_t FileDownloader::HandleHttpHeaderData(char * buffer, size_t size, size_t nitems)
{
    std::unique_ptr<char[]> recvBuf(new char[size * nitems + 1]());
    memcpy_s(recvBuf.get(), size * nitems, buffer, size * nitems);
    std::string data = recvBuf.get();
    m_recvHeaderBuf.append(data);
    if (data == "\r\n")
    {
        // httpͷ�ѽ������
        HttpResponse response(m_recvHeaderBuf);

        if (m_httpHeaderCallback) m_httpHeaderCallback(response);

        if (response.GetStatusCode()[0] != '3')
        {
            if (response.GetStatusCode()[0] != '2')
            {
                // not redirect, ERROR occured 
                m_bDownloadRunning = false;
            }
            else
            {
                // OK
                // ֪ͨ�������ݼ�����ʼ
                if (m_recvContentStartCallback) m_recvContentStartCallback(response);
            }
        }
        m_recvHeaders.push_back(response);
        m_recvHeaderBuf.clear();
    }

    return size * nitems;
}

int FileDownloader::HandleProgress(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    if (m_progressCallback) m_progressCallback(dltotal, dlnow, ultotal, ulnow);

    if (!m_bDownloadRunning)
    {
        return 1;
    }
    return 0;
}

int FileDownloader::FetchFileInfoFromServer(_Out_opt_ HttpResponse* result)
{
    CURLcode error = CURLE_OK;
    if (!m_curl)
    {
        assert(false);
        return false;
    }

    m_recvHeaders.clear();

    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT_MS, 10000);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0);// use progress callback
    if (m_bHandleRedirect)
    {
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);// enable redirect
        curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, 10);// max redirect
    }

    curl_easy_setopt(m_curl, CURLOPT_URL, UTF16ToUTF8(GetCurrentUrl()).c_str());// set url

    curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1); // ����HEAD����

    // callbacks
    curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, this);
    curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);

    error = curl_easy_perform(m_curl);

    const char *lastURL = nullptr;
    curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL, &lastURL);

    m_url.ParseURL(UTF8ToUTF16(lastURL));

    if (result && !m_recvHeaders.empty())
    {
        *result = *m_recvHeaders.crbegin();
    }

    return error;
}

int FileDownloader::GetURLContent(__in const ICurlWriter& writer, _Out_opt_ HttpResponse* result)
{
    CURLcode error = CURLE_OK;
    if (!m_curl)
    {
        assert(false);
        return false;
    }

    m_recvHeaders.clear();

    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT_MS, 10000);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0);// use progress callback
    if (m_bHandleRedirect)
    {
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);// enable redirect
        curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, 10);// max redirect
    }

    curl_easy_setopt(m_curl, CURLOPT_URL, UTF16ToUTF8(GetCurrentUrl()).c_str());// set url

    curl_easy_setopt(m_curl, CURLOPT_HTTPGET, 1); // ����GET����

    // callbacks
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &writer);
    error = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ICurlWriter::WriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, this);
    curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);

    error = curl_easy_perform(m_curl);

    const char *lastURL = nullptr;
    curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL, &lastURL);
    m_url.ParseURL(UTF8ToUTF16(lastURL));

    if (result && !m_recvHeaders.empty())
    {
        *result = *m_recvHeaders.crbegin();
    }

    return error;
}

int FileDownloader::SimpleSaveAsFile(std::wstring saveFilePath, _Out_opt_ HttpResponse* result)
{
    class DownloadFile : public ICurlWriter
    {
    public:
        DownloadFile(const std::wstring& saveFilePath)
            : m_file(nullptr)
        {
            m_file = _wfopen(saveFilePath.c_str(), _T("wb"));
            if (!m_file)
            {
                throw std::runtime_error("Create save file FAILED!");
            }
        }
        ~DownloadFile()
        {
            fclose(m_file);
        }
        virtual size_t HandleWriteData(char *ptr, size_t size, size_t nmemb)
        {
            size_t bytesWritten = fwrite(ptr, size, nmemb, m_file);
            fflush(m_file);
            return bytesWritten;
        }
    private:
        FILE* m_file;
    };

    try
    {
        DownloadFile file(saveFilePath);
        return GetURLContent(file, result);
    }
    catch (const std::exception&)
    {
        return CURLE_FAILED_INIT;
    }
}

void FileDownloader::SetHttpHeaderCallback(HttpHeaderCallback func)
{
    m_httpHeaderCallback = func;
}

void FileDownloader::SetRecvContentStartCallback(HttpHeaderCallback func)
{
    m_recvContentStartCallback = func;
}

void FileDownloader::SetDownloadProgressCallback(DownloadProgressCallback func)
{
    m_progressCallback = func;
}

void FileDownloader::StopDownload()
{
    m_bDownloadRunning = false;
}

bool FileDownloader::IsDownloadRunning()
{
    return m_bDownloadRunning;
}


//post����
int  FileDownloader::PostHttpRequest(const string & strUrl, const string & filePath, map<string, string>params, SuccessFinishedCallBack callback)
{
	DebugLog("========================postUrl:%s", strUrl);
	DebugLog("========================filePath:%s", filePath);
	string strResponse;//��������
	int progress = 0;
	CURL * curl;
	CURLcode res;

	struct curl_httppost * post = NULL;
	struct curl_httppost * last = NULL;
	struct curl_slist * headers = NULL;
	static const char buf[] = "Expect:";
	//�����ϴ��ļ��ı�
	if (!filePath.empty()) {
		curl_formadd(&post, &last,
			CURLFORM_COPYNAME, "file",
			CURLFORM_FILE, filePath.c_str(),
			CURLFORM_CONTENTTYPE, "multipart/form-data",
			CURLFORM_END);
	}
	//���ò���
	for (auto iter = params.begin(); iter != params.end(); iter++) {
		curl_formadd(&post, &last,
			CURLFORM_COPYNAME, iter->first.c_str(),
			CURLFORM_COPYCONTENTS, iter->second.c_str(),
			CURLFORM_END);
	}
	//
	curl = curl_easy_init();//��ʼ�����

	if (NULL == curl) {
		return CURLE_FAILED_INIT;
	}
	////
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);//���CURLOPT_VERBOSE��Ϊ1������ʾ�����з��͵�ʵ��Э���ϸ��
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, onDebug);//���ｫ����onDebug����������Ϣ
/////
	headers = curl_slist_append(headers, buf);//
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);//��
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);//�Ƿ���Ҫ����
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, updateProgress);//���Ⱥ���
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress);//��������
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);//����ʱ��
	res = curl_easy_perform(curl);
	DebugLog("========================filePath:%d", res);
	if (res == CURLE_OK) {
		callback(strResponse);
	}
	curl_global_cleanup();
	curl_easy_cleanup(curl);
	curl_formfree(post);
	curl_slist_free_all(headers);
	return res;
}

int FileDownloader::onDebug(CURL * curl, curl_infotype itype, char * pData, size_t size, void * lpvoid)
{
	if (itype == CURLINFO_TEXT) {
		printf("================[text] %s\n", pData);
	}
	else if (itype == CURLINFO_HEADER_IN){
		printf("==================[HEADER_IN] %s\n", pData);
	}
	else if (itype == CURLINFO_HEADER_OUT){
		printf("=================[HEADER_IN] %s\n", pData);
	}
	else if (itype == CURLINFO_DATA_IN){
		printf("====================[HEADER_IN] %s\n", pData);
	}
	else if (itype == CURLINFO_DATA_OUT){
		printf("====================[HEADER_IN] %s\n", pData);
	}
	return (int)itype;
}

size_t FileDownloader::write_data(void * buffer, size_t size, size_t nmenb, void * lpvoid)
{
	string * str = dynamic_cast<string *>((string*)lpvoid);
	if (NULL == str || NULL == buffer) {
		return -1;
	}
	char * pData = (char *)buffer;
	str->append(pData, size*nmenb);
	return nmenb;
}

int FileDownloader::updateProgress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow)
{
	DebugLog("========================����%f", rUlNow);
	return 0;
}

FileDownloader* FileDownloader::__instance = NULL;
FileDownloader* FileDownloader::getInstance()
{
	if (NULL == __instance) {
		__instance = new FileDownloader(L"11111");
	}
	return __instance;
}