#ifndef _GoogleTranslate_COMPONENT_H
#define _GoogleTranslate_COMPONENT_H
/*
*  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
*   
*/
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>

#include "TranslateServiceProvided.h"
#include "HttpClient.h"

class GoogleTranslate: public Component, public ITranslateService
{
protected:
	// service method for provider
	ITranslateService *ptrTranslateService;

public:
	GoogleTranslate();
	GoogleTranslate(const std::string &compId);
	virtual ~GoogleTranslate();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();

public:
	virtual std::wstring Translate(std::wstring text, std::string originalLanguageCode,	std::string translatedLanguageCode);

private:
	bool ParseTranslationData(std::wstring& result, const std::wstring& translationData);

private:
	HttpClient httpClient;
};

#endif

