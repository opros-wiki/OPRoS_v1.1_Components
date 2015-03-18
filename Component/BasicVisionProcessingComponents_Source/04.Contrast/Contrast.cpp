
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

#include "Contrast.h"

//
// constructor declaration
//
IplImage* m_orig_img;

int m_in_width;
int m_in_height;
int m_Contrast;

Contrast::Contrast()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Contrast::Contrast(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Contrast::~Contrast() {
}

void Contrast::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Contrast::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_Contrast = atoi(getProperty("Contrast").c_str());

	return OPROS_SUCCESS;
}

ReturnType Contrast::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Contrast::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	return OPROS_SUCCESS;
}

ReturnType Contrast::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onExecute()
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

		for(int i = 0; i < m_orig_img->height; i++)
		{
			for(int j = 0; j< m_orig_img->width; j++)
			{
				//RGB�� ä�� ������ BGR
				//CvScalar ������ �������� �Ű������� ��
				CvScalar v = cvGet2D(m_orig_img,i,j);
				//limit() : RGB���� ��� ��쿡 ����(0-255 ���̿� ���� �����ϵ���)
				//128�� �������� -1~1�� ���� ���ϸ� 128 ��ó�� ���� ���� ���ϰ� ���̳����� ���� ��ȭ�� Ŀ�� ��� ��
				v.val[2] = limit(v.val[2] + ((v.val[2] -128) * m_Contrast /100) );
				v.val[1] = limit(v.val[1] + ((v.val[1] -128) * m_Contrast /100) );
				v.val[0] = limit(v.val[0] + ((v.val[0] -128) * m_Contrast /100) );
			
				cvSet2D(m_orig_img, i,j,v);

			}

		}

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

//RGB�� ���� �Լ�
double Contrast::limit(double value){

	return( (value > 255) ? 255 : ((value < 0) ? 0 : value) );

}
	
ReturnType Contrast::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onPeriodChanged()
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
	return new Contrast();
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
	return new Contrast();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

