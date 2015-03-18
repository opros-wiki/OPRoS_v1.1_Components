#include "HttpClient.h"

#include <iostream>
#include <iomanip>

#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/algorithm/string.hpp>

HttpClient::HttpClient()
{
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

std::string HttpClient::WcsToMbs( const std::wstring& str, const std::locale& locale /*= std::locale()*/ )
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(locale);
	std::mbstate_t state = std::mbstate_t();
	std::vector<char> buf((str.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = str.c_str();
	char* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.out(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == std::codecvt_base::error)
		throw std::runtime_error("can't convert wstring to string");   
	return std::string(&buf[0]);
}

std::wstring HttpClient::MbsToWcs( const std::string& str, const std::locale& locale /*= std::locale()*/ )
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(locale);
	std::mbstate_t state = std::mbstate_t();
	std::vector<wchar_t> buf(str.size() + 1);
	char const* in_next = str.c_str();
	wchar_t* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.in(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == std::codecvt_base::error)
		throw std::runtime_error("can't convert string to wstring");   
	return std::wstring(&buf[0]);
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
