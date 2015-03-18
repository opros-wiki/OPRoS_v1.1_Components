
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

#include "Translate.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_result_img;

CvMat *m_transformMatrix;

int m_in_width;
int m_in_height;

int m_x;
int m_y;

Translate::Translate()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Translate::Translate(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Translate::~Translate() {
}

void Translate::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Translate::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_transformMatrix = cvCreateMat( 2, 3, CV_32FC1);

	m_in_width = 0;
	m_in_height = 0;

	m_x = atoi(getProperty("X").c_str());
	m_y = atoi(getProperty("Y").c_str());

	return OPROS_SUCCESS;
}

ReturnType Translate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Translate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	cvReleaseMat(&m_transformMatrix);

	return OPROS_SUCCESS;
}

ReturnType Translate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onExecute()
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
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//*************************이동
		// float형 2D화소 행렬
		CvPoint2D32f original[3];		// 이동전
	    CvPoint2D32f translate[3];		// 이동후

		// 이동전의 원본이미지(기준점 0,0)
	    original[0] = cvPoint2D32f( 0, 0 );						// (좌상단)
	    original[1] = cvPoint2D32f( m_orig_img->width, 0 );	// (우상단)
	    original[2] = cvPoint2D32f( 0, m_orig_img->height );	// (좌하단)

	    // 이동후의 좌표(m_translateX, m_up_down 만큼이동)
		translate[0] = cvPoint2D32f( m_x, m_y );
	    translate[1] = cvPoint2D32f( m_x + m_orig_img->width, m_y );
	    translate[2] = cvPoint2D32f( m_x, m_y + m_orig_img->height );

	    // 어파인변환배열생성
	    cvGetAffineTransform( original, translate, m_transformMatrix );

	    // 이미지를 어파인변환배열을 이용하여 이동
	    cvWarpAffine( m_orig_img, m_result_img, m_transformMatrix, 
						CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

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
	
ReturnType Translate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onPeriodChanged()
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
	return new Translate();
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
	return new Translate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

