

#ifndef _SpeechService_PROVIDED_PORT_H
#define _SpeechService_PROVIDED_PORT_H

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
		

#ifndef _ISpeechService_CLASS_DEF
#define _ISpeechService_CLASS_DEF
/*
 * ISpeechService
 *
 *  The comonent inherits this class and implements methods. 
 */
class ISpeechService
{
 public:
    virtual bool SetParameter(Property parameter)=0;
    virtual Property GetParameter()=0;
    virtual int GetError()=0;
    virtual bool PlaySpeech(string speechData,int mode)=0;
    virtual bool StopSpeech()=0;
 };
#endif

/*
 * 
 * SpeechService : public ProvidedServicePort
 *
 */
class SpeechServiceProvided
	:public ProvidedServicePort, public ISpeechService
{
protected:
    ISpeechService *pcom;


   typedef ProvidedMethod<bool> SetParameterFuncType;
   SetParameterFuncType *SetParameterFunc;

   typedef ProvidedMethod<Property> GetParameterFuncType;
   GetParameterFuncType *GetParameterFunc;

   typedef ProvidedMethod<int> GetErrorFuncType;
   GetErrorFuncType *GetErrorFunc;

   typedef ProvidedMethod<bool> PlaySpeechFuncType;
   PlaySpeechFuncType *PlaySpeechFunc;

   typedef ProvidedMethod<bool> StopSpeechFuncType;
   StopSpeechFuncType *StopSpeechFunc;


public: // default method
   virtual bool SetParameter(Property parameter)
   {
		opros_assert(SetParameterFunc != NULL);
		
            return (*SetParameterFunc)(parameter);
		
   }
   virtual Property GetParameter()
   {
		opros_assert(GetParameterFunc != NULL);
		
            return (*GetParameterFunc)();
		
   }
   virtual int GetError()
   {
		opros_assert(GetErrorFunc != NULL);
		
            return (*GetErrorFunc)();
		
   }
   virtual bool PlaySpeech(string speechData,int mode)
   {
		opros_assert(PlaySpeechFunc != NULL);
		
            return (*PlaySpeechFunc)(speechData,mode);
		
   }
   virtual bool StopSpeech()
   {
		opros_assert(StopSpeechFunc != NULL);
		
            return (*StopSpeechFunc)();
		
   }


public:
    //
    // Constructor
    //
    SpeechServiceProvided(ISpeechService *fn)
    {
        pcom = fn;

        SetParameterFunc = NULL;
        GetParameterFunc = NULL;
        GetErrorFunc = NULL;
        PlaySpeechFunc = NULL;
        StopSpeechFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&ISpeechService::SetParameter,pcom,"SetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&ISpeechService::GetParameter,pcom,"GetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&ISpeechService::GetError,pcom,"GetError");

        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&ISpeechService::PlaySpeech,pcom,"PlaySpeech");

        opros_assert(ptr_method != NULL);
        addMethod("PlaySpeech",ptr_method);
        PlaySpeechFunc = reinterpret_cast<PlaySpeechFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&ISpeechService::StopSpeech,pcom,"StopSpeech");

        opros_assert(ptr_method != NULL);
        addMethod("StopSpeech",ptr_method);
        StopSpeechFunc = reinterpret_cast<StopSpeechFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
