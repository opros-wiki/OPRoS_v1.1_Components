
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



#include "Chromakey.h"

//
// constructor declaration
//
int m_in_height;                 // 원본영상Height
int m_in_width;                  // 원본영상Width
int m_in2_height;                // Background영상Height
int m_in2_width;                 // Background영상Width

int m_low_h;
int m_high_h;
int m_polly_hull;
int m_perimScale;
int m_num;
int m_approx_level;
int m_morpology_repeat;

// 원본영상 이미지관련
IplImage* m_image_buff;			 // Original Image
IplImage* m_image_extracted;     // Extracted Image
// 마스크관련
IplImage* m_image_mask;			 // Mask Image
IplImage* m_image_inverseMask;   // Inverse Mask Image
IplImage* m_image_mask_temp;	 // Mask temp Image
// HSV관련
IplImage* m_image_hsv;
IplImage* m_image_h;
// 배경영상 이미지관련
IplImage* m_image_BG_in;         // Background Input image
IplImage* m_image_BG;            // Background Converted Image(Resized to Camera Image)
IplImage* m_image_extractedBG;   // Extracted Background Image
// 결과IplImage
IplImage* m_image_destination;	 

void find_connected_components( IplImage* mask, int poly1_hull0, float perimScale, int* num, CvRect* bbs, CvPoint* centers);


