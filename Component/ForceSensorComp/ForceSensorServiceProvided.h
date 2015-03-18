#ifndef _ForceSensorService_PROVIDED_PORT_H
#define _ForceSensorService_PROVIDED_PORT_H

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

#ifndef _IForceSensorService_CLASS_DEF
#define _IForceSensorService_CLASS_DEF
/*
 * IForceSensorService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IForceSensorService
{
 public:
    virtual ReturnType SetParameter(Property parameter)=0;
    virtual Property GetParameter()=0;
    virtual ReturnType GetError()=0;
    virtual ReturnType Enable()=0;
    virtual ReturnType Disable()=0;
    virtual vector<double> GetSensorValue()=0;
 };
#endif

/*
 * 
 * ForceSensorService : public ProvidedServicePort
 *
 */
class ForceSensorServiceProvided
	:public ProvidedServicePort, public IForceSensorService
{
protected:
    IForceSensorService *pcom;


   typedef ProvidedMethod<ReturnType> SetParameterFuncType;
   SetParameterFuncType *SetParameterFunc;

   typedef ProvidedMethod<Property> GetParameterFuncType;
   GetParameterFuncType *GetParameterFunc;

   typedef ProvidedMethod<ReturnType> GetErrorFuncType;
   GetErrorFuncType *GetErrorFunc;

   typedef ProvidedMethod<ReturnType> EnableFuncType;
   EnableFuncType *EnableFunc;

   typedef ProvidedMethod<ReturnType> DisableFuncType;
   DisableFuncType *DisableFunc;

   typedef ProvidedMethod< vector<double> > GetSensorValueFuncType;
   GetSensorValueFuncType *GetSensorValueFunc;


public: // default method
   virtual ReturnType SetParameter(Property parameter)
   {
		opros_assert(SetParameterFunc != NULL);
		
            return (*SetParameterFunc)(parameter);
		
   }
   virtual Property GetParameter()
   {
		opros_assert(GetParameterFunc != NULL);
		
            return (*GetParameterFunc)();
		
   }
   virtual ReturnType GetError()
   {
		opros_assert(GetErrorFunc != NULL);
		
            return (*GetErrorFunc)();
		
   }
   virtual ReturnType Enable()
   {
		opros_assert(EnableFunc != NULL);
		
            return (*EnableFunc)();
		
   }
   virtual ReturnType Disable()
   {
		opros_assert(DisableFunc != NULL);
		
            return (*DisableFunc)();
		
   }
   virtual vector<double> GetSensorValue()
   {
		opros_assert(GetSensorValueFunc != NULL);
		
            return (*GetSensorValueFunc)();
		
   }


public:
    //
    // Constructor
    //
    ForceSensorServiceProvided(IForceSensorService *fn)
    {
        pcom = fn;

        SetParameterFunc = NULL;
        GetParameterFunc = NULL;
        GetErrorFunc = NULL;
        EnableFunc = NULL;
        DisableFunc = NULL;
        GetSensorValueFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IForceSensorService::SetParameter,pcom,"SetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IForceSensorService::GetParameter,pcom,"GetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IForceSensorService::GetError,pcom,"GetError");

        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IForceSensorService::Enable,pcom,"Enable");

        opros_assert(ptr_method != NULL);
        addMethod("Enable",ptr_method);
        EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IForceSensorService::Disable,pcom,"Disable");

        opros_assert(ptr_method != NULL);
        addMethod("Disable",ptr_method);
        DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IForceSensorService::GetSensorValue,pcom,"GetSensorValue");

        opros_assert(ptr_method != NULL);
        addMethod("GetSensorValue",ptr_method);
        GetSensorValueFunc = reinterpret_cast<GetSensorValueFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
