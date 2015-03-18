
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



#include "ColorTracking.h"

//
// constructor declaration
//

IplImage* m_orig_img;			// 입력 영상
IplImage* m_dest_img;			// 처리 영상
IplImage* m_hsv_img;			// hsv처리
IplImage* m_gray_img;			// 1채널 영상

int m_in_width;
int m_in_height;

std::string m_color;			// 설정값 저장	
int circle_cnt, rect_cnt;		// 총 검출된 수를 저장할 곳

//출력을 위한 선언
CvRect* rects;					// 검출된 사각형 윤곽을 사각형 구조체에 담는다
CvSeq* circles;					// 검출된 원 정보
CvMemStorage* storage0 ;		// 데이터 출력을 위해 전역 메모리 변수 하나 필요


ColorTracking::ColorTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ColorTracking::ColorTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ColorTracking::~ColorTracking() {
}

void ColorTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);

	// export variable setup
}

// Call back Declaration
ReturnType ColorTracking::onInitialize()
{
	//이미지 변수 초기화
	m_orig_img		= NULL;
	m_dest_img		= NULL;
	m_hsv_img		= NULL;
	m_gray_img		= NULL;

	m_in_width		= 0;
	m_in_height		= 0;

	//설정값 읽기
	m_color =  getProperty("TrackingColor");

	// 데이터 아웃시 필요한 변수 초기화
	circle_cnt = 0;					// 검출된 원의 갯수
	rect_cnt = 0;					// 검출된 사각형의 갯수
	circles = NULL;					// 검출된 원을 담을 시퀸스 구조체 
	rects = NULL;					// 검출된 사각형을 담을 렉트 구조체


	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ColorTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onDestroy()
{
	// 이미지용 메모리의 해제
    cvReleaseImage(&m_orig_img);
	cvReleaseImage(&m_dest_img);
	cvReleaseImage(&m_hsv_img);
	cvReleaseImage(&m_gray_img);

	cvReleaseMemStorage(&storage0);

	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;
	// 데이터 포트 백터
	std::vector<PositionDataType> data;

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
		if(m_hsv_img == NULL){
			m_hsv_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		}
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		//영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//HSV변환
		cvCvtColor(m_orig_img, m_hsv_img, CV_BGR2HSV);

		//hsv 영역 축소 후, 설정값에 따라 해당 영역 이진 영상 추출
		color_config(m_hsv_img, m_color);
		
		//영상정리
		image_filter(m_gray_img);

		//검출 갯수 담을 변수 초기화
		circle_cnt = 0;
		rect_cnt = 0;
		
		//검출된 원을 위한 메모리 공간 할당
		storage0 = cvCreateMemStorage(0);
		
		//원 그리기
		draw_circle(m_gray_img);
		
		//사각형 그리기
		draw_square(m_gray_img);

//// DataOut
		//한가지라도 검출되면 
		if(circles || rects != NULL)
		{
			//원 데이터가 존재함
			if(circles)
			{
				//원의 갯수만큼 
				for(int k = 0; k < circles->total; k++)
				{
					float* cir;
					int circle_x, circle_y;
					double radi;

					//검출된 원을 저장한 circles에서 원의 파라미터를 cir에 저장
					//원의 중심 좌표 및 반지름이 배열에 순서대로 저장됨
					cir = (float*)cvGetSeqElem(circles, k);
					//검출된 원을 저장한 circles에서 원의 파라미터를 cir에 저장
					//원의 중심 좌표 및 반지름이 배열에 순서대로 저장됨
					circle_x    = cvRound(cir[0]);     //중심점 x 좌표
					circle_y    = cvRound(cir[1]);     //중심점 y 좌표
					radi        = (double)cvRound(cir[2]);     //반지름

					PositionDataType base;
					base.setName("circle");
					base.setX(circle_x);
					base.setY(circle_y);
					base.setRadian(radi);

					base.setHeight(NULL);
					base.setWidth(NULL);

					data.push_back(base);
			
				}

			}

			//사각형 데이터가 존재함
			if(rects != NULL)
			{
				for(int j = 0; j < rect_cnt; j++)
				{
					int rect_x, rect_y, rect_width, rect_height;

					rect_x = rects[j].x;
					rect_y = rects[j].y;
					rect_width = rects[j].width;
					rect_height = rects[j].height;

					PositionDataType base;
					base.setName("rectangle");
					base.setX(rect_x);
					base.setY(rect_y);
					base.setHeight(rect_height);
					base.setWidth(rect_width);

					base.setRadian(NULL);

					data.push_back(base);
				}
			}

			PositionOut.push(data);
		}
			
//// ImageOut
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

	return OPROS_SUCCESS;
}
	
ReturnType ColorTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}