Chromakey::Chromakey()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Chromakey::Chromakey(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

Chromakey::~Chromakey() {
}

void Chromakey::portSetup() {

	// data port setup

	addPort("ImageIn1", &ImageIn1);

	addPort("ImageIn2", &ImageIn2);

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType Chromakey::onInitialize()
{
	// 원본영상 이미지관련
	if(m_image_buff        != NULL)
		m_image_buff        = NULL;
    if(m_image_extracted   != NULL)
		m_image_extracted   = NULL;

    // 마스크관련
	if(m_image_mask   != NULL)
		m_image_mask        = NULL;
	if(m_image_inverseMask   != NULL)
		m_image_inverseMask = NULL;
	if(m_image_mask_temp  != NULL)
		m_image_mask_temp = NULL;
    
    // 배경영상관련
	if(m_image_BG_in   != NULL)
		m_image_BG_in       = NULL;
    if(m_image_BG   != NULL)
		m_image_BG          = NULL;
    if(m_image_extractedBG   != NULL)
		m_image_extractedBG = NULL;

    // 결과
	if(m_image_destination   != NULL)
		m_image_destination = NULL;

	// HSV관련
	if(m_image_h   != NULL)
		m_image_h = NULL;
	if(m_image_hsv   != NULL)
		m_image_hsv = NULL;

	m_low_h			= atoi(getProperty("low_h").c_str());
	m_high_h		= atoi(getProperty("high_h").c_str());
	m_polly_hull	= atoi(getProperty("polly_hull").c_str());
	m_perimScale	= atoi(getProperty("perimScale").c_str());
	m_num			= atoi(getProperty("num").c_str());
	m_approx_level	= atoi(getProperty("approx_level").c_str());
	m_morpology_repeat = atoi(getProperty("morpology_repeat").c_str());

	m_in_height     = 0;
    m_in_width      = 0;
    m_in2_height    = 0;
    m_in2_width     = 0;

	return OPROS_SUCCESS;
}

ReturnType Chromakey::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Chromakey::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onDestroy()
{
	// 원본영상 이미지관련
	if(m_image_buff        != NULL)
        cvReleaseImage(&m_image_buff);
    if(m_image_extracted   != NULL)
        cvReleaseImage(&m_image_extracted);
    // 마스크관련
	if(m_image_mask        != NULL)
        cvReleaseImage(&m_image_mask);
    if(m_image_inverseMask != NULL)
        cvReleaseImage(&m_image_inverseMask);
	if(m_image_mask_temp != NULL)
        cvReleaseImage(&m_image_mask_temp);
    // 배경영상 이미지관련
	if(m_image_BG_in       != NULL)
        cvReleaseImage(&m_image_BG_in);     
	if(m_image_BG          != NULL)
        cvReleaseImage(&m_image_BG);
    if(m_image_extractedBG != NULL)
        cvReleaseImage(&m_image_extractedBG);
	// 결과
    if(m_image_destination != NULL)
        cvReleaseImage(&m_image_destination);
	// HSV관련
	if(m_image_hsv != NULL)
        cvReleaseImage(&m_image_hsv);
	if(m_image_h != NULL)
        cvReleaseImage(&m_image_h);

	return OPROS_SUCCESS;
}

ReturnType Chromakey::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData1 = ImageIn1.pop();
	opros_any *pData2 = ImageIn2.pop();

	RawImage result;

	RawImage Image1;
	RawImage Image2;
	
	// Port for Background image
    if (pData2 != NULL) 
    {
        // 포트로 부터 이미지 취득
		Image2 = ImageIn2.getContent(*pData2);
		RawImageData *RawImage2 = Image2.getImage();

		// 현재영상의 크기를 취득
		m_in2_width = RawImage2->getWidth();
		m_in2_height = RawImage2->getHeight();

		// 원본영상의 이미지영역 확보
		if(m_image_BG_in == NULL){
			m_image_BG_in = cvCreateImage(cvSize(m_in2_width, m_in2_height), IPL_DEPTH_8U, 3);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_BG_in->imageData, RawImage2->getData(), RawImage2->getSize());

		delete pData2;

		if(pData1 != NULL)
		{
			Image1 = ImageIn1.getContent(*pData1);

			RawImageData *RawImage1 = Image1.getImage();

			// 현재영상의 크기를 취득
			m_in_width = RawImage1->getWidth();
			m_in_height = RawImage1->getHeight();

			 // 원본영상 이미지관련
			if(m_image_buff == NULL)
				m_image_buff		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			if(m_image_extracted == NULL)
				m_image_extracted   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );

			// 마스크관련
			if(m_image_mask == NULL)
				m_image_mask        = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
			if(m_image_inverseMask == NULL)
				m_image_inverseMask = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
			if(m_image_mask_temp == NULL)
				m_image_mask_temp   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
	            
			// HSV관련
			if(m_image_hsv == NULL)
				m_image_hsv			= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
			if(m_image_h == NULL)
				m_image_h			= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );

			// 배경영상 이미지관련
			if(m_image_BG == NULL)
				m_image_BG          = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
			if(m_image_extractedBG == NULL)
				m_image_extractedBG = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );

			// 결과
			if(m_image_destination == NULL)
				m_image_destination = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );

			// 배경영상 리사이즈
			if(m_image_BG_in != NULL)
				cvResize(m_image_BG_in, m_image_BG, CV_INTER_LINEAR);

			// 영상에 대한 정보를 확보!memcpy
			memcpy(m_image_buff->imageData, RawImage1->getData(), RawImage1->getSize());

			// 색상변환 BGR->HSV
			cvCvtColor(m_image_buff, m_image_hsv, CV_BGR2HSV);

			// HSV채널분리
			cvSplit(m_image_hsv, m_image_h, NULL, NULL, NULL);

			// 원본영상의 H에서 배경(지정색) 추출
			cvInRangeS( m_image_h, cvScalar(m_low_h), cvScalar(m_high_h), m_image_mask );	// m_image_mask : 지정색(배경들어갈 공간)
			cvNot( m_image_mask, m_image_inverseMask );										// m_image_inverseMask : 원본영상(배경공간제외)

			// 임의의 IplImage(1채널)
			cvSet(m_image_mask_temp, cvScalar(0), m_image_mask);							// 배경부분은 검은색
			cvSet(m_image_mask_temp, cvScalar(255), m_image_inverseMask);					// 그외에는 흰색		

			// 노이즈제거
			find_connected_components(m_image_mask_temp, m_polly_hull, m_perimScale, NULL, NULL, NULL);

			// 노이즈제거된 마스크에서 흑백공간분리
			cvInRangeS( m_image_mask_temp, cvScalar(0), cvScalar(1), m_image_mask );	
			cvInRangeS( m_image_mask_temp, cvScalar(255), cvScalar(256), m_image_inverseMask );

			// 마스크를 이용하여 원본으로 들어갈 영역 추출
			cvSetZero( m_image_extracted );
			cvCopy( m_image_buff, m_image_extracted, m_image_inverseMask );					// 원본영상(배경제외한 흰색부분)

			// 마스크를 이용하여 배경으로 들어갈 영역 추출
			cvSetZero( m_image_extractedBG );
			cvCopy( m_image_BG, m_image_extractedBG, m_image_mask );						// 배경영상(검은색부분)
			
			
			// 배경 + 원본
			cvAdd( m_image_extractedBG, m_image_extracted, m_image_destination, NULL);

			// RawImage의 이미지 포인터 변수 할당
			RawImageData *pimage = result.getImage();
			
			// 입력된 이미지 사이즈 및 채널수로 로 재 설정
			pimage->resize(m_in_width, m_in_height, m_image_destination->nChannels);
			
			// 영상의 총 크기(pixels수) 취득
			int size = m_in_width * m_in_height * m_image_destination->nChannels;
			
			// 영상 데이터로부터 영상값만을 할당하기 위한 변수
			unsigned char *ptrdata = pimage->getData();
			
			// 현재 프레임 영상을 사이즈 만큼 memcpy
			memcpy(ptrdata, m_image_destination->imageData, size);

			// 포트아웃
			opros_any mdata = result;
			ImageOut.push(result);//전달

			delete pData1;
		}
    }	

	return OPROS_SUCCESS;
}

