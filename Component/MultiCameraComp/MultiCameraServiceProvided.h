

#ifndef _MultiCameraService_PROVIDED_PORT_H
#define _MultiCameraService_PROVIDED_PORT_H

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
		

#ifndef _IMultiCameraService_CLASS_DEF
#define _IMultiCameraService_CLASS_DEF
/*
 * IMultiCameraService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IMultiCameraService
{
 public:
    virtual ReturnType SetParameter(Property parameter)=0;
    virtual Property GetParameter()=0;
    virtual ReturnType GetError()=0;
    virtual vector< vector<unsigned char> > GetImageData()=0;
 };
#endif

/*
 * 
 * MultiCameraService : public ProvidedServicePort
 *
 */
class MultiCameraServiceProvided
	:public ProvidedServicePort, public IMultiCameraService
{
protected:
    IMultiCameraService *pcom;


   typedef ProvidedMethod<ReturnType> SetParameterFuncType;
   SetParameterFuncType *SetParameterFunc;

   typedef ProvidedMethod<Property> GetParameterFuncType;
   GetParameterFuncType *GetParameterFunc;

   typedef ProvidedMethod<ReturnType> GetErrorFuncType;
   GetErrorFuncType *GetErrorFunc;

   typedef ProvidedMethod< vector< vector<unsigned char> > > GetImageDataFuncType;
   GetImageDataFuncType *GetImageDataFunc;


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
   virtual vector< vector<unsigned char> > GetImageData()
   {
		opros_assert(GetImageDataFunc != NULL);
		
            return (*GetImageDataFunc)();
		
   }


public:
    //
    // Constructor
    //
    MultiCameraServiceProvided(IMultiCameraService *fn)
    {
        pcom = fn;

        SetParameterFunc = NULL;
        GetParameterFunc = NULL;
        GetErrorFunc = NULL;
        GetImageDataFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IMultiCameraService::SetParameter,pcom,"SetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IMultiCameraService::GetParameter,pcom,"GetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IMultiCameraService::GetError,pcom,"GetError");

        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IMultiCameraService::GetImageData,pcom,"GetImageData");

        opros_assert(ptr_method != NULL);
        addMethod("GetImageData",ptr_method);
        GetImageDataFunc = reinterpret_cast<GetImageDataFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
