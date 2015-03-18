
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>

#include "RGBValueControl.h"

//
// constructor declaration
//
IplImage* m_orig_img;

int m_in_width;
int m_in_height;

int R_Value;
int G_Value;
int B_Value;

RGBValueControl::RGBValueControl()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
RGBValueControl::RGBValueControl(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

RGBValueControl::~RGBValueControl() {
}

void RGBValueControl::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType RGBValueControl::onInitialize()
{
	// IplImage �� �ʱ�ȭ
	if(m_orig_img != NULL)
		m_orig_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	R_Value = atoi(getProperty("R_Value").c_str());
	G_Value = atoi(getProperty("G_Value").c_str());
	B_Value = atoi(getProperty("B_Value").c_str());

	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBValueControl::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onDestroy()
{
	// ����� �̹����� ������
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;

	if(pData != NULL){
		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// ���������� �̹������� Ȯ��
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//���� rgb����� ���
		cvAddS(m_orig_img, CV_RGB(R_Value,G_Value,B_Value), m_orig_img, NULL);

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_orig_img->width, m_orig_img->height, m_orig_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_orig_img->width * m_orig_img->height * m_orig_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_orig_img->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;

	}

	return OPROS_SUCCESS;
}
	
ReturnType RGBValueControl::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}







#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new RGBValueControl();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new RGBValueControl();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

