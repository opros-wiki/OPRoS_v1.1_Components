
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

#include "RGBValueControl.h"

//
// constructor declaration
//
IplImage* m_orig_img;

int m_in_width;
int m_in_height;

int R_Value;
int G_Value;
int B_Value;

RGBValueControl::RGBValueControl()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
RGBValueControl::RGBValueControl(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

RGBValueControl::~RGBValueControl() {
}

void RGBValueControl::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType RGBValueControl::onInitialize()
{
	// IplImage 의 초기화
	if(m_orig_img != NULL)
		m_orig_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	R_Value = atoi(getProperty("R_Value").c_str());
	G_Value = atoi(getProperty("G_Value").c_str());
	B_Value = atoi(getProperty("B_Value").c_str());

	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBValueControl::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onDestroy()
{
	// 사용한 이미지의 릴리스
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onExecute()
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

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//영상에 rgb상수값 계산
		cvAddS(m_orig_img, CV_RGB(R_Value,G_Value,B_Value), m_orig_img, NULL);

		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_orig_img->width, m_orig_img->height, m_orig_img->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_orig_img->width * m_orig_img->height * m_orig_img->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_orig_img->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;

	}

	return OPROS_SUCCESS;
}
	
ReturnType RGBValueControl::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onPeriodChanged()
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
	return new RGBValueControl();
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
	return new RGBValueControl();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