//영역 추출 후 이진화
void ColorTracking::color_config(IplImage* image, std::string config){
	
	//추출할 영역 변수
	CvScalar hsv_min, hsv_max, hsv_min2, hsv_max2;
	if(image != NULL)
	{
		IplImage* m_tem1_img = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);//영역 추출 이미지
		IplImage* m_tem2_img = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);//영역 추출 이미지
	
		//필요한 색상 영역 으로 축소
		if(config == "Red")
		{
			//빨강 - 영역 두개로 잡아봄
			hsv_min = cvScalar(0, 85, 100, 0);
			hsv_max = cvScalar(10, 255, 255, 0);

			hsv_min2 = cvScalar(170, 85, 100, 0);
			hsv_max2 = cvScalar(220, 255, 255, 0);

		}
		else if(config =="Green")
		{
			//초록
			hsv_min = cvScalar(55, 80, 100, 0);
			hsv_max = cvScalar(75, 255, 255, 0);
		}
		else if(config == "Blue")
		{
			//파랑
			hsv_min = cvScalar(100, 100,100 , 0);
			hsv_max = cvScalar(130, 255, 200, 0);
		}
		else if(config =="Yellow")
		{
			//노랑
			hsv_min = cvScalar(20, 100, 100, 0);
			hsv_max = cvScalar(35, 255, 255, 0);
		}
		
		if(config == "Red")
		{
			//일단 레드는 두 영역으로 잡아봄
			cvInRangeS(image, hsv_min, hsv_max, m_tem1_img);
			cvInRangeS(image, hsv_min2, hsv_max2, m_tem2_img);

			//공통영역 추출
			cvOr(m_tem1_img, m_tem2_img , m_gray_img);
			
		}	
		else
		{
			//레드가 아닐 때는 그대로
			cvInRangeS(image, hsv_min, hsv_max, m_gray_img);
		}

		cvReleaseImage(&m_tem1_img);
		cvReleaseImage(&m_tem2_img);
	}
}

//////////////////////
//
//    필터 적용
//
//////////////////////
void ColorTracking::image_filter(IplImage* image)
{

	cvDilate(image, image, NULL, 2);//팽창
	cvErode(image, image, NULL, 2);//침식	
		
	//가우시안 필터 적용
	cvSmooth( image, image, CV_GAUSSIAN, 9, 9 );

}


//////////////////////
//
//   원 검출
//
//////////////////////
void ColorTracking::draw_circle(IplImage* image)
{
	
	CvSeq* m_circle = NULL;					// 원 정보
	CvMemStorage* storage1 = NULL;			// 메모리 할당

	//검출된 원을 위한 메모리 공간 할당
	storage1 = cvCreateMemStorage(0);

	//원 갯수 저장 변수
	circle_cnt = 0;

	//원의 중심점을 검출하기 위한 누산기의 해상도
	// 1이면 입력영상과 같은 크기, 2이면 입력 영상의 가로/세로의 반크기의 누산기
	double dp = 1.5; 
	double min_dist = 300;				//검출된 원의 중심 사이의 최소거리.작을 수록 많은 원이 검출 됨-? 
	double cannyThreshold = 100;		//cvCanny 함수 임계값
	double accThreshold = 50;			//cvCanny 함수 축적평면의 임계값
	int min_radius = 50;				//최소 반지름
	int max_radius = 150;				//최대 반지름
	int cx, cy = 0;
		
	//소스영상, 메모리스토리지 포인터, 메소드인자, 영상의 해상도, 인접한 두 원사이의 최소거리
	m_circle = cvHoughCircles(image, storage1, CV_HOUGH_GRADIENT,
		dp,min_dist, cannyThreshold, accThreshold, min_radius, max_radius);

		//원이 1개 라도 있으면
	if(m_circle->total >0 )
	{
		// 데이터를 내보내기 위한 전역 선언한 시퀸스로 복사
		circles = cvCloneSeq(m_circle, storage0);

		//원 그리기
		for(int k = 0; k < m_circle->total; k++)
		{
			float* circle;
			int radius;

			//검출된 원을 저장한 circles에서 원의 파라미터를 circle에 저장
			//원의 중심 좌표 및 반지름이 배열에 순서대로 저장됨
			circle = (float*)cvGetSeqElem(m_circle, k);
			cx     = cvRound(circle[0]);     //중심점 x 좌표
			cy     = cvRound(circle[1]);     //중심점 y 좌표
			radius = cvRound(circle[2]);     //반지름

			//원그리기
			
			if(radius > min_radius && radius < max_radius)
			{
				//중심점
				cvCircle(m_orig_img, cvPoint(cx, cy), 3, CV_RGB(240,0,255), -1, 8, 0);

				//검출라인
				cvCircle(m_orig_img, cvPoint(cx, cy), radius, CV_RGB(255,255,255), 3, 8, 0);

			}	
		}
	}
	else // 원이 없으면
	{
		circles = NULL;
		circle_cnt = 0;
	}

	cvReleaseMemStorage(&storage1);
}

