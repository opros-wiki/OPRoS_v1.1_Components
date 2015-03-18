

#ifndef _KinectService_PROVIDED_PORT_H
#define _KinectService_PROVIDED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>


#include "device/Property.h"
#include "KinectDataType.h"		

#ifndef _IKinectService_CLASS_DEF
#define _IKinectService_CLASS_DEF
/*
 * IKinectService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IKinectService
{
 public:
	 virtual bool SetProperty(OPRoS::Property parameter) = 0;
	virtual OPRoS::Property GetProperty() = 0;
    virtual bool SetCameraAngle(float32_t degree)=0;
    virtual float32_t GetCameraAngle()=0;
    virtual std::vector<Skeleton> GetSkeletonAll()=0;
    virtual Skeleton GetSkeleton(uint32_t id)=0;
	virtual std::vector<uint32_t> GetSkeletonID() = 0;
    virtual ImageFrame GetImage()=0;
    virtual DepthFrame GetDepthImage()=0;
    virtual Position GetPixelDepth(int32_t x,int32_t y)=0;
 };
#endif

/*
 * 
 * KinectService : public ProvidedServicePort
 *
 */
class KinectServiceProvided
	:public ProvidedServicePort, public IKinectService
{
protected:
    IKinectService *pcom;


	typedef ProvidedMethod<bool> SetPropertyFuncType;
	SetPropertyFuncType *SetPropertyFunc;

   typedef ProvidedMethod<OPRoS::Property> GetPropertyFuncType;
   GetPropertyFuncType *GetPropertyFunc;

   typedef ProvidedMethod<bool> SetCameraAngleFuncType;
   SetCameraAngleFuncType *SetCameraAngleFunc;

   typedef ProvidedMethod<float32_t> GetCameraAngleFuncType;
   GetCameraAngleFuncType *GetCameraAngleFunc;

   typedef ProvidedMethod<std::vector<Skeleton>> GetSkeletonAllFuncType;
   GetSkeletonAllFuncType *GetSkeletonAllFunc;

   typedef ProvidedMethod<Skeleton> GetSkeletonFuncType;
   GetSkeletonFuncType *GetSkeletonFunc;

   typedef ProvidedMethod<std::vector<uint32_t>> GetSkeletonIDFuncType;
   GetSkeletonIDFuncType *GetSkeletonIDFunc;

   typedef ProvidedMethod<ImageFrame> GetImageFuncType;
   GetImageFuncType *GetImageFunc;

   typedef ProvidedMethod<DepthFrame> GetDepthImageFuncType;
   GetDepthImageFuncType *GetDepthImageFunc;

   typedef ProvidedMethod<Position> GetPixelDepthFuncType;
   GetPixelDepthFuncType *GetPixelDepthFunc;


public: // default method
	virtual bool SetProperty(OPRoS::Property parameter)
   {
		opros_assert(SetPropertyFunc != NULL);
		
            return (*SetPropertyFunc)(parameter);
		
   }
	virtual OPRoS::Property GetProperty()
   {
		opros_assert(GetPropertyFunc != NULL);
		
            return (*GetPropertyFunc)();
		
   }
   virtual bool SetCameraAngle(float32_t degree)
   {
		opros_assert(SetCameraAngleFunc != NULL);
		
            return (*SetCameraAngleFunc)(degree);
		
   }
   virtual float GetCameraAngle()
   {
		opros_assert(GetCameraAngleFunc != NULL);
		
            return (*GetCameraAngleFunc)();
		
   }
   virtual std::vector<Skeleton> GetSkeletonAll()
   {
		opros_assert(GetSkeletonAllFunc != NULL);
		
            return (*GetSkeletonAllFunc)();
		
   }
   virtual Skeleton GetSkeleton(uint32_t id)
   {
		opros_assert(GetSkeletonFunc != NULL);
		
            return (*GetSkeletonFunc)(id);
		
   }
   virtual std::vector<uint32_t> GetSkeletonID()
   {
		opros_assert(GetSkeletonIDFunc != NULL);
		
            return (*GetSkeletonIDFunc)();
		
   }
   virtual ImageFrame GetImage()
   {
		opros_assert(GetImageFunc != NULL);
		
            return (*GetImageFunc)();
		
   }
   virtual DepthFrame GetDepthImage()
   {
		opros_assert(GetDepthImageFunc != NULL);
		
            return (*GetDepthImageFunc)();
		
   }
   virtual Position GetPixelDepth(int32_t x,int32_t y)
   {
		opros_assert(GetPixelDepthFunc != NULL);
		
            return (*GetPixelDepthFunc)(x,y);
		
   }


public:
    //
    // Constructor
    //
    KinectServiceProvided(IKinectService *fn)
    {
        pcom = fn;

        SetPropertyFunc = NULL;
        GetPropertyFunc = NULL;
        SetCameraAngleFunc = NULL;
        GetCameraAngleFunc = NULL;
        GetSkeletonAllFunc = NULL;
        GetSkeletonFunc = NULL;
        GetSkeletonIDFunc = NULL;
        GetImageFunc = NULL;
        GetDepthImageFunc = NULL;
        GetPixelDepthFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IKinectService::SetProperty,pcom,"SetProperty");

        opros_assert(ptr_method != NULL);
        addMethod("SetProperty",ptr_method);
        SetPropertyFunc = reinterpret_cast<SetPropertyFuncType *>(ptr_method);
        ptr_method = NULL;

    
		ptr_method = makeProvidedMethod(&IKinectService::GetProperty, pcom, "GetProperty");

        opros_assert(ptr_method != NULL);
        addMethod("GetProperty",ptr_method);
        GetPropertyFunc = reinterpret_cast<GetPropertyFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::SetCameraAngle,pcom,"SetCameraAngle");

        opros_assert(ptr_method != NULL);
        addMethod("SetCameraAngle",ptr_method);
        SetCameraAngleFunc = reinterpret_cast<SetCameraAngleFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetCameraAngle,pcom,"GetCameraAngle");

        opros_assert(ptr_method != NULL);
        addMethod("GetCameraAngle",ptr_method);
        GetCameraAngleFunc = reinterpret_cast<GetCameraAngleFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetSkeletonAll,pcom,"GetSkeletonAll");

        opros_assert(ptr_method != NULL);
        addMethod("GetSkeletonAll",ptr_method);
        GetSkeletonAllFunc = reinterpret_cast<GetSkeletonAllFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetSkeleton,pcom,"GetSkeleton");

        opros_assert(ptr_method != NULL);
        addMethod("GetSkeleton",ptr_method);
        GetSkeletonFunc = reinterpret_cast<GetSkeletonFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetSkeletonID,pcom,"GetSkeletonID");

        opros_assert(ptr_method != NULL);
        addMethod("GetSkeletonID",ptr_method);
        GetSkeletonIDFunc = reinterpret_cast<GetSkeletonIDFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetImage,pcom,"GetImage");

        opros_assert(ptr_method != NULL);
        addMethod("GetImage",ptr_method);
        GetImageFunc = reinterpret_cast<GetImageFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetDepthImage,pcom,"GetDepthImage");

        opros_assert(ptr_method != NULL);
        addMethod("GetDepthImage",ptr_method);
        GetDepthImageFunc = reinterpret_cast<GetDepthImageFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IKinectService::GetPixelDepth,pcom,"GetPixelDepth");

        opros_assert(ptr_method != NULL);
        addMethod("GetPixelDepth",ptr_method);
        GetPixelDepthFunc = reinterpret_cast<GetPixelDepthFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
