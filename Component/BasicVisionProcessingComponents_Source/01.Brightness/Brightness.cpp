
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

#include "Brightness.h"

// ����ó�� �޸��̹���
IplImage* m_orig_img;

// ���� ���� ��� ���� ����
int m_in_width;
int m_in_height;
int m_Brightness;

Brightness::Brightness()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Brightness::Brightness(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Brightness::~Brightness() {
}

void Brightness::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Brightness::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	// xml ������Ƽ GET
	m_Brightness = atoi(getProperty("Brightness").c_str());

	return OPROS_SUCCESS;
}

ReturnType Brightness::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Brightness::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	return OPROS_SUCCESS;
}

ReturnType Brightness::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;

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

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//���� Brightness�� ���
		cvAddS(m_orig_img, CV_RGB(m_Brightness,m_Brightness,m_Brightness), m_orig_img, NULL);

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
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType Brightness::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onPeriodChanged()
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
	return new Brightness();
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
	return new Brightness();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

