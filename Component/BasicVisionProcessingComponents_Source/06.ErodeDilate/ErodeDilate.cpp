
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

#include "ErodeDilate.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

std::string m_Change_mode;
int m_Repeat_count;
std::string m_RGB_mode;

ErodeDilate::ErodeDilate()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ErodeDilate::ErodeDilate(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ErodeDilate::~ErodeDilate() {
}

void ErodeDilate::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ErodeDilate::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_Change_mode = getProperty("ChangeMode");
	m_Repeat_count = atoi(getProperty("RepeatCount").c_str());
	m_RGB_mode = getProperty("RGBMode");

	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ErodeDilate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;

	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// 원본영상의 이미지영역 확보
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		
		if(m_RGB_mode == "Gray"){	// m_RGB_mode:1 흑백
			// 컬러영상을 그레이스케일로 변환
			cvCvtColor( m_orig_img, m_gray_img, CV_RGB2GRAY );

			// 그레이이미지(1채널)을 3채널로 변경, 팽창침식연산위해 다시 m_image_buff에 저장
			cvMerge(m_gray_img, m_gray_img, m_gray_img, NULL, m_orig_img);
		}

		if(m_Change_mode == "Erode"){	// m_change_mode:Erode 침식
			cvErode(m_orig_img, m_result_img, NULL, m_Repeat_count);
		}else if(m_Change_mode == "Dilate"){				// m_change_mode:Dilate 팽창
			cvDilate(m_orig_img, m_result_img, NULL, m_Repeat_count);
		}else{
			cvCopy(m_orig_img, m_result_img);
		}


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

	return OPROS_SUCCESS;
}
	
ReturnType ErodeDilate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ErodeDilate::onPeriodChanged()
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
	return new ErodeDilate();
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
	return new ErodeDilate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

