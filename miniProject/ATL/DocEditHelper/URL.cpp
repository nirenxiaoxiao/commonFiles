#include "URL.h"

#include <regex>

Url::Url(const std::wstring& url)
    : m_port(0)
{
    ParseURL(url);
}

Url::Url(std::wstring scheme, std::wstring serverName, std::wstring path,
    int port, std::wstring query, std::wstring fragment, std::wstring username, std::wstring password)
    : m_sScheme(scheme)
    , m_sServerName(serverName)
    , m_port(port)
    , m_sPath(path)
    , m_sFragment(fragment)
    , m_sUsername(username)
    , m_sPassword(password)
{
    if (m_sPath.empty() || (!m_sPath.empty() && m_sPath[0] != L'/'))
    {
        m_sPath.insert(0, L"/");
    }
    ParseQuery(query);
    ParsePath(path);
}

Url::Url()
{
}

Url::~Url()
{
}

std::wstring Url::GetURL() const
{
    std::wstring url;
    url.append(m_sScheme + L"://");
    if (!m_sUsername.empty() || !m_sUsername.empty())
    {
        url.append(m_sUsername + L":" + m_sPassword + L"@");
    }
    url.append(m_sServerName);
    if (m_port != 80 && m_port != 443)
    {
        wchar_t buf[50];
        swprintf_s(buf, 50, L"%d", m_port);
        url.append(L":");
        url.append(buf);
    }
    url.append(GetObjectString());
    return url;
}

std::wstring Url::GetScheme() const
{
    return m_sScheme;
}

std::wstring Url::GetServerName() const
{
    return m_sServerName;
}

int Url::GetPort() const
{
    return m_port;
}

std::wstring Url::GetServerPart() const
{
    std::wstring url;
    url.append(m_sScheme + L"://");
    if (!m_sUsername.empty() || !m_sUsername.empty())
    {
        url.append(m_sUsername + L":" + m_sPassword + L"@");
    }
    url.append(m_sServerName);
    if (m_port != 80 && m_port != 443)
    {
        wchar_t buf[50];
        swprintf_s(buf, 50, L"%d", m_port);
        url.append(L":");
        url.append(buf);
    }
    return url;
}

std::wstring Url::GetObjectString() const
{
    std::wstring objectString = m_sPath;
    // append queries
    if (!m_urlQueries.empty())
    {
        objectString.append(L"?");
        for (auto iter = m_urlQueries.cbegin(); iter != m_urlQueries.cend(); iter++)
        {
            if (iter != m_urlQueries.cbegin())
            {
                objectString.append(L"&");
            }
            std::wstring queryPair = iter->first + L"=" + iter->second;
            objectString.append(queryPair);
        }
    }
    // append fragment
    if (!m_sFragment.empty())
    {
        objectString.append(L"#" + m_sFragment);
    }

    return objectString;
}

std::wstring Url::GetDirectory() const
{
    return m_sDirectory;
}

std::wstring Url::GetFileName() const
{
    return m_sFileName;
}

std::wstring Url::GetUsername() const
{
    return m_sUsername;
}

std::wstring Url::GetPassword() const
{
    return m_sPassword;
}

const std::map<std::wstring, std::wstring>& Url::GetQueries() const
{
    return m_urlQueries;
}

std::wstring Url::GetFragment() const
{
    return m_sFragment;
}

void Url::SetScheme(const std::wstring & scheme)
{
    m_sScheme = scheme;
}

void Url::SetServerName(const std::wstring & servername)
{
    m_sServerName = servername;
}

void Url::SetPort(int port)
{
    m_port = port;
}

void Url::SetDirectory(const std::wstring & dir)
{
    m_sDirectory = dir;
}

void Url::SetFileName(const std::wstring & filename)
{
    m_sFileName = filename;
}

void Url::SetUsername(const std::wstring & username)
{
    m_sUsername = username;
}

