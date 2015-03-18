

#ifndef _SpeechService_REQUIRED_PORT_H
#define _SpeechService_REQUIRED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>


#include "Property.h"
		
#include "string.h"
		



/*
 * 
 * SpeechService : public RequiredServicePort
 *
 */
class SpeechServiceRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<bool> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<int> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<bool> PlaySpeechFuncType;
	PlaySpeechFuncType *PlaySpeechFunc;

	typedef RequiredMethod<bool> StopSpeechFuncType;
	StopSpeechFuncType *StopSpeechFunc;

public:
	//
	// Constructor
	//
	SpeechServiceRequired()
	{
            SetParameterFunc = NULL;
            GetParameterFunc = NULL;
            GetErrorFunc = NULL;
            PlaySpeechFunc = NULL;
            StopSpeechFunc = NULL;
            
	   setup();
	}

	// method implementation for required method
	bool SetParameter(Property parameter)
	{
            opros_assert(SetParameterFunc != NULL);
	
            return (*SetParameterFunc)(parameter);
		
	}

	Property GetParameter()
	{
            opros_assert(GetParameterFunc != NULL);
	
            return (*GetParameterFunc)();
		
	}

	int GetError()
	{
            opros_assert(GetErrorFunc != NULL);
	
            return (*GetErrorFunc)();
		
	}

	bool PlaySpeech(string speechData,int mode)
	{
            opros_assert(PlaySpeechFunc != NULL);
	
            return (*PlaySpeechFunc)(speechData,mode);
		
	}

	bool StopSpeech()
	{
            opros_assert(StopSpeechFunc != NULL);
	
            return (*StopSpeechFunc)();
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&SpeechServiceRequired::SetParameter,"SetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&SpeechServiceRequired::GetParameter,"GetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&SpeechServiceRequired::GetError,"GetError");
        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&SpeechServiceRequired::PlaySpeech,"PlaySpeech");
        opros_assert(ptr_method != NULL);
        addMethod("PlaySpeech",ptr_method);
        PlaySpeechFunc = reinterpret_cast<PlaySpeechFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&SpeechServiceRequired::StopSpeech,"StopSpeech");
        opros_assert(ptr_method != NULL);
        addMethod("StopSpeech",ptr_method);
        StopSpeechFunc = reinterpret_cast<StopSpeechFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
