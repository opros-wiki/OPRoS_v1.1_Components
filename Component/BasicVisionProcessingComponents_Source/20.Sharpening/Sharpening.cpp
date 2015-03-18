
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

#include "Sharpening.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;

CvMat *kernel_mat;

int m_in_width;
int m_in_height;

std::string m_result_type;
std::string m_do_sharpening;

Sharpening::Sharpening()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Sharpening::Sharpening(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Sharpening::~Sharpening() {
}

void Sharpening::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Sharpening::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_result_type = getProperty("ResultType");
	m_do_sharpening = getProperty("DoSharpening");

	// 행렬생성
	kernel_mat = cvCreateMat( 3, 3, CV_32FC1);

	return OPROS_SUCCESS;
}

ReturnType Sharpening::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Sharpening::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	cvReleaseMat(&kernel_mat);

	return OPROS_SUCCESS;
}

ReturnType Sharpening::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onExecute()
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

		int h = m_orig_img->height;
		int w = m_orig_img->width;
		
		if(m_result_type == "Gray"){	// m_RGB_mode:Gray 흑백
			// 컬러영상을 그레이스케일로 변환
			cvCvtColor( m_orig_img, m_gray_img, CV_RGB2GRAY );

			// 그레이이미지(1채널)을 3채널로 변경, 샤프닝연산위해 다시 m_image_buff에 저장
			cvMerge(m_gray_img, m_gray_img, m_gray_img, NULL, m_orig_img);
		}

		// 샤프닝처리일경우
		if(m_do_sharpening == "True"){

			IplImage * rgb[3];
			IplImage * rgb_temp[3];

			for(int i=0; i<3; i++){

				rgb[i] = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

				rgb_temp[i] = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

			}

			// 3채널->1채널로 분리
			cvSplit(m_orig_img, rgb[0], rgb[1], rgb[2], NULL);

			for(int index=0; index<3; index++){				

				uchar * image_data = (uchar*)rgb[index]->imageData;

				uchar * change_data = (uchar*)rgb_temp[index]->imageData;

				// 라플라시안연산 후, 원본영상과 합치기
				for(int i=1; i<h-1; i++){

					for(int j=1; j<w-1; j++){

						change_data[i*w+j] = limit(4*image_data[i*w+j] - image_data[(i-1)*w+j] -
										image_data[i*w+j+1] - image_data[(i+1)*w+j] - image_data[i*w+j-1]);

						change_data[i*w+j] = limit(image_data[i*w+j] + change_data[i*w+j]);
					}
				}
			}

			// 분리했던 채널 다시 합치기
			cvMerge(rgb_temp[0], rgb_temp[1], rgb_temp[2], NULL, m_result_img);

			// 사용된 이미지 release
			for(int i=0; i<3; i++){

				cvReleaseImage(&rgb[i]);
				
				cvReleaseImage(&rgb_temp[i]);
				
			}

		}else if(m_do_sharpening == "False"){
			
			// 샤프닝처리 없을 경우, 출력버퍼로 복사
			cvCopy(m_orig_img, m_result_img, NULL);
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

template<typename T>
T Sharpening::limit(const T& value)
{
	return ( (value > 255) ? 255 : ((value < 0) ? 0 : value) );
}
	
ReturnType Sharpening::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onPeriodChanged()
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
	return new Sharpening();
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
	return new Sharpening();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

