
#ifndef _ColorTracking_COMPONENT_H
#define _ColorTracking_COMPONENT_H
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
		


class ColorTracking: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;
	OutputDataPort< std::vector<PositionDataType> > PositionOut;
	InputDataPort< RawImage > ImageIn;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	ColorTracking();
	ColorTracking(const std::string &compId);
	virtual ~ColorTracking();
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


	void color_config(IplImage* image, std::string config);
	void image_filter(IplImage* image);
	void draw_circle(IplImage* image);
	void draw_square(IplImage* image);

};

#endif

