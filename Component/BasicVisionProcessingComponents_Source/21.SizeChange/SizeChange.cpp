
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

#include "SizeChange.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

int m_change_width;
int m_change_height;

SizeChange::SizeChange()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
SizeChange::SizeChange(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

SizeChange::~SizeChange() {
}

void SizeChange::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType SizeChange::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_change_width = atoi(getProperty("Width").c_str());
	m_change_height = atoi(getProperty("Height").c_str());

	return OPROS_SUCCESS;
}

ReturnType SizeChange::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType SizeChange::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType SizeChange::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onExecute()
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

		// �̹��� ũ�� ����
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_change_width, m_change_height), IPL_DEPTH_8U, 3);
		}

		// �������
		cvResize(m_orig_img, m_result_img, CV_INTER_LINEAR);

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_result_img->width, m_result_img->height, m_result_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_result_img->width * m_result_img->height * m_result_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_result_img->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType SizeChange::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onPeriodChanged()
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
	return new SizeChange();
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
	return new SizeChange();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

