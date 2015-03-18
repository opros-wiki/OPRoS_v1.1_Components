
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

#include "EdgeDetection.h"

//
// constructor declaration
//
IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;
IplImage* m_edge_img;
IplImage* m_corner_img;

CvSeq* lines;					//���� ����
CvSeq* circles;					//�� ����

CvMemStorage* storage;			// �޸� �Ҵ�

int m_in_width;
int m_in_height;

std::string m_detect_type;

EdgeDetection::EdgeDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
EdgeDetection::EdgeDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

EdgeDetection::~EdgeDetection() {
}

void EdgeDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType EdgeDetection::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_edge_img != NULL)
		m_edge_img = NULL;
	if(m_corner_img != NULL)
		m_corner_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_detect_type = getProperty("DetectionType");

	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EdgeDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onDestroy()
{
	// �̹����� �޸��� ����
	//cvReleaseImage �� IPlImage����ü�� �����ϰ� �޸𸮸� ��ȯ�Ѵ�.
	if(m_orig_img != NULL)
        cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
        cvReleaseImage(&m_gray_img);
    if(m_result_img != NULL)
        cvReleaseImage(&m_result_img);
	if(m_edge_img != NULL)
		cvReleaseImage(&m_edge_img);
	if(m_corner_img != NULL)
		cvReleaseImage(&m_corner_img);
	
	cvReleaseMemStorage(&storage); //�޸� ���� ����

	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onExecute()
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
		if(m_edge_img == NULL){
			m_edge_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_corner_img == NULL){
			m_corner_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 1);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//�׷��̷� ��ȯ - ���� ������ 1ä�� ������ �Ѱ��־�� ��
		cvCvtColor(m_orig_img, m_gray_img, CV_RGB2GRAY);;

		//���� ����
		if(m_detect_type == "Edge")
		{
			//cvCanny�Լ��� ���� ����
			// �ҽ�����, ������ ��, ù��° �Ӱ谪, �ι�° �Ӱ谪, ����ũ��(sobel���꿡 �ʿ�)
			cvCanny(m_gray_img, m_edge_img, 50, 100, 3);
					
			//������ ������ ����ϱ� ���� 8��Ʈ�� �����
			cvMerge(m_edge_img, m_edge_img, m_edge_img, NULL, m_orig_img);
		}
		//���� ����
		else if(m_detect_type == "Line")
		{
			//����� ������ ���� �޸� ���� �Ҵ�
			storage = cvCreateMemStorage(0);
			
			//cvCanny�� ���� ����
			cvCanny(m_gray_img, m_edge_img, 50, 100, 3);
			
			//���� ���� ���󿡼� ������ȯ���� �糡���� �ִ� ���� ����
			//�ҽ�����, ������ ��, �ɼ�,�������κ����� �Ÿ� ����, ���� ����, �Ӱ谪,�Ķ����1,�Ķ����2
			//CV_HOUGH_PROBABILISTIC : �糡���� �ִ� ���� ����.
			//�Ķ����1�� �ּҼ����� ����,�Ķ����2�� �� ���л����� �ִ����
			lines = cvHoughLines2(m_edge_img, storage, CV_HOUGH_PROBABILISTIC, 1 ,
				CV_PI/180, 100,5,5);
			
			//���� �׸���
			for(int k = 0; k < lines->total; k++)
			{
				//������ȯ���� ������ ������ �����ϰ� �ִ� lines���� lineSeg�� ����.
				//lineSeg[0]�� lineSeg[1]�� ������ �� ������ ����
				CvPoint* lineSeg = (CvPoint*)cvGetSeqElem(lines, k);
				//�׸� ��, ��������1,��2, ����,�β�,����� ȭ��
				cvLine(m_orig_img, lineSeg[0], lineSeg[1], CV_RGB(255, 0, 0), 1, 8);

			}
			
		}
		//�ڳ� ����
		else if(m_detect_type == "Corner")
		{
			//�ڳ��� ���� �Լ�
			//�ҽ�����, ��������, �˰��򿡼� ���� Sobel �������� ũ��
			cvPreCornerDetect(m_gray_img, m_corner_img, 3);
			
			//���밪���� ��ȯ �� �ش밪�� �� �ؼҰ��� �ѹ��� �����ϱ� �����̶�� ��
			cvAbs(m_corner_img, m_corner_img);
			
			//corner�� �Ӱ谪�� 0.01���� ũ�� ������ �������� �ʰ� ��
			//�ڳ����� �ʹ� ���� ������� �ʵ��� ����
			cvThreshold(m_corner_img, m_corner_img, 0.03, 0, CV_THRESH_TOZERO);
			
			//�ڳ��� �׸��⿡ �ʿ��� ������ ����
			double fValue;
			double maxValue;
			int    x,y;
			int    m,n;
			bool   bChange;
			int    nSize = 3;  
			int    nCornerCount = 0;

			//�ڳ���(��) �׸���
			//m_corner���� (2 * nSize + 1) * (2 * nSize + 1)ũ���� �����츦 �̵����� ���鼭
			//(x,y)��ġ�� ���� �����쿡 ���� ���ǵǴ� �̿�ȭ�Ұ����� ū �������� Ȯ���ϴ� �������
			//���� �ذ������� Ȯ���Ͽ� ���� �ذ��̸� ������ ǥ����
			for(y = nSize; y < m_corner_img->height - nSize; y++)
			{
				for(x = nSize; x < m_corner_img->width - nSize; x++)
				{
					bChange = false;

					maxValue = cvGetReal2D(m_corner_img, y, x);

					for(n = y-nSize; n <= y+nSize; n++){
						for(m = x-nSize; m <= x+nSize; m++)
						{
							if(m == x && n == y)
								continue;
							fValue = cvGetReal2D(m_corner_img, n, m);//���� ��ǥ�� �ȼ� ���� �����ؼ� ����
							if(fValue >= maxValue)
							{
								bChange = true;
								break;
							}
						}
					}
					if(!bChange)
					{
						nCornerCount++;
						
						cvCircle(m_orig_img, cvPoint(x,y), 5, CV_RGB(255, 0, 0), 2);
					}

				}
			
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
	
ReturnType EdgeDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onPeriodChanged()
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
	return new EdgeDetection();
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
	return new EdgeDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

