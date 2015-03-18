
#include "stdafx.h"
	
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



#include "ObjectTracking.h"

#define		H_DIMENSION		16			//	히스토그램차원
#define		H_RANGE_MIN		0			//  hsv의 h널최소값
#define		H_RANGE_MAX		180			//  hsv의 h채널최소값
#define		V_MIN	10					//	hsv의 v채널최소값
#define		V_MAX	256					//	hsv의 v채널최대값
#define		S_MIN	30					//	hsv의 s채널최소값
#define		S_MAX	256					//	hsv의 s채널최대값
#define		SELECT_OFF				0	//	영역선택off
#define		SELECT_ON				1	//	영역선택on
#define		TRACKING_STOP			0	//	트래킹안함
#define		TRACKING_START			-1	//	트래킹시작
#define		TRACKING_NOW			1	//	트래킹진행중

//
// constructor declaration
//
IplImage	*m_image_buff;			// 입력이미지 저장 버퍼
IplImage	*m_image_hsv;			// hsv이미지 저장 버퍼
IplImage	*m_image_hue;			// hsv에서 hue값 저장 버퍼
IplImage	*m_image_mask;			// 마스크이미지 저장 버퍼
IplImage	*backprojectImage;
IplImage	*m_image_gray;			// 흑백이미지 저장 버퍼
IplImage	*m_chg_image_buff;		// 결과이미지

CvPoint		origin;
CvRect		selection;				// 드래그영역

int	select_object;					// 영역지정 상태
int	track_object;					// 트래킹 상태

CvHistogram	*hist;		

//	CamShift에 이용될 트래킹영역관련
CvRect			track_window;
CvBox2D			track_box;
CvConnectedComp	track_comp;

int hdims;
float hRangesArray[2];
float *hRanges;

int m_in_height;
int m_in_width;

HANDLE m_mutex;				// 별도 스레드용 핸들
int m_state;				// 스레드 실행 플러그

// 히스토그램계산
void CalculateHist( CvHistogram *hist, IplImage *m_image_hsv, IplImage *m_image_mask, CvRect selection );
// 컬러변환
CvScalar hsv2rgb( float hue );

void on_mouse(int event, int x, int y, int flags, void* param ){
	if( m_chg_image_buff == NULL ){
        return;
	}

	if( m_chg_image_buff->origin == 1 ){
        y = m_chg_image_buff->height - y;
	}
	// 드래그 하고 있는 좌표반영
    if( select_object == SELECT_ON ){
        selection.x = MIN( x, origin.x );
        selection.y = MIN( y, origin.y );
        selection.width = selection.x + CV_IABS( x - origin.x );
        selection.height = selection.y + CV_IABS( y - origin.y );
        
        selection.x = MAX( selection.x, 0 );
        selection.y = MAX( selection.y, 0 );
        selection.width = MIN( selection.width, m_chg_image_buff->width );
        selection.height = MIN( selection.height, m_chg_image_buff->height );
        selection.width = selection.width - selection.x;
        selection.height = selection.height - selection.y;
    }
	// 마우스이벤트
    switch( event ){
		case CV_EVENT_LBUTTONDOWN:
			// 마우스왼쪽버튼 클릭
			// 영역선택on
			origin = cvPoint( x, y );
			selection = cvRect( x, y, 0, 0 );
			select_object = SELECT_ON;
			break;
		case CV_EVENT_LBUTTONUP:
			// 마우스왼쫀버튼 클릭해제
			// 영역선택off 트래킹on
			select_object = SELECT_OFF;
			if( selection.width > 0 && selection.height > 0 ){
				track_object = TRACKING_START;
			}
			break;
    }
}


ObjectTracking::ObjectTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ObjectTracking::ObjectTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ObjectTracking::~ObjectTracking() {
}

void ObjectTracking::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);
	addPort("PositionOut", &PositionOut);



	// export variable setup


}

