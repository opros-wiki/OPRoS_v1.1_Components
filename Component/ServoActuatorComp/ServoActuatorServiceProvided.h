#ifndef _ServoActuatorService_PROVIDED_PORT_H
#define _ServoActuatorService_PROVIDED_PORT_H

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

#ifndef _IServoActuatorService_CLASS_DEF
#define _IServoActuatorService_CLASS_DEF
/*
 * IServoActuatorService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IServoActuatorService
{
 public:
    virtual ReturnType SetParameter(Property parameter)=0;
    virtual Property GetParameter()=0;
    virtual ReturnType GetError()=0;
    virtual ReturnType Enable()=0;
    virtual ReturnType Disable()=0;
    virtual ReturnType SetPower(double power,unsigned long time)=0;
    virtual double GetPower()=0;
    virtual ReturnType SetVelocity(double velocity,unsigned long time)=0;
    virtual double GetVelocity()=0;
    virtual ReturnType SetPosition(double position,unsigned long time)=0;
    virtual double GetPosition()=0;
 };
#endif

/*
 * 
 * ServoActuatorService : public ProvidedServicePort
 *
 */
class ServoActuatorServiceProvided
	:public ProvidedServicePort, public IServoActuatorService
{
protected:
    IServoActuatorService *pcom;


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

   typedef ProvidedMethod<ReturnType> SetPowerFuncType;
   SetPowerFuncType *SetPowerFunc;

   typedef ProvidedMethod<double> GetPowerFuncType;
   GetPowerFuncType *GetPowerFunc;

   typedef ProvidedMethod<ReturnType> SetVelocityFuncType;
   SetVelocityFuncType *SetVelocityFunc;

   typedef ProvidedMethod<double> GetVelocityFuncType;
   GetVelocityFuncType *GetVelocityFunc;

   typedef ProvidedMethod<ReturnType> SetPositionFuncType;
   SetPositionFuncType *SetPositionFunc;

   typedef ProvidedMethod<double> GetPositionFuncType;
   GetPositionFuncType *GetPositionFunc;


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
   virtual ReturnType SetPower(double power,unsigned long time)
   {
		opros_assert(SetPowerFunc != NULL);
		
            return (*SetPowerFunc)(power,time);
		
   }
   virtual double GetPower()
   {
		opros_assert(GetPowerFunc != NULL);
		
            return (*GetPowerFunc)();
		
   }
   virtual ReturnType SetVelocity(double velocity,unsigned long time)
   {
		opros_assert(SetVelocityFunc != NULL);
		
            return (*SetVelocityFunc)(velocity,time);
		
   }
   virtual double GetVelocity()
   {
		opros_assert(GetVelocityFunc != NULL);
		
            return (*GetVelocityFunc)();
		
   }
   virtual ReturnType SetPosition(double position,unsigned long time)
   {
		opros_assert(SetPositionFunc != NULL);
		
            return (*SetPositionFunc)(position,time);
		
   }
   virtual double GetPosition()
   {
		opros_assert(GetPositionFunc != NULL);
		
            return (*GetPositionFunc)();
		
   }


public:
    //
    // Constructor
    //
    ServoActuatorServiceProvided(IServoActuatorService *fn)
    {
        pcom = fn;

        SetParameterFunc = NULL;
        GetParameterFunc = NULL;
        GetErrorFunc = NULL;
        EnableFunc = NULL;
        DisableFunc = NULL;
        SetPowerFunc = NULL;
        GetPowerFunc = NULL;
        SetVelocityFunc = NULL;
        GetVelocityFunc = NULL;
        SetPositionFunc = NULL;
        GetPositionFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IServoActuatorService::SetParameter,pcom,"SetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::GetParameter,pcom,"GetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::GetError,pcom,"GetError");

        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::Enable,pcom,"Enable");

        opros_assert(ptr_method != NULL);
        addMethod("Enable",ptr_method);
        EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::Disable,pcom,"Disable");

        opros_assert(ptr_method != NULL);
        addMethod("Disable",ptr_method);
        DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::SetPower,pcom,"SetPower");

        opros_assert(ptr_method != NULL);
        addMethod("SetPower",ptr_method);
        SetPowerFunc = reinterpret_cast<SetPowerFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::GetPower,pcom,"GetPower");

        opros_assert(ptr_method != NULL);
        addMethod("GetPower",ptr_method);
        GetPowerFunc = reinterpret_cast<GetPowerFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::SetVelocity,pcom,"SetVelocity");

        opros_assert(ptr_method != NULL);
        addMethod("SetVelocity",ptr_method);
        SetVelocityFunc = reinterpret_cast<SetVelocityFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::GetVelocity,pcom,"GetVelocity");

        opros_assert(ptr_method != NULL);
        addMethod("GetVelocity",ptr_method);
        GetVelocityFunc = reinterpret_cast<GetVelocityFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::SetPosition,pcom,"SetPosition");

        opros_assert(ptr_method != NULL);
        addMethod("SetPosition",ptr_method);
        SetPositionFunc = reinterpret_cast<SetPositionFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IServoActuatorService::GetPosition,pcom,"GetPosition");

        opros_assert(ptr_method != NULL);
        addMethod("GetPosition",ptr_method);
        GetPositionFunc = reinterpret_cast<GetPositionFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
