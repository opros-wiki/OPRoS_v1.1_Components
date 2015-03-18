
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

#include "Contrast.h"

//
// constructor declaration
//
IplImage* m_orig_img;

int m_in_width;
int m_in_height;
int m_Contrast;

Contrast::Contrast()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Contrast::Contrast(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Contrast::~Contrast() {
}

void Contrast::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Contrast::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_Contrast = atoi(getProperty("Contrast").c_str());

	return OPROS_SUCCESS;
}

ReturnType Contrast::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Contrast::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	return OPROS_SUCCESS;
}

ReturnType Contrast::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onExecute()
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

		for(int i = 0; i < m_orig_img->height; i++)
		{
			for(int j = 0; j< m_orig_img->width; j++)
			{
				//RGB의 채널 순서는 BGR
				//CvScalar 데이터 구조형을 매개변수로 함
				CvScalar v = cvGet2D(m_orig_img,i,j);
				//limit() : RGB값이 벗어날 경우에 보정(0-255 사이에 값이 존재하도록)
				//128을 기준으로 -1~1의 값을 곱하면 128 근처의 값은 조금 변하고 차이날수록 값의 변화가 커서 대비를 줌
				v.val[2] = limit(v.val[2] + ((v.val[2] -128) * m_Contrast /100) );
				v.val[1] = limit(v.val[1] + ((v.val[1] -128) * m_Contrast /100) );
				v.val[0] = limit(v.val[0] + ((v.val[0] -128) * m_Contrast /100) );
			
				cvSet2D(m_orig_img, i,j,v);

			}

		}

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

//RGB값 보정 함수
double Contrast::limit(double value){

	return( (value > 255) ? 255 : ((value < 0) ? 0 : value) );

}
	
ReturnType Contrast::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onPeriodChanged()
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
	return new Contrast();
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
	return new Contrast();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

