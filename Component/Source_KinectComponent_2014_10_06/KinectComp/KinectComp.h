
#ifndef _KinectComp_COMPONENT_H
#define _KinectComp_COMPONENT_H
/*
*  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
*   
*/
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>
#include <device/OprosLock.h>
#include <device/OprosDevice.h>

#include "Kinect.h"

#include "KinectServiceProvided.h"

class KinectComp: public Component
	,public IKinectService
{
protected:
	// event
	OutputEventPort< KinectEvent > mKinectOutputEvent;

protected:
	// service
	// method for provider
	IKinectService *ptrKinectService;

public:
	KinectComp();
	KinectComp(const std::string &compId);
	virtual ~KinectComp();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();
	
public:
	virtual bool SetProperty(OPRoS::Property parameter);
	virtual OPRoS::Property GetProperty();
	virtual bool SetCameraAngle(float32_t degree);
	virtual float32_t GetCameraAngle();
	virtual std::vector<Skeleton> GetSkeletonAll();
	virtual Skeleton GetSkeleton(uint32_t id);
	virtual std::vector<uint32_t> GetSkeletonID();
	virtual ImageFrame GetImage();
	virtual DepthFrame GetDepthImage();
	virtual Position GetPixelDepth(int32_t x,int32_t y);

private:
	ReturnType SendEvent(const KinectEvent& event); 

private:
	Kinect* mpKinect;
	OprosDeviceHandle mhOprosAPI;
	
	std::vector<uint32_t> mSaveID;
};

#endif

