/*
*  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
*  
*/
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/algorithm/string.hpp>

#include "GoogleTranslate.h"
#include "TranslateServiceRequired.h"

#ifndef _WINDLL
int main()
{
	//std::locale::global(std::locale(""));
	GoogleTranslate translate;
	translate.initialize();
	translate.start();

	for (;;)
	{
		std::string originalCode;
		std::string translationCode;
		std::string message;
		std::cin >> originalCode >> translationCode;
		std::getline(std::cin, message);
		std::wstring text = HttpClient::MbsToWcs(message);

		std::wstring translated = translate.Translate(text, originalCode, translationCode);

		std::wcout << translated << std::endl;
	}
	return 0;
}
#endif

//
// constructor declaration
//
GoogleTranslate::GoogleTranslate()
{
	portSetup();
}

//
// constructor declaration (with name)
//
GoogleTranslate::GoogleTranslate(const std::string &name) :
Component(name)
{
	portSetup();
}

//
// destructor declaration
//

GoogleTranslate::~GoogleTranslate()
{
}

void GoogleTranslate::portSetup()
{
	ProvidedServicePort *pa1;
	pa1 = new TranslateServiceProvided(this);
	addPort("TranslateService", pa1);
}

// Call back Declaration
ReturnType GoogleTranslate::onInitialize()
{
	// user code here
	std::locale::global(std::locale(""));
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GoogleTranslate::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

std::wstring GoogleTranslate::Translate(std::wstring text, std::string originalLanguageCode, std::string translatedLanguageCode)
{
	typedef boost::program_options::detail::utf8_codecvt_facet utf8_codecvt_facet;

	std::string utf8TextData = HttpClient::WcsToMbs(text, std::locale(std::locale(""), new utf8_codecvt_facet));

	HttpRequest request;
	request.adress = "translate.google.com";
	request.path = "/translate_a/t?client=t&text=";
	request.path += HttpClient::ConvertToHex((unsigned char*)utf8TextData.c_str(), utf8TextData.size(), "%");
	request.path += "&sl=" + originalLanguageCode + "&tl=" + translatedLanguageCode;

	request.SetProperty("Host", request.adress);
	request.SetProperty("Connection", "close");
	request.SetProperty("User-Agent", "Mozilla/5.0 (OPRoS)");
	request.SetProperty("Accept", "*/*");

	boost::optional<HttpResponse> response = httpClient.Request(request);

	if( !response || response->statusCode != 200)
	{
		return L"";
	}

	std::wstring translationData = HttpClient::MbsToWcs(std::string(response->content->begin(), response->content->end())
		, std::locale(std::locale(""), new utf8_codecvt_facet));

	std::wstring result;
	if (ParseTranslationData(result, translationData) == false)
	{
		return L"";
	}

	return result;
}

bool GoogleTranslate::ParseTranslationData(std::wstring& result, const std::wstring& translationData)
{
	const std::wstring startString(L"[[");

	if (translationData.size() < startString.size() || translationData.substr(0, startString.size()) != startString)
	{
		return false;
	}

	typedef std::list<std::vector<std::wstring>> StatementList;
	StatementList statementList;

	bool startParagraphParsing = false;
	std::wstring temp;
	temp.reserve(100);
	for (std::wstring::const_iterator itor = translationData.begin() + startString.size(); itor != translationData.end(); ++itor)
	{
		if (startParagraphParsing == false)
		{			
			if (*itor == L'[')
			{
				startParagraphParsing = true;
				continue;
			}
			else if (*itor == L']' && *(itor + 1) == L']')
			{
				break;
			}		
			continue;
		}

		StatementList::value_type statement;
		bool startStatementParsingFlag = false;
		for (; itor != translationData.end(); ++itor)
		{
			if (startStatementParsingFlag == false)
			{
				if (*itor == L'"')
				{
					startStatementParsingFlag = true;
					continue;
				}
				else if(*itor == L']')
				{
					--itor;
					break;
				}
				continue;
			}
			else if (startStatementParsingFlag == true)
			{
				if (*itor == '\\')
				{
					++itor;
					temp.push_back(*itor);
				}
				else if(*itor == '"')
				{
					statement.push_back(temp);

					startStatementParsingFlag = false;
					temp.clear();
					continue;
				}
				else
				{
					temp.push_back(*itor);
				}
			}
		}

		statementList.push_back(statement);
		startParagraphParsing = false;
	}

	result.clear();
	for (StatementList::iterator itor = statementList.begin(); itor != statementList.end(); ++itor)
	{
		StatementList::value_type& statement = *itor;

		if (statement.size() < 4)
		{
			break;
		}

		result += statement[0];
	}

	return true;
}

#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
	__declspec(dllexport) Component *getComponent();
	__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new GoogleTranslate();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new GoogleTranslate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

