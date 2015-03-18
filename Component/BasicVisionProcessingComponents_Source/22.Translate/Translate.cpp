
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

#include "Translate.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_result_img;

CvMat *m_transformMatrix;

int m_in_width;
int m_in_height;

int m_x;
int m_y;

Translate::Translate()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Translate::Translate(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Translate::~Translate() {
}

void Translate::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Translate::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_transformMatrix = cvCreateMat( 2, 3, CV_32FC1);

	m_in_width = 0;
	m_in_height = 0;

	m_x = atoi(getProperty("X").c_str());
	m_y = atoi(getProperty("Y").c_str());

	return OPROS_SUCCESS;
}

ReturnType Translate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Translate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	cvReleaseMat(&m_transformMatrix);

	return OPROS_SUCCESS;
}

ReturnType Translate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onExecute()
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
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//*************************�̵�
		// float�� 2Dȭ�� ���
		CvPoint2D32f original[3];		// �̵���
	    CvPoint2D32f translate[3];		// �̵���

		// �̵����� �����̹���(������ 0,0)
	    original[0] = cvPoint2D32f( 0, 0 );						// (�»��)
	    original[1] = cvPoint2D32f( m_orig_img->width, 0 );	// (����)
	    original[2] = cvPoint2D32f( 0, m_orig_img->height );	// (���ϴ�)

	    // �̵����� ��ǥ(m_translateX, m_up_down ��ŭ�̵�)
		translate[0] = cvPoint2D32f( m_x, m_y );
	    translate[1] = cvPoint2D32f( m_x + m_orig_img->width, m_y );
	    translate[2] = cvPoint2D32f( m_x, m_y + m_orig_img->height );

	    // �����κ�ȯ�迭����
	    cvGetAffineTransform( original, translate, m_transformMatrix );

	    // �̹����� �����κ�ȯ�迭�� �̿��Ͽ� �̵�
	    cvWarpAffine( m_orig_img, m_result_img, m_transformMatrix, 
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
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType Translate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onPeriodChanged()
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
	return new Translate();
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
	return new Translate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

