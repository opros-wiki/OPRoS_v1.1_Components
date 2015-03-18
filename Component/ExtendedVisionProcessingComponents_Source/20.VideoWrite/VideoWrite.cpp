
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

#include "VideoWrite.h"

//
// constructor declaration
//

IplImage* m_orig_img;

std::string m_codec;			//설정값 변수 저장

int m_in_width;
int m_in_height;

int fourcc;						//코덱 종류 인자값 저장
double fps;						//초당 프레임 저장

CvFont font;					//화면 상단에 녹화중을 알리는 텍스트 삽입
CvVideoWriter* VideoOut;		//비디오 압축 선언

VideoWrite::VideoWrite()
		:ImageIn(OPROS_LAST,1)
{
	portSetup();
}

//
// constructor declaration (with name)
//
VideoWrite::VideoWrite(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{
	
	portSetup();
}

//
// destructor declaration
//

VideoWrite::~VideoWrite() {
}

void VideoWrite::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);

	// export variable setup
}

// Call back Declaration
ReturnType VideoWrite::onInitialize()
{

	//이미지 및 변수 초기화
	m_orig_img		= NULL;

	VideoOut		= NULL;

	m_in_width		= 0;
	m_in_height		= 0;

	m_codec =  getProperty("Codec");

	m_videoFile = getProperty("VideoFile");
	if (m_videoFile == "") m_videoFile = "save.avi";

	//초당 프레임 설정
	fps = 30.0;

	//폰트 init
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0,0,3,8);

	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onStart()
{

	return OPROS_SUCCESS;
}
	
ReturnType VideoWrite::onStop()
{	
	
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onDestroy()
{
	// 이미지용 메모리의 해제
    cvReleaseImage(&m_orig_img);

	//멈추거나 종료될 때 저장되게 끔	
	if(VideoOut != NULL)
	{
		std::cout<< "---------------  Saved!  ---------------" << std::endl;
	}

	cvReleaseVideoWriter(&VideoOut);

	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onEvent(Event *evt)
{

	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onExecute()
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
		if(m_orig_img == NULL)
		{
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		//비디오 녹화 준비
		if(VideoOut == NULL)
		{
			std::string cpath = getProperty("opros.component.dir");
			cpath += m_videoFile;

			//이미지 압축할 메모리 할당
			VideoOut = cvCreateVideoWriter(cpath.c_str(), fourcc, fps, cvSize(m_in_width, m_in_height), 1);
	
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		
		//압축할 때 쓸 이미지
		IplImage* m_frame_img;
		m_frame_img   = cvCreateImage(cvGetSize(m_orig_img), IPL_DEPTH_8U, 3);

		cvCopy(m_orig_img, m_frame_img, 0);	//압축용

		if(VideoOut != NULL)
		{
			//프레임 압축
			cvWriteFrame(VideoOut, m_frame_img);
		}
		
		//텍스트 삽입
		cvPutText(m_orig_img, "Record" , cvPoint(20,30), &font, CV_RGB(255, 0, 0));

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

		//사용이 끝난 메모리 헤제
		cvReleaseImage(&m_frame_img);
	}

	return OPROS_SUCCESS;
}
	
ReturnType VideoWrite::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onPeriodChanged()
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
	return new VideoWrite();
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
	return new VideoWrite();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

