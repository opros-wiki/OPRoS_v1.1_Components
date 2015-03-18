
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

IplImage* m_orig_img;			// 입력 영상
IplImage* m_dest_img;			// 최종 출력 영상 
IplImage* m_bird_img;			// 조감도 영상 출력용
IplImage* m_gray_img;			// 이진영상

int m_in_width;
int m_in_height;

//체스판 변환에 쓰일 변수들
int m_board_w;
int m_board_h;
int m_board_n;
CvSize m_board_sz;

//설정값
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
	//이미지 및 변수 초기화
	m_orig_img		= NULL;
	m_dest_img		= NULL;
	m_bird_img		= NULL;
	m_gray_img		= NULL;

	m_in_width		= 0;
	m_in_height		= 0;

	//설정값 불러오기
	m_Zvalue =   atoi(getProperty("Zvalue").c_str());

	// path
	m_cpath = getProperty("opros.component.dir");
	if (m_cpath == "") m_cpath = ".";

	//체스판 파라메터
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
	
	// 이미지용 메모리의 해제
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
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;
	RawImage result2;

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
		if(m_dest_img == NULL){
			m_dest_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_bird_img == NULL){
			m_bird_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		cvCopy(m_orig_img, m_dest_img, 0);
		
		//이진화
		cvCvtColor( m_dest_img, m_gray_img, CV_BGR2GRAY);

		IplImage* m_map_x			= cvCreateImage(cvGetSize(m_orig_img), IPL_DEPTH_32F,1);
		IplImage* m_map_y			= cvCreateImage(cvGetSize(m_orig_img), IPL_DEPTH_32F,1);	

		//데이터 파일 읽어와서 담기
		std::string ip = m_cpath + "Intrinsics.xml";
		std::string dp = m_cpath + "Distortion.xml";

		CvMat* intrinsic = (CvMat*)cvLoad(ip.c_str());
		CvMat* distortion = (CvMat*)cvLoad(dp.c_str());

	//	CvMat* intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	//	CvMat* distortion = (CvMat*)cvLoad("Distortion.xml");

		//카메라 내부 행렬파일과 왜곡행렬 파일 읽어오기에 성공
		if(intrinsic !=NULL && distortion !=NULL)
		{
			//읽어들인 파일을 이용하여 왜곡 제거를 위한 지도를 구성
			cvInitUndistortMap(intrinsic, distortion, m_map_x, m_map_y);
			
			IplImage* m_temp = cvCloneImage(m_dest_img);
			
			//왜곡 제거
			cvRemap(m_temp, m_dest_img , m_map_x, m_map_y);			

			//변수 초기화
			CvPoint2D32f* corners = new CvPoint2D32f[m_board_n];
			int corner_count = 0;
			int found = 0;

			// 체스판 코너 찾기
			found = cvFindChessboardCorners(m_dest_img, m_board_sz, corners, &corner_count, 
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

			// 검출된 코너로 부터 서브픽셀 정확도의 코너 좌표를 구함
			cvFindCornerSubPix(m_gray_img, corners, corner_count,cvSize(11, 11), cvSize(-1, -1),
						cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			//실제 객체 좌표와 영상에서 검출된 코너점 대응시키기
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

			//파랑,녹색,빨간색,노란색순서로 점을 그린다.
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[0]), 9, CV_RGB(0,0,255), 3); 
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[1]), 9, CV_RGB(0,255,0), 3); 
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[2]), 9, CV_RGB(255,0, 0), 3); 
			cvCircle(m_orig_img, cvPointFrom32f(imgPts[3]), 9, CV_RGB(255,255,0), 3);

			//검출된 체스판 그리기
			cvDrawChessboardCorners( m_orig_img, m_board_sz, corners, corner_count, found );

			//호모그래피 구하기
			CvMat* H = cvCreateMat(3, 3, CV_32F);
			cvGetPerspectiveTransform( objPts, imgPts, H);

			//높이 설정값 저장
			float Z = (float)m_Zvalue;
			IplImage* m_temp_img		= cvCloneImage(m_orig_img);

			//높이 설정 - 컨피그래이션으로 변경 가능
			CV_MAT_ELEM(*H, float, 2, 2) = Z;
			
			//호모그래피를 이용하여 정면 또는 조감도 계산
			cvWarpPerspective( m_orig_img, m_temp_img, H,
				CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS );

			cvCopy(m_temp_img, m_bird_img, 0);	//출력용

			//사용이 끝난 메모리 해제
			cvReleaseImage(&m_temp);
			cvReleaseImage(&m_temp_img);
			cvReleaseMat(&H);
		}
		else
		{	
			//읽은 파일이 없으면 검은화면 출력 후 콘솔창에 메시지 출력...
			cvSetZero(m_bird_img);
			std::cout<<" Could not found camera data file!!!"<< std::endl;
		}


//// ImageOut 1
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
		//opros_any mdata = result;
		ImageOut.push(result);//전달


//// ImageOut 2
		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage2 = result2.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage2->resize(m_bird_img->width, m_bird_img->height, m_bird_img->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size2 = m_bird_img->width * m_bird_img->height * m_bird_img->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata2 = pimage2->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata2, m_bird_img->imageData, size2);

		// 포트아웃
		opros_any mdata2 = result2;
		ImageOut2.push(result2);//전달
		
		delete pData;

		//사용이 끝난 메모리 해제
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

