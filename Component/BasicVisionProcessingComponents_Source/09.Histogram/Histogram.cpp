
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

#include "Histogram.h"

//
// constructor declaration
//
int m_in_height;                // 처리할 이미지의 Height
int m_in_width;                 // 처리할 이미지의  Width

IplImage* m_orig_img;			// Original Image
IplImage* m_red;                // red channel 
IplImage* m_green;				// green channel
IplImage* m_blue;				// blue channel
IplImage* m_hist_img;           // 히스토그램이 그려질 곳


//***히스토그램 선언
CvHistogram* hist_red;          // red channel histogram
CvHistogram* hist_green;		// green channel histogram
CvHistogram* hist_blue;			// blue channel histogram

int hist_size;					// 히스토그램 전체 사이즈
int bin_w;						// 히스토그램 막대 사이즈

Histogram::Histogram()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Histogram::Histogram(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Histogram::~Histogram() {
}

void Histogram::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Histogram::onInitialize()
{
	float range[] = {0,255};                 //빈의 범위 쌍 지정
	float* ranges[] = {range};               //빈의 범위를 저장하는 포인터 배열 최대 최소값.

	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_red != NULL)
		m_red = NULL;
	if(m_green != NULL)
		m_green = NULL;
	if(m_blue != NULL)
		m_blue = NULL;
	if(m_hist_img != NULL)
		m_hist_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	hist_size = 256;                         //히스토그램 빈의 크기

	//0-255의 색상값을 표현하기 위해 256개의 빈을 이용하여 0-255 범위에 1의 등간격으로 히스토그램을 생성 한다.
	//새 히스토그램 만들기 - 히스토그램 차원,차원크기, 타입,x축범위,막대간격
	hist_red = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	hist_green = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	hist_blue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	
	//히스토그램 초기화
	cvClearHist(hist_red);
	cvClearHist(hist_green);
	cvClearHist(hist_blue);

	return OPROS_SUCCESS;
}

ReturnType Histogram::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Histogram::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onDestroy()
{
	// 이미지용 메모리의 해제
	//cvReleaseImage 는 IPlImage구조체를 삭제하고 메모리를 반환한다.
	if(m_orig_img       != NULL)			//원본
        cvReleaseImage(&m_orig_img);		
	if(m_red              != NULL)			//레드
        cvReleaseImage(&m_red);
	if(m_green            != NULL)			//그린
        cvReleaseImage(&m_green);
	if(m_blue             != NULL)			//블루
        cvReleaseImage(&m_blue);
	if(m_hist_img           !=NULL)			//히스토그램 영상
		cvReleaseImage(&m_hist_img);

	//히스토그램 메모리 해제
	if(hist_red != NULL)
		cvReleaseHist(&hist_red);
	if(hist_red != NULL)
		cvReleaseHist(&hist_green);
	if(hist_red != NULL)
		cvReleaseHist(&hist_blue);

	return OPROS_SUCCESS;
}

ReturnType Histogram::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onExecute()
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
		if(m_red == NULL){
			m_red = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_green == NULL){
			m_green = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_blue == NULL){
			m_blue = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_hist_img == NULL){
			m_hist_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		//cvRound : 실수형을 반올림해서 정수형으로 변환
		//히스토그램 이미지의 넓이를 빈의 크기로 나누면 하나의 빈에 해당하는 값이 나옴
		bin_w = cvRound( ( double )m_hist_img->width / hist_size );

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//cvSplit - 이미지를 채널별로 분리함
		//RGB의 히스토그램을 따로 그리기 위해 분리.
		cvSplit(m_orig_img, m_blue, m_green, m_red, NULL);
		
		//cvCalcHist 히스토그램 계산 함수(소스영상, 히스토그램 포인터)
		cvCalcHist(&m_red, hist_red);
		cvCalcHist(&m_green, hist_green);
		cvCalcHist(&m_blue, hist_blue);
		
		//히스토그램의 최소값과 최대값을 구함
		//주로 정규화 등을 하기 전에 사용한다고 함
		float max_value_r = 0;
		float max_value_g = 0;
		float max_value_b = 0;

		//소스 영상에 스케일과 시프트연산 수행하여 하나의 배열을 다른 배열로 선형변환-이미지의 스케일을변환
		//(소스영상, 목적영상, 스케일,시프트)
        cvGetMinMaxHistValue( hist_red, 0, &max_value_r, 0, 0 );

		cvConvertScale( hist_red->bins, hist_red->bins, 
		( ( double )m_hist_img->height ) / max_value_r, 0 );

		cvGetMinMaxHistValue( hist_green, NULL, &max_value_g, NULL, NULL );

		cvConvertScale( hist_green->bins, hist_green->bins, 
		( ( double )m_hist_img->height ) / max_value_g, 0 );

		cvGetMinMaxHistValue( hist_blue, NULL, &max_value_b, NULL, NULL );

		cvConvertScale( hist_blue->bins, hist_blue->bins, 
		( ( double )m_hist_img->height ) / max_value_b, 0 );
		
	    //영상 데이터값을 색상값으로 채우는 함수.
	    // (소스영상, 색상 값, 마스크)
		cvSet( m_hist_img, cvScalarAll( 255 ), 0 ); //흰색으로 채움 -CV_RGB(255,255,255) 가능

		for(int i =0; i <hist_size; i++)
		{
			//직사각형 그리기 함수
			//(직사각형이 그려질 영상, 직사각형 모서리, 직사각형 반대편 모서리,선 색상, 선 두께)
			//cvGetReal1D 1차원 히스토그램에서 1번째 막대의 빈도수를 가져오는 함수)
			//(1차원배열, 위치인덱스)
			cvRectangle(
		  		m_hist_img,
		  		cvPoint( i * bin_w, m_hist_img->height ),
				cvPoint(( i + 1 ) * bin_w, m_hist_img->height - cvRound( cvGetReal1D(hist_red->bins, i) ) ),
				CV_RGB(255,0,0)
			);
			cvRectangle(
		  		m_hist_img,
		  		cvPoint( i * bin_w, m_hist_img->height ),
				cvPoint(( i + 1 ) * bin_w, m_hist_img->height - cvRound( cvGetReal1D(hist_green->bins, i) ) ),
				CV_RGB(0,255,0),
				1
			);
			cvRectangle(
		  		m_hist_img,
		  		cvPoint( i * bin_w, m_hist_img->height ),
				cvPoint(( i + 1 ) * bin_w, m_hist_img->height - cvRound( cvGetReal1D(hist_blue->bins, i) ) ),
				CV_RGB(0,0,255),
				1
			);
			
		}		

		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_hist_img->width, m_hist_img->height, m_hist_img->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_hist_img->width * m_hist_img->height * m_hist_img->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_hist_img->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType Histogram::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onPeriodChanged()
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
	return new Histogram();
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
	return new Histogram();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

