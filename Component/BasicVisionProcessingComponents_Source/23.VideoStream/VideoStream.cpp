
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
// 카메라로부터 받은 영상 데이터의 cv용 변수
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

	// 비디오영상으로부터의 커넥션 확인
	if((m_capture = cvCaptureFromFile(filePath.c_str())) == NULL) {
	
		// 통보 메세지 출력
		std::cout<<"[ERROR-VedioStream::onInitialize()] No Video File"<<std::endl;
		
		// 비정상 리턴타입 반환
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
	// 사용된 영상장치 변수의 반환
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
	RawImage result;					// 데이터 포트타입은 RawImage
	IplImage *camera_frame = NULL;		// 한 프레임 관련 이미지 변수

	// CvCapture 구조체 내부의 할당된 메모리의 호출
	camera_frame = cvQueryFrame(m_capture);
	
	// 정상적 취득 프레임이 아닐경우의 처리
	if(camera_frame == NULL){

		// 영상 취득불가 메세지 출력
		std::cout<<"[ERROR-VedioStream::onExecute()] There are no frames"<<std::endl;

		// 에러처리에 대한 통보
		return OPROS_INPUT_NULL;

	}

	// 정상적 취득 프레임일 경우 해당 이미지의 영역 생성 / RGB 3채널
	IplImage* frame = cvCreateImage(cvGetSize(camera_frame), IPL_DEPTH_8U, 3);

	// 취득영상이 반전 되어있지 않은 경우의 처리
	if(camera_frame->origin == IPL_ORIGIN_TL){

		// 미반전시 그대로의 영상 사용
		cvCopy(camera_frame, frame);

	}
	// 취득영상이 반전 되어 있는 경우의 처리
	else{

		//반전시 반전된 영상을 사용함
		cvFlip(camera_frame, frame);

	}

	// 영상의 복사 작업(영상데이터 & 영상 크기(Pixel수)
	// RawImage의 이미지 포인터 변수 할당
	RawImageData *pimage = result.getImage();
	
	// 입력된 이미지 사이즈 및 채널수로 로 재 설정
	pimage->resize(frame->width, frame->height, frame->nChannels);
	
	// 영상의 총 크기(pixels수) 취득
	int size = frame->width * frame->height * frame->nChannels;
	
	// 영상 데이터로부터 영상값만을 할당하기 위한 변수
	unsigned char *ptrdata = pimage->getData();
	
	// 현재 프레임 영상을 사이즈 만큼 memcpy
	memcpy(ptrdata, frame->imageData, size);

	// 현재 프레임의 CV이미지 릴리즈
	cvReleaseImage(&frame);

	// 포트아웃
	opros_any mdata = result;
	ImageOut.push(result);//전달

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

