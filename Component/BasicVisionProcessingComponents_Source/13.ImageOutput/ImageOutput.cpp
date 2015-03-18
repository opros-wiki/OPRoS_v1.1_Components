#include "Stdafx.h"

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

#include "ImageOutput.h"

// InputPort로부터 받은 영상을 저장해둘 CV IplImage
IplImage* m_orig_img;

int m_in_height;			// 영상의 세로 사이즈
int m_in_width;				// 영상의 가로 사이즈

HANDLE m_mutex;				// 별도 스레드용 핸들
int m_state;				// 스레드 실행 플러그

//
// constructor declaration
//
ImageOutput::ImageOutput()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ImageOutput::ImageOutput(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ImageOutput::~ImageOutput() {
}

void ImageOutput::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

//*******************************************************************
//
//	영상데이터를 CVWindowViewer에 출력하는 스레드 함수 영역
//	실행작업 : Viewer 생성 / 데이터 출력 / Viewer 해제
//	리턴타입 : ReturnType(NONE(0))
//	주의사항 : 별도의 Dialog Box형태로 출력되는 경우 스레드를 사용
//
//*******************************************************************
UINT ImageFunc( LPVOID pParam )
{
	// 스레드 실행
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	// OutputImage라는 타이틀로 뷰어를 생성
	cvNamedWindow("OutputImage", CV_WINDOW_AUTOSIZE);	
	
	// 영상데이터를 뷰어에 출력
	while (true) {
		// 스레드의 종료 지연 선언
		::WaitForSingleObject(m_mutex, INFINITE);
		
		// 스레드 기동 플래그일 경우
		if (m_state == 1) {				
			
			// 영상데이터를 뷰어에 Show
			if (m_orig_img != NULL) {
				cvShowImage("OutputImage",m_orig_img);
				cvWaitKey(1);
			}

			m_state = 0; // ready to get image
		}
		// 스레드 중지 플래그 일경우
		else if(m_state == 2) { // stop thread
			::ReleaseMutex(m_mutex);
			break;
		}

		// 사용한 핸들의 릴리스
		::ReleaseMutex(m_mutex);
			
		::Sleep(1);
	}
	
	// 스레드가 종료될때 사용한 영상 IplImage를 릴리스
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	// 뷰어 윈도우의 해제
	cvDestroyWindow("OutputImage");

	::WaitForSingleObject(m_mutex, INFINITE);
	m_state = 3; // thread stopped
	::ReleaseMutex(m_mutex);

	return 0;
}

// Call back Declaration
//*******************************************************************
//
//	핸들을 생성하고, 지정한 스레드를 기동시킴
//	실행작업 : Mutex핸들 생성 / ImageFunc 스레드 기동
//	리턴타입 : OPROS_INTERNAL_FAULT / OPROS_SUCCESS
//	주의사항 : 없음
//
//*******************************************************************
ReturnType ImageOutput::onInitialize()
{

	m_state = 0; // ready state
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex 핸들 생성
	
	// 핸들 예외처리
	if (m_mutex == 0) {
		std::cout << "[ERROR-ImageOut::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}
	
	m_orig_img = NULL;			// 영상 IplImage 초기화
	m_in_height = 0;			// 영상 세로 정보 변수 초기화
	m_in_width = 0;				// 영상 가로 정보 변수 초기화

	// ImageFunc 스레드 실행
	AfxBeginThread(ImageFunc, (LPVOID) this);	

	return OPROS_SUCCESS;
}

//*******************************************************************
//
//	핸들을 생성하고, 지정한 스레드를 기동시킴
//	실행작업 : Mutex핸들 생성 / ImageFunc 스레드 기동
//	리턴타입 : OPROS_INTERNAL_FAULT / OPROS_SUCCESS
//	주의사항 : 없음
//
//*******************************************************************
ReturnType ImageOutput::onDestroy()
{
	// 스레드 종료 지연
	::WaitForSingleObject(m_mutex, INFINITE);
	m_state = 2; // state for stopping the image viewing thread
	::ReleaseMutex(m_mutex);

	// wait until the thread is really stopped
	while (true) {
		::WaitForSingleObject(m_mutex, INFINITE);

		if (m_state == 3) {
			::ReleaseMutex(m_mutex);
			break;
		}

		::ReleaseMutex(m_mutex);
			
		::Sleep(1);
	}

	if (m_mutex != 0)
		::CloseHandle(m_mutex);

	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onStart()
{

	return OPROS_SUCCESS;
}
	
ReturnType ImageOutput::onStop()
{
	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onExecute()
{
	opros_any *pData = ImageIn.pop();
	
	if(pData != NULL){

		::WaitForSingleObject(m_mutex, INFINITE);

		if (m_state != 0) { // NOT capturing state
			::ReleaseMutex(m_mutex);
			delete pData;
			return OPROS_SUCCESS;
		}

		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		m_state = 1;

		::ReleaseMutex(m_mutex);

		delete pData;
	}

	return OPROS_SUCCESS;
}


ReturnType ImageOutput::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageOutput::onPeriodChanged()
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
	return new ImageOutput();
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
	return new ImageOutput();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

