
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

#define		SELECT_OFF				0	//	��������off
#define		SELECT_ON				1	//	��������on
#define		TRACKING_STOP			0	//	Ʈ��ŷ����
#define		TRACKING_START			-1	//	Ʈ��ŷ����
#define		TRACKING_NOW			1	//  Ʈ��ŷ������

//
// constructor declaration
//
IplImage	*m_image_buff;				// �Է��̹��� ���� ����
IplImage	*m_image_gray;				// ����̹��� ���� ����
IplImage	*m_image_select;			// ���ÿ�������ũ
IplImage	*m_prev_grey;				// OpticalFlow�����
IplImage	*m_pyramid;					// OpticalFlow�����
IplImage	*m_prev_pyramid;			// OpticalFlow�����
IplImage	*swap_temp;					// OpticalFlow�����
IplImage	*m_chg_image_buff;			// ����̹���

CvPoint			origin;
CvRect			selection;				// �巡�׿���

int	select_object;						// �������� ����
int	track_object;						// Ʈ��ŷ ����

int win_size = 10;
const int MAX_COUNT = 500;
CvPoint2D32f* points_sel[2] = {0,0}, *swap_points;
char* status = 0;
int count = 0;
int flags = 0;

int m_in_height;
int m_in_width;

HANDLE m_mutex;							// ���� ������� �ڵ�
int m_state;							// ������ ���� �÷���

void on_mouse(int event, int x, int y, int flags, void* param ){
	if( m_chg_image_buff == NULL ){
        return;
	}

	if( m_chg_image_buff->origin == 1 ){
        y = m_chg_image_buff->height - y;
	}
	// �巡�� �ϰ� �ִ� ��ǥ�ݿ�
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
	// ���콺�̺�Ʈ
    switch( event ){
		case CV_EVENT_LBUTTONDOWN:
			// ���콺���ʹ�ư Ŭ��
			// ��������on
			origin = cvPoint( x, y );
			selection = cvRect( x, y, 0, 0 );
			select_object = SELECT_ON;
			track_object = TRACKING_STOP;
			break;
		case CV_EVENT_LBUTTONUP:
			// ���콺���˹�ư Ŭ������
			// ��������off Ʈ��ŷon
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
//	�������͸� CVWindowViewer�� ����ϴ� ������ �Լ� ����
//	�����۾� : Viewer ���� / ������ ��� / Viewer ����
//	����Ÿ�� : ReturnType(NONE(0))
//	���ǻ��� : ������ Dialog Box���·� ��µǴ� ��� �����带 ���
//
//*******************************************************************
UINT ImageFunc( LPVOID pParam )
{
	// ������ ����
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	// OutputImage��� Ÿ��Ʋ�� �� ����
	cvNamedWindow("OutputImage", CV_WINDOW_AUTOSIZE);
	// ���콺�̺�Ʈ �ݹ��Լ� ����
	cvSetMouseCallback("OutputImage", on_mouse, (void *)m_chg_image_buff);
	
	// �������͸� �� ���
	while (true) {
		// �������� ���� ���� ����
		::WaitForSingleObject(m_mutex, INFINITE);
		
		// ������ �⵿ �÷����� ���
		if (m_state == 1) {				
			
			// �������͸� �� Show
			if (m_chg_image_buff != NULL) {
				cvShowImage("OutputImage",m_chg_image_buff);
				cvWaitKey(1);
			}

			m_state = 0; // ready to get image
		}
		// ������ ���� �÷��� �ϰ��
		else if(m_state == 2) { // stop thread
			::ReleaseMutex(m_mutex);
			break;
		}

		// ����� �ڵ��� ������
		::ReleaseMutex(m_mutex);
			
		::Sleep(1);
	}
	
	// �����尡 ����ɶ� ����� ���� IplImage�� ������
	if(m_chg_image_buff != NULL)
		cvReleaseImage(&m_chg_image_buff);

	// ��� �������� ����
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
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex �ڵ� ����
	
	// �ڵ� ����ó��
	if (m_mutex == 0) {
		std::cout << "[ERROR-ImageOut::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}
	
	m_in_height = 0;			// ���� ���� ���� ���� �ʱ�ȭ
	m_in_width = 0;				// ���� ���� ���� ���� �ʱ�ȭ

	// ImageFunc ������ ����
	AfxBeginThread(ImageFunc, (LPVOID) this);

	// �����ʱ�ȭ
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
	// ������ ���� ����
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

	// ��������
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

		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���翵���� ũ�⸦ ���
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

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		cvCopy(m_image_buff, m_chg_image_buff);
		cvCvtColor(m_chg_image_buff, m_image_gray, CV_BGR2GRAY);

		if(track_object != TRACKING_STOP)
		{
			// ���ÿ��� ����ũ�� ����
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
				// ���úκи� ���ɿ��� ������ ���������� ä��
				cvSetImageROI( m_image_select, selection );
				cvSet(m_image_select, cvScalar(255));
				cvResetImageROI(m_image_select);

				/* automatic initialization */
				IplImage* eig = cvCreateImage( cvGetSize(m_image_gray), 32, 1 );
				IplImage* temp = cvCreateImage( cvGetSize(m_image_gray), 32, 1 );
				double quality = 0.01;
				double min_distance = 10;

				count = MAX_COUNT;
				// �ѷ��� �ڳ��� ����
				cvGoodFeaturesToTrack( m_image_gray, eig, temp, points_sel[1], &count,
                                   quality, min_distance, m_image_select, 3, 0, 0.04 );
				// �ڳ��� ����(��ȭ�Ҵ�����)
				cvFindCornerSubPix( m_image_gray, points_sel[1], count,
					cvSize(win_size, win_size), cvSize(-1,-1),
					cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
				cvReleaseImage( &eig );
				cvReleaseImage( &temp );

				track_object = TRACKING_NOW;
			}

			// ������ƮƮ��ŷ
			else if(track_object = TRACKING_NOW)
			{	
				int i,k;

				if(count > 0)
				{
					// ��ī�� ī���� �˰���
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
						// Ư¡���� ȭ�鿡 ǥ��
						cvCircle( m_chg_image_buff, cvPointFrom32f(points_sel[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);

						// Ư¡����ǥ����
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

		// ���콺�� �巡�׽� �ش翵�� ǥ��
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

