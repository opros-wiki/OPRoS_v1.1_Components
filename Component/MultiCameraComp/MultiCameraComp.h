
#ifndef _MultiCameraComp_COMPONENT_H
#define _MultiCameraComp_COMPONENT_H
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




#include "MultiCameraServiceProvided.h"
#include "MultiCamera.h"

class MultiCameraComp: public Component
	,public IMultiCameraService
{
protected:
//data
	OutputDataPort< vector< vector<unsigned char> > > multiImageData;

	//	Shared(Dynamic Linked) Library's Handle
#if defined(WIN32)
	OprosApiHandle hOprosAPI;
#else
	void *hOprosAPI;
#endif

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	MultiCamera *multiCamera;	

	Property parameter;
	vector< vector<unsigned char> > imageBuffer;

protected:
// service
// method for provider
	IMultiCameraService *ptrMultiCameraService;

public:
virtual ReturnType SetParameter(Property parameter);
virtual Property GetParameter();
virtual ReturnType GetError();
virtual vector< vector<unsigned char> > GetImageData();

protected:
// data


//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	MultiCameraComp();
	MultiCameraComp(const std::string &compId);
	virtual ~MultiCameraComp();
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

