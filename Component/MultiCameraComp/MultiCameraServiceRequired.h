

#ifndef _MultiCameraService_REQUIRED_PORT_H
#define _MultiCameraService_REQUIRED_PORT_H

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
 * MultiCameraService : public RequiredServicePort
 *
 */
class MultiCameraServiceRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<vector< vector<unsigned char> >> GetImageDataFuncType;
	GetImageDataFuncType *GetImageDataFunc;

public:
	//
	// Constructor
	//
	MultiCameraServiceRequired()
	{
            SetParameterFunc = NULL;
            GetParameterFunc = NULL;
            GetErrorFunc = NULL;
            GetImageDataFunc = NULL;
            
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

	vector< vector<unsigned char> > GetImageData()
	{
            opros_assert(GetImageDataFunc != NULL);
	
            return (*GetImageDataFunc)();
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&MultiCameraServiceRequired::SetParameter,"SetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&MultiCameraServiceRequired::GetParameter,"GetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&MultiCameraServiceRequired::GetError,"GetError");
        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&MultiCameraServiceRequired::GetImageData,"GetImageData");
        opros_assert(ptr_method != NULL);
        addMethod("GetImageData",ptr_method);
        GetImageDataFunc = reinterpret_cast<GetImageDataFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