//////////////////////
//
//   사각형 검출
//
//////////////////////
void ColorTracking::draw_square(IplImage* image)
{
	CvMemStorage* storage2 = NULL;			// 메모리 할당
	CvMemStorage* storage3 = NULL;			// 메모리 할당
	storage2 = cvCreateMemStorage(0);		//사각형 검출을 위한 윤곽선 추출 메모리
	storage3 = cvCreateMemStorage(0);		//검출된 사각형을 위한 메모리

	CvSeq* contours ;						//윤곽선						
	CvSeq* result ;							//윤곽선 중에 사각형이 될만한 윤곽선 저장
	
	CvPoint corner[4];						//사각형이 될 네 점

	rects = NULL;							//사각형이 될 네 점으로 만들 사각형 구조체 - 데이터 내보내기 위해 사용

	rects = new CvRect [100];				//사각형의 갯수를 미리 알 수없기에 제한을 둠
		
	//윤곽선추출
	cvFindContours(image, storage2, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	//외곽선 검출 정보로 사각형 잇기
	for(;contours !=0; contours = contours ->h_next)
	{
		//꼭지점 근사화
		//cvApproxPoly 외곽선정보, 크기, 메모리, 메소드, 정밀도, 0이면 해당 외곽선만 검사
		result = cvApproxPoly( contours, sizeof(CvContour), storage3, 
			CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );	

		//꼭지점이 4개, 외곽선픽셀수가 1000개 이상 일때 사각형으로 간주
		//컨백스 헐은 일단제외 cvCheckContourConvexity(result)
		if( result->total == 4 &&
					cvContourArea(result,CV_WHOLE_SEQ,0) > 500)
		{

			//초기 위치 설정
			CvPoint* st = (CvPoint*)cvGetSeqElem(result, 0);

			///////첫번째 꼭짓점 추출 - 임의의 점에서 가장 먼 점
			double fMaxDist = 0.0;

			for(int i = 1; i < result->total; i++)
			{
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, i);

				double fDist = sqrt((double)( ( st->x - pt->x) * (st->x - pt->x)
					+ (st->y - pt->y) * (st->y - pt->y) ));

				if(fDist > fMaxDist)
				{
					corner[0] = *pt;
					fMaxDist = fDist;
				}
			}

			///////두번째 꼭짓점 추출 - 첫번째 점에서 가장 먼 점
			fMaxDist = 0.0;

			for(int i = 1; i < result->total; i++)
			{
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, i);

				double fDist = sqrt((double)( ( corner[0].x - pt->x) * (corner[0].x - pt->x)
					+ (corner[0].y - pt->y) * (corner[0].y - pt->y) ));

				if(fDist > fMaxDist)
				{
					corner[1] = *pt;
					fMaxDist = fDist;
				}
			}

			////////세번째 꼭짓점 추출 - 첫번째 점과 두번째 점에서 가장 먼 점
			fMaxDist = 0.0;
			
			for(int i = 1; i < result->total; i++)
			{
				
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, i);

				int tempx;
				int tempy;

				//첫번째 점과 두번째 사이의 중간 점 찾기 
				//좌표는 음수가 있을 수가 있어서 큰 숫자 판별후에 계산 함
				
				//x좌표
				if(corner[0].x >= corner[1].x)
				{
					
					tempx = corner[0].x - (corner[0].x - corner[1].x) / 2;
				
				}
				else if(corner[0].x < corner[1].x)
				{
					
					tempx = corner[0].x + (corner[1].x - corner[0].x) / 2;
				
				}

				//y좌표
				if(corner[0].y >= corner[1].y)
				{

					tempy = corner[0].y - (corner[0].y - corner[1].y) / 2;

				}
				else if(corner[0].y < corner[1].y)
				{

					tempy = corner[0].y - (corner[1].y - corner[0].y) / 2;
		
				}

				//구한 중간점에서 가장 먼 점 찾기
				double fDist = sqrt((double)( ( tempx - pt->x) * (tempx - pt->x)
					+ (tempy - pt->y) * (tempy - pt->y) ));

				if(fDist > fMaxDist)
				{
					corner[2] = *pt;
					fMaxDist = fDist;

				}
			}

			//////네번째 꼭짓점 추출
			
			//벡터 내적을 이용하여 좌표평면에서 사각형의 너비의 최대값을 구하는 계산식 사용
			//이미 구한 세 개의 꼭지점과 나머지 한점, 즉 윤곽선 상의 모든 점(x,y)을 세 개의
			//삼각형으로나눈 뒤 그 넓이의 합이 최대가 되는 점을 네 번째 꼭지점으로 삼음.

			int x1 = corner[0].x;
			int y1 = corner[0].y;

			int x2 = corner[1].x;
			int y2 = corner[1].y;
			
			int x3 = corner[2].x;
			int y3 = corner[2].y;

			int nMaxDim = 0;

			for(int j = 0; j < result->total; j++ )
			{
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, j);

				int x = pt->x;
				int y = pt->y;

				int nDim = abs( ( x1 * y2 + x2 * y + x  * y1 ) - ( x2 * y1 + x  * y2 + x1 * y  ) )
						  +abs( ( x1 * y  + x  * y3+ x3 * y1 ) - ( x  * y1 + x3 * y  + x1 * y3 ) )
						  +abs( ( x  * y2 + x2 * y3+ x3 * y  ) - ( x2 * y  + x3 * y2 + x  * y3 ) );

				if(nDim > nMaxDim)
				{
					corner[3] = *pt;
					nMaxDim = nDim;
				}
			}

			//첫번째 -> 두번째
			cvLine(m_orig_img, cvPoint(corner[0].x, corner[0].y), cvPoint(corner[1].x, corner[1].y), CV_RGB(255,120,0), 3);
			//첫번째 -> 세번째
			cvLine(m_orig_img, cvPoint(corner[0].x, corner[0].y), cvPoint(corner[2].x, corner[2].y), CV_RGB(255,120,0), 3);
			//세번째 -> 첫번째
			cvLine(m_orig_img, cvPoint(corner[3].x, corner[3].y), cvPoint(corner[1].x, corner[1].y), CV_RGB(255,120,0), 3);
			//세번째 -> 두번째
			cvLine(m_orig_img, cvPoint(corner[3].x, corner[3].y), cvPoint(corner[2].x, corner[2].y), CV_RGB(255,120,0), 3);

			//꼭짓점 그리기
			cvCircle(m_orig_img, cvPoint(corner[0].x, corner[0].y), 5, CV_RGB(255,255,255), -1);
			cvCircle(m_orig_img, cvPoint(corner[1].x, corner[1].y), 5, CV_RGB(255,255,255), -1);
			cvCircle(m_orig_img, cvPoint(corner[2].x, corner[2].y), 5, CV_RGB(255,255,255), -1);
			cvCircle(m_orig_img, cvPoint(corner[3].x, corner[3].y), 5, CV_RGB(255,255,255), -1);
	
			//사각형 크기 계산
			int mWidth = abs(corner[3].x - corner[0].x);
			int mHeight = abs(corner[3].y - corner[0].y);

			//렉트 구조체에 넣어서 보존
			rects[rect_cnt].x = corner[0].x;
			rects[rect_cnt].y = corner[0].y;
			rects[rect_cnt].width = mWidth;
			rects[rect_cnt].height = mHeight;

			//인덱스 증가
			rect_cnt++;

			//준비한 배열보다 크면 스톱
			if(rect_cnt > 99 )
			{
				break;
			}
		}

		//사용한 메모리 해제
		cvClearSeq(contours);
		cvClearSeq(result);
	}
	//사용한 메모리 해제
	cvReleaseMemStorage(&storage2);
	cvReleaseMemStorage(&storage3);

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
	return new ColorTracking();
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
	return new ColorTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

