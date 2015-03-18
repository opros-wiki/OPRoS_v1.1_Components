
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

#include "RGBToGray.h"

// InputPort�κ��� ���� ������ �����ص� CV IplImage
IplImage* m_orig_img;
IplImage* m_result_img;
IplImage* m_gray_img;
IplImage* m_binary_img;

int m_in_height;			// ������ ���� ������
int m_in_width;				// ������ ���� ������
int m_Threshold;

//
// constructor declaration
//
RGBToGray::RGBToGray()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
RGBToGray::RGBToGray(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

RGBToGray::~RGBToGray() {
}

void RGBToGray::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType RGBToGray::onInitialize()
{

	// ����� IplImage�� �ʱ�ȭ
	if(m_orig_img != NULL){
		m_orig_img = NULL;
	}
	if(m_result_img != NULL){
		m_result_img = NULL;
	}
	if(m_gray_img != NULL){
		m_gray_img = NULL;
	}
	if(m_binary_img != NULL){
		m_binary_img = NULL;
	}

	m_in_width = 0;				// ���� �������� ���α���
	m_in_height = 0;			// ���� �������� ���α���

	m_Threshold = atoi(getProperty("Threshold").c_str());

	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBToGray::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onDestroy()
{
	// ����� ���� �̹����� ������
	if(m_orig_img != NULL){
		cvReleaseImage(&m_orig_img);
	}
	if(m_result_img != NULL){
		cvReleaseImage(&m_result_img);
	}
	if(m_gray_img != NULL){
		cvReleaseImage(&m_gray_img);
	}
	if(m_binary_img != NULL){
		cvReleaseImage(&m_binary_img);
	}

	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onExecute()
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
		// ��������� �̹������� Ȯ��
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// gray ������ �̹��� ���� Ȯ��
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		// ���̳ʸ� ������ �̹��� ���� Ȯ��
		if(m_binary_img == NULL){
			m_binary_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// RGB������ gray �̹����� ��ȯ
		cvCvtColor( m_orig_img, m_gray_img, CV_RGB2GRAY );
		
		// ���̳ʸ� �̹����� ��ȯ
		cvThreshold( m_gray_img, m_binary_img, m_Threshold, 255, CV_THRESH_BINARY);
		
		// 1ä�� ���̳ʸ� ������ 3ä�η� ����
		cvMerge(m_binary_img, m_binary_img, m_binary_img, NULL, m_result_img);

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_result_img->width, m_result_img->height, m_result_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_result_img->width * m_result_img->height * m_result_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_result_img->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;

	}


	// ��ȯ�� ������ outputPort�� Push

	return OPROS_SUCCESS;
}
	
ReturnType RGBToGray::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onPeriodChanged()
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
	return new RGBToGray();
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
	return new RGBToGray();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

