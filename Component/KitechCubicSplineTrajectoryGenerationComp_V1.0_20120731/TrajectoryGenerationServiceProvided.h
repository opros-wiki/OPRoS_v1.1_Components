#ifndef _TrajectoryGenerationService_PROVIDED_PORT_H
#define _TrajectoryGenerationService_PROVIDED_PORT_H

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

#ifndef _ITrajectoryGenerationService_CLASS_DEF
#define _ITrajectoryGenerationService_CLASS_DEF
/*
 * ITrajectoryGenerationService
 *
 *  The comonent inherits this class and implements methods. 
 */
class ITrajectoryGenerationService
{
public:
	virtual ReturnType SetParameter(Property parameter)=0;
	virtual Property GetParameter()=0;
	virtual ReturnType GetError()=0;
	virtual ReturnType GenerateTrajectory(vector< valarray<double> > path,double deltaTime)=0;
	virtual vector < valarray<double> > GetAcceleration()=0;
	virtual vector < valarray<double> > GetVelocity()=0;
	virtual vector < valarray<double> > GetPosition()=0;
};
#endif

/*
 * 
 * TrajectoryGenerationService : public ProvidedServicePort
 *
 */
class TrajectoryGenerationServiceProvided : public ProvidedServicePort, public ITrajectoryGenerationService
{
protected:
	ITrajectoryGenerationService *pcom;
	
	typedef ProvidedMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef ProvidedMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef ProvidedMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef ProvidedMethod<ReturnType> GenerateTrajectoryFuncType;
	GenerateTrajectoryFuncType *GenerateTrajectoryFunc;

	typedef ProvidedMethod<vector < valarray<double> > > GetAccelerationFuncType;
	GetAccelerationFuncType *GetAccelerationFunc;

	typedef ProvidedMethod<vector < valarray<double> > > GetVelocityFuncType;
	GetVelocityFuncType *GetVelocityFunc;

	typedef ProvidedMethod<vector < valarray<double> > > GetPositionFuncType;
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

	virtual ReturnType GenerateTrajectory(vector< valarray<double> > path,double deltaTime)
	{
		opros_assert(GenerateTrajectoryFunc != NULL);

		return (*GenerateTrajectoryFunc)(path,deltaTime);
	}

	virtual vector < valarray<double> > GetAcceleration()
	{
		opros_assert(GetAccelerationFunc != NULL);

		return (*GetAccelerationFunc)();
	}

	virtual vector < valarray<double> > GetVelocity()
	{
		opros_assert(GetVelocityFunc != NULL);

		return (*GetVelocityFunc)();
	}

	virtual vector < valarray<double> > GetPosition()
	{
		opros_assert(GetPositionFunc != NULL);

		return (*GetPositionFunc)();
	}

public:
    //
    // Constructor
    //
	TrajectoryGenerationServiceProvided(ITrajectoryGenerationService *fn)
	{
		pcom = fn;

		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		GenerateTrajectoryFunc = NULL;
		GetAccelerationFunc = NULL;
		GetVelocityFunc = NULL;
		GetPositionFunc = NULL;

		setup();
	}

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::SetParameter,pcom,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::GetParameter,pcom,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::GetError,pcom,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::GenerateTrajectory,pcom,"GenerateTrajectory");
		opros_assert(ptr_method != NULL);
		addMethod("GenerateTrajectory",ptr_method);
		GenerateTrajectoryFunc = reinterpret_cast<GenerateTrajectoryFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::GetAcceleration,pcom,"GetAcceleration");
		opros_assert(ptr_method != NULL);
		addMethod("GetAcceleration",ptr_method);
		GetAccelerationFunc = reinterpret_cast<GetAccelerationFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::GetVelocity,pcom,"GetVelocity");
		opros_assert(ptr_method != NULL);
		addMethod("GetVelocity",ptr_method);
		GetVelocityFunc = reinterpret_cast<GetVelocityFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&ITrajectoryGenerationService::GetPosition,pcom,"GetPosition");
		opros_assert(ptr_method != NULL);
		addMethod("GetPosition",ptr_method);
		GetPositionFunc = reinterpret_cast<GetPositionFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
