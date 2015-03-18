
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

#include "ImageSegmentation.h"

//
// constructor declaration
//

int m_in_height;                // 처리할 이미지의 Height
int m_in_width;                 // 차리할 이미지의  Width
int segPort_width;				// 분할영역 출력 포트 이미지 가로
int segPort_height;				// 분할영역 출력 포트 이미지 세로

IplImage* m_image_buff;			// Original Image
IplImage* m_image_dest;         // OutPut Image 원본 출력 
IplImage* m_image_curr;			// 임시 출력
IplImage* m_resize;				// 분할영역 출력

CvRect* m_rec_seg;				//관심영역으로 지정할 렉트

std::string m_segment;			// 설정값 받아서 저장
int m_number;					// 설정값 받아서 저장 

int div_value;					//가로세로 나눠질 갯수
int selected ;					//선택 영역

std::string outString;			//내보낼 스트링 저장 변수

ImageSegmentation::ImageSegmentation()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ImageSegmentation::ImageSegmentation(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ImageSegmentation::~ImageSegmentation() {
}

void ImageSegmentation::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageOut2", &ImageOut2);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ImageSegmentation::onInitialize()
{
	// 이미지용 메모리 초기화
    m_image_buff		= NULL; 
	m_image_dest		= NULL;
	m_image_curr		= NULL;
	m_resize			= NULL;

	//변수 초기화
	m_in_width			= 0;             
    m_in_height			= 0;

	outString			= "";

	//분할 렉트영역
	m_rec_seg = NULL;

	//분할된 이미지 크기 저장
	segPort_width = 0;
	segPort_height = 0;

	//설정값 읽기
	m_segment =  getProperty("Segmentation");
	m_number =   atoi(getProperty("Number").c_str());

	//실시간 변경이 아니므로 처음에 판별하기
	if(m_segment == "1*1")
	{
		div_value = 1;	
	}
	else if(m_segment == "2*2")
	{
		div_value = 2;
	}
	else if(m_segment == "3*3")
	{
		div_value = 3;
	}
	else
	{
		div_value = 0;

	}
		
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ImageSegmentation::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onDestroy()
{
	// 이미지용 메모리의 해제
    cvReleaseImage(&m_image_buff);
	cvReleaseImage(&m_image_dest);
	cvReleaseImage(&m_image_curr);
    cvReleaseImage(&m_resize);

	 if(m_rec_seg != NULL)
		delete m_rec_seg;

	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;
	RawImage result2;

	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();
		std::vector<PositionDataType> data;

		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// 원본영상의 이미지영역 확보
		if(m_image_buff == NULL){
			m_image_buff = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_image_dest == NULL){
			m_image_dest = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_image_curr == NULL){
			m_image_curr = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		}
		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());
		
		cvCopy(m_image_buff, m_image_dest, 0);	//출력용

		//검은색으로 셋팅
		cvSetZero(m_image_curr);

		//설정값 변동이 제대로 되었으면
		if(div_value > 0)
		{
			//해당 설정값으로 이미지 분할하기
			image_segmentation(div_value);
			
		}
		else
		{
			//변동이 제대로 되지 않았을 경우의 예외처리
			if(m_resize != NULL)
			{
				 cvReleaseImage(&m_resize);
			}
			
			//기존 이미지 크기 반영
			m_resize = cvCreateImage(cvGetSize(m_image_curr), IPL_DEPTH_8U, 3);
			
			//기본 화면 출력
			cvCopy(m_image_curr, m_resize);

		}

//// DataOut		
		if(outString != "")
		{
			std::string data_string;

			data_string = outString;

			PositionDataType base;
			
			base.setName(data_string);

			base.setX(NULL);
			base.setY(NULL);	
			base.setHeight(NULL);
			base.setWidth(NULL);
			base.setRadian(NULL);

			data.push_back(base);
	
			PositionOut.push(data);

		}

//// ImageOut 1
		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_image_dest->width, m_image_dest->height, m_image_dest->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_image_dest->width * m_image_dest->height * m_image_dest->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_image_dest->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달
		
//// ImageOut 2
		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage2 = result2.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage2->resize(m_resize->width, m_resize->height, m_resize->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size2 = m_resize->width * m_resize->height * m_resize->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata2 = pimage2->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata2, m_resize->imageData, size2);

		// 포트아웃
		opros_any mdata2 = result2;
		ImageOut2.push(result2);//전달
		
		delete pData;

	}
	return OPROS_SUCCESS;
}
	
