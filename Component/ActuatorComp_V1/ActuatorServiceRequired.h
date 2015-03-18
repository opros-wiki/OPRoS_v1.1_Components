#ifndef _ActuatorService_REQUIRED_PORT_H
#define _ActuatorService_REQUIRED_PORT_H

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
 * ActuatorService : public RequiredServicePort
 *
 */
class ActuatorServiceRequired : public RequiredServicePort
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

public:
	//
	// Constructor
	//
	ActuatorServiceRequired()
	{
		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		EnableFunc = NULL;
		DisableFunc = NULL;
		SetPowerFunc = NULL;
		GetPowerFunc = NULL;

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
	
    // generated setup function
    virtual void setup()
    {
		Method *ptr_method;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::SetParameter,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::GetParameter,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::GetError,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::Enable,"Enable");
		opros_assert(ptr_method != NULL);
		addMethod("Enable",ptr_method);
		EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::Disable,"Disable");
		opros_assert(ptr_method != NULL);
		addMethod("Disable",ptr_method);
		DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::SetPower,"SetPower");
		opros_assert(ptr_method != NULL);
		addMethod("SetPower",ptr_method);
		SetPowerFunc = reinterpret_cast<SetPowerFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&ActuatorServiceRequired::GetPower,"GetPower");
		opros_assert(ptr_method != NULL);
		addMethod("GetPower",ptr_method);
		GetPowerFunc = reinterpret_cast<GetPowerFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
