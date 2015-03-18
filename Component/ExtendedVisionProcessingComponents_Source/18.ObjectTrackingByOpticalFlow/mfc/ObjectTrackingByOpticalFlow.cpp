
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



#include "ObjectTrackingByOpticalFlow.h"

#define		SELECT_OFF				0	//	영역선택off
#define		SELECT_ON				1	//	영역선택on
#define		TRACKING_STOP			0	//	트래킹안함
#define		TRACKING_START			-1	//	트래킹시작
#define		TRACKING_NOW			1	//  트래킹진행중

//
// constructor declaration
//
IplImage	*m_image_buff;				// 입력이미지 저장 버퍼
IplImage	*m_image_gray;				// 흑백이미지 저장 버퍼
IplImage	*m_image_select;			// 선택영역마스크
IplImage	*m_prev_grey;				// OpticalFlow연산용
IplImage	*m_pyramid;					// OpticalFlow연산용
IplImage	*m_prev_pyramid;			// OpticalFlow연산용
IplImage	*swap_temp;					// OpticalFlow연산용
IplImage	*m_chg_image_buff;			// 결과이미지

CvPoint			origin;
CvRect			selection;				// 드래그영역

int	select_object;						// 영역지정 상태
int	track_object;						// 트래킹 상태

int win_size = 10;
const int MAX_COUNT = 500;
CvPoint2D32f* points_sel[2] = {0,0}, *swap_points;
char* status = 0;
int count = 0;
int flags = 0;

int m_in_height;
int m_in_width;

HANDLE m_mutex;							// 별도 스레드용 핸들
int m_state;							// 스레드 실행 플러그

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
			track_object = TRACKING_STOP;
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

ObjectTrackingByOpticalFlow::ObjectTrackingByOpticalFlow()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ObjectTrackingByOpticalFlow::ObjectTrackingByOpticalFlow(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ObjectTrackingByOpticalFlow::~ObjectTrackingByOpticalFlow() {
}

void ObjectTrackingByOpticalFlow::portSetup() {

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
ReturnType ObjectTrackingByOpticalFlow::onInitialize()
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
	if(m_image_select != NULL)
		m_image_select = NULL;
	if(m_image_gray != NULL)
		m_image_gray = NULL;

	m_in_height = 0;
    m_in_width  = 0;
	
	select_object = SELECT_OFF;
	track_object = TRACKING_STOP;

	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ObjectTrackingByOpticalFlow::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onDestroy()
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
	if(m_image_select != NULL)
		cvReleaseImage(&m_image_select);
	if(m_image_gray != NULL)			
		cvReleaseImage(&m_image_gray);

	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onExecute()
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
		if(m_image_gray == NULL)
			m_image_gray		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);			
		if(m_prev_grey == NULL)
			m_prev_grey		= cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
		if(m_pyramid == NULL)	
			m_pyramid		= cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
		if(m_prev_pyramid == NULL)
			m_prev_pyramid	= cvCreateImage( cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );

		if(points_sel[0] == NULL)
			points_sel[0]	= (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points_sel[0][0]));
		if(points_sel[1] == NULL)
			points_sel[1]	= (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points_sel[0][0]));

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		cvCopy(m_image_buff, m_chg_image_buff);
		cvCvtColor(m_chg_image_buff, m_image_gray, CV_BGR2GRAY);

		if(track_object != TRACKING_STOP)
		{
			// 선택영역 마스크로 지정
			if(track_object == TRACKING_START)
			{
				points_sel[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points_sel[0][0]));
				points_sel[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points_sel[0][0]));
				status = (char*)cvAlloc(MAX_COUNT);
				flags = 0;
				
				if(m_image_select != NULL)
					cvReleaseImage(&m_image_select);

				m_image_select = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
				
				cvSetZero(m_image_select);
				// 선택부분만 관심영역 지정후 지정색으로 채움
				cvSetImageROI( m_image_select, selection );
				cvSet(m_image_select, cvScalar(255));
				cvResetImageROI(m_image_select);

				/* automatic initialization */
				IplImage* eig = cvCreateImage( cvGetSize(m_image_gray), 32, 1 );
				IplImage* temp = cvCreateImage( cvGetSize(m_image_gray), 32, 1 );
				double quality = 0.01;
				double min_distance = 10;

				count = MAX_COUNT;
				// 뚜렷한 코너점 검춤
				cvGoodFeaturesToTrack( m_image_gray, eig, temp, points_sel[1], &count,
                                   quality, min_distance, m_image_select, 3, 0, 0.04 );
				// 코너점 검출(부화소단위로)
				cvFindCornerSubPix( m_image_gray, points_sel[1], count,
					cvSize(win_size, win_size), cvSize(-1,-1),
					cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
				cvReleaseImage( &eig );
				cvReleaseImage( &temp );

				track_object = TRACKING_NOW;
			}

			// 오브젝트트래킹
			else if(track_object = TRACKING_NOW)
			{	
				int i,k;

				if(count > 0)
				{
					// 루카스 카나데 알고리즘
					cvCalcOpticalFlowPyrLK( m_prev_grey, m_image_gray, m_prev_pyramid, m_pyramid,
						points_sel[0], points_sel[1], count, cvSize(win_size,win_size), 3, status, 0,
						cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );
					flags |= CV_LKFLOW_PYR_A_READY;
					
					std::vector<PositionDataType> data;
					for( i = k = 0; i < count; i++ )
					{

						if( !status[i] )
							continue;

						points_sel[1][k++] = points_sel[1][i];
						// 특징점을 화면에 표시
						cvCircle( m_chg_image_buff, cvPointFrom32f(points_sel[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);

						// 특징점좌표정보
						PositionDataType base("", NULL, NULL, NULL, NULL, NULL);
						base.setX(cvPointFrom32f(points_sel[1][i]).x);
						base.setY(cvPointFrom32f(points_sel[1][i]).y);
						data.push_back(base);
					}
					if(data.size() > 0)
						PositionOut.push(data);

					count = k;

				}				
			}
			CV_SWAP( m_prev_grey, m_image_gray, swap_temp );
			CV_SWAP( m_prev_pyramid, m_pyramid, swap_temp );
			CV_SWAP( points_sel[0], points_sel[1], swap_points );
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
	
ReturnType ObjectTrackingByOpticalFlow::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onPeriodChanged()
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
	return new ObjectTrackingByOpticalFlow();
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
	return new ObjectTrackingByOpticalFlow();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

