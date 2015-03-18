
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

#include <cv.h>
#include <highgui.h>

#include "ColorConvert.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_result_img;
IplImage* m_gray_img;

int m_in_width;
int m_in_height;

std::string transCode;
int m_convert_code = 0;

ColorConvert::ColorConvert()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ColorConvert::ColorConvert(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ColorConvert::~ColorConvert() {
}

void ColorConvert::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ColorConvert::onInitialize()
{
	// ���� IplImage �ʱ�ȭ
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;				// ���� �������� ���� ����
	m_in_height = 0;			// ���� �������� ���� ����

	transCode = getProperty("TransNum");	// �ܺ� ������Ƽ ���

	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ColorConvert::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onDestroy()
{
	// ����� �̹����� ������
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onExecute()
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
		// ��������� �̹������� Ȯ��
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// gray ������ �̹��� ���� Ȯ��
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// �ܺ� ������Ƽ ���� ó��
		if(transCode == ""){
		
			std::cout << "[ERROR-ColorConvert::onExecute()] There is no Extenal Parameter" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;
		}

		// �ܺ� ������Ƽ ���� �����ϴ� ��ȯ �ڵ��� ����
		if(transCode =="GRAY")
			m_convert_code = CV_RGB2GRAY;
		if(transCode =="RGB")
			m_convert_code = 0;
		if(transCode =="BGR")
			m_convert_code = CV_RGB2BGR;
		if(transCode =="XYZ")
			m_convert_code = CV_RGB2XYZ;
		if(transCode =="YCrCb")
			m_convert_code = CV_RGB2YCrCb;
		if(transCode =="HSV")
			m_convert_code = CV_RGB2HSV;
		if(transCode =="HLS")
			m_convert_code = CV_RGB2HLS;
		if(transCode =="Lab")
			m_convert_code = CV_RGB2Lab;
		if(transCode =="Luv")
			m_convert_code = CV_RGB2Luv;
		
		if(m_convert_code != 0){
			
			//�׷��� ������ ��쿡�� 1ä��->3ä�� ���� �ʿ�
			if(transCode == "GRAY")
			{
				//�׷��̷� ��ȯ
				cvCvtColor(m_orig_img, m_gray_img, m_convert_code);;

				// Convert to 3channel image ��� ���߱� ���� ���� - 16��Ʈ�� ��쿡�� ���� ��ġ�� ����?
				// �ַ� ������ �÷������̽��� �и��� �̹����� ��ĥ�� ����Ѵٰ� ��.(�̹���ä�� 1,2,3,4, ����̹���):4ä�α��� ����
				cvMerge(m_gray_img, m_gray_img, m_gray_img, NULL, m_result_img);

			}
			else
			{

				//�� �� �̹��� ��ȯ
				cvCvtColor(m_orig_img, m_result_img, m_convert_code);

			}
		}
		else{
		
			cvCopy(m_orig_img, m_result_img);
		
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
	
ReturnType ColorConvert::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorConvert::onPeriodChanged()
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
	return new ColorConvert();
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
	return new ColorConvert();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

