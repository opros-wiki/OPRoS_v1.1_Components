
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

#include "HVR2300ImageCapture.h"

int m_img_width;
int m_img_height;
int CAMERA_NO;

IplImage* m_image_tp;
IplImage* m_image;
int m_brightness;
bool getFlag;

//
// constructor declaration
//
HVR2300ImageCapture::HVR2300ImageCapture()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
HVR2300ImageCapture::HVR2300ImageCapture(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

HVR2300ImageCapture::~HVR2300ImageCapture() {
}

void HVR2300ImageCapture::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType HVR2300ImageCapture::onInitialize()
{
	m_img_width = 640;
	m_img_height = 480;

	m_brightness = atoi(getProperty("Brightness").c_str());

	getFlag = false;

	if(m_image_tp != NULL)
		m_image_tp = NULL;
	if(m_image != NULL)
		m_image = NULL;

	if(m_image_tp == NULL){
		m_image_tp = cvCreateImage(cvSize(m_img_width, m_img_height),
								IPL_DEPTH_8U, 3);
	}
	if(m_image == NULL){
		m_image = cvCreateImage(cvSize(m_img_width, m_img_height),
								IPL_DEPTH_8U, 1);
	}

	if(!HVR_CameraInit(VGA, HVR2300R)){
	
		std::cout<<"There is no Camera Device(HVR2300)"<<std::endl;

	}

	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HVR2300ImageCapture::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onDestroy()
{
	if(m_image_tp != NULL)
		cvReleaseImage(&m_image_tp);
	if(m_image != NULL)
		m_image = NULL;

	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onExecute()
{

	RawImage result;					// 데이터 포트타입은 RawImage

	BYTE *buff;
	buff = new BYTE[m_img_width * m_img_height];

	if(1 != HVR_camGetImageData(m_img_width * m_img_height, buff, CAMERA_NO)){
	
		std::cout<<"Buffer fail"<<std::endl;
		Sleep(30);
		getFlag = false;
	
	}else{

		getFlag = true;

	} 

	if(getFlag == true){

		m_image->imageData = (char*)buff;
		cvCvtColor(m_image, m_image_tp, CV_BayerGB2BGR);

		// 영상의 복사 작업(영상데이터 & 영상 크기(Pixel수)
		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_image_tp->width, m_image_tp->height, m_image_tp->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_image_tp->width * m_image_tp->height * m_image_tp->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_image_tp->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

	}

	delete buff;

	return OPROS_SUCCESS;
}
	
ReturnType HVR2300ImageCapture::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}


BOOL HVR2300ImageCapture::HVR_CameraInit(int res, int pid){
	
	int DrvInfo[MAX_DEVICE];
	int ConnectNo;

	ConnectNo = HVR_camDrvInit(&DrvInfo[0]);
	Sleep(800);

	if(ConnectNo == 0)
		return 0;
	else
		CAMERA_NO = 0;

	if(!HVR_camSensorInit(pid, CAMERA_NO)){
		std::cout<<"Please check Product No."<<std::endl;
		return 0;
	}

	if(!HVR_ispResolutionCmd(res, pid, CAMERA_NO))
		return 0;
	
	// 1000 이값은 property로 조절?
	HVR_ispSetExposureTime(m_brightness, pid, CAMERA_NO);

	return 1;
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
	return new HVR2300ImageCapture();
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
	return new HVR2300ImageCapture();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

