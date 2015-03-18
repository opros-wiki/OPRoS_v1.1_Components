#ifndef _InverseKinematicsService_REQUIRED_PORT_H
#define _InverseKinematicsService_REQUIRED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>

#include "ApiTypes.h"
#include "Property.h"

/*
 * 
 * InverseKinematicsService : public RequiredServicePort
 *
 */
class InverseKinematicsServiceRequired : public RequiredServicePort
{
protected:
	typedef RequiredMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<ObjectPosition> SolveForwardKinematicsFuncType;
	SolveForwardKinematicsFuncType *SolveForwardKinematicsFunc;

	typedef RequiredMethod< vector<double> > SolveInverseKinematicsFuncType;
	SolveInverseKinematicsFuncType *SolveInverseKinematicsFunc;

public:
	//
	// Constructor
	//
	InverseKinematicsServiceRequired()
	{
		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		SolveForwardKinematicsFunc = NULL;
		SolveInverseKinematicsFunc = NULL;

		setup();
	}

	// method implementation for required method
	ReturnType SetParameter(Property parameter)
	{
		opros_assert(SetParameterFunc != NULL);

		return (*SetParameterFunc)();
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

	ObjectPosition SolveForwardKinematics(vector<double> actualJointPosition)
	{
		opros_assert(SolveForwardKinematicsFunc != NULL);

		return (*SolveForwardKinematicsFunc)(actualJointPosition);
	}

	vector<double> SolveInverseKinematics(vector<double> actualJointPosition,ObjectPosition desiredPosition)
	{
		opros_assert(SolveInverseKinematicsFunc != NULL);

		return (*SolveInverseKinematicsFunc)(actualJointPosition,desiredPosition);
	}
	
    // generated setup function
    virtual void setup()
    {
		Method *ptr_method;

		ptr_method = makeRequiredMethod(&InverseKinematicsServiceRequired::SetParameter,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&InverseKinematicsServiceRequired::GetParameter,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&InverseKinematicsServiceRequired::GetError,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&InverseKinematicsServiceRequired::SolveForwardKinematics,"SolveForwardKinematics");
		opros_assert(ptr_method != NULL);
		addMethod("SolveForwardKinematics",ptr_method);
		SolveForwardKinematicsFunc = reinterpret_cast<SolveForwardKinematicsFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&InverseKinematicsServiceRequired::SolveInverseKinematics,"SolveInverseKinematics");
		opros_assert(ptr_method != NULL);
		addMethod("SolveInverseKinematics",ptr_method);
		SolveInverseKinematicsFunc = reinterpret_cast<SolveInverseKinematicsFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
