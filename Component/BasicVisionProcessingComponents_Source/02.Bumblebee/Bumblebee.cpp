
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

IplImage *pImage_L;			//ī�޶��¿���
IplImage *pImage_R;			//ī�޶�쿵��

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

	// ���� ȹ���� ���� FlyCapture API �ڵ�
	FlyCaptureError error; // Error �ڵ� ����ü

	error = flycaptureCreateContext(&pContext); // Context ����
	if(error != FLYCAPTURE_OK){
		std::cout << "[ERROR-Bumblebee::onInitialize()] No Bumblebee Camera Context" << std::endl;
		// ������ ����Ÿ�� ��ȯ
		return OPROS_INPUT_NULL;
	}
	flycaptureInitialize(pContext, 0); // ī�޶� �ʱ�ȭ

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
	
	flycaptureStop(pContext); // ī�޶� ���� ����	
    flycaptureDestroyContext(pContext);  // ������ Context �ı�

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
			// ������ ����Ÿ�� ��ȯ
			return OPROS_INPUT_NULL;
		}
		captureStartFlg = false;
	}

	flycaptureGrabImage2(pContext,&pImage);					// �̹���ȹ��
	flycaptureConvertImage(pContext,&pImage,&pImageBGR);	// ȹ���� �̹��� RGB���·� ��ȯ

	if(pImage_L != NULL)
		cvReleaseImage(&pImage_L);
	if(pImage_R != NULL)
		cvReleaseImage(&pImage_R);
	pImage_L=cvCreateImage(cvSize(width,height),8,3);
	pImage_R=cvCreateImage(cvSize(width,height),8,3);
	
	// �̹������� �¿�и�
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
		
	// RawImage�� �̹��� ������ ���� �Ҵ�
	RawImageData *pImage1 = result1.getImage();
	RawImageData *pImage2 = result2.getImage();

	// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
	pImage1->resize(pImage_L->width, pImage_L->height, pImage_L->nChannels);
	pImage2->resize(pImage_R->width, pImage_R->height, pImage_R->nChannels);

	// ������ �� ũ��(pixels��) ���
	int size1 = pImage_L->width * pImage_L->height * pImage_L->nChannels;
	int size2 = pImage_R->width * pImage_R->height * pImage_R->nChannels;
		
	// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
	unsigned char *ptrData1 = pImage1->getData();
	unsigned char *ptrData2 = pImage2->getData();
		
	// ���� ������ ������ ������ ��ŭ memcpy
	memcpy(ptrData1, pImage_L->imageData, size1);
	memcpy(ptrData2, pImage_R->imageData, size2);

	// ��Ʈ�ƿ�
	opros_any mdata1 = result1;
	ImageOut1.push(result1);//����
	opros_any mdata2 = result2;
	ImageOut2.push(result2);//����

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

