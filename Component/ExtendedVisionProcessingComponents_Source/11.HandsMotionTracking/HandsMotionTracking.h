
#ifndef _HandsMotionTracking_COMPONENT_H
#define _HandsMotionTracking_COMPONENT_H
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

typedef struct 
{
	BOOL	bVisitedFlag;
	CvPoint ptReturnPoint;
} Visited;


class HandsMotionTracking: public Component
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
	HandsMotionTracking();
	HandsMotionTracking(const std::string &compId);
	virtual ~HandsMotionTracking();
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

	// 레이블링 결과 얻기
	void	DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight, CvRect* rect);

	
	// _Labling 내부 사용 함수
	int		__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, 
		int *StartX, int *StartY, int *EndX, int *EndY,Visited* Point);
	int		__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel);

	void	_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber);
	void	_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber);


};

#endif

