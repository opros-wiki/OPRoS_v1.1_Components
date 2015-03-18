

#ifndef _TranslateService_PROVIDED_PORT_H
#define _TranslateService_PROVIDED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>

#include <string>

#include "wstringSerialize.h"

#ifndef _ITranslateService_CLASS_DEF
#define _ITranslateService_CLASS_DEF

/*
* ITranslateService
*
*  The comonent inherits this class and implements methods. 
*/
class ITranslateService
{
public:
	virtual std::wstring Translate(std::wstring text,std::string originalLanguageCode,std::string translatedLanguageCode)=0;
};
#endif

/*
* 
* TranslateService : public ProvidedServicePort
*
*/
class TranslateServiceProvided
	:public ProvidedServicePort, public ITranslateService
{
protected:
	ITranslateService *pcom;


	typedef ProvidedMethod<std::wstring> TranslateFuncType;
	TranslateFuncType *TranslateFunc;


public: // default method
	virtual std::wstring Translate(std::wstring text,std::string originalLanguageCode,std::string translatedLanguageCode)
	{
		opros_assert(TranslateFunc != NULL);

		return (*TranslateFunc)(text,originalLanguageCode,translatedLanguageCode);
	}


public:
	//
	// Constructor
	//
	TranslateServiceProvided(ITranslateService *fn)
	{
		pcom = fn;

		TranslateFunc = NULL;


		setup();
	}

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeProvidedMethod(&ITranslateService::Translate,pcom,"Translate");

		opros_assert(ptr_method != NULL);
		addMethod("Translate",ptr_method);
		TranslateFunc = reinterpret_cast<TranslateFuncType *>(ptr_method);
		ptr_method = NULL;
	}
};
#endif
