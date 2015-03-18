
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

#include "BirdsEyeView.h"

//
// constructor declaration
//

IplImage* m_orig_img;			// �Է� ����
IplImage* m_dest_img;			// ���� ��� ���� 
IplImage* m_bird_img;			// ������ ���� ��¿�
IplImage* m_gray_img;			// ��������

int m_in_width;
int m_in_height;

//ü���� ��ȯ�� ���� ������
int m_board_w;
int m_board_h;
int m_board_n;
CvSize m_board_sz;

//������
int m_Zvalue;


BirdsEyeView::BirdsEyeView()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
BirdsEyeView::BirdsEyeView(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

BirdsEyeView::~BirdsEyeView() {
}

void BirdsEyeView::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageOut2", &ImageOut2);

	addPort("ImageIn", &ImageIn);

	// export variable setup

}

// Call back Declaration
ReturnType BirdsEyeView::onInitialize()
{
	//�̹��� �� ���� �ʱ�ȭ
	m_orig_img		= NULL;
	m_dest_img		= NULL;
	m_bird_img		= NULL;
	m_gray_img		= NULL;

	m_in_width		= 0;
	m_in_height		= 0;

	//������ �ҷ�����
	m_Zvalue =   atoi(getProperty("Zvalue").c_str());

	// path
	m_cpath = getProperty("opros.component.dir");
	if (m_cpath == "") m_cpath = ".";

	//ü���� �Ķ����
	m_board_w = 10;
	m_board_h = 7;
	m_board_n = m_board_w * m_board_h;
	m_board_sz = cvSize(m_board_w, m_board_h);

	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType BirdsEyeView::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onDestroy()
{
	
	// �̹����� �޸��� ����
    cvReleaseImage(&m_orig_img);
	cvReleaseImage(&m_dest_img);
	cvReleaseImage(&m_bird_img);
	cvReleaseImage(&m_gray_img);

	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;
	RawImage result2;

	if(pData != NULL){
		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// ���������� �̹������� Ȯ��
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_dest_img == NULL){
			m_dest_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_bird_img == NULL){
			m_bird_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		cvCopy(m_orig_img, m_dest_img, 0);
		
		//����ȭ
		cvCvtColor( m_dest_img, m_gray_img, CV_BGR2GRAY);

		IplImage* m_map_x			= cvCreateImage(cvGetSize(m_orig_img), IPL_DEPTH_32F,1);
		IplImage* m_map_y			= cvCreateImage(cvGetSize(m_orig_img), IPL_DEPTH_32F,1);	

		//������ ���� �о�ͼ� ���
		std::string ip = m_cpath + "Intrinsics.xml";
		std::string dp = m_cpath + "Distortion.xml";

		CvMat* intrinsic = (CvMat*)cvLoad(ip.c_str());
		CvMat* distortion = (CvMat*)cvLoad(dp.c_str());

	//	CvMat* intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	//	CvMat* distortion = (CvMat*)cvLoad("Distortion.xml");

		//ī�޶� ���� ������ϰ� �ְ���� ���� �о���⿡ ����
		if(intrinsic !=NULL && distortion !=NULL)
		{
			//�о���� ������ �̿��Ͽ� �ְ� ���Ÿ� ���� ������ ����
			cvInitUndistortMap(intrinsic, distortion, m_map_x, m_map_y);
			
			IplImage* m_temp = cvCloneImage(m_dest_img);
			
			//�ְ� ����
			cvRemap(m_temp, m_dest_img , m_map_x, m_map_y);			

			//���� �ʱ�ȭ
			CvPoint2D32f* corners = new CvPoint2D32f[m_board_n];
			int corner_count = 0;
			int found = 0;

			// ü���� �ڳ� ã��
			found = cvFindChessboardCorners(m_dest_img, m_board_sz, corners, &corner_count, 
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

			// ����� �ڳʷ� ���� �����ȼ� ��Ȯ���� �ڳ� ��ǥ�� ����
			cvFindCornerSubPix(m_gray_img, corners, corner_count,cvSize(11, 11), cvSize(-1, -1),
						cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			//���� ��ü ��ǥ�� ���󿡼� ����� �ڳ��� ������Ű��
			CvPoint2D32f objPts[4], imgPts[4];

			objPts[0].x = 0;	
			objPts[0].y = 0;

			objPts[1].x = m_board_w - 1;	
			objPts[1].y = 0;

			objPts[2].x = 0;	
			objPts[2].y = m_board_h - 1;

			objPts[3].x = m_board_w - 1;	
			objPts[3].y = m_board_h - 1;

			imgPts[0] = corners[0];
			imgPts[1] = corners[m_board_w - 1];
			imgPts[2] = corners[(m_board_h - 1)*m_board_w];
			imgPts[3] = corners[(m_board_h - 1)*m_board_w + m_board_w - 1];

			//�Ķ�,���,������,����������� ���� �׸���.
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[0]), 9, CV_RGB(0,0,255), 3); 
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[1]), 9, CV_RGB(0,255,0), 3); 
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[2]), 9, CV_RGB(255,0, 0), 3); 
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[3]), 9, CV_RGB(255,255,0), 3);

			//����� ü���� �׸���
			cvDrawChessboardCorners( m_orig_img, m_board_sz, corners, corner_count, found );

			//ȣ��׷��� ���ϱ�
			CvMat* H = cvCreateMat(3, 3, CV_32F);
			cvGetPerspectiveTransform( objPts, imgPts, H);

			//���� ������ ����
			float Z = (float)m_Zvalue;
			IplImage* m_temp_img		= cvCloneImage(m_orig_img);

			//���� ���� - ���Ǳ׷��̼����� ���� ����
			CV_MAT_ELEM(*H, float, 2, 2) = Z;
			
			//ȣ��׷��Ǹ� �̿��Ͽ� ���� �Ǵ� ������ ���
			cvWarpPerspective( m_orig_img, m_temp_img, H,
				CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS );

			cvCopy(m_temp_img, m_bird_img, 0);	//��¿�

			//����� ���� �޸� ����
			cvReleaseImage(&m_temp);
			cvReleaseImage(&m_temp_img);
			cvReleaseMat(&H);
		}
		else
		{	
			//���� ������ ������ ����ȭ�� ��� �� �ܼ�â�� �޽��� ���...
			cvSetZero(m_bird_img);
			std::cout<<" Could not found camera data file!!!"<< std::endl;
		}


//// ImageOut 1
		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_orig_img->width, m_orig_img->height, m_orig_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_orig_img->width * m_orig_img->height * m_orig_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_orig_img->imageData, size);

		// ��Ʈ�ƿ�
		//opros_any mdata = result;
		ImageOut.push(result);//����


//// ImageOut 2
		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage2 = result2.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage2->resize(m_bird_img->width, m_bird_img->height, m_bird_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size2 = m_bird_img->width * m_bird_img->height * m_bird_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata2 = pimage2->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata2, m_bird_img->imageData, size2);

		// ��Ʈ�ƿ�
		opros_any mdata2 = result2;
		ImageOut2.push(result2);//����
		
		delete pData;

		//����� ���� �޸� ����
		cvReleaseImage(&m_map_x);
		cvReleaseImage(&m_map_y);
	}

	return OPROS_SUCCESS;
}
	
ReturnType BirdsEyeView::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onPeriodChanged()
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
	return new BirdsEyeView();
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
	return new BirdsEyeView();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

