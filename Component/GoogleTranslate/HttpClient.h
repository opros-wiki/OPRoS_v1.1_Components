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
	static std::string WcsToMbs(const std::wstring& str, const std::locale& locale = std::locale());
	static std::wstring MbsToWcs(const std::string& str, const std::locale& locale = std::locale());
	static std::string ConvertToHex(const unsigned char* data, size_t size, const std::string delimiter = "");

private:
	boost::asio::io_service io;
};

#endif