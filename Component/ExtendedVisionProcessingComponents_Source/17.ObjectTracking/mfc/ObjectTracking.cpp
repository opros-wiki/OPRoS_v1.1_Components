
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

#define		H_DIMENSION		16			//	������׷�����
#define		H_RANGE_MIN		0			//  hsv�� h���ּҰ�
#define		H_RANGE_MAX		180			//  hsv�� hä���ּҰ�
#define		V_MIN	10					//	hsv�� vä���ּҰ�
#define		V_MAX	256					//	hsv�� vä���ִ밪
#define		S_MIN	30					//	hsv�� sä���ּҰ�
#define		S_MAX	256					//	hsv�� sä���ִ밪
#define		SELECT_OFF				0	//	��������off
#define		SELECT_ON				1	//	��������on
#define		TRACKING_STOP			0	//	Ʈ��ŷ����
#define		TRACKING_START			-1	//	Ʈ��ŷ����
#define		TRACKING_NOW			1	//	Ʈ��ŷ������

//
// constructor declaration
//
IplImage	*m_image_buff;			// �Է��̹��� ���� ����
IplImage	*m_image_hsv;			// hsv�̹��� ���� ����
IplImage	*m_image_hue;			// hsv���� hue�� ���� ����
IplImage	*m_image_mask;			// ����ũ�̹��� ���� ����
IplImage	*backprojectImage;
IplImage	*m_image_gray;			// ����̹��� ���� ����
IplImage	*m_chg_image_buff;		// ����̹���

CvPoint		origin;
CvRect		selection;				// �巡�׿���

int	select_object;					// �������� ����
int	track_object;					// Ʈ��ŷ ����

CvHistogram	*hist;		

//	CamShift�� �̿�� Ʈ��ŷ��������
CvRect			track_window;
CvBox2D			track_box;
CvConnectedComp	track_comp;

int hdims;
float hRangesArray[2];
float *hRanges;

int m_in_height;
int m_in_width;

HANDLE m_mutex;				// ���� ������� �ڵ�
int m_state;				// ������ ���� �÷���

// ������׷����
void CalculateHist( CvHistogram *hist, IplImage *m_image_hsv, IplImage *m_image_mask, CvRect selection );
// �÷���ȯ
CvScalar hsv2rgb( float hue );

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
ReturnType ObjectTracking::onInitialize()
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

	// Ʈ��ŷ���°���
	select_object = SELECT_OFF;
	track_object = TRACKING_STOP;

	// ������׷�����
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

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		cvCopy(m_image_buff, m_chg_image_buff);
		// ����ȯ BGR->HSV
		cvCvtColor(m_image_buff, m_image_hsv, CV_BGR2HSV);

		bool data = false;
		if(track_object != TRACKING_STOP)
		{	
			CalculateHist(hist, m_image_hsv, m_image_mask, selection );
			// HUEä���̹����� 
			cvCalcBackProject(&m_image_hue, backprojectImage, hist);
			cvAnd(backprojectImage, m_image_mask, backprojectImage, 0);
			// ��ü�� �߽����� ũ�⸦ ���� ����
			cvCamShift( backprojectImage, track_window,
						cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1),
						&track_comp, &track_box);

			// ��üŽ�� �������� ũ�Ⱑ �ǽð����� ������Ʈ
			track_window = track_comp.rect;

			// ��ü����ǥ��
			CvPoint pt;
			pt = cvPoint(track_window.x, track_window.y);
			cvDrawRect(	m_chg_image_buff, 
						pt, 
						cvPoint(pt.x + track_window.width, pt.y + track_window.height), 
						CV_RGB(255,255,0),
						3);

			// ��ü��������
			std::vector<PositionDataType> data;
			PositionDataType base("", NULL, NULL, NULL, NULL, NULL);
			base.setX(pt.x);
			base.setY(pt.y);
			base.setWidth(track_window.width);
			base.setHeight(track_window.height);
			data.push_back(base);
			PositionOut.push(data);

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

// ������׷� ���
void CalculateHist( CvHistogram	*hist, IplImage *m_image_hsv, IplImage *m_image_mask, CvRect selection ){
	float	max_val;

	cvInRangeS(m_image_hsv, 
				   cvScalar(H_RANGE_MIN, S_MIN, MIN(V_MIN, V_MAX), 0),
				   cvScalar(H_RANGE_MAX, S_MAX, MAX(V_MIN, V_MAX), 0), 
				   m_image_mask);

	// HSVä�κи�
	cvSplit(m_image_hsv, m_image_hue, NULL, NULL, NULL);

	// ���ɿ����� ������׷� �м�
	//if(track_object < TRACKING_START)
	if(track_object < 0)
	{
		max_val = 0.0f;
		cvSetImageROI( m_image_hue, selection );
		cvSetImageROI( m_image_mask, selection );
		// m_image_mask�� ���ɿ������� ������׷� ��� ��, hist�� ����
		cvCalcHist( &m_image_hue, hist, 0, m_image_mask );
		cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
		if( max_val == 0.0f ){
			cvConvertScale( hist->bins, hist->bins, 0.0, 0 );
		} else{
			cvConvertScale( hist->bins, hist->bins, 255.0 / max_val, 0 );
		}

		// �̹��� ��ü�� ���ɿ������� �缳��
		cvResetImageROI( m_image_hue );
		cvResetImageROI( m_image_mask );

		track_window = selection;
		track_object = TRACKING_NOW;		
	}

}

// �÷���ȯ
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

