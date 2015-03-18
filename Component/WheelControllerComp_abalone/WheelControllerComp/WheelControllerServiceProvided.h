#ifndef _WheelControllerService_PROVIDED_PORT_H
#define _WheelControllerService_PROVIDED_PORT_H

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

#ifndef _IWheelControllerService_CLASS_DEF
#define _IWheelControllerService_CLASS_DEF
/*
 * IWheelControllerService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IWheelControllerService
{
public:
	virtual ReturnType SetParameter(Property parameter)=0;
	virtual Property GetParameter()=0;
	virtual ReturnType GetError()=0;
	virtual ReturnType Enable()=0;
	virtual ReturnType Disable()=0;
	virtual ReturnType SetPosition(ObjectLocation position)=0;
	virtual ObjectLocation GetPosition()=0;
	virtual vector<long> GetOdometery()=0;
	virtual ReturnType DriveWheel(double linearVelocity,double angularVelocity)=0;
	virtual ReturnType MoveWheel(double distance,double linearVelocity)=0;
	virtual ReturnType RotateWheel(double angle,double angularVelocity)=0;
	virtual ReturnType StopWheel()=0;
	virtual bool IsWheelRunning()=0;
};
#endif

/*
 * 
 * WheelControllerService : public ProvidedServicePort
 *
 */
class WheelControllerServiceProvided : public ProvidedServicePort, public IWheelControllerService
{
protected:
	IWheelControllerService *pcom;

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

	typedef ProvidedMethod<ReturnType> SetPositionFuncType;
	SetPositionFuncType *SetPositionFunc;

	typedef ProvidedMethod<ObjectLocation> GetPositionFuncType;
	GetPositionFuncType *GetPositionFunc;

	typedef ProvidedMethod<vector<long> > GetOdometeryFuncType;
	GetOdometeryFuncType *GetOdometeryFunc;

	typedef ProvidedMethod<ReturnType> DriveWheelFuncType;
	DriveWheelFuncType *DriveWheelFunc;

	typedef ProvidedMethod<ReturnType> MoveWheelFuncType;
	MoveWheelFuncType *MoveWheelFunc;

	typedef ProvidedMethod<ReturnType> RotateWheelFuncType;
	RotateWheelFuncType *RotateWheelFunc;

	typedef ProvidedMethod<ReturnType> StopWheelFuncType;
	StopWheelFuncType *StopWheelFunc;

	typedef ProvidedMethod<bool> IsWheelRunningFuncType;
	IsWheelRunningFuncType *IsWheelRunningFunc;

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

	virtual ReturnType SetPosition(ObjectLocation position)
	{
		opros_assert(SetPositionFunc != NULL);

		return (*SetPositionFunc)(position);
	}

	virtual ObjectLocation GetPosition()
	{
		opros_assert(GetPositionFunc != NULL);

		return (*GetPositionFunc)();
	}

	virtual vector<long> GetOdometery()
	{
		opros_assert(GetOdometeryFunc != NULL);

		return (*GetOdometeryFunc)();
	}

	virtual ReturnType DriveWheel(double linearVelocity,double angularVelocity)
	{
		opros_assert(DriveWheelFunc != NULL);

		return (*DriveWheelFunc)(linearVelocity,angularVelocity);
	}

	virtual ReturnType MoveWheel(double distance,double linearVelocity)
	{
		opros_assert(MoveWheelFunc != NULL);

		return (*MoveWheelFunc)(distance,linearVelocity);
	}
	
	virtual ReturnType RotateWheel(double angle,double angularVelocity)
	{
		opros_assert(RotateWheelFunc != NULL);

		return (*RotateWheelFunc)(angle,angularVelocity);
	}
	
	virtual ReturnType StopWheel()
	{
		opros_assert(StopWheelFunc != NULL);

		return (*StopWheelFunc)();
	}

	virtual bool IsWheelRunning()
	{
		opros_assert(IsWheelRunningFunc != NULL);

		return (*IsWheelRunningFunc)();
	}

public:
	//
	// Constructor
	//
	WheelControllerServiceProvided(IWheelControllerService *fn)
	{
		pcom = fn;

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

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeProvidedMethod(&IWheelControllerService::SetParameter,pcom,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::GetParameter,pcom,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::GetError,pcom,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::Enable,pcom,"Enable");
		opros_assert(ptr_method != NULL);
		addMethod("Enable",ptr_method);
		EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::Disable,pcom,"Disable");
		opros_assert(ptr_method != NULL);
		addMethod("Disable",ptr_method);
		DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::SetPosition,pcom,"SetPosition");
		opros_assert(ptr_method != NULL);
		addMethod("SetPosition",ptr_method);
		SetPositionFunc = reinterpret_cast<SetPositionFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::GetPosition,pcom,"GetPosition");
		opros_assert(ptr_method != NULL);
		addMethod("GetPosition",ptr_method);
		GetPositionFunc = reinterpret_cast<GetPositionFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::GetOdometery,pcom,"GetOdometery");
		opros_assert(ptr_method != NULL);
		addMethod("GetOdometery",ptr_method);
		GetOdometeryFunc = reinterpret_cast<GetOdometeryFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::DriveWheel,pcom,"DriveWheel");
		opros_assert(ptr_method != NULL);
		addMethod("DriveWheel",ptr_method);
		DriveWheelFunc = reinterpret_cast<DriveWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::MoveWheel,pcom,"MoveWheel");
		opros_assert(ptr_method != NULL);
		addMethod("MoveWheel",ptr_method);
		MoveWheelFunc = reinterpret_cast<MoveWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::RotateWheel,pcom,"RotateWheel");
		opros_assert(ptr_method != NULL);
		addMethod("RotateWheel",ptr_method);
		RotateWheelFunc = reinterpret_cast<RotateWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::StopWheel,pcom,"StopWheel");
		opros_assert(ptr_method != NULL);
		addMethod("StopWheel",ptr_method);
		StopWheelFunc = reinterpret_cast<StopWheelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IWheelControllerService::IsWheelRunning,pcom,"IsWheelRunning");
		opros_assert(ptr_method != NULL);
		addMethod("IsWheelRunning",ptr_method);
		IsWheelRunningFunc = reinterpret_cast<IsWheelRunningFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
