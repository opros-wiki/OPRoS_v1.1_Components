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

// InputPort�κ��� ���� ������ �����ص� CV IplImage
IplImage* m_orig_img;

int m_in_height;			// ������ ���� ������
int m_in_width;				// ������ ���� ������

HANDLE m_mutex;				// ���� ������� �ڵ�
int m_state;				// ������ ���� �÷���

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
//	�������͸� CVWindowViewer�� ����ϴ� ������ �Լ� ����
//	�����۾� : Viewer ���� / ������ ��� / Viewer ����
//	����Ÿ�� : ReturnType(NONE(0))
//	���ǻ��� : ������ Dialog Box���·� ��µǴ� ��� �����带 ���
//
//*******************************************************************
UINT ImageFunc( LPVOID pParam )
{
	// ������ ����
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	// OutputImage��� Ÿ��Ʋ�� �� ����
	cvNamedWindow("OutputImage", CV_WINDOW_AUTOSIZE);	
	
	// �������͸� �� ���
	while (true) {
		// �������� ���� ���� ����
		::WaitForSingleObject(m_mutex, INFINITE);
		
		// ������ �⵿ �÷����� ���
		if (m_state == 1) {				
			
			// �������͸� �� Show
			if (m_orig_img != NULL) {
				cvShowImage("OutputImage",m_orig_img);
				cvWaitKey(1);
			}

			m_state = 0; // ready to get image
		}
		// ������ ���� �÷��� �ϰ��
		else if(m_state == 2) { // stop thread
			::ReleaseMutex(m_mutex);
			break;
		}

		// ����� �ڵ��� ������
		::ReleaseMutex(m_mutex);
			
		::Sleep(1);
	}
	
	// �����尡 ����ɶ� ����� ���� IplImage�� ������
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	// ��� �������� ����
	cvDestroyWindow("OutputImage");

	::WaitForSingleObject(m_mutex, INFINITE);
	m_state = 3; // thread stopped
	::ReleaseMutex(m_mutex);

	return 0;
}

// Call back Declaration
//*******************************************************************
//
//	�ڵ��� �����ϰ�, ������ �����带 �⵿��Ŵ
//	�����۾� : Mutex�ڵ� ���� / ImageFunc ������ �⵿
//	����Ÿ�� : OPROS_INTERNAL_FAULT / OPROS_SUCCESS
//	���ǻ��� : ����
//
//*******************************************************************
ReturnType ImageOutput::onInitialize()
{

	m_state = 0; // ready state
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex �ڵ� ����
	
	// �ڵ� ����ó��
	if (m_mutex == 0) {
		std::cout << "[ERROR-ImageOut::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}
	
	m_orig_img = NULL;			// ���� IplImage �ʱ�ȭ
	m_in_height = 0;			// ���� ���� ���� ���� �ʱ�ȭ
	m_in_width = 0;				// ���� ���� ���� ���� �ʱ�ȭ

	// ImageFunc ������ ����
	AfxBeginThread(ImageFunc, (LPVOID) this);	

	return OPROS_SUCCESS;
}

//*******************************************************************
//
//	�ڵ��� �����ϰ�, ������ �����带 �⵿��Ŵ
//	�����۾� : Mutex�ڵ� ���� / ImageFunc ������ �⵿
//	����Ÿ�� : OPROS_INTERNAL_FAULT / OPROS_SUCCESS
//	���ǻ��� : ����
//
//*******************************************************************
ReturnType ImageOutput::onDestroy()
{
	// ������ ���� ����
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

		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
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

