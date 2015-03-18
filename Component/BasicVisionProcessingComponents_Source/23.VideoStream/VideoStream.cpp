
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



#include "VideoStream.h"

//
// constructor declaration
//
// ī�޶�κ��� ���� ���� �������� cv�� ����
CvCapture* m_capture;

VideoStream::VideoStream()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
VideoStream::VideoStream(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

VideoStream::~VideoStream() {
}

void VideoStream::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType VideoStream::onInitialize()
{
	std::string filePath;
	m_videoFile = getProperty("VideoFile");
	if (m_videoFile == "") m_videoFile = "videoCapture.avi";

	filePath = getProperty("opros.component.dir") + m_videoFile;

	// �����������κ����� Ŀ�ؼ� Ȯ��
	if((m_capture = cvCaptureFromFile(filePath.c_str())) == NULL) {
	
		// �뺸 �޼��� ���
		std::cout<<"[ERROR-VedioStream::onInitialize()] No Video File"<<std::endl;
		
		// ������ ����Ÿ�� ��ȯ
		return OPROS_INPUT_NULL;
	}

	return OPROS_SUCCESS;
}

ReturnType VideoStream::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType VideoStream::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onDestroy()
{
	// ���� ������ġ ������ ��ȯ
	if(m_capture!=NULL)
		cvReleaseCapture(&m_capture);

	return OPROS_SUCCESS;
}

ReturnType VideoStream::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onExecute()
{
	RawImage result;					// ������ ��ƮŸ���� RawImage
	IplImage *camera_frame = NULL;		// �� ������ ���� �̹��� ����

	// CvCapture ����ü ������ �Ҵ�� �޸��� ȣ��
	camera_frame = cvQueryFrame(m_capture);
	
	// ������ ��� �������� �ƴҰ���� ó��
	if(camera_frame == NULL){

		// ���� ���Ұ� �޼��� ���
		std::cout<<"[ERROR-VedioStream::onExecute()] There are no frames"<<std::endl;

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
	
ReturnType VideoStream::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onPeriodChanged()
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
	return new VideoStream();
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
	return new VideoStream();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

