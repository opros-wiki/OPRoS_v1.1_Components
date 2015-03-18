
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

#include "EdgeDetection.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;
IplImage* m_edge_img;
IplImage* m_corner_img;

CvSeq* lines;					//라인 검출
CvSeq* circles;					//원 검출

CvMemStorage* storage;			// 메모리 할당

int m_in_width;
int m_in_height;

std::string m_detect_type;

EdgeDetection::EdgeDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
EdgeDetection::EdgeDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

EdgeDetection::~EdgeDetection() {
}

void EdgeDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType EdgeDetection::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_edge_img != NULL)
		m_edge_img = NULL;
	if(m_corner_img != NULL)
		m_corner_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_detect_type = getProperty("DetectionType");

	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EdgeDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onDestroy()
{
	// 이미지용 메모리의 해제
	//cvReleaseImage 는 IPlImage구조체를 삭제하고 메모리를 반환한다.
	if(m_orig_img != NULL)
        cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
        cvReleaseImage(&m_gray_img);
    if(m_result_img != NULL)
        cvReleaseImage(&m_result_img);
	if(m_edge_img != NULL)
		cvReleaseImage(&m_edge_img);
	if(m_corner_img != NULL)
		cvReleaseImage(&m_corner_img);
	
	cvReleaseMemStorage(&storage); //메모리 공간 해제

	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onExecute()
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
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_edge_img == NULL){
			m_edge_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_corner_img == NULL){
			m_corner_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 1);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//그레이로 변환 - 엣지 추출은 1채널 영상을 넘겨주어야 함
		cvCvtColor(m_orig_img, m_gray_img, CV_RGB2GRAY);;

		//엣지 검출
		if(m_detect_type == "Edge")
		{
			//cvCanny함수로 엣지 검출
			// 소스영상, 저장할 곳, 첫번째 임계값, 두번째 임계값, 필터크기(sobel연산에 필요)
			cvCanny(m_gray_img, m_edge_img, 50, 100, 3);
					
			//추출한 영상을 출력하기 위해 8비트로 만들기
			cvMerge(m_edge_img, m_edge_img, m_edge_img, NULL, m_orig_img);
		}
		//직선 검출
		else if(m_detect_type == "Line")
		{
			//검출된 직선을 위한 메모리 공간 할당
			storage = cvCreateMemStorage(0);
			
			//cvCanny로 엣지 검출
			cvCanny(m_gray_img, m_edge_img, 50, 100, 3);
			
			//엣지 검출 영상에서 허프변환으로 양끝점이 있는 선분 검출
			//소스영상, 저장할 곳, 옵션,원점으로부터의 거리 간격, 라디안 간격, 임계값,파라미터1,파라미터2
			//CV_HOUGH_PROBABILISTIC : 양끝점이 있는 선분 검출.
			//파라미터1은 최소선분의 길이,파라미터2는 두 선분사이의 최대길이
			lines = cvHoughLines2(m_edge_img, storage, CV_HOUGH_PROBABILISTIC, 1 ,
				CV_PI/180, 100,5,5);
			
			//직선 그리기
			for(int k = 0; k < lines->total; k++)
			{
				//허프변환으로 검출한 선분을 저장하고 있는 lines에서 lineSeg에 저장.
				//lineSeg[0]과 lineSeg[1]에 선분의 양 끝점이 있음
				CvPoint* lineSeg = (CvPoint*)cvGetSeqElem(lines, k);
				//그릴 곳, 연결할점1,점2, 색상,두께,고려할 화소
				cvLine(m_orig_img, lineSeg[0], lineSeg[1], CV_RGB(255, 0, 0), 1, 8);

			}
			
		}
		//코너 검출
		else if(m_detect_type == "Corner")
		{
			//코너점 검출 함수
			//소스영상, 목적영상, 알고리즘에서 사용될 Sobel 연산자의 크기
			cvPreCornerDetect(m_gray_img, m_corner_img, 3);
			
			//절대값으로 변환 양 극대값과 음 극소값을 한번에 검출하기 위함이라고 함
			cvAbs(m_corner_img, m_corner_img);
			
			//corner의 임계값이 0.01보다 크면 내용을 변경하지 않게 함
			//코너점이 너무 많이 검출되지 않도록 조절
			cvThreshold(m_corner_img, m_corner_img, 0.03, 0, CV_THRESH_TOZERO);
			
			//코너점 그리기에 필요한 변수들 선언
			double fValue;
			double maxValue;
			int    x,y;
			int    m,n;
			bool   bChange;
			int    nSize = 3;  
			int    nCornerCount = 0;

			//코너점(원) 그리기
			//m_corner에서 (2 * nSize + 1) * (2 * nSize + 1)크기의 윈도우를 이동시켜 가면서
			//(x,y)위치의 값이 윈도우에 의해 정의되는 이웃화소값보다 큰 값인지를 확인하는 방법으로
			//지역 극값인지를 확인하여 지역 극값이면 원으로 표시함
			for(y = nSize; y < m_corner_img->height - nSize; y++)
			{
				for(x = nSize; x < m_corner_img->width - nSize; x++)
				{
					bChange = false;

					maxValue = cvGetReal2D(m_corner_img, y, x);

					for(n = y-nSize; n <= y+nSize; n++){
						for(m = x-nSize; m <= x+nSize; m++)
						{
							if(m == x && n == y)
								continue;
							fValue = cvGetReal2D(m_corner_img, n, m);//지정 좌표의 픽셀 값을 리턴해서 받음
							if(fValue >= maxValue)
							{
								bChange = true;
								break;
							}
						}
					}
					if(!bChange)
					{
						nCornerCount++;
						
						cvCircle(m_orig_img, cvPoint(x,y), 5, CV_RGB(255, 0, 0), 2);
					}

				}
			
			}

		}
		


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
	
ReturnType EdgeDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onPeriodChanged()
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
	return new EdgeDetection();
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
	return new EdgeDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

