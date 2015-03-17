#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <map>

#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>

class HttpResponse
{
public:
	void SetProperty(const std::string& field, const std::string& data);

public:
	std::string httpVersion;
	int statusCode;
	std::string statusMessage;
	std::map<std::string, std::string> propertyMap;
	boost::shared_ptr<std::vector<unsigned char>> content;
};

class HttpRequest
{
public:
	void SetProperty(const std::string& field, const std::string& data);

public:
	std::string adress;
	std::string path;
	std::map<std::string, std::string> propertyMap;
};

class HttpClient
{
public:
	HttpClient();
	virtual ~HttpClient();

public:
	boost::optional<HttpResponse> Request(const HttpRequest& httpRequest);

public:
	static std::string WcsToUtf8(const std::wstring& str);
	static std::wstring MbsToWcs(const std::string& str);
	static std::string ConvertToHex(const unsigned char* data, size_t size, const std::string delimiter = "");

private:
	static size_t WcsToUtf8(char output[6], const wchar_t& ch);

private:
	boost::asio::io_service io;
};

#endif