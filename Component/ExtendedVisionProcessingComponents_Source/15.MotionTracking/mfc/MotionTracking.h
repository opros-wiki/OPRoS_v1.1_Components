
#include "MotionTrackingApp.h"
	
#ifndef _MotionTracking_COMPONENT_H
#define _MotionTracking_COMPONENT_H
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


#include "RawImage.h"
		


class MotionTracking: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	MotionTracking();
	MotionTracking(const std::string &compId);
	virtual ~MotionTracking();
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

