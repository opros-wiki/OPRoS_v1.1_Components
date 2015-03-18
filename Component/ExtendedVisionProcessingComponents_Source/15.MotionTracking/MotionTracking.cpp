
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

#include "MotionTracking.h"

int m_area;

IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

CvPoint outPoint[256];	// ������ ������
int width[256], height[256]; //������ �簢�� ����/���� ������
	

BBox bbox = BBox(12);
GMM gmm = GMM(20);

//
// constructor declaration
//
MotionTracking::MotionTracking()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
MotionTracking::MotionTracking(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

MotionTracking::~MotionTracking() {
}

void MotionTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);

	addPort("PositionDataOut", &PositionDataOut);

	// export variable setup


}

// Call back Declaration
ReturnType MotionTracking::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;
	m_area = 0;

	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType MotionTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onExecute()
{
	
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;
	std::vector<PositionDataType> data;

	if(pData != NULL){
		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		m_area = m_in_width * m_in_height;

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

		
		cvCopy(m_orig_img, m_result_img, NULL);

		gmm.update(m_orig_img);

		int cnt = 0;

		if(gmm.frameCnt > 30){
		
			bbox.update(gmm.foreground());

			for(int i = 0; i < bbox.length(); i++){

				int image_width = 0;
				int image_height = 0;
				
				// ����� �̹����� ���̴� ����
				image_width = bbox[i].width;
				image_height = bbox[i].height;

				// ���̰� �������� 1/3 ���� ũ�ٸ� �׸�������
				if((int)m_area * 0.33 > image_width * image_height){
					
					cvRectangle(m_result_img,
						cvPoint(bbox[i].x, bbox[i].y),
						cvPoint(bbox[i].x + bbox[i].width, bbox[i].y + bbox[i].height),
						cvScalar(255, 255, 0), 1, 8, 0);

					//�簢�� ������ ����
					outPoint[cnt].x = bbox[i].x;
					outPoint[cnt].y = bbox[i].y;
					
					width[cnt] = bbox[i].width;
					height[cnt] = bbox[i].height;

					cnt++;

				}
			
			}

		}

		if(cnt > 0)
		{
			for(int i = 0; i < cnt -1; i++)
			{
				PositionDataType base;

				base.setName("");
				base.setX(outPoint[i].x);
				base.setY(outPoint[i].y);
				base.setHeight(height[i]);
				base.setWidth(width[i]);
				
				base.setRadian(NULL);
				
				data.push_back(base);

			}

			PositionDataOut.push(data);
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
	
ReturnType MotionTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onPeriodChanged()
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
	return new MotionTracking();
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
	return new MotionTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

