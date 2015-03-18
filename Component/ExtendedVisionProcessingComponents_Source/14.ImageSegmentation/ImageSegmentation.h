
#ifndef _ImageSegmentation_COMPONENT_H
#define _ImageSegmentation_COMPONENT_H
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

//OpenCV용 인클루드 파일
#include <cv.h>
#include <highgui.h>

#include "RawImage.h"
#include <vector>
#include "PositionDataType.h"
		


class ImageSegmentation: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;
	OutputDataPort< RawImage > ImageOut2;
	OutputDataPort< std::vector<PositionDataType> > PositionOut;
	InputDataPort< RawImage > ImageIn;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	ImageSegmentation();
	ImageSegmentation(const std::string &compId);
	virtual ~ImageSegmentation();
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

	//영역분할 함수
	void image_segmentation(int d_v);

};

#endif

