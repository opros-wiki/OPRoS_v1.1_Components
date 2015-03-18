
#ifndef _FastFourierTransform_COMPONENT_H
#define _FastFourierTransform_COMPONENT_H
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
		
class FastFourierTransform: public Component
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
	FastFourierTransform();
	FastFourierTransform(const std::string &compId);
	virtual ~FastFourierTransform();
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

	void FFT(int dir);
	void SetImage();
	void FFT1d(double* re, double* im, int N, int dir);
	void GetSpectrumImage();
	void GetPhaseImage();
	void GetImage();
	BOOL IsPowerOf2(int n);
	void Free();
	template<typename T> 
	void swap(T& lha, T& rha);
	template<typename T> 
	T limit(const T& value);


};

#endif

