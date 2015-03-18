#ifndef _PathPlanningService_PROVIDED_PORT_H
#define _PathPlanningService_PROVIDED_PORT_H

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
 * IPathPlanningService
 *
 * The comonent inherits this class and implements methods. 
*/
class IPathPlanningService
{
public:
	virtual bool SetParameter(Property parameter)=0;
	virtual Property GetParameter()=0;
	virtual int GetError()=0;
	virtual vector<ObjectLocation> FindPath(ObjectLocation startPosition,ObjectLocation endPosition)=0;
};

/*
 * 
 * PathPlanningService : public ProvidedServicePort
 *
 */
class PathPlanningServiceProvided : public ProvidedServicePort, public IPathPlanningService
{
protected:
	IPathPlanningService *pcom;

	typedef ProvidedMethod<bool> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef ProvidedMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef ProvidedMethod<int> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef ProvidedMethod< vector<ObjectLocation> > FindPathFuncType;
	FindPathFuncType *FindPathFunc;

public: // default method
	virtual bool SetParameter(Property parameter)
	{
		assert(SetParameterFunc != NULL);
		return (*SetParameterFunc)(parameter);
	}

	virtual Property GetParameter()
	{
		assert(GetParameterFunc != NULL);
		return (*GetParameterFunc)();
	}

	virtual int GetError()
	{
		assert(GetErrorFunc != NULL);
		return (*GetErrorFunc)();
	}

	virtual vector<ObjectLocation> FindPath(ObjectLocation startPosition,ObjectLocation endPosition)
	{
		assert(FindPathFunc != NULL);
		return (*FindPathFunc)(startPosition,endPosition);
	}

public:
	//
	// Constructor
	//
	PathPlanningServiceProvided(IPathPlanningService *fn)
	{
		pcom = fn;

		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		FindPathFunc = NULL;

		setup();
	}

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeProvidedMethod(&IPathPlanningService::SetParameter,pcom,"SetParameter");
		assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IPathPlanningService::GetParameter,pcom,"GetParameter");
		assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IPathPlanningService::GetError,pcom,"GetError");
		assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IPathPlanningService::FindPath,pcom,"FindPath");
		assert(ptr_method != NULL);
		addMethod("FindPath",ptr_method);
		FindPathFunc = reinterpret_cast<FindPathFuncType *>(ptr_method);
		ptr_method = NULL;
	}
};
#endif
