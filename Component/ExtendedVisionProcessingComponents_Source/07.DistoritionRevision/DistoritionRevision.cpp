
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

#include "DistoritionRevision.h"

IplImage* m_orig_img;
IplImage* m_result_img;
IplImage* m_gray_img;
IplImage* m_map_x;
IplImage* m_map_y;

CvMat* m_image_points;
CvMat* m_object_points;
CvMat* m_point_counts;
CvMat* m_intrinsic_matrix;
CvMat* m_distortion_coeffs;

CvPoint2D32f* corners;

CvSize m_board_sz;

int m_in_width;
int m_in_height;

int m_board_w;
int m_board_h;
int m_n_board;
int m_board_n;
int m_board_dt;
int corner_count;
int successes;
int m_frame;
int m_exeCount;
int step;

bool successesFlag;
bool m_firstFlag;
bool saveFlag;

//
// constructor declaration
//
DistoritionRevision::DistoritionRevision()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
DistoritionRevision::DistoritionRevision(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

DistoritionRevision::~DistoritionRevision() {
}

void DistoritionRevision::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType DistoritionRevision::onInitialize()
{
	
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_map_x != NULL)
		m_map_x = NULL;
	if(m_map_y != NULL)
		m_map_y = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_board_w = 10;
	m_board_h = 7;
	m_n_board = 9;
	m_board_n = m_board_w * m_board_h;
	m_board_dt = 20;
	m_frame = 0;
	m_exeCount = 0;

	m_board_sz = cvSize(m_board_w, m_board_h);

	if(m_image_points != NULL)
		m_image_points = NULL;
	if(m_object_points != NULL)
		m_object_points = NULL;
	if(m_point_counts != NULL)
		m_point_counts = NULL;
	if(m_intrinsic_matrix != NULL)
		m_intrinsic_matrix = NULL;
	if(m_distortion_coeffs != NULL)
		m_distortion_coeffs = NULL;

	corners = new CvPoint2D32f[m_board_n];

	successesFlag = false;
	m_firstFlag = true;
	saveFlag = false;

	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType DistoritionRevision::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onDestroy()
{
	
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_map_x != NULL)
		cvReleaseImage(&m_map_x);
	if(m_map_y != NULL)
		cvReleaseImage(&m_map_y);

	if(m_image_points != NULL)
		cvReleaseMat(&m_image_points);
	if(m_object_points != NULL)
		cvReleaseMat(&m_object_points);
	if(m_point_counts != NULL)
		cvReleaseMat(&m_point_counts);
	if(m_intrinsic_matrix != NULL)
		cvReleaseMat(&m_intrinsic_matrix);
	if(m_distortion_coeffs != NULL)
		cvReleaseMat(&m_distortion_coeffs);

	m_firstFlag = false;

	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onExecute()
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
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height),
				IPL_DEPTH_8U, 3);
		}
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height),
				IPL_DEPTH_8U, 1);
		}
		if(m_map_x == NULL){
			m_map_x = cvCreateImage(cvSize(m_in_width, m_in_height),
				IPL_DEPTH_32F, 1);
		}
		if(m_map_y == NULL){
			m_map_y = cvCreateImage(cvSize(m_in_width, m_in_height),
				IPL_DEPTH_32F, 1);
		}

		if(m_image_points == NULL){
			m_image_points = cvCreateMat(m_n_board * m_board_n, 2, CV_32FC1);
		}
		if(m_object_points == NULL){
			m_object_points = cvCreateMat(m_n_board * m_board_n, 3, CV_32FC1);
		}
		if(m_point_counts == NULL){
			m_point_counts = cvCreateMat(m_n_board, 1, CV_32SC1);
		}
		if(m_intrinsic_matrix == NULL){
			m_intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1);
		}
		if(m_distortion_coeffs == NULL){
			m_distortion_coeffs = cvCreateMat(4, 1, CV_32FC1);
		}

		if(m_firstFlag == true){
			m_frame = 0;
			successes = 0;
			step = 0;
			successesFlag = false;
			m_exeCount = 0;

			m_firstFlag = false;
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		if(successes < m_n_board){
		
			if(m_frame++ % m_board_dt == 0){

				// 체스판 코너 찾기
				int found = cvFindChessboardCorners(m_orig_img, m_board_sz, corners,
									&corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | 
									CV_CALIB_CB_FILTER_QUADS);

				// 검출된 코너로 부터 서브픽셀 정확도의 코너 좌표를 구함
				cvCvtColor(m_orig_img, m_gray_img, CV_BGR2GRAY);
				cvFindCornerSubPix(m_gray_img, corners, corner_count,
					cvSize(11, 11), cvSize(-1, -1),
					cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

				// 코너를 화면에 출력
				cvDrawChessboardCorners(m_orig_img, m_board_sz, corners,
										corner_count, found);

				//정상적 코너 영상이면 데이터를 저장
				if(corner_count == m_board_n){
				
					step = successes * m_board_n;

					for(int i = step, j = 0; j < m_board_n; ++i, ++j){
					
						CV_MAT_ELEM(*m_image_points, float, i, 0) = corners[j].x;
						CV_MAT_ELEM(*m_image_points, float, i, 1) = corners[j].y;
						CV_MAT_ELEM(*m_object_points, float, i, 0) = j / m_board_w;
						CV_MAT_ELEM(*m_object_points, float, i, 1) = j % m_board_w;
						CV_MAT_ELEM(*m_object_points, float, i, 2) = 0.0f;
					
					}

					CV_MAT_ELEM(*m_point_counts, int, successes, 0) = m_board_n;
					successes++;

				}
			}
		
		}

		// 칼리브레이션이 끝났다면
		if(successes == m_n_board){
		
			successesFlag = true;
		
		}

		if(successesFlag == true && m_exeCount == 0){
		
			// 검출된 코너 갯수에 맞게끔 행렬 할당
			CvMat* object_points2 = cvCreateMat(successes * m_board_n, 3, CV_32FC1);
			CvMat* image_points2 = cvCreateMat(successes * m_board_n, 2, CV_32FC1);
			CvMat* point_counts2 = cvCreateMat(successes, 1, CV_32SC1);
			
			// 검출점을 행렬로
			for(int i = 0; i < successes * m_board_n; i++){
			
				CV_MAT_ELEM(*image_points2, float, i, 0) = 
					CV_MAT_ELEM(*m_image_points, float, i, 0);
				CV_MAT_ELEM(*image_points2, float, i, 1) =
					CV_MAT_ELEM(*m_image_points, float, i, 1);
				CV_MAT_ELEM(*object_points2, float, i, 0) =
					CV_MAT_ELEM(*m_object_points, float, i, 0);
				CV_MAT_ELEM(*object_points2, float, i, 1) =
					CV_MAT_ELEM(*m_object_points, float, i, 1);
				CV_MAT_ELEM(*object_points2, float, i, 2) =
					CV_MAT_ELEM(*m_object_points, float, i, 2);
			
			}

			for(int i = 0; i < successes; i++){
			
				CV_MAT_ELEM(*point_counts2, int, i, 0) = 
					CV_MAT_ELEM(*m_point_counts, int, i, 0);
			
			}

			// 초점 거리 비율을 1.0으로 설정하여 내부행렬 초기화
			CV_MAT_ELEM(*m_intrinsic_matrix, float, 0, 0) = 1.0f;
			CV_MAT_ELEM(*m_intrinsic_matrix, float, 1, 1) = 1.0f;

			// 칼리브레이션 실행
			cvCalibrateCamera2(object_points2, image_points2,
							point_counts2, cvGetSize(m_orig_img),
							m_intrinsic_matrix, m_distortion_coeffs,
							NULL, NULL, CV_CALIB_FIX_ASPECT_RATIO);

			// 내부행렬 및 왜곡계수를 파일로 출력
			if(saveFlag == false){
				std::string ipath = getProperty("opros.component.dir") + "Intrinsics.xml";
				std::string dpath = getProperty("opros.component.dir") + "Distortion.xml";

				cvSave(ipath.c_str(), m_intrinsic_matrix);
				cvSave(dpath.c_str(), m_distortion_coeffs);
				
				saveFlag = true;
			}
			
			cvInitUndistortMap(m_intrinsic_matrix, m_distortion_coeffs, 
							m_map_x, m_map_y);

			cvReleaseMat(&object_points2);
			cvReleaseMat(&image_points2);
			cvReleaseMat(&point_counts2);
			
			m_exeCount = 1;
		}

		if(successesFlag == true){
		
			cvCopy(m_orig_img, m_result_img, NULL);
			cvRemap(m_result_img, m_orig_img, m_map_x, m_map_y);
			cvCopy(m_orig_img, m_result_img, NULL);
		
		}else{
			
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
	
ReturnType DistoritionRevision::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onPeriodChanged()
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
	return new DistoritionRevision();
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
	return new DistoritionRevision();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

