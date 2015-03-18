
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

#include "ImageCapture.h"

//
// constructor declaration
//

// ī�޶�κ��� ���� ���� �������� cv�� ����
CvCapture* m_capture;

ImageCapture::ImageCapture()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ImageCapture::ImageCapture(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

ImageCapture::~ImageCapture() {
}

void ImageCapture::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
//*******************************************************************
//
//	������Ʈ �⵿�� �ʱ�ȭ �۾�
//	�����۾� : ����ó�� ��ġ�� Ŀ�ؼ� ����
//	����Ÿ�� : ReturnType(OPROS_INPUT_NULL / OPROS_SUCCESS)
//	���ǻ��� : onStart �� �ƴ� onInitialize���� �ʱ��۾� �����Ұ�
//
//*******************************************************************
ReturnType ImageCapture::onInitialize()
{
	
	// ��ġ(0����ġ)�κ����� Ŀ�ؼ� Ȯ��
	if((m_capture = cvCaptureFromCAM(0)) == NULL) {
	
		// �뺸 �޼��� ���
		std::cout<<"[ERROR-ImageCapture::onInitialize()] No Camera Device"<<std::endl;
		
		// ������ ����Ÿ�� ��ȯ
		return OPROS_INPUT_NULL;
	}
	
	return OPROS_SUCCESS;
}

//*******************************************************************
//
//	������Ʈ ��������� �ʱ�ȭ �۾�
//	�����۾� : ���� ������ġ ������ ���� ����
//	����Ÿ�� : ReturnType(OPROS_SUCCESS)
//	���ǻ��� : onStop �� �ƴ� onDestroy���� ���������۾� �����Ұ�
//
//*******************************************************************
ReturnType ImageCapture::onDestroy()
{
	// ���� ������ġ ������ ��ȯ
	if(m_capture!=NULL)
		cvReleaseCapture(&m_capture);

	return OPROS_SUCCESS;
}

ReturnType ImageCapture::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ImageCapture::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageCapture::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageCapture::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageCapture::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageCapture::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

//*******************************************************************
//
//	���� ������ġ�� ���� ������ ����Ͽ� OutputPort�� Push
//	�����۾� : �����̹����� ��� �� Push
//	����Ÿ�� : ReturnType(OPROS_INPUT_NULL / OPROS_SUCCESS)
//	���ǻ��� : ���� �̹��� ����ü(IplImage)�� ���ֱ⸶�� ����
//			 : ������ 3ä�η� ����Ͽ� Push
//			 : ī�޶� ��ġ�� ���� ������ �����Ǿ� ���ð�츦 ���
//
//*******************************************************************
ReturnType ImageCapture::onExecute()
{
	RawImage result;					// ������ ��ƮŸ���� RawImage
	IplImage *camera_frame = NULL;		// �� ������ ���� �̹��� ����

	// CvCapture ����ü ������ �Ҵ�� �޸��� ȣ��
	camera_frame = cvQueryFrame(m_capture);
	
	// ������ ��� �������� �ƴҰ���� ó��
	if(camera_frame == NULL){

		// ���� ���Ұ� �޼��� ���
		std::cout<<"[ERROR-ImageCapture::onExecute()] There are no frames"<<std::endl;

		// ����ó���� ���� �뺸
		return OPROS_INPUT_NULL;

	}

	// ������ ��� �������� ��� �ش� �̹����� ���� ���� / RGB 3ä��
	IplImage* frame = cvCreateImage(cvGetSize(camera_frame), IPL_DEPTH_8U, 3);

	// ��濵���� ���� �Ǿ����� ���� ����� ó��
	if(camera_frame->origin == IPL_ORIGIN_TL){

		// �̹����� �״���� ���� ���
		cvCopy(camera_frame, frame);

	}
	// ��濵���� ���� �Ǿ� �ִ� ����� ó��
	else{

		//������ ������ ������ �����
		cvFlip(camera_frame, frame);

	}

	// ������ ���� �۾�(�������� & ���� ũ��(Pixel��)
	// RawImage�� �̹��� ������ ���� �Ҵ�
	RawImageData *pimage = result.getImage();
	
	// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
	pimage->resize(frame->width, frame->height, frame->nChannels);
	
	// ������ �� ũ��(pixels��) ���
	int size = frame->width * frame->height * frame->nChannels;
	
	// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
	unsigned char *ptrdata = pimage->getData();
	
	// ���� ������ ������ ������ ��ŭ memcpy
	memcpy(ptrdata, frame->imageData, size);

	// ���� �������� CV�̹��� ������
	cvReleaseImage(&frame);

	// ��Ʈ�ƿ�
	opros_any mdata = result;
	ImageOut.push(result);//����

	return OPROS_SUCCESS;
}
	
ReturnType ImageCapture::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageCapture::onPeriodChanged()
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
	return new ImageCapture();
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
	return new ImageCapture();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

