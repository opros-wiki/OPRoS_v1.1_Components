#ifndef _ServoActuatorService_REQUIRED_PORT_H
#define _ServoActuatorService_REQUIRED_PORT_H

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

/*
 * 
 * ServoActuatorService : public RequiredServicePort
 *
 */
class ServoActuatorServiceRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<ReturnType> EnableFuncType;
	EnableFuncType *EnableFunc;

	typedef RequiredMethod<ReturnType> DisableFuncType;
	DisableFuncType *DisableFunc;

	typedef RequiredMethod<ReturnType> SetPowerFuncType;
	SetPowerFuncType *SetPowerFunc;

	typedef RequiredMethod<double> GetPowerFuncType;
	GetPowerFuncType *GetPowerFunc;

	typedef RequiredMethod<ReturnType> SetVelocityFuncType;
	SetVelocityFuncType *SetVelocityFunc;

	typedef RequiredMethod<double> GetVelocityFuncType;
	GetVelocityFuncType *GetVelocityFunc;

	typedef RequiredMethod<ReturnType> SetPositionFuncType;
	SetPositionFuncType *SetPositionFunc;

	typedef RequiredMethod<double> GetPositionFuncType;
	GetPositionFuncType *GetPositionFunc;

public:
	//
	// Constructor
	//
	ServoActuatorServiceRequired()
	{
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

	// method implementation for required method
	ReturnType SetParameter(Property parameter)
	{
            opros_assert(SetParameterFunc != NULL);
	
            return (*SetParameterFunc)(parameter);
		
	}

	Property GetParameter()
	{
            opros_assert(GetParameterFunc != NULL);
	
            return (*GetParameterFunc)();
		
	}

	ReturnType GetError()
	{
            opros_assert(GetErrorFunc != NULL);
	
            return (*GetErrorFunc)();
		
	}

	ReturnType Enable()
	{
            opros_assert(EnableFunc != NULL);
	
            return (*EnableFunc)();
		
	}

	ReturnType Disable()
	{
            opros_assert(DisableFunc != NULL);
	
            return (*DisableFunc)();
		
	}

	ReturnType SetPower(double power,unsigned long time)
	{
            opros_assert(SetPowerFunc != NULL);
	
            return (*SetPowerFunc)(power,time);
		
	}

	double GetPower()
	{
            opros_assert(GetPowerFunc != NULL);
	
            return (*GetPowerFunc)();
		
	}

	ReturnType SetVelocity(double velocity,unsigned long time)
	{
            opros_assert(SetVelocityFunc != NULL);
	
            return (*SetVelocityFunc)(velocity,time);
		
	}

	double GetVelocity()
	{
            opros_assert(GetVelocityFunc != NULL);
	
            return (*GetVelocityFunc)();
		
	}

	ReturnType SetPosition(double position,unsigned long time)
	{
            opros_assert(SetPositionFunc != NULL);
	
            return (*SetPositionFunc)(position,time);
		
	}

	double GetPosition()
	{
            opros_assert(GetPositionFunc != NULL);
	
            return (*GetPositionFunc)();
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::SetParameter,"SetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::GetParameter,"GetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::GetError,"GetError");
        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::Enable,"Enable");
        opros_assert(ptr_method != NULL);
        addMethod("Enable",ptr_method);
        EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::Disable,"Disable");
        opros_assert(ptr_method != NULL);
        addMethod("Disable",ptr_method);
        DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::SetPower,"SetPower");
        opros_assert(ptr_method != NULL);
        addMethod("SetPower",ptr_method);
        SetPowerFunc = reinterpret_cast<SetPowerFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::GetPower,"GetPower");
        opros_assert(ptr_method != NULL);
        addMethod("GetPower",ptr_method);
        GetPowerFunc = reinterpret_cast<GetPowerFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::SetVelocity,"SetVelocity");
        opros_assert(ptr_method != NULL);
        addMethod("SetVelocity",ptr_method);
        SetVelocityFunc = reinterpret_cast<SetVelocityFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::GetVelocity,"GetVelocity");
        opros_assert(ptr_method != NULL);
        addMethod("GetVelocity",ptr_method);
        GetVelocityFunc = reinterpret_cast<GetVelocityFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::SetPosition,"SetPosition");
        opros_assert(ptr_method != NULL);
        addMethod("SetPosition",ptr_method);
        SetPositionFunc = reinterpret_cast<SetPositionFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&ServoActuatorServiceRequired::GetPosition,"GetPosition");
        opros_assert(ptr_method != NULL);
        addMethod("GetPosition",ptr_method);
        GetPositionFunc = reinterpret_cast<GetPositionFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
