
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

#include "OpeningClosing.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

std::string m_trans_type;
std::string m_result_type;
int m_repeat_count;

OpeningClosing::OpeningClosing()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
OpeningClosing::OpeningClosing(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

OpeningClosing::~OpeningClosing() {
}

void OpeningClosing::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType OpeningClosing::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_trans_type = getProperty("TransType");
	m_result_type = getProperty("ResultType");
	m_repeat_count = atoi(getProperty("RepeatCount").c_str());

	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType OpeningClosing::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onExecute()
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
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// ������ Ŭ��¡ ���� ����
		if(m_result_type == "Gray"){	// m_RGB_mode:Gray ���
			// �÷������� �׷��̽����Ϸ� ��ȯ
			cvCvtColor( m_orig_img, m_gray_img, CV_RGB2GRAY );

			// �׷����̹���(1ä��)�� 3ä�η� ����, ����ݱ⿬������ �ٽ� m_image_buff�� ����
			cvMerge(m_gray_img, m_gray_img, m_gray_img, NULL, m_orig_img);
		}

		if(m_trans_type == "Closing"){	// m_change_mode:Closing �ݱ� ��â �� ħ��

			cvDilate(m_orig_img, m_result_img, NULL, m_repeat_count);

			cvErode(m_result_img, m_result_img, NULL, m_repeat_count);

		}else if(m_trans_type == "Opening"){// m_change_mode:Opening ���� ħ�� �� ��â

			cvErode(m_orig_img, m_result_img, NULL, m_repeat_count);

			cvDilate(m_result_img, m_result_img, NULL, m_repeat_count);

		}
		
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
	
ReturnType OpeningClosing::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onPeriodChanged()
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
	return new OpeningClosing();
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
	return new OpeningClosing();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

