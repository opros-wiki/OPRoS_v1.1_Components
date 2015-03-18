
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

#include "VideoWrite.h"

//
// constructor declaration
//

IplImage* m_orig_img;

std::string m_codec;			//������ ���� ����

int m_in_width;
int m_in_height;

int fourcc;						//�ڵ� ���� ���ڰ� ����
double fps;						//�ʴ� ������ ����

CvFont font;					//ȭ�� ��ܿ� ��ȭ���� �˸��� �ؽ�Ʈ ����
CvVideoWriter* VideoOut;		//���� ���� ����

VideoWrite::VideoWrite()
		:ImageIn(OPROS_LAST,1)
{
	portSetup();
}

//
// constructor declaration (with name)
//
VideoWrite::VideoWrite(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{
	
	portSetup();
}

//
// destructor declaration
//

VideoWrite::~VideoWrite() {
}

void VideoWrite::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);

	// export variable setup
}

// Call back Declaration
ReturnType VideoWrite::onInitialize()
{

	//�̹��� �� ���� �ʱ�ȭ
	m_orig_img		= NULL;

	VideoOut		= NULL;

	m_in_width		= 0;
	m_in_height		= 0;

	m_codec =  getProperty("Codec");

	m_videoFile = getProperty("VideoFile");
	if (m_videoFile == "") m_videoFile = "save.avi";

	//�ʴ� ������ ����
	fps = 30.0;

	//��Ʈ init
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0,0,3,8);

	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onStart()
{

	return OPROS_SUCCESS;
}
	
ReturnType VideoWrite::onStop()
{	
	
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onDestroy()
{
	// �̹����� �޸��� ����
    cvReleaseImage(&m_orig_img);

	//���߰ų� ����� �� ����ǰ� ��	
	if(VideoOut != NULL)
	{
		std::cout<< "---------------  Saved!  ---------------" << std::endl;
	}

	cvReleaseVideoWriter(&VideoOut);

	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onEvent(Event *evt)
{

	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onExecute()
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
		if(m_orig_img == NULL)
		{
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		//���� ��ȭ �غ�
		if(VideoOut == NULL)
		{
			std::string cpath = getProperty("opros.component.dir");
			cpath += m_videoFile;

			//�̹��� ������ �޸� �Ҵ�
			VideoOut = cvCreateVideoWriter(cpath.c_str(), fourcc, fps, cvSize(m_in_width, m_in_height), 1);
	
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		
		//������ �� �� �̹���
		IplImage* m_frame_img;
		m_frame_img   = cvCreateImage(cvGetSize(m_orig_img), IPL_DEPTH_8U, 3);

		cvCopy(m_orig_img, m_frame_img, 0);	//�����

		if(VideoOut != NULL)
		{
			//������ ����
			cvWriteFrame(VideoOut, m_frame_img);
		}
		
		//�ؽ�Ʈ ����
		cvPutText(m_orig_img, "Record" , cvPoint(20,30), &font, CV_RGB(255, 0, 0));

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

		//����� ���� �޸� ����
		cvReleaseImage(&m_frame_img);
	}

	return OPROS_SUCCESS;
}
	
ReturnType VideoWrite::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoWrite::onPeriodChanged()
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
	return new VideoWrite();
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
	return new VideoWrite();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

