#ifndef _InverseKinematicsService_PROVIDED_PORT_H
#define _InverseKinematicsService_PROVIDED_PORT_H

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

#ifndef _IInverseKinematicsService_CLASS_DEF
#define _IInverseKinematicsService_CLASS_DEF
/*
 * IInverseKinematicsService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IInverseKinematicsService
{
public:
	virtual ReturnType SetParameter(Property parameter)=0;
	virtual Property GetParameter()=0;
	virtual ReturnType GetError()=0;
	virtual ObjectPosition SolveForwardKinematics(vector<double> actualJointPosition)=0;
	virtual vector<double> SolveInverseKinematics(vector<double> actualJointPosition, ObjectPosition desiredPosition)=0;
};
#endif

/*
 * 
 * InverseKinematicsService : public ProvidedServicePort
 *
 */
class InverseKinematicsServiceProvided : public ProvidedServicePort, public IInverseKinematicsService
{
protected:
	IInverseKinematicsService *pcom;
	
	typedef ProvidedMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef ProvidedMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef ProvidedMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef ProvidedMethod<ObjectPosition> SolveForwardKinematicsFuncType;
	SolveForwardKinematicsFuncType *SolveForwardKinematicsFunc;

	typedef ProvidedMethod <vector<double> > SolveInverseKinematicsFuncType;
	SolveInverseKinematicsFuncType *SolveInverseKinematicsFunc;


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

	virtual ObjectPosition SolveForwardKinematics(vector<double> actualJointPosition)
	{
		opros_assert(SolveForwardKinematicsFunc != NULL);

		return (*SolveForwardKinematicsFunc)(actualJointPosition);
	}

	virtual vector<double> SolveInverseKinematics(vector<double> actualJointPosition,ObjectPosition desiredPosition)
	{
		opros_assert(SolveInverseKinematicsFunc != NULL);

		return (*SolveInverseKinematicsFunc)(actualJointPosition,desiredPosition);
	}

public:
	//
	// Constructor
	//
	InverseKinematicsServiceProvided(IInverseKinematicsService *fn)
	{
		pcom = fn;

		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		SolveForwardKinematicsFunc = NULL;
		SolveInverseKinematicsFunc = NULL;

		setup();
	}

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeProvidedMethod(&IInverseKinematicsService::SetParameter,pcom,"SetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IInverseKinematicsService::GetParameter,pcom,"GetParameter");
		opros_assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IInverseKinematicsService::GetError,pcom,"GetError");
		opros_assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IInverseKinematicsService::SolveForwardKinematics,pcom,"SolveForwardKinematics");
		opros_assert(ptr_method != NULL);
		addMethod("SolveForwardKinematics",ptr_method);
		SolveForwardKinematicsFunc = reinterpret_cast<SolveForwardKinematicsFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IInverseKinematicsService::SolveInverseKinematics,pcom,"SolveInverseKinematics");
		opros_assert(ptr_method != NULL);
		addMethod("SolveInverseKinematics",ptr_method);
		SolveInverseKinematicsFunc = reinterpret_cast<SolveInverseKinematicsFuncType *>(ptr_method);
		ptr_method = NULL;
    }
};
#endif
