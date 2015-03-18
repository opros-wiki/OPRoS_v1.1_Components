
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



#include "Bumblebee.h"
#include "pgrflycapture.h"

//
// constructor declaration
//
FlyCaptureContext pContext;
FlyCaptureImage pImage;
FlyCaptureImage pImageBGR;

IplImage *pImage_L;			//카메라좌영상
IplImage *pImage_R;			//카메라우영상

int width;
int height;
bool captureStartFlg;

Bumblebee::Bumblebee()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Bumblebee::Bumblebee(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

Bumblebee::~Bumblebee() {
}

void Bumblebee::portSetup() {

	// data port setup

	addPort("ImageOut1", &ImageOut1);

	addPort("ImageOut2", &ImageOut2);



	// export variable setup


}

// Call back Declaration
ReturnType Bumblebee::onInitialize()
{
	width = 640;
	height = 480;

	// 영상 획득을 위한 FlyCapture API 코드
	FlyCaptureError error; // Error 코드 구조체

	error = flycaptureCreateContext(&pContext); // Context 생성
	if(error != FLYCAPTURE_OK){
		std::cout << "[ERROR-Bumblebee::onInitialize()] No Bumblebee Camera Context" << std::endl;
		// 비정상 리턴타입 반환
		return OPROS_INPUT_NULL;
	}
	flycaptureInitialize(pContext, 0); // 카메라 초기화

	pImageBGR.pixelFormat = FLYCAPTURE_BGR;
	pImageBGR.pData = new unsigned char[2248*2048*3];	

	if(pImage_L != NULL)
		pImage_L = NULL;
	if(pImage_R != NULL)
		pImage_R = NULL;

	captureStartFlg = true;

	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Bumblebee::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onDestroy()
{
	delete[] pImageBGR.pData;
	
	flycaptureStop(pContext); // 카메라 동작 멈춤	
    flycaptureDestroyContext(pContext);  // 생성된 Context 파괴

	if(pImage_L != NULL)
		cvReleaseImage(&pImage_L);
	if(pImage_R != NULL)
		cvReleaseImage(&pImage_R);

	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onExecute()
{
	RawImage result1;
	RawImage result2;

	if(captureStartFlg){
		FlyCaptureError error;
		error =  flycaptureStartCustomImage(pContext,3,0,0,width,height,100.0f,FLYCAPTURE_RAW16);
		if(error != FLYCAPTURE_OK){
			std::cout << "[ERROR-Bumblebee::onExecute()] Capture was not started." << std::endl;
			// 비정상 리턴타입 반환
			return OPROS_INPUT_NULL;
		}
		captureStartFlg = false;
	}

	flycaptureGrabImage2(pContext,&pImage);					// 이미지획득
	flycaptureConvertImage(pContext,&pImage,&pImageBGR);	// 획득한 이미지 RGB형태로 변환

	if(pImage_L != NULL)
		cvReleaseImage(&pImage_L);
	if(pImage_R != NULL)
		cvReleaseImage(&pImage_R);
	pImage_L=cvCreateImage(cvSize(width,height),8,3);
	pImage_R=cvCreateImage(cvSize(width,height),8,3);
	
	// 이미지에서 좌우분리
	for(int i=0;i < height; i++)  {
	   for(int j=0;j < width; j++)   {
			pImage_L->imageDataOrigin[(i*width+j)*3]=pImageBGR.pData[(i*width*2+j)*4];
			pImage_L->imageDataOrigin[(i*width+j)*3+1]=pImageBGR.pData[(i*width*2+j)*4+1];
			pImage_L->imageDataOrigin[(i*width+j)*3+2]=pImageBGR.pData[(i*width*2+j)*4+2];

			pImage_R->imageDataOrigin[(i*width+j)*3]=pImageBGR.pData[(i*width*2+width+j)*4];
			pImage_R->imageDataOrigin[(i*width+j)*3+1]=pImageBGR.pData[(i*width*2+width+j)*4+1];
			pImage_R->imageDataOrigin[(i*width+j)*3+2]=pImageBGR.pData[(i*width*2+width+j)*4+2];
	   }
	}
		
	// RawImage의 이미지 포인터 변수 할당
	RawImageData *pImage1 = result1.getImage();
	RawImageData *pImage2 = result2.getImage();

	// 입력된 이미지 사이즈 및 채널수로 로 재 설정
	pImage1->resize(pImage_L->width, pImage_L->height, pImage_L->nChannels);
	pImage2->resize(pImage_R->width, pImage_R->height, pImage_R->nChannels);

	// 영상의 총 크기(pixels수) 취득
	int size1 = pImage_L->width * pImage_L->height * pImage_L->nChannels;
	int size2 = pImage_R->width * pImage_R->height * pImage_R->nChannels;
		
	// 영상 데이터로부터 영상값만을 할당하기 위한 변수
	unsigned char *ptrData1 = pImage1->getData();
	unsigned char *ptrData2 = pImage2->getData();
		
	// 현재 프레임 영상을 사이즈 만큼 memcpy
	memcpy(ptrData1, pImage_L->imageData, size1);
	memcpy(ptrData2, pImage_R->imageData, size2);

	// 포트아웃
	opros_any mdata1 = result1;
	ImageOut1.push(result1);//전달
	opros_any mdata2 = result2;
	ImageOut2.push(result2);//전달

	return OPROS_SUCCESS;
}
	
ReturnType Bumblebee::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onPeriodChanged()
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
	return new Bumblebee();
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
	return new Bumblebee();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