void find_connected_components( IplImage* mask, int poly1_hull0, float perimScale, int* num, CvRect* bbs, CvPoint* centers){
	static CvMemStorage* mem_storage = NULL;
	static CvSeq* contours = NULL;

	// 열기, 닫기
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_OPEN, m_morpology_repeat);
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_CLOSE, m_morpology_repeat);

	// 큰영역을 둘러싼 외곽선만 찾기
	if( mem_storage == NULL)
		mem_storage = cvCreateMemStorage(0);
	else
		cvClearMemStorage(mem_storage);

	CvContourScanner scanner = cvStartFindContours(
		mask, mem_storage, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	CvSeq* c;
	int numCont = 0;
	while( (c = cvFindNextContour( scanner)) != NULL)
	{
		double len = cvContourPerimeter( c );

		// 외곽선 길이의 최소 임계값 계산
		double q = (mask->height + mask->width)/perimScale;

		// 외곽선의 너무 작은 영역은 제거
		if( len < q) {
			cvSubstituteContour( scanner, NULL );
		}else{
			// 외관선 길이가 충분히 큰 경우, 엣지를 스무딩한다
			CvSeq* c_new;

			if( poly1_hull0 ){
				// 다각형 근사화
				c_new = cvApproxPoly( c, sizeof(CvContour), mem_storage, CV_POLY_APPROX_DP, m_approx_level, 0 );
			}else{
				// 컨벡스 헐 근사화
				c_new = cvConvexHull2(c, mem_storage, CV_CLOCKWISE, 1);
			}
			cvSubstituteContour( scanner, c_new);
			numCont++;
		}
	}
	contours = cvEndFindContours( &scanner );

	// 편의를 위한 변수 선언
	const CvScalar CVX_WHITE = CV_RGB(0xff, 0xff, 0xff);
	const CvScalar CVX_BLACK = CV_RGB(0x00, 0x00, 0x00);

	// 찾아진 영역을 영상에 그린다
	cvZero( mask );
	IplImage* maskTemp;

	// 영역의 중심과 바운딩 박스를 구한다
	if( num != NULL )
	{
		// 사용자가 통계적 수치를 수집하기를 원한다
		int N = *num, numFilled = 0, i = 0;
		CvMoments moments;
		double M00, M01, M10;
		maskTemp = cvCloneImage(mask);

		for( i=0, c = contours; c!=NULL; c=c->h_next, i++)
		{
			if(i < N)
			{
				//(*num)까지만 작업수행
				cvDrawContours(maskTemp, c, CVX_WHITE, CVX_WHITE, -1, CV_FILLED, 8);

				//각 외관선의 중심을 찾는다
				if(centers != NULL)
				{
					cvMoments(maskTemp, &moments, 1);
					M00 = cvGetSpatialMoment(&moments, 0, 0);
					M10 = cvGetSpatialMoment(&moments, 1, 0);
					M01 = cvGetSpatialMoment(&moments, 0, 1);
					centers[i].x = (int)(M10/M00);
					centers[i].y = (int)(M01/M00);
				}

				// 영역을 감싸는 바운딩 박스를 구한다
				if(bbs != NULL){
					bbs[i] = cvBoundingRect(c);
				}
				cvZero(maskTemp);
				numFilled++;
			}

			// 채워진 외관선을 mask영상에 그린다
			cvDrawContours( mask, c, CVX_WHITE, CVX_WHITE, -1, CV_FILLED, 8);
		}// end of for
		*num = numFilled;
		cvReleaseImage( &maskTemp);
	}

	//아니면 단순히 마스크에 그리기만 한다
	else
	{
		// 사용자가 통계적 수치를 계산하는 대신, 외관선만 그리기를 원한다
		for( c=contours; c!=NULL; c=c->h_next) {
			cvDrawContours( mask, c, CVX_WHITE, CVX_BLACK, -1, CV_FILLED, 8);
		}
	}
}

ReturnType Chromakey::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onPeriodChanged()
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
	return new Chromakey();
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
	return new Chromakey();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

