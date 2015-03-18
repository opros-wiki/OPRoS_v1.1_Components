
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

#include "HumanDetection.h"

IplImage* m_orig_img;
IplImage* m_result_img;
Mat m_img_mat;
HOGDescriptor m_hog;

int m_in_width;
int m_in_height;

CvPoint outPoint;	// ������ ������
int rect_width, rect_height;

//
// constructor declaration
//
HumanDetection::HumanDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
HumanDetection::HumanDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

HumanDetection::~HumanDetection() {
}

void HumanDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);

	addPort("PositionDataOut", &PositionDataOut);



	// export variable setup


}

// Call back Declaration
ReturnType HumanDetection::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HumanDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onExecute()
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

		// ���������� �̹������� Ȯ��
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		m_img_mat = Mat(m_orig_img);

		vector<Rect> found, found_filtered;

		m_hog.detectMultiScale(m_img_mat, found, 0, Size(8,8), Size(32,32), 1.05, 2);

		int i, j;

		// �������� ����
		for(i = 0; i < found.size(); i++){
		
			Rect r = found[i];

			for(j = 0; j < found.size(); j++){
			
				if(j != i && (r & found[i]) == r){
				
					break;
				
				}
			
			}
			if(j == found.size()){
			
				found_filtered.push_back(r);
			
			}
		
		}
		outPoint.x = 0;
		outPoint.y = 0;
		rect_width = 0; 
		rect_height = 0;

		// ��� ���⿡���� ��ġ����
		for(i = 0; i < found_filtered.size(); i++){
		
			Rect r = found_filtered[i];

			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			rectangle(m_img_mat, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);

			//�簢�� ������ ����
			outPoint.x = r.tl().x;
			outPoint.y = r.tl().y;

			rect_width = r.br().x - r.tl().x;
			rect_height = r.br().y - r.tl().y;
		
		}

		// ��ġ ���� ���
		if(outPoint.x != 0 && outPoint.y !=0)
		{
			PositionDataType base;

			base.setName("");
			base.setX(outPoint.x);
			base.setY(outPoint.y);
			base.setHeight(rect_height);
			base.setWidth(rect_width);
			
			base.setRadian(NULL);
			
			data.push_back(base);

			PositionDataOut.push(data);
		}

		m_result_img = &IplImage(m_img_mat);


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
	
ReturnType HumanDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onPeriodChanged()
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
	return new HumanDetection();
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
	return new HumanDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