void Url::SetPassword(const std::wstring & password)
{
    m_sPassword = password;
}

void Url::SetFragment(const std::wstring & fragment)
{
    m_sFragment = fragment;
}

void Url::ApplyNewQueryValues(const std::map<std::wstring, std::wstring>& newQueries)
{
    m_urlQueries = newQueries;
}

bool Url::ParseURL(const std::wstring& url)
{
    // (https?):\/\/(([^\s@:]*):([^\s@:]*)@)?([-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z0-9]{2,4})(:([0-9]{1,5}))?\b \
    // ([-a-zA-Z0-9@:%_\+.~//=]*)(\?([-a-zA-Z0-9@:%&_\+.~//=]*))?(#([-a-zA-Z0-9%_\+.~//=]*))?
    //             scheme: (https?)
    // username:password@: ([^\s@:]*):([^\s@:]*)@
    //         serverName: [-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,4}
    //               port: :([0-9]{1,5})
    //               path: [-a-zA-Z0-9@:%_\+.~//=]*
    //              query: \?([-a-zA-Z0-9@:%&_\+.~//=]*)
    //           fragment: #([-a-zA-Z0-9%_\+.~//=]*)
    // 
    // example: http://username:password@abcdef.example.com:8080/pathname/file?key1=value1&key2=value2&key3=value3#fragment
    // matches:
    //  0. <the url>
    //  1. http
    //  2. username:password@
    //  3. username
    //  4. password
    //  5. abcdef.example.com
    //  6. :8080
    //  7. 8080
    //  8. /pathname/file
    //  9. ?key1=value1&key2=value2&key3=value3
    // 10. key1=value1&key2=value2&key3=value3
    // 11. #fragment
    // 12. fragment
    std::wregex urlRegex(L"(https?):\\/\\/(([^\\s@:]*):([^\\s@:]*)@)?([-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z0-9]{2,4})(:([0-9]{1,5}))?\\b"
        L"([-a-zA-Z0-9@:%_\\+.~/=]*)(\\?([-a-zA-Z0-9@:%&_\\+.~/=]*))?(#([-a-zA-Z0-9%_\\+.~/=]*))?");

    bool bMatch = std::regex_match(url, urlRegex);

    if (bMatch)
    {
        std::wsmatch match;
        std::regex_search(url, match, urlRegex);

        m_sScheme = match[1];
        m_sUsername = match[3];
        m_sPassword = match[4];
        m_sServerName = match[5];
        m_port = _wtoi(match[7].str().c_str());
        if (m_port == 0)
        {
            if (m_sScheme == L"http") m_port = 80;
            if (m_sScheme == L"https") m_port = 443;
        }
        m_sPath = match[8];
        m_sFragment = match[12];

        std::wstring query = match[10];

        ParseQuery(query);
        ParsePath(m_sPath);

        return true;
    }
    else
    {
        return false;
    }
}

bool Url::ParseQuery(std::wstring query)
{
    std::wregex queryRegex(L"([^\\s=&]+=[^\\s=&]+&)*([^\\s=&]+=[^\\s=&]+)");
    bool bMatch = std::regex_match(query, queryRegex);

    m_urlQueries.clear();
    if (bMatch)
    {
        while (query != L"")
        {
            std::wstring queryPair = query.substr(0, query.find_first_of(L'&'));
            query = query.substr(query.find_first_of(L'&') != -1 ? query.find_first_of(L'&') + 1 : query.length(), query.length());

            std::wstring key = queryPair.substr(0, queryPair.find_first_of(L'='));
            std::wstring value = queryPair.substr(queryPair.find_first_of(L'=') + 1, queryPair.length());

            m_urlQueries.insert(std::make_pair(key, value));
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool Url::ParsePath(std::wstring path)
{
    size_t slashPos = path.find_last_of(L'/');
    m_sDirectory = path.substr(0, slashPos);
    m_sFileName = path.substr(slashPos + 1, path.length());
    return true;
}
