
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

#include <cv.h>
#include <highgui.h>

#include "ColorConvert.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_result_img;
IplImage* m_gray_img;

int m_in_width;
int m_in_height;

std::string transCode;
int m_convert_code = 0;

ColorConvert::ColorConvert()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ColorConvert::ColorConvert(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ColorConvert::~ColorConvert() {
}

void ColorConvert::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ColorConvert::onInitialize()
{
	// 영상 IplImage 초기화
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;				// 영상 사이즈의 가로 길이
	m_in_height = 0;			// 영상 사이즈의 세로 길이

	transCode = getProperty("TransNum");	// 외부 프로퍼티 취득

	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ColorConvert::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onDestroy()
{
	// 사용한 이미지의 릴리스
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onExecute()
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
		// 결과영상의 이미지영역 확보
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// gray 영상의 이미지 영역 확보
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// 외부 프로퍼티 예외 처리
		if(transCode == ""){
		
			std::cout << "[ERROR-ColorConvert::onExecute()] There is no Extenal Parameter" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;
		}

		// 외부 프로퍼티 값에 대응하는 변환 코드의 지정
		if(transCode =="GRAY")
			m_convert_code = CV_RGB2GRAY;
		if(transCode =="RGB")
			m_convert_code = 0;
		if(transCode =="BGR")
			m_convert_code = CV_RGB2BGR;
		if(transCode =="XYZ")
			m_convert_code = CV_RGB2XYZ;
		if(transCode =="YCrCb")
			m_convert_code = CV_RGB2YCrCb;
		if(transCode =="HSV")
			m_convert_code = CV_RGB2HSV;
		if(transCode =="HLS")
			m_convert_code = CV_RGB2HLS;
		if(transCode =="Lab")
			m_convert_code = CV_RGB2Lab;
		if(transCode =="Luv")
			m_convert_code = CV_RGB2Luv;
		
		if(m_convert_code != 0){
			
			//그레이 영상일 경우에는 1채널->3채널 변경 필요
			if(transCode == "GRAY")
			{
				//그레이로 변환
				cvCvtColor(m_orig_img, m_gray_img, m_convert_code);;

				// Convert to 3channel image 출력 맞추기 위해 병합 - 16비트일 경우에는 네장 합치는 형식?
				// 주로 각각의 컬러스페이스로 분리된 이미지를 합칠때 사용한다고 함.(이미지채널 1,2,3,4, 결과이미지):4채널까지 가능
				cvMerge(m_gray_img, m_gray_img, m_gray_img, NULL, m_result_img);

			}
			else
			{

				//그 밖 이미지 변환
				cvCvtColor(m_orig_img, m_result_img, m_convert_code);

			}
		}
		else{
		
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
	
ReturnType ColorConvert::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onPeriodChanged()
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
	return new ColorConvert();
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
	return new ColorConvert();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

