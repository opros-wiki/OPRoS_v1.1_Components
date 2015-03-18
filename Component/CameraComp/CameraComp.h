
#ifndef _CameraComp_COMPONENT_H
#define _CameraComp_COMPONENT_H
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
#include <OprosLock.h>



#include "CameraServiceProvided.h"
#include "Camera.h"


class CameraComp: public Component
	,public ICameraService
{
protected:
	//data
	OutputDataPort< vector<unsigned char> > imageData;

	//	Shared(Dynamic Linked) Library's Handle
#if defined(WIN32)
	OprosApiHandle hOprosAPI;
#else
	void *hOprosAPI;
#endif

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	Camera *camera;	

	OprosLock _lock;
	Property parameter;
	std::vector<unsigned char> imageBuffer;

protected:
// service
// method for provider
	ICameraService *ptrCameraService;

public:
virtual ReturnType SetParameter(Property parameter);
virtual Property GetParameter();
virtual ReturnType GetError();
virtual vector<unsigned char> GetImage();

protected:
// data


//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	CameraComp();
	CameraComp(const std::string &compId);
	virtual ~CameraComp();
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


};

#endif

