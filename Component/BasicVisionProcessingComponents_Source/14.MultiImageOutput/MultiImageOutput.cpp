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



#include "MultiImageOutput.h"

//
// constructor declaration
//
// InputPort로부터 받은 영상을 저장해둘 CV IplImage
IplImage* m_orig_img1;
IplImage* m_orig_img2;

int m_in_height1;			// 영상의 세로 사이즈
int m_in_width1;				// 영상의 가로 사이즈
int m_in_height2;			// 영상의 세로 사이즈
int m_in_width2;			// 영상의 가로 사이즈

HANDLE m_mutex;				// 별도 스레드용 핸들
int m_state;				// 스레드 실행 플러그

MultiImageOutput::MultiImageOutput()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
MultiImageOutput::MultiImageOutput(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

MultiImageOutput::~MultiImageOutput() {
}

void MultiImageOutput::portSetup() {

	// data port setup

	addPort("ImageIn1", &ImageIn1);

	addPort("ImageIn2", &ImageIn2);



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
	cvNamedWindow("OutputImage2", CV_WINDOW_AUTOSIZE);
	
	// 영상데이터를 뷰어에 출력
	while (true) {
		// 스레드의 종료 지연 선언
		::WaitForSingleObject(m_mutex, INFINITE);
		
		// 스레드 기동 플래그일 경우
		if (m_state == 1) {				
			
			// 영상데이터를 뷰어에 Show
			if (m_orig_img1 != NULL) {
				cvShowImage("OutputImage",m_orig_img1);
				cvWaitKey(1);
			}
			if (m_orig_img2 != NULL) {
				cvShowImage("OutputImage2",m_orig_img2);
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
	if(m_orig_img1 != NULL)
		cvReleaseImage(&m_orig_img1);
	if(m_orig_img2 != NULL)
		cvReleaseImage(&m_orig_img2);

	// 뷰어 윈도우의 해제
	cvDestroyWindow("OutputImage");
	cvDestroyWindow("OutputImage2");

	::WaitForSingleObject(m_mutex, INFINITE);
	m_state = 3; // thread stopped
	::ReleaseMutex(m_mutex);

	return 0;
}

// Call back Declaration
ReturnType MultiImageOutput::onInitialize()
{
	// user code here
	m_state = 0; // ready state
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex 핸들 생성
	
	// 핸들 예외처리
	if (m_mutex == 0) {
		std::cout << "[ERROR-ImageOut::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}
	
	if(m_orig_img1 != NULL)
	    m_orig_img1  = NULL;
	if(m_orig_img2 != NULL)
	    m_orig_img2  = NULL;

	m_in_height1 = 0;
    m_in_width1  = 0;
	m_in_height2 = 0;
    m_in_width2  = 0;

	// ImageFunc 스레드 실행
	AfxBeginThread(ImageFunc, (LPVOID) this);

	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType MultiImageOutput::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onDestroy()
{
	// user code here
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

ReturnType MultiImageOutput::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onExecute()
{
	opros_any *pData1 = ImageIn1.pop();
	opros_any *pData2 = ImageIn2.pop();
	
	if(pData1 != NULL || pData2 != NULL) 
	{
		::WaitForSingleObject(m_mutex, INFINITE);

		if (m_state != 0) { // NOT capturing state
			::ReleaseMutex(m_mutex);
			if (pData1 != NULL) delete pData1;
			if (pData1 != NULL) delete pData2;
			return OPROS_SUCCESS;
		}

		if (pData1 != NULL) 
		{
			// 포트로 부터 이미지 취득
			RawImage Image1 = ImageIn1.getContent(*pData1);
			RawImageData *RawImage1 = Image1.getImage();

			// 현재영상의 크기를 취득
			m_in_width1 = RawImage1->getWidth();
			m_in_height1 = RawImage1->getHeight();

			if(m_orig_img1 == NULL)
			{
				m_orig_img1 = cvCreateImage(cvSize(m_in_width1, m_in_height1), IPL_DEPTH_8U, 3);
			}

			// 영상에 대한 정보를 확보!memcpy
			memcpy(m_orig_img1->imageData, RawImage1->getData(), RawImage1->getSize());
	
			delete pData1;
		}

		if (pData2 != NULL) 
		{
			// 포트로 부터 이미지 취득
			RawImage Image2 = ImageIn2.getContent(*pData2);
			RawImageData *RawImage2 = Image2.getImage();

			// 현재영상의 크기를 취득
			m_in_width2 = RawImage2->getWidth();
			m_in_height2 = RawImage2->getHeight();

			if(m_orig_img2 == NULL)
			{
				m_orig_img2 = cvCreateImage(cvSize(m_in_width2, m_in_height2), IPL_DEPTH_8U, 3);
			}

			// 영상에 대한 정보를 확보!memcpy
			memcpy(m_orig_img2->imageData, RawImage2->getData(), RawImage2->getSize());

			delete pData2;
		}

		m_state = 1;

		::ReleaseMutex(m_mutex);
	}

	return OPROS_SUCCESS;
}
	
ReturnType MultiImageOutput::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MultiImageOutput::onPeriodChanged()
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
	return new MultiImageOutput();
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
	return new MultiImageOutput();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

