
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

#include "FrequencyFilter.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;
IplImage* m_final_img;
IplImage* m_image_Re;
IplImage* m_image_Im;
IplImage* m_pDFT_A;

int m_in_width;
int m_in_height;

std::string m_low_Pass_Filtering;
std::string m_high_Pass_Filtering;
float m_cutoff_Frequency;

FrequencyFilter::FrequencyFilter()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FrequencyFilter::FrequencyFilter(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FrequencyFilter::~FrequencyFilter() {
}

void FrequencyFilter::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType FrequencyFilter::onInitialize()
{

	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_final_img != NULL)
		m_final_img = NULL;
	if(m_image_Re != NULL)
		m_image_Re = NULL;
	if(m_image_Im != NULL)
		m_image_Im = NULL;
	if(m_pDFT_A != NULL)
		m_pDFT_A = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_low_Pass_Filtering = getProperty("LowPassFiltering");
	m_high_Pass_Filtering = getProperty("HighPassFiltering");
	m_cutoff_Frequency = atof(getProperty("CutOffFrequency").c_str());

	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FrequencyFilter::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onDestroy()
{
	// 사용한 모든 이미지버퍼 메모리 영역 해제
	if(m_orig_img != NULL)
        cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);
	if(m_final_img != NULL)
		cvReleaseImage(&m_final_img);
	if(m_image_Re != NULL)
		cvReleaseImage(&m_image_Re);
	if(m_image_Im != NULL)
		cvReleaseImage(&m_image_Im);
	if(m_pDFT_A != NULL)
		cvReleaseImage(&m_pDFT_A);

	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onExecute()
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

		// 받은 영상의 2의 승수임을 확인
		if(!Check2Square(m_in_width) || !Check2Square(m_in_height)){

			std::cout << "This image is not a multifplier of 2" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;
		
		}

		// 받은 영상의 가로 세로 사이즈가 같은지 확인
		if(m_in_width != m_in_height){
		
			std::cout << "Size(width and height) of Image is not equal" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;

		}

		// 원본영상의 이미지영역 확보
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// 바이너리 영상영역의 확보
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		// 수행결과 영상영역의 확보
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		// 출력결과 영상영역의 확보
		if(m_final_img == NULL){
			m_final_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// Re영역 영상영역의 확보(실수)
		if(m_image_Re == NULL){
			m_image_Re = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 1);
		}
		// Im영역 영상영역의 확보(허수)
		if(m_image_Im == NULL){
			m_image_Im = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 1);
		}
		// 주파수 변환 영상영역의 확보.
		if(m_pDFT_A == NULL){
			m_pDFT_A = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 2);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// 둘다 none 이 아니거나, 둘중에 하나가 none 일경우
		if((m_low_Pass_Filtering != "none" || m_high_Pass_Filtering != "none") &&  
			(m_low_Pass_Filtering == "none" || m_high_Pass_Filtering == "none")){

			// 입력 받은 영상을 이진화 시킴
			cvCvtColor( m_orig_img, m_gray_img, CV_BGR2GRAY );

			// 주파수영역으로의 작업을 위한 깊이 정보 변경
			cvConvertScale(m_gray_img, m_image_Re);		// 8U -> 32F
			// m_image_Im의 초기화
			cvZero(m_image_Im);

			// shift center
			// 입력영상을 실수부로 변환한 이미지가 홀수인 화소의 부호를 변경하여
			// 푸리에변환에 의한 주파수 영역의 원점을 중심으로 이동시키기 위함
			ChangePosition(m_image_Re);

			cvMerge(m_image_Re, m_image_Im, NULL, NULL, m_pDFT_A);

			// m_pDFT_A에 대해 푸리에 변환을 수행
			cvDFT(m_pDFT_A, m_pDFT_A, CV_DXT_FORWARD);

			// 이상적 저주파 통과 필터링 실행
			if(m_low_Pass_Filtering == "ideal" && m_high_Pass_Filtering == "none"){
				
				IdealLowPassFiltering(m_pDFT_A, m_cutoff_Frequency);
				
			}
			// 버터워스 저주파 통과 필터링 실행
			else if(m_low_Pass_Filtering == "butterworth" && m_high_Pass_Filtering == "none"){

				ButterworthLowPassFiltering(m_pDFT_A, m_cutoff_Frequency, 2);
				
			}
			// 가우시안 저주파 통과 필터링 실행
			else if(m_low_Pass_Filtering == "gaussian" && m_high_Pass_Filtering == "none"){

				GaussianLowPassFiltering(m_pDFT_A, m_cutoff_Frequency);
				
			}
			// 이상적 고주파 통과 필터링 실행
			else if(m_high_Pass_Filtering == "ideal" && m_low_Pass_Filtering == "none"){
				
				IdealHighPassFiltering(m_pDFT_A, m_cutoff_Frequency);
				
			}
			// 버터워스 고주파 통과 필터링 실행
			else if(m_high_Pass_Filtering == "butterworth" && m_low_Pass_Filtering == "none"){
				
				ButterworthHighPassFiltering(m_pDFT_A, m_cutoff_Frequency, 2);
				
			}
			// 가우시안 고주파 통과 필터링 실행
			else if(m_high_Pass_Filtering == "gaussian" && m_low_Pass_Filtering == "none"){
				
				GaussianHighpassFiltering(m_pDFT_A, m_cutoff_Frequency);
				

			}
			else{
				//none
			}

			// 퓨리에 역변환 실행
			cvDFT(m_pDFT_A, m_pDFT_A, CV_DXT_INV_SCALE);

			// 다중 채널의 행렬을 단일 채널 행렬로 분할(Re, Im으로)
			cvSplit(m_pDFT_A, m_image_Re, m_image_Im, NULL, NULL);
			
			// 저주파일때만 실행
			if((m_low_Pass_Filtering == "ideal" || m_low_Pass_Filtering == "butterworth" || m_low_Pass_Filtering == "gaussian")
				&& m_high_Pass_Filtering == "none"){
				ChangePosition(m_image_Re);
				cvScale(m_image_Re, m_result_img, 1);
			}

			// 고주파일때만 실행
			if((m_high_Pass_Filtering == "ideal" || m_high_Pass_Filtering == "butterworth" || m_high_Pass_Filtering == "gaussian")
				&& m_low_Pass_Filtering == "none"){
				
				// 스펙트럼의 진폭을 계산 Mag=sqrt(Re^2 + Im^2)
				cvPow(m_image_Re, m_image_Re, 2.0);
				cvPow(m_image_Im, m_image_Im, 2.0);
				cvAdd(m_image_Re, m_image_Re, m_image_Re);
				cvPow(m_image_Re, m_image_Re, 0.5); 
				
				// 진폭 화상의 픽셀치가 min과 max사이에 분포하로독 스케일링
				double min_val, max_val;
				cvMinMaxLoc(m_image_Re, &min_val, &max_val, NULL, NULL);
				cvScale(m_image_Re, m_result_img, 255.0/max_val);
			}
			
			// 1채널 영상의 3채널 영상으로의 변환
			cvMerge(m_result_img, m_result_img, m_result_img, NULL, m_final_img);

			// 아웃풋 push
			// RawImage의 이미지 포인터 변수 할당
			RawImageData *pimage = result.getImage();
			
			// 입력된 이미지 사이즈 및 채널수로 로 재 설정
			pimage->resize(m_final_img->width, m_final_img->height, m_final_img->nChannels);
			
			// 영상의 총 크기(pixels수) 취득
			int size = m_final_img->width * m_final_img->height * m_final_img->nChannels;
			
			// 영상 데이터로부터 영상값만을 할당하기 위한 변수
			unsigned char *ptrdata = pimage->getData();
			
			// 현재 프레임 영상을 사이즈 만큼 memcpy
			memcpy(ptrdata, m_final_img->imageData, size);

			// 포트아웃
			opros_any mdata = result;
			ImageOut.push(result);//전달

			delete pData;

		}else{

			// 아웃풋 push
			// 아웃풋 push
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
		
	}

	return OPROS_SUCCESS;
}

