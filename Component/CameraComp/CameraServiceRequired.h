

#ifndef _CameraService_REQUIRED_PORT_H
#define _CameraService_REQUIRED_PORT_H

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
 * CameraService : public RequiredServicePort
 *
 */
class CameraServiceRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<vector<unsigned char>> GetImageFuncType;
	GetImageFuncType *GetImageFunc;

public:
	//
	// Constructor
	//
	CameraServiceRequired()
	{
            SetParameterFunc = NULL;
            GetParameterFunc = NULL;
            GetErrorFunc = NULL;
            GetImageFunc = NULL;
            
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

	vector<unsigned char> GetImage()
	{
            opros_assert(GetImageFunc != NULL);
	
            return (*GetImageFunc)();
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&CameraServiceRequired::SetParameter,"SetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&CameraServiceRequired::GetParameter,"GetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&CameraServiceRequired::GetError,"GetError");
        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&CameraServiceRequired::GetImage,"GetImage");
        opros_assert(ptr_method != NULL);
        addMethod("GetImage",ptr_method);
        GetImageFunc = reinterpret_cast<GetImageFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
