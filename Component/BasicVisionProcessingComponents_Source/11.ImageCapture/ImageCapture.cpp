
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

// 카메라로부터 받은 영상 데이터의 cv용 변수
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
//	컴포넌트 기동시 초기화 작업
//	실행작업 : 영상처리 장치와 커넥션 실행
//	리턴타입 : ReturnType(OPROS_INPUT_NULL / OPROS_SUCCESS)
//	주의사항 : onStart 가 아닌 onInitialize에서 초기작업 실행할것
//
//*******************************************************************
ReturnType ImageCapture::onInitialize()
{
	
	// 장치(0번장치)로부터의 커넥션 확인
	if((m_capture = cvCaptureFromCAM(0)) == NULL) {
	
		// 통보 메세지 출력
		std::cout<<"[ERROR-ImageCapture::onInitialize()] No Camera Device"<<std::endl;
		
		// 비정상 리턴타입 반환
		return OPROS_INPUT_NULL;
	}
	
	return OPROS_SUCCESS;
}

//*******************************************************************
//
//	컴포넌트 실행종료시 초기화 작업
//	실행작업 : 사용된 영상장치 변수의 해제 실행
//	리턴타입 : ReturnType(OPROS_SUCCESS)
//	주의사항 : onStop 가 아닌 onDestroy에서 해제종료작업 실행할것
//
//*******************************************************************
ReturnType ImageCapture::onDestroy()
{
	// 사용된 영상장치 변수의 반환
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
//	취득된 영상장치로 부터 영상을 취득하여 OutputPort로 Push
//	실행작업 : 영상이미지의 취득 및 Push
//	리턴타입 : ReturnType(OPROS_INPUT_NULL / OPROS_SUCCESS)
//	주의사항 : 취득된 이미지 구조체(IplImage)는 매주기마다 해제
//			 : 영상은 3채널로 취득하여 Push
//			 : 카메라 장치에 따라 영상이 반전되어 나올경우를 대비
//
//*******************************************************************
ReturnType ImageCapture::onExecute()
{
	RawImage result;					// 데이터 포트타입은 RawImage
	IplImage *camera_frame = NULL;		// 한 프레임 관련 이미지 변수

	// CvCapture 구조체 내부의 할당된 메모리의 호출
	camera_frame = cvQueryFrame(m_capture);
	
	// 정상적 취득 프레임이 아닐경우의 처리
	if(camera_frame == NULL){

		// 영상 취득불가 메세지 출력
		std::cout<<"[ERROR-ImageCapture::onExecute()] There are no frames"<<std::endl;

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

