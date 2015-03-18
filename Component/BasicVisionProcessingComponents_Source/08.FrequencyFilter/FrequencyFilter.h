
#ifndef _FrequencyFilter_COMPONENT_H
#define _FrequencyFilter_COMPONENT_H
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
		
class FrequencyFilter: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;
	InputDataPort< RawImage > ImageIn;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	FrequencyFilter();
	FrequencyFilter(const std::string &compId);
	virtual ~FrequencyFilter();
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

	// 주파수 변환을 위한 중심각의 변경
	void ChangePosition(IplImage *pImage);

	// 이상적 저주파 통과 필터링
	void IdealLowPassFiltering(IplImage *pDFT, double D0);

	// 이상적 고주파 통과 필터링
	void IdealHighPassFiltering(IplImage *pDFT, double D0);

	// 버터워스 저주파 통과 필터링
	void ButterworthLowPassFiltering(IplImage *pDFT, double D0, int nCh);

	// 버터워스 고주파 통과 필터링
	void ButterworthHighPassFiltering(IplImage *pDFT, double D0, int nCh);

	// 가우시안 저주파 통과 필터링
	void GaussianLowPassFiltering(IplImage *pDFT, double D0);

	// 가우시안 고주파 통과 필터링
	void GaussianHighpassFiltering(IplImage *pDFT, double D0);

	// 2의 승수 확인
	bool Check2Square(int number);

};

#endif