//*******************************************************************
//
//	영상데이터를 CVWindowViewer에 출력하는 스레드 함수 영역
//	실행작업 : Viewer 생성 / 데이터 출력 / Viewer 해제
//	리턴타입 : ReturnType(NONE(0))
//	주의사항 : 별도의 Dialog Box형태로 출력되는 경우 스레드를 사용
//
//*******************************************************************
UINT ImageFunc( LPVOID pParam )
{
	// 스레드 실행
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	// OutputImage라는 타이틀로 뷰어를 생성
	cvNamedWindow("OutputImage", CV_WINDOW_AUTOSIZE);
	// 마우스이벤트 콜백함수 지정
	cvSetMouseCallback("OutputImage", on_mouse, (void *)m_chg_image_buff);
	
	// 영상데이터를 뷰어에 출력
	while (true) {
		// 스레드의 종료 지연 선언
		::WaitForSingleObject(m_mutex, INFINITE);
		
		// 스레드 기동 플래그일 경우
		if (m_state == 1) {				
			
			// 영상데이터를 뷰어에 Show
			if (m_chg_image_buff != NULL) {
				cvShowImage("OutputImage",m_chg_image_buff);
				cvWaitKey(1);
			}

			m_state = 0; // ready to get image
		}
		// 스레드 중지 플래그 일경우
		else if(m_state == 2) { // stop thread
			::ReleaseMutex(m_mutex);
			break;
		}

		// 사용한 핸들의 릴리스
		::ReleaseMutex(m_mutex);
			
		::Sleep(1);
	}
	
	// 스레드가 종료될때 사용한 영상 IplImage를 릴리스
	if(m_chg_image_buff != NULL)
		cvReleaseImage(&m_chg_image_buff);

	// 뷰어 윈도우의 해제
	cvDestroyWindow("OutputImage");

	::WaitForSingleObject(m_mutex, INFINITE);
	m_state = 3; // thread stopped
	::ReleaseMutex(m_mutex);

	return 0;
}

