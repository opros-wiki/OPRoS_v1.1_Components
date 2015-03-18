
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

#include "Histogram.h"

//
// constructor declaration
//
int m_in_height;                // ó���� �̹����� Height
int m_in_width;                 // ó���� �̹�����  Width

IplImage* m_orig_img;			// Original Image
IplImage* m_red;                // red channel 
IplImage* m_green;				// green channel
IplImage* m_blue;				// blue channel
IplImage* m_hist_img;           // ������׷��� �׷��� ��


//***������׷� ����
CvHistogram* hist_red;          // red channel histogram
CvHistogram* hist_green;		// green channel histogram
CvHistogram* hist_blue;			// blue channel histogram

int hist_size;					// ������׷� ��ü ������
int bin_w;						// ������׷� ���� ������

Histogram::Histogram()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Histogram::Histogram(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Histogram::~Histogram() {
}

void Histogram::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Histogram::onInitialize()
{
	float range[] = {0,255};                 //���� ���� �� ����
	float* ranges[] = {range};               //���� ������ �����ϴ� ������ �迭 �ִ� �ּҰ�.

	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_red != NULL)
		m_red = NULL;
	if(m_green != NULL)
		m_green = NULL;
	if(m_blue != NULL)
		m_blue = NULL;
	if(m_hist_img != NULL)
		m_hist_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	hist_size = 256;                         //������׷� ���� ũ��

	//0-255�� ������ ǥ���ϱ� ���� 256���� ���� �̿��Ͽ� 0-255 ������ 1�� ������� ������׷��� ���� �Ѵ�.
	//�� ������׷� ����� - ������׷� ����,����ũ��, Ÿ��,x�����,���밣��
	hist_red = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	hist_green = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	hist_blue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	
	//������׷� �ʱ�ȭ
	cvClearHist(hist_red);
	cvClearHist(hist_green);
	cvClearHist(hist_blue);

	return OPROS_SUCCESS;
}

ReturnType Histogram::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Histogram::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onDestroy()
{
	// �̹����� �޸��� ����
	//cvReleaseImage �� IPlImage����ü�� �����ϰ� �޸𸮸� ��ȯ�Ѵ�.
	if(m_orig_img       != NULL)			//����
        cvReleaseImage(&m_orig_img);		
	if(m_red              != NULL)			//����
        cvReleaseImage(&m_red);
	if(m_green            != NULL)			//�׸�
        cvReleaseImage(&m_green);
	if(m_blue             != NULL)			//���
        cvReleaseImage(&m_blue);
	if(m_hist_img           !=NULL)			//������׷� ����
		cvReleaseImage(&m_hist_img);

	//������׷� �޸� ����
	if(hist_red != NULL)
		cvReleaseHist(&hist_red);
	if(hist_red != NULL)
		cvReleaseHist(&hist_green);
	if(hist_red != NULL)
		cvReleaseHist(&hist_blue);

	return OPROS_SUCCESS;
}

ReturnType Histogram::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onExecute()
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
		if(m_red == NULL){
			m_red = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_green == NULL){
			m_green = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_blue == NULL){
			m_blue = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_hist_img == NULL){
			m_hist_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		//cvRound : �Ǽ����� �ݿø��ؼ� ���������� ��ȯ
		//������׷� �̹����� ���̸� ���� ũ��� ������ �ϳ��� �� �ش��ϴ� ���� ����
		bin_w = cvRound( ( double )m_hist_img->width / hist_size );

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//cvSplit - �̹����� ä�κ��� �и���
		//RGB�� ������׷��� ���� �׸��� ���� �и�.
		cvSplit(m_orig_img, m_blue, m_green, m_red, NULL);
		
		//cvCalcHist ������׷� ��� �Լ�(�ҽ�����, ������׷� ������)
		cvCalcHist(&m_red, hist_red);
		cvCalcHist(&m_green, hist_green);
		cvCalcHist(&m_blue, hist_blue);
		
		//������׷��� �ּҰ��� �ִ밪�� ����
		//�ַ� ����ȭ ���� �ϱ� ���� ����Ѵٰ� ��
		float max_value_r = 0;
		float max_value_g = 0;
		float max_value_b = 0;

		//�ҽ� ���� �����ϰ� ����Ʈ���� �����Ͽ� �ϳ��� �迭�� �ٸ� �迭�� ������ȯ-�̹����� ����������ȯ
		//(�ҽ�����, ��������, ������,����Ʈ)
        cvGetMinMaxHistValue( hist_red, 0, &max_value_r, 0, 0 );

		cvConvertScale( hist_red->bins, hist_red->bins, 
		( ( double )m_hist_img->height ) / max_value_r, 0 );

		cvGetMinMaxHistValue( hist_green, NULL, &max_value_g, NULL, NULL );

		cvConvertScale( hist_green->bins, hist_green->bins, 
		( ( double )m_hist_img->height ) / max_value_g, 0 );

		cvGetMinMaxHistValue( hist_blue, NULL, &max_value_b, NULL, NULL );

		cvConvertScale( hist_blue->bins, hist_blue->bins, 
		( ( double )m_hist_img->height ) / max_value_b, 0 );
		
	    //���� �����Ͱ��� �������� ä��� �Լ�.
	    // (�ҽ�����, ���� ��, ����ũ)
		cvSet( m_hist_img, cvScalarAll( 255 ), 0 ); //������� ä�� -CV_RGB(255,255,255) ����

		for(int i =0; i <hist_size; i++)
		{
			//���簢�� �׸��� �Լ�
			//(���簢���� �׷��� ����, ���簢�� �𼭸�, ���簢�� �ݴ��� �𼭸�,�� ����, �� �β�)
			//cvGetReal1D 1���� ������׷����� 1��° ������ �󵵼��� �������� �Լ�)
			//(1�����迭, ��ġ�ε���)
			cvRectangle(
		  		m_hist_img,
		  		cvPoint( i * bin_w, m_hist_img->height ),
				cvPoint(( i + 1 ) * bin_w, m_hist_img->height - cvRound( cvGetReal1D(hist_red->bins, i) ) ),
				CV_RGB(255,0,0)
			);
			cvRectangle(
		  		m_hist_img,
		  		cvPoint( i * bin_w, m_hist_img->height ),
				cvPoint(( i + 1 ) * bin_w, m_hist_img->height - cvRound( cvGetReal1D(hist_green->bins, i) ) ),
				CV_RGB(0,255,0),
				1
			);
			cvRectangle(
		  		m_hist_img,
		  		cvPoint( i * bin_w, m_hist_img->height ),
				cvPoint(( i + 1 ) * bin_w, m_hist_img->height - cvRound( cvGetReal1D(hist_blue->bins, i) ) ),
				CV_RGB(0,0,255),
				1
			);
			
		}		

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_hist_img->width, m_hist_img->height, m_hist_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_hist_img->width * m_hist_img->height * m_hist_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_hist_img->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType Histogram::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onPeriodChanged()
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
	return new Histogram();
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
	return new Histogram();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

