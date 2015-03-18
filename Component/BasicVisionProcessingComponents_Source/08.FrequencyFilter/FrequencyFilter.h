
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

	// ���ļ� ��ȯ�� ���� �߽ɰ��� ����
	void ChangePosition(IplImage *pImage);

	// �̻��� ������ ��� ���͸�
	void IdealLowPassFiltering(IplImage *pDFT, double D0);

	// �̻��� ������ ��� ���͸�
	void IdealHighPassFiltering(IplImage *pDFT, double D0);

	// ���Ϳ��� ������ ��� ���͸�
	void ButterworthLowPassFiltering(IplImage *pDFT, double D0, int nCh);

	// ���Ϳ��� ������ ��� ���͸�
	void ButterworthHighPassFiltering(IplImage *pDFT, double D0, int nCh);

	// ����þ� ������ ��� ���͸�
	void GaussianLowPassFiltering(IplImage *pDFT, double D0);

	// ����þ� ������ ��� ���͸�
	void GaussianHighpassFiltering(IplImage *pDFT, double D0);

	// 2�� �¼� Ȯ��
	bool Check2Square(int number);

};

#endif

