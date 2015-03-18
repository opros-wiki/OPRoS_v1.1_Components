
#ifndef _FaceDetectionASM_COMPONENT_H
#define _FaceDetectionASM_COMPONENT_H
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
#include <cxcore.h>
#include <cvaux.h>

#include "RawImage.h"
#include "PositionDataType.h"

#include "asmfitting.h"

class FaceDetectionASM: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;
	InputDataPort< RawImage > ImageIn;
	OutputDataPort< std::vector<PositionDataType> > PositionDataOut;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	FaceDetectionASM();
	FaceDetectionASM(const std::string &compId);
	virtual ~FaceDetectionASM();
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

	bool DetectOneFace(asm_shape& Shape, const IplImage* image);
	bool DetectAllFace(asm_shape** Shape, int& n_shapes, const IplImage* image);
	bool InitDetectCascade(const char* cascade_name);


};

#endif

