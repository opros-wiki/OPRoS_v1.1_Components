
#ifndef _FaceDetectionHaarlike_COMPONENT_H
#define _FaceDetectionHaarlike_COMPONENT_H
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

#include <cv.h>
#include <highgui.h>

#include "RawImage.h"
#include "PositionDataType.h"
		


class FaceDetectionHaarlike: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;
	OutputDataPort< std::vector<PositionDataType> > PositionDataOut;
	InputDataPort< RawImage > ImageIn;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	FaceDetectionHaarlike();
	FaceDetectionHaarlike(const std::string &compId);
	virtual ~FaceDetectionHaarlike();
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
	void detectFace();

};

#endif

