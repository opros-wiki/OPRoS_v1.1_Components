#include "HttpClient.h"

#include <iostream>
#include <iomanip>
#include <clocale>

#include <boost/algorithm/string.hpp>

HttpClient::HttpClient()
{
	std::setlocale(LC_ALL, "");
}


HttpClient::~HttpClient()
{
}

boost::optional<HttpResponse> HttpClient::Request( const HttpRequest& httpRequest )
{
	using namespace boost::asio::ip;

	boost::system::error_code errorCode;
	tcp::resolver resolver(io);
	tcp::resolver::query query(httpRequest.adress, "http");
	tcp::resolver::iterator endPointItor = resolver.resolve(query, errorCode);
	if (errorCode)
	{
		return boost::optional<HttpResponse>();
	}

	tcp::socket tcpSocket(io);
	boost::asio::connect(tcpSocket, endPointItor, errorCode);

	if (errorCode)
	{
		return boost::optional<HttpResponse>();
	}

	boost::asio::streambuf requestStreamBuffer;
	std::ostream requestStream(&requestStreamBuffer);
	std::map<std::string, std::string>::const_iterator requestPropertyItor = httpRequest.propertyMap.begin();
	const  std::map<std::string, std::string>::const_iterator requestPropertyItorEnd = httpRequest.propertyMap.end();
	
	requestStream << "GET " << httpRequest.path << " HTTP/1.0\r\n";
	for(; requestPropertyItor != requestPropertyItorEnd; ++requestPropertyItor)
	{
		requestStream << requestPropertyItor->first << ": " << requestPropertyItor->second << "\r\n";
	}
	requestStream << "\r\n";

	boost::asio::write(tcpSocket, requestStreamBuffer, errorCode);
	if (errorCode)
	{
		return boost::optional<HttpResponse>();
	}

	HttpResponse result;
	boost::asio::streambuf responseStreamBuffer;
	std::istream responseStream(&responseStreamBuffer);
	
	boost::asio::read_until(tcpSocket, responseStreamBuffer, "\r\n\r\n", errorCode);
	if (errorCode)
	{
		return boost::optional<HttpResponse>();
	}

	responseStream >> result.httpVersion;
	responseStream >> result.statusCode;
	std::getline(responseStream, result.statusMessage);
	boost::algorithm::trim(result.statusMessage);
	
	for(std::string responseProperty; std::getline(responseStream, responseProperty) && responseProperty != "\r";)
	{
		size_t delimIndex = responseProperty.find(':');
		std::string field = responseProperty.substr(0, delimIndex);
		std::string value = responseProperty.substr(delimIndex + 1);

		boost::algorithm::trim(field);
		boost::algorithm::trim(value);

		result.SetProperty(field, value);
	}

	for(; boost::asio::read(tcpSocket, responseStreamBuffer, boost::asio::transfer_at_least(1), errorCode); )
	{}
	if(errorCode != boost::asio::error::eof)
	{
		return boost::optional<HttpResponse>();;
	}
	errorCode.clear();

	result.content.reset(new std::vector<unsigned char>);
	result.content->reserve(responseStreamBuffer.size());
	responseStream >> std::noskipws;
	std::copy(std::istream_iterator<unsigned char>(responseStream), std::istream_iterator<unsigned char>(), std::back_inserter(*result.content));

	return result;
}

std::string HttpClient::WcsToUtf8( const std::wstring& str)
{
	std::string result;

	size_t size = str.size();

	result.reserve(size * 6);
	for (size_t i = 0; i < size; ++i)
	{
		char buff[6];
		result.append(buff, buff + WcsToUtf8(buff, str[i]));		
	}			
	return result;
}

size_t HttpClient::WcsToUtf8( char output[6], const wchar_t& ch )
{
	if(ch <= 0x7F)
	{
		output[0] = char(ch);
		return 1;
	}
	else if(ch <= 0x7FF)
	{
		output[0] = 0xC0 | char(ch >> 6);
		output[1] = 0x80 | char(ch & 0x3F);
		return 2;
	}
	else if(ch <= 0xFFFF)
	{
		output[0] = 0xE0 | char(ch >> 12);
		output[1] = 0x80 | char((ch >> 6) & 0x3F);
		output[2] = 0x80 | char(ch & 0x3F);
		return 3;
	}
#if WCHAR_MAX > 0xFFFF
	else if(ch <= 0x1FFFFF)
	{
		output[0] = 0xF0 | char(ch >> 18);
		output[1] = 0x80 | char((ch >> 12) & 0x3F);
		output[2] = 0x80 | char((ch >> 6) & 0x3F);
		output[3] = 0x80 | char(ch & 0x3F);
		return 4;
	}
	else if(ch <= 0x3FFFFFF)
	{
		output[0] = 0xF8 | char(ch >> 24);
		output[1] = 0x80 | char((ch >> 18) & 0x3F);
		output[2] = 0x80 | char((ch >> 12) & 0x3F);
		output[3] = 0x80 | char((ch >> 6) & 0x3F);
		output[4] = 0x80 | char(ch & 0x3F);
		return 5;
	}
	else if(ch <= 0x7FFFFFFF)
	{
		output[0] = 0xFC | char(ch >> 30);
		output[1] = 0x80 | char((ch >> 24) & 0x3F);
		output[2] = 0x80 | char((ch >> 18) & 0x3F);
		output[3] = 0x80 | char((ch >> 12) & 0x3F);
		output[4] = 0x80 | char((ch >> 6) & 0x3F);
		output[5] = 0x80 | char(ch & 0x3F);
		return 6;
	}
#endif
	return 0;
}

std::wstring HttpClient::MbsToWcs(const std::string& str)
{
	if (str.size() == 0)
		return std::wstring();

	std::wstring result;
	result.resize(str.size());

	size_t convertedSize = std::mbstowcs(&result[0], str.c_str(), result.size());
	if(convertedSize <= 0)
		return std::wstring();
	result.resize(convertedSize);
	return result;
}

std::string HttpClient::ConvertToHex( const unsigned char* data, size_t size, const std::string delimiter /*= ""*/ )
{
	std::stringstream stringStream;

	for (size_t i = 0; i < size; i++)
	{
		unsigned int temp = data[i];
		stringStream << delimiter << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << temp;
	}

	return stringStream.str();
}

void HttpRequest::SetProperty( const std::string& field, const std::string& data )
{
	propertyMap.insert(std::pair<std::string, std::string>(field, data));
}

void HttpResponse::SetProperty( const std::string& field, const std::string& data )
{
	propertyMap.insert(std::pair<std::string, std::string>(field, data));
}
