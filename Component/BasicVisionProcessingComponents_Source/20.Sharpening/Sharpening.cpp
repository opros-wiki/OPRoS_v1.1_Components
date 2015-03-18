
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

#include "Sharpening.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;

CvMat *kernel_mat;

int m_in_width;
int m_in_height;

std::string m_result_type;
std::string m_do_sharpening;

Sharpening::Sharpening()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Sharpening::Sharpening(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Sharpening::~Sharpening() {
}

void Sharpening::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Sharpening::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_result_type = getProperty("ResultType");
	m_do_sharpening = getProperty("DoSharpening");

	// ��Ļ���
	kernel_mat = cvCreateMat( 3, 3, CV_32FC1);

	return OPROS_SUCCESS;
}

ReturnType Sharpening::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Sharpening::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	cvReleaseMat(&kernel_mat);

	return OPROS_SUCCESS;
}

ReturnType Sharpening::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onExecute()
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

		int h = m_orig_img->height;
		int w = m_orig_img->width;
		
		if(m_result_type == "Gray"){	// m_RGB_mode:Gray ���
			// �÷������� �׷��̽����Ϸ� ��ȯ
			cvCvtColor( m_orig_img, m_gray_img, CV_RGB2GRAY );

			// �׷����̹���(1ä��)�� 3ä�η� ����, �����׿������� �ٽ� m_image_buff�� ����
			cvMerge(m_gray_img, m_gray_img, m_gray_img, NULL, m_orig_img);
		}

		// ������ó���ϰ��
		if(m_do_sharpening == "True"){

			IplImage * rgb[3];
			IplImage * rgb_temp[3];

			for(int i=0; i<3; i++){

				rgb[i] = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

				rgb_temp[i] = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

			}

			// 3ä��->1ä�η� �и�
			cvSplit(m_orig_img, rgb[0], rgb[1], rgb[2], NULL);

			for(int index=0; index<3; index++){				

				uchar * image_data = (uchar*)rgb[index]->imageData;

				uchar * change_data = (uchar*)rgb_temp[index]->imageData;

				// ���ö�þȿ��� ��, ��������� ��ġ��
				for(int i=1; i<h-1; i++){

					for(int j=1; j<w-1; j++){

						change_data[i*w+j] = limit(4*image_data[i*w+j] - image_data[(i-1)*w+j] -
										image_data[i*w+j+1] - image_data[(i+1)*w+j] - image_data[i*w+j-1]);

						change_data[i*w+j] = limit(image_data[i*w+j] + change_data[i*w+j]);
					}
				}
			}

			// �и��ߴ� ä�� �ٽ� ��ġ��
			cvMerge(rgb_temp[0], rgb_temp[1], rgb_temp[2], NULL, m_result_img);

			// ���� �̹��� release
			for(int i=0; i<3; i++){

				cvReleaseImage(&rgb[i]);
				
				cvReleaseImage(&rgb_temp[i]);
				
			}

		}else if(m_do_sharpening == "False"){
			
			// ������ó�� ���� ���, ��¹��۷� ����
			cvCopy(m_orig_img, m_result_img, NULL);
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

template<typename T>
T Sharpening::limit(const T& value)
{
	return ( (value > 255) ? 255 : ((value < 0) ? 0 : value) );
}
	
ReturnType Sharpening::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onPeriodChanged()
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
	return new Sharpening();
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
	return new Sharpening();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

