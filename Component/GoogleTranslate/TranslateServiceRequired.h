

#ifndef _TranslateService_REQUIRED_PORT_H
#define _TranslateService_REQUIRED_PORT_H

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


/*
 * 
 * TranslateService : public RequiredServicePort
 *
 */
class TranslateServiceRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<std::wstring> TranslateFuncType;
	TranslateFuncType *TranslateFunc;

public:
	//
	// Constructor
	//
	TranslateServiceRequired()
	{
            TranslateFunc = NULL;
            
	   setup();
	}

	// method implementation for required method
	std::wstring Translate(std::wstring text,std::string originalLanguageCode,std::string translatedLanguageCode)
	{
            opros_assert(TranslateFunc != NULL);
	
            return (*TranslateFunc)(text,originalLanguageCode,translatedLanguageCode);
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&TranslateServiceRequired::Translate,"Translate");
        opros_assert(ptr_method != NULL);
        addMethod("Translate",ptr_method);
        TranslateFunc = reinterpret_cast<TranslateFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