void FrequencyFilter::ChangePosition(IplImage *pImage)
{
	int x, y;
	float fValue;

	for(y = 0; y < pImage->height; y++)
		for(x = 0; x < pImage->width; x++)
		{
			fValue = (float)cvGetReal2D(pImage, y, x);
			if((x+y)%2 == 1)
				fValue = - fValue;
			cvSetReal2D(pImage, y, x, fValue);
		}
	
}

// 이상적 고주파 영역 통과 필터링
void FrequencyFilter::IdealHighPassFiltering(IplImage *pDFT, double D0)
{
	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u <pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			if(D < D0)
				H = 0.0;
			else
				H = 1.0;

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}
}

// 이상적 저주파 영역 통과 필터링
void FrequencyFilter::IdealLowPassFiltering(IplImage *pDFT, double D0)
{
	int u, v;
	double D; // 거리정보
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));

			if(D <= D0)
				H = 1.0;
			else
				H = 0.0;
			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}
}

// 버터워스 고주파 영역 통과 필터링
void FrequencyFilter::ButterworthHighPassFiltering(IplImage *pDFT, double D0, int nCh)
{

	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			if(D == 0)
				D = 0.1;

			H = 1.0 / (1.0 + pow(D0 / D, 2*nCh));
			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}


}

// 버터워스 저주파 영역 통과 필터링
void FrequencyFilter::ButterworthLowPassFiltering(IplImage *pDFT, double D0, int nCh)
{

	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			H = 1.0 / (1.0 + pow(D / D0, 2*nCh));

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}

}

// 가우시안 고주파 영역 통과 필터링
void FrequencyFilter::GaussianHighpassFiltering(IplImage *pDFT, double D0)
{

	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			H = 1.0 - exp(-D * D / (2.0 * D0 * D0));

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);
		
		}

}

// 가우시안 저주파 영역 통과 필터링
void FrequencyFilter::GaussianLowPassFiltering(IplImage *pDFT, double D0)
{
	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			H = exp(-D * D / (2.0 * D0 * D0));

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);
		
		}
}

// 2의 승수 체크 함수
bool FrequencyFilter::Check2Square(int number)
{
	
	int ref = 1;

	while(ref < number)
		ref <<= 1;

	if(ref == number)
		return true;
	else
		return false;

}

	
ReturnType FrequencyFilter::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onPeriodChanged()
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
	return new FrequencyFilter();
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
	return new FrequencyFilter();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

