#ifndef _TrajectoryGenerationService_REQUIRED_PORT_H
#define _TrajectoryGenerationService_REQUIRED_PORT_H

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
 * TrajectoryGenerationService : public RequiredServicePort
 *
 */
class TrajectoryGenerationServiceRequired : public RequiredServicePort
{
protected:
	typedef RequiredMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<ReturnType> GenerateTrajectoryFuncType;
	GenerateTrajectoryFuncType *GenerateTrajectoryFunc;

	typedef RequiredMethod<vector < valarray<double> > > GetAccelerationFuncType;
	GetAccelerationFuncType *GetAccelerationFunc;

	typedef RequiredMethod<vector < valarray<double> > > GetVelocityFuncType;
	GetVelocityFuncType *GetVelocityFunc;

	typedef RequiredMethod<vector < valarray<double> > > GetPositionFuncType;
	GetPositionFuncType *GetPositionFunc;

public:
	//
	// Constructor
	//
	TrajectoryGenerationServiceRequired()
	{
		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		GenerateTrajectoryFunc = NULL;
		GetAccelerationFunc = NULL;
		GetVelocityFunc = NULL;
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

	ReturnType GenerateTrajectory(vector< valarray<double> > path,double deltaTime)
	{
		opros_assert(GenerateTrajectoryFunc != NULL);

		return (*GenerateTrajectoryFunc)(path,deltaTime);
	}

	vector < valarray<double> > GetAcceleration()
	{
		opros_assert(GetAccelerationFunc != NULL);

		return (*GetAccelerationFunc)();
	}

	vector < valarray<double> > GetVelocity()
	{
		opros_assert(GetVelocityFunc != NULL);

		return (*GetVelocityFunc)();
	}

	vector < valarray<double> > GetPosition()
	{
		opros_assert(GetPositionFunc != NULL);

		return (*GetPositionFunc)();
	}
	
	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::SetParameter,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::GetParameter,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::GetError,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::GenerateTrajectory,"GenerateTrajectory");
		opros_assert(ptr_method != NULL);
		addMethod("GenerateTrajectory",ptr_method);
		GenerateTrajectoryFunc = reinterpret_cast<GenerateTrajectoryFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::GetAcceleration,"GetAcceleration");
		opros_assert(ptr_method != NULL);
		addMethod("GetAcceleration",ptr_method);
		GetAccelerationFunc = reinterpret_cast<GetAccelerationFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::GetVelocity,"GetVelocity");
		opros_assert(ptr_method != NULL);
		addMethod("GetVelocity",ptr_method);
		GetVelocityFunc = reinterpret_cast<GetVelocityFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&TrajectoryGenerationServiceRequired::GetPosition,"GetPosition");
		opros_assert(ptr_method != NULL);
		addMethod("GetPosition",ptr_method);
		GetPositionFunc = reinterpret_cast<GetPositionFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
