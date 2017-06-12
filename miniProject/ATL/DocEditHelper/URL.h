#pragma once

#include <string>
#include <map>

#ifdef ParseURL
#undef ParseURL
#endif // ParseURL

class Url
{
public:
    Url(const std::wstring& url);
    Url(std::wstring scheme, std::wstring serverName, std::wstring path, int port, 
        std::wstring query, std::wstring fragment, std::wstring username, std::wstring password);
    Url();

    bool ParseURL(const std::wstring& url);
    ~Url();

public:
    std::wstring GetURL() const;
    std::wstring GetScheme() const;
    std::wstring GetServerName() const;
    int GetPort() const;
    std::wstring GetServerPart() const;   // 获得URL的前半部分，如"http://www.example.com:8080"
    std::wstring GetObjectString() const; // 获得URL网站后面的字符串，如"/update/update.zip?key=value#fragment"
    std::wstring GetDirectory() const;    // "/update"
    std::wstring GetFileName() const;     // "update.zip"
    std::wstring GetUsername() const;
    std::wstring GetPassword() const;
    const std::map<std::wstring, std::wstring>& GetQueries() const;
    std::wstring GetFragment() const;

    void SetScheme(const std::wstring& scheme);
    void SetServerName(const std::wstring& servername);
    void SetPort(int port);
    void SetDirectory(const std::wstring& dir);    // "/update"
    void SetFileName(const std::wstring& filename);     // "update.zip"
    void SetUsername(const std::wstring& username);
    void SetPassword(const std::wstring& password);
    void SetFragment(const std::wstring& fragment);
    void ApplyNewQueryValues(const std::map<std::wstring, std::wstring>& newQueries);
private:
    bool ParseQuery(std::wstring query);
    bool ParsePath(std::wstring path);
private:
    std::wstring m_sScheme;
    std::wstring m_sServerName;
    int m_port;
    std::wstring m_sPath;
    std::wstring m_sDirectory;
    std::wstring m_sFileName;
    std::map<std::wstring, std::wstring> m_urlQueries;
    std::wstring m_sFragment;
    std::wstring m_sUsername;
    std::wstring m_sPassword;
};