ReturnType ImageSegmentation::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}



void ImageSegmentation::image_segmentation(int d_v)
{
	int d_value = d_v;		//넘어온 값 저장
	int line = 0;			//현재 라인 알려줄 변수
	int seg = 0;			//분할될 화면의 갯수

	int width = m_in_width;				//들어온 화면의 가로
	int height = m_in_height;			//들어온 화면의 세로
	int move_distX =0, move_distY = 0;	//움직일 단위

	CvRect m_rec_roi;					//분할된 크기별 사각형
	CvPoint pt1,pt2;					//사각형 만들 점들		
	
	IplImage* m_selected = NULL;		// 분할된 영역 저장
	IplImage* m_temp = NULL;			// 숫자 출력용 임시 영상

	std::string tempString;
	
	//넘어온 이미지 크기와 같은 이미지 생성
	m_temp   = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	
	//복사
	cvCopy(m_image_dest, m_temp, 0 );

	//초기화
	if(m_rec_seg != NULL)
	{
		delete m_rec_seg;
		m_rec_seg = NULL;

	}
	m_rec_roi.x = 0;
	m_rec_roi.y = 0;
	m_rec_roi.width = 0;
	m_rec_roi.height =0;

	// 분할될 화면 갯수 계산
	seg = d_value * d_value;

	//관심영역 지정을 위한 사각형
	m_rec_seg = new CvRect [seg];
	cvWaitKey(10);
	
	//사각형 그릴때 움직일 거리 (분할된 한 영역의 가로/세로가 됨)
	move_distX = (int)(width/d_value);			
	move_distY = (int)(height/d_value);

	//첫번째 줄에서 시작
	line = 1;
		
	//첫 시작점으로 초기화
	pt1 = cvPoint(0, 0);
	pt2 = cvPoint((int)(width/d_value), (int)(height/d_value));

	//분할된 영역 처리 - 그리기, 번호넣기
	for(int i = 0; i < seg; i++)
	{
		//좌표 넣기
		m_rec_seg[i].x = pt1.x;
		m_rec_seg[i].y = pt1.y;
		m_rec_seg[i].width =  pt2.x;
		m_rec_seg[i].height =  pt2.y;		

		//그릴 틈을 잠시 줘봄
		cvWaitKey(10);

		// 좌표로 사각형 그리기
		cvDrawRect( m_image_dest, 
			cvPoint(m_rec_seg[i].x, m_rec_seg[i].y),
			cvPoint(m_rec_seg[i].width, m_rec_seg[i].height),
			CV_RGB(255 ,255, 0),1,8);

		//해당 영역이 설정값에서 선택된 영역인지 판별
		if(m_number - 1 == i)
		{	
			selected = m_number;
		}

		//다음 그리기에서는 줄바꿔서 그리기(Y값 이동 시점)
		if(line == (int)((i+1) / d_value))
		{
			pt1.x = 0;
			pt1.y = pt1.y + move_distY;
			pt2.x = pt1.x + move_distX;
			pt2.y = pt1.y + move_distY;

			line = line + 1; // 라인 올리기
		}
		else
		{
			//아니면 X값만 변함
			pt1.x = pt1.x + move_distX;
			pt2.x = pt2.x + move_distX;
		
		}	
		
		//텍스트 삽입
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, 8);
		char* text;
		
		//원본 영상에 분할된 칸에 각각 번호를 매김
		text = new char [seg];
		
		//관심영역 설정
		cvSetImageROI(m_temp, m_rec_seg[i]);

		//숫자를 텍스트로 변환
		sprintf(&text[i], "%d",i+1);

		//입력은 출력할 영상에
		cvPutText(m_image_dest, &text[i], cvPoint( m_rec_seg[i].x+10, m_rec_seg[i].y + 30), 
			&font, CV_RGB(255, 0, 0));
		
		//관심영역 해제
		cvResetImageROI(m_temp);
	
	}

	//올바르게 선택된 영역이 있으면
	if(selected > 0 && selected <= seg)
	{
		
		//초기화 한번 해주고
		if(m_selected != NULL)
		{
			m_selected = NULL;
		}

		//해당 영역을 복사
		m_rec_roi.x = m_rec_seg[selected-1].x;
		m_rec_roi.y = m_rec_seg[selected-1].y;
		m_rec_roi.width = m_rec_seg[selected-1].width -m_rec_roi.x;
		m_rec_roi.height = m_rec_seg[selected-1].height - m_rec_roi.y;

		//복사된 영역 크기의 이미지 생성
		m_selected = cvCreateImage(cvSize(m_rec_roi.width, m_rec_roi.height ),IPL_DEPTH_8U, 3);

		
		//한번 더 체크
		if(m_rec_roi.width > 0 && m_rec_roi.height > 0 )
		{		
			//관심영역 설정
			cvSetImageROI(m_temp, m_rec_roi);
			
			//해당 관심영역을 복사 
			cvCopy(m_temp, m_selected,0);

			cvWaitKey(10);
						
			//관심영역 해제
			cvResetImageROI(m_temp);

			//선택된 영역 빨간 사각형으로 표시
			cvDrawRect( m_image_dest, cvPoint(m_rec_roi.x, m_rec_roi.y),
				cvPoint(m_rec_roi.x + m_rec_roi.width , m_rec_roi.y + m_rec_roi.height),
			CV_RGB(255 ,0, 0),3,8);

			//숫자를 텍스트로 변환
			std::stringstream sstr;
			sstr << selected;
			tempString = sstr.str();
		}
	}
	else
	{
		tempString = "";

	}
	outString  = "";
	//선택된 이미지가 존재하면
	if(m_selected != NULL)
	{
		//그 선택된 이미지 크기 보존 - 출력할 이미지 크기 설정에 사용
		segPort_width = m_selected->width;
		segPort_height = m_selected->height;


		//메모리 한번 해제 하고
		if(m_resize != NULL)
		{
			 cvReleaseImage(&m_resize);
		}
	
		//선택된 이미지의 크기에 맞게 출력 이미지 생성
		m_resize = cvCreateImage(cvSize(segPort_width,segPort_height), IPL_DEPTH_8U, 3);
		
		// 이미지 복사
		cvCopy(m_selected, m_resize);

		outString = tempString;

	}
	else //선택된 이미지가 존재하지 않으면
	{		
		//출력할 이미지 크기 저장 변수
		int resize_width, resize_height;

		//이전에 보존된 이미지 사이즈가 없다면
		if(segPort_width == 0 && segPort_height == 0)
		{
			//초기 사이즈로 설정- 원본 이미지 크기
			resize_width = width;
			resize_height = height;

		}
		else
		{
			//이전에 보존된 사이즈 적용
			resize_width = segPort_width;
			resize_height = segPort_height;
		}
		
		//메모리 해제
		if(m_resize != NULL)
		{
			 cvReleaseImage(&m_resize);
		}
		//선택된 이미지가 없더라도 윈도우 크기 유지
		m_resize = cvCreateImage(cvSize(resize_width, resize_height), IPL_DEPTH_8U, 3);

		//선택된 이미지가 없을 경우에는 검은색 화면 출력
		cvSetZero(m_resize);

	}

	//사용한 메모리 해제
	cvReleaseImage(&m_selected);
	cvReleaseImage(&m_temp);

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
	return new ImageSegmentation();
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
	return new ImageSegmentation();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

