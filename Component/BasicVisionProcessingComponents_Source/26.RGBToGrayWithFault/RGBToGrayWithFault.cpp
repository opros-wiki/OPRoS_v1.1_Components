
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

#include "RGBToGrayWithFault.h"

// InputPort로부터 받은 영상을 저장해둘 CV IplImage
IplImage* m_orig_img;
IplImage* m_result_img;
IplImage* m_gray_img;
IplImage* m_binary_img;

int m_in_height;			// 영상의 세로 사이즈
int m_in_width;				// 영상의 가로 사이즈
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

	// 사용할 IplImage의 초기화
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

	m_in_width = 0;				// 영상 사이즈의 가로길이
	m_in_height = 0;			// 영상 사이즈의 세로길이

	m_faultCount = 150;
	m_currentCount = 0;

	m_isConvert = false;
	m_w = 0;
	m_h = 0;

	m_Threshold = atoi(getProperty("Threshold").c_str());

	std::string strFaultCount = getProperty("FaultCount");
	if (strFaultCount != "") m_faultCount = atoi(strFaultCount.c_str());

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
	m_isConvert = true;

	std::cout << "        - RGB -> Grey" << std::endl;

	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onDestroy()
{
	// 사용한 영상 이미지의 릴리스
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
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;


	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		if (!m_isConvert) {
			m_currentCount++;
			if (m_currentCount > m_faultCount) {
				delete pData;

				m_currentCount = 0;
				throw std::exception("UserException : too slow");
			}

			ImageOut.push(Image);//전달
			delete pData;

			return OPROS_SUCCESS;
		}

		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// 원본영상의 이미지영역 확보
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		else if (m_w != m_in_width || m_h != m_in_height) {
			cvReleaseImage(&m_orig_img);
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// 결과영상의 이미지영역 확보
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		else if (m_w != m_in_width || m_h != m_in_height) {
			cvReleaseImage(&m_result_img);
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// gray 영상의 이미지 영역 확보
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		else if (m_w != m_in_width || m_h != m_in_height) {
			cvReleaseImage(&m_gray_img);
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// 바이너리 영상의 이미지 영역 확보
		if(m_binary_img == NULL){
			m_binary_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		else if (m_w != m_in_width || m_h != m_in_height) {
			cvReleaseImage(&m_binary_img);
			m_binary_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		m_w = m_in_width;
		m_h = m_in_height;

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// RGB영상을 gray 이미지로 변환
		cvCvtColor( m_orig_img, m_gray_img, CV_RGB2GRAY );
		
		// 바이너리 이미지로 변환
		cvThreshold( m_gray_img, m_binary_img, m_Threshold, 255, CV_THRESH_BINARY);
		
		// 1채널 바이너리 영상을 3채널로 머지
		cvMerge(m_binary_img, m_binary_img, m_binary_img, NULL, m_result_img);

		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_result_img->width, m_result_img->height, m_result_img->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_result_img->width * m_result_img->height * m_result_img->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_result_img->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;

	}


	// 변환한 영상을 outputPort로 Push

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

