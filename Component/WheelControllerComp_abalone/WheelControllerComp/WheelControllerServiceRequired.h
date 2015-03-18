#ifndef _WheelControllerService_REQUIRED_PORT_H
#define _WheelControllerService_REQUIRED_PORT_H

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
#include "ApiTypes.h"

/*
 * 
 * WheelControllerService : public RequiredServicePort
 *
 */
class WheelControllerServiceRequired : public RequiredServicePort
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

	typedef RequiredMethod<ReturnType> SetPositionFuncType;
	SetPositionFuncType *SetPositionFunc;

	typedef RequiredMethod<ObjectLocation> GetPositionFuncType;
	GetPositionFuncType *GetPositionFunc;

	typedef RequiredMethod<vector<long>> GetOdometeryFuncType;
	GetOdometeryFuncType *GetOdometeryFunc;

	typedef RequiredMethod<ReturnType> DriveWheelFuncType;
	DriveWheelFuncType *DriveWheelFunc;

	typedef RequiredMethod<ReturnType> MoveWheelFuncType;
	MoveWheelFuncType *MoveWheelFunc;

	typedef RequiredMethod<ReturnType> RotateWheelFuncType;
	RotateWheelFuncType *RotateWheelFunc;

	typedef RequiredMethod<ReturnType> StopWheelFuncType;
	StopWheelFuncType *StopWheelFunc;

	typedef RequiredMethod<bool> IsWheelRunningFuncType;
	IsWheelRunningFuncType *IsWheelRunningFunc;

public:
	//
	// Constructor
	//
	WheelControllerServiceRequired()
	{
		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		EnableFunc = NULL;
		DisableFunc = NULL;
		SetPositionFunc = NULL;
		GetPositionFunc = NULL;
		GetOdometeryFunc = NULL;
		DriveWheelFunc = NULL;
		MoveWheelFunc = NULL;
		RotateWheelFunc = NULL;
		StopWheelFunc = NULL;
		IsWheelRunningFunc = NULL;

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

	ReturnType SetPosition(ObjectLocation position)
	{
		opros_assert(SetPositionFunc != NULL);
	
		return (*SetPositionFunc)(position);
	}

	ObjectLocation GetPosition()
	{
		opros_assert(GetPositionFunc != NULL);

		return (*GetPositionFunc)();
	}

	vector<long> GetOdometery()
	{
		opros_assert(GetOdometeryFunc != NULL);

		return (*GetOdometeryFunc)();
	}

	ReturnType DriveWheel(double linearVelocity,double angularVelocity)
	{
		opros_assert(DriveWheelFunc != NULL);

		return (*DriveWheelFunc)(linearVelocity,angularVelocity);
	}

	ReturnType MoveWheel(double distance,double linearVelocity)
	{
		opros_assert(MoveWheelFunc != NULL);

		return (*MoveWheelFunc)(distance,linearVelocity);
	}

	ReturnType RotateWheel(double angle,double angularVelocity)
	{
		opros_assert(RotateWheelFunc != NULL);

		return (*RotateWheelFunc)(angle,angularVelocity);
	}

	ReturnType StopWheel()
	{
		opros_assert(StopWheelFunc != NULL);

		return (*StopWheelFunc)();
	}

	bool IsWheelRunning()
	{
		opros_assert(IsWheelRunningFunc != NULL);

		return (*IsWheelRunningFunc)();
	}

    // generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::SetParameter,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::GetParameter,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::GetError,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::Enable,"Enable");
		opros_assert(ptr_method != NULL);
		addMethod("Enable",ptr_method);
		EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::Disable,"Disable");
		opros_assert(ptr_method != NULL);
		addMethod("Disable",ptr_method);
		DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::SetPosition,"SetPosition");
		opros_assert(ptr_method != NULL);
		addMethod("SetPosition",ptr_method);
		SetPositionFunc = reinterpret_cast<SetPositionFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::GetPosition,"GetPosition");
		opros_assert(ptr_method != NULL);
		addMethod("GetPosition",ptr_method);
		GetPositionFunc = reinterpret_cast<GetPositionFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::GetOdometery,"GetOdometery");
		opros_assert(ptr_method != NULL);
		addMethod("GetOdometery",ptr_method);
		GetOdometeryFunc = reinterpret_cast<GetOdometeryFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::DriveWheel,"DriveWheel");
		opros_assert(ptr_method != NULL);
		addMethod("DriveWheel",ptr_method);
		DriveWheelFunc = reinterpret_cast<DriveWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::MoveWheel,"MoveWheel");
		opros_assert(ptr_method != NULL);
		addMethod("MoveWheel",ptr_method);
		MoveWheelFunc = reinterpret_cast<MoveWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::RotateWheel,"RotateWheel");
		opros_assert(ptr_method != NULL);
		addMethod("RotateWheel",ptr_method);
		RotateWheelFunc = reinterpret_cast<RotateWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::StopWheel,"StopWheel");
		opros_assert(ptr_method != NULL);
		addMethod("StopWheel",ptr_method);
		StopWheelFunc = reinterpret_cast<StopWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&WheelControllerServiceRequired::IsWheelRunning,"IsWheelRunning");
		opros_assert(ptr_method != NULL);
		addMethod("IsWheelRunning",ptr_method);
		IsWheelRunningFunc = reinterpret_cast<IsWheelRunningFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
