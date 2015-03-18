
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

#include "Rotate.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_trans_img;
IplImage* m_result_img;

CvMat *m_rotate_mat;

int m_in_width;
int m_in_height;

float m_angle;

Rotate::Rotate()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Rotate::Rotate(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Rotate::~Rotate() {
}

void Rotate::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Rotate::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_trans_img != NULL)
		m_trans_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_rotate_mat = cvCreateMat( 2, 3, CV_32FC1);

	m_in_width = 0;
	m_in_height = 0;

	m_angle = (float) atof(getProperty("Angle").c_str());

	return OPROS_SUCCESS;
}

ReturnType Rotate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Rotate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_trans_img != NULL)
		cvReleaseImage(&m_trans_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	cvReleaseMat(&m_rotate_mat);

	return OPROS_SUCCESS;
}

ReturnType Rotate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onExecute()
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
		if(m_trans_img == NULL){
			m_trans_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//*************************ȸ��
		// ȸ�� �߽� ��ġ ���� 
		CvPoint2D32f center = cvPoint2D32f( m_orig_img->width/2.0, m_orig_img->height/2.0);
		// ȸ�������� ���� ��Ļ���
		cv2DRotationMatrix( center, m_angle, 1.0, m_rotate_mat );
		// cv2DRotationMatrix( center, 50.0, 1.0, m_rotate_mat );
		// �̹���ȸ��
		cvWarpAffine( m_orig_img, m_result_img, m_rotate_mat, 
						CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

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
		// opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType Rotate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onPeriodChanged()
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
	return new Rotate();
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
	return new Rotate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