// Call back Declaration
ReturnType ObjectTracking::onInitialize()
{
	m_state = 0; // ready state
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex 핸들 생성
	
	// 핸들 예외처리
	if (m_mutex == 0) {
		std::cout << "[ERROR-ImageOut::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}
	
	m_in_height = 0;			// 영상 세로 정보 변수 초기화
	m_in_width = 0;				// 영상 가로 정보 변수 초기화

	// ImageFunc 스레드 실행
	AfxBeginThread(ImageFunc, (LPVOID) this);

	// 버퍼초기화
	if(m_image_buff != NULL)
		m_image_buff = NULL;
	if(m_chg_image_buff != NULL)
		m_chg_image_buff = NULL;
	if(m_image_hsv != NULL)
		m_image_hsv = NULL;
	if(m_image_hue != NULL)
		m_image_hue = NULL;
	if(m_image_mask != NULL)
		m_image_mask = NULL;
	if(backprojectImage != NULL)
		backprojectImage = NULL;
	if(m_image_gray != NULL)			
		m_image_gray = NULL;
	if(hist != NULL)
		hist = NULL;

	// 트래킹상태관련
	select_object = SELECT_OFF;
	track_object = TRACKING_STOP;

	// 히스토그램관련
	hdims = H_DIMENSION;		
	hRangesArray[0] = H_RANGE_MIN;
	hRangesArray[1] = H_RANGE_MAX;
	hRanges = hRangesArray;	

	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ObjectTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onDestroy()
{
	// 스레드 종료 지연
	::WaitForSingleObject(m_mutex, INFINITE);
	m_state = 2; // state for stopping the image viewing thread
	::ReleaseMutex(m_mutex);

	// wait until the thread is really stopped
	while (true) {
		::WaitForSingleObject(m_mutex, INFINITE);

		if (m_state == 3) {
			::ReleaseMutex(m_mutex);
			break;
		}

		::ReleaseMutex(m_mutex);
			
		::Sleep(1);
	}

	if (m_mutex != 0)
		::CloseHandle(m_mutex);

	// 버퍼해제
	if(m_image_buff != NULL)
		cvReleaseImage(&m_image_buff);
	if(m_chg_image_buff != NULL)
		cvReleaseImage(&m_chg_image_buff);
	if(m_image_hsv != NULL)
		cvReleaseImage(&m_image_hsv);
	if(m_image_hue != NULL)
		cvReleaseImage(&m_image_hue);
	if(m_image_mask != NULL)
		cvReleaseImage(&m_image_mask);
	if(backprojectImage != NULL)
		cvReleaseImage(&backprojectImage);
	if(m_image_gray != NULL)			
		cvReleaseImage(&m_image_gray);
	if(hist != NULL)
		cvReleaseHist(&hist);


	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onExecute()
{
	opros_any *pData = ImageIn.pop();
	
	if(pData != NULL){

		::WaitForSingleObject(m_mutex, INFINITE);

		if (m_state != 0) { // NOT capturing state
			::ReleaseMutex(m_mutex);
			delete pData;
			return OPROS_SUCCESS;
		}

		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		if(m_image_buff == NULL)
			m_image_buff		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		if(m_chg_image_buff == NULL)
			m_chg_image_buff	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		if(m_image_hsv == NULL)
			m_image_hsv			= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		if(m_image_hue == NULL)
			m_image_hue			= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		if(m_image_mask == NULL)
			m_image_mask		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		if(backprojectImage == NULL)
			backprojectImage	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		if(m_image_gray == NULL)
			m_image_gray		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		if(hist == NULL)
			hist				= cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hRanges, 1 );

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		cvCopy(m_image_buff, m_chg_image_buff);
		// 색상변환 BGR->HSV
		cvCvtColor(m_image_buff, m_image_hsv, CV_BGR2HSV);

		bool data = false;
		if(track_object != TRACKING_STOP)
		{	
			CalculateHist(hist, m_image_hsv, m_image_mask, selection );
			// HUE채널이미지와 
			cvCalcBackProject(&m_image_hue, backprojectImage, hist);
			cvAnd(backprojectImage, m_image_mask, backprojectImage, 0);
			// 물체의 중심점과 크기를 얻어낼수 있음
			cvCamShift( backprojectImage, track_window,
						cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1),
						&track_comp, &track_box);

			// 물체탐색 윈도우의 크기가 실시간으로 업데이트
			track_window = track_comp.rect;

			// 물체영역표시
			CvPoint pt;
			pt = cvPoint(track_window.x, track_window.y);
			cvDrawRect(	m_chg_image_buff, 
						pt, 
						cvPoint(pt.x + track_window.width, pt.y + track_window.height), 
						CV_RGB(255,255,0),
						3);

			// 물체영역정보
			std::vector<PositionDataType> data;
			PositionDataType base("", NULL, NULL, NULL, NULL, NULL);
			base.setX(pt.x);
			base.setY(pt.y);
			base.setWidth(track_window.width);
			base.setHeight(track_window.height);
			data.push_back(base);
			PositionOut.push(data);

		}

		// 마우스로 드래그시 해당영역 표시
		if(select_object && selection.width > 0 && selection.height > 0)
		{
			cvSetImageROI(m_chg_image_buff, selection); 
			cvXorS(m_chg_image_buff, cvScalarAll(255), m_chg_image_buff,0);
			cvResetImageROI(m_chg_image_buff);
		}

		m_state = 1;

		::ReleaseMutex(m_mutex);

		delete pData;
	}

	return OPROS_SUCCESS;
}

// 히스토그램 계산
void CalculateHist( CvHistogram	*hist, IplImage *m_image_hsv, IplImage *m_image_mask, CvRect selection ){
	float	max_val;

	cvInRangeS(m_image_hsv, 
				   cvScalar(H_RANGE_MIN, S_MIN, MIN(V_MIN, V_MAX), 0),
				   cvScalar(H_RANGE_MAX, S_MAX, MAX(V_MIN, V_MAX), 0), 
				   m_image_mask);

	// HSV채널분리
	cvSplit(m_image_hsv, m_image_hue, NULL, NULL, NULL);

	// 관심영역의 히스토그램 분석
	//if(track_object < TRACKING_START)
	if(track_object < 0)
	{
		max_val = 0.0f;
		cvSetImageROI( m_image_hue, selection );
		cvSetImageROI( m_image_mask, selection );
		// m_image_mask의 관심영역으로 히스토그램 계산 후, hist에 저장
		cvCalcHist( &m_image_hue, hist, 0, m_image_mask );
		cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
		if( max_val == 0.0f ){
			cvConvertScale( hist->bins, hist->bins, 0.0, 0 );
		} else{
			cvConvertScale( hist->bins, hist->bins, 255.0 / max_val, 0 );
		}

		// 이미지 전체를 관심영역으로 재설정
		cvResetImageROI( m_image_hue );
		cvResetImageROI( m_image_mask );

		track_window = selection;
		track_object = TRACKING_NOW;		
	}

}

// 컬러변환
CvScalar hsv2rgb( float hue ){
	int rgb[3], p, sector;
	static const int sector_data[][3] = 
		{ {0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2} };

	hue *= 0.033333333333333333333333333333333f;
	sector = cvFloor(hue);
	p = cvRound(255*(hue - sector));
	p ^= sector & 1? 255 : 0;

	rgb[sector_data[sector][0]] = 255;
	rgb[sector_data[sector][1]] = 0;
	rgb[sector_data[sector][2]] = p;

	return cvScalar( rgb[2], rgb[1], rgb[0], 0 );
}

	
ReturnType ObjectTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onPeriodChanged()
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
	return new ObjectTracking();
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
	return new ObjectTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

