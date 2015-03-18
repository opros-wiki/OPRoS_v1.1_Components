
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

#include "AreaDetection.h"

#define SOBEL_Y_APERTURE_SIZE 3

#define SCALE	1
#define	SHIFT	0

IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_destination_image_y;
IplImage* m_destination_edge;
IplImage* m_edge_img;
IplImage* m_result_img;
IplImage* m_gray_temp;
IplImage* m_point_img;

int m_in_width;
int m_in_height;

CvMemStorage* storage;
CvSeq* contours;

CvPoint newPoint;
CvPoint oldPoint;

//
// constructor declaration
//
AreaDetection::AreaDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
AreaDetection::AreaDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

AreaDetection::~AreaDetection() {
}

void AreaDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionDataOut", &PositionDataOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType AreaDetection::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_edge_img != NULL)
		m_edge_img = NULL;
	if(m_destination_image_y != NULL)
		m_destination_image_y = NULL;
	if(m_destination_edge != NULL)
		m_destination_edge = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_gray_temp != NULL)
		m_gray_temp = NULL;
	if(m_point_img != NULL)
		m_point_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	storage = cvCreateMemStorage(0);
	contours = NULL;

	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType AreaDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_edge_img != NULL)
		cvReleaseImage(&m_edge_img);
	if(m_destination_image_y != NULL)
		cvReleaseImage(&m_destination_image_y);
	if(m_destination_edge != NULL)
		cvReleaseImage(&m_destination_edge);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);
	if(m_gray_temp != NULL)
		cvReleaseImage(&m_gray_temp);
	if(m_point_img != NULL)
		cvReleaseImage(&m_point_img);

	cvReleaseMemStorage(&storage);

	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onExecute()
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
		if(m_edge_img == NULL){
			m_edge_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_destination_image_y == NULL){
			m_destination_image_y = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_16S, 1);
		}
		if(m_destination_edge == NULL){
			m_destination_edge = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_gray_temp == NULL){
			m_gray_temp = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_point_img == NULL){
			m_point_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		cvCvtColor(m_orig_img, m_gray_img, CV_RGB2GRAY);

		cvSobel(m_gray_img, m_destination_image_y, 1, 0, SOBEL_Y_APERTURE_SIZE);

		cvConvertScaleAbs(m_destination_image_y, m_destination_edge, SCALE, SHIFT);

		cvThreshold(m_destination_edge, m_destination_edge, 0.0, 255.0, CV_THRESH_OTSU);

		// ���̺� �۾� ����
		CBlobLabeling blob;
		blob.SetParam(m_destination_edge, 200);
		blob.DoLabeling();

		// ���� ����
		cvSmooth(m_destination_edge, m_destination_edge, CV_MEDIAN, 3, 0);

		// �������� ���� ����
		CvPoint *point;
		contours = cvHoughLines2(m_destination_edge, storage, CV_HOUGH_PROBABILISTIC,
									1, CV_PI/180, 100, 50, 10);
		
		cvZero(m_result_img);
		cvZero(m_point_img);

		// ���� ���� ���ϱ�
		for(int i = 0; i < contours->total; i++){
			point = (CvPoint *)cvGetSeqElem(contours, i);
			
			//������ ���ؼ� �������� +- 5�� �̻��� ���� �׸��� �ʴ´�
			double dx, dy, da;
			dx = point[1].x - point[0].x;
			dy = point[1].y - point[0].y;
			if(!dx) 
				dx = 1e-6;
			da = atan(dy/dx);

			if(dx<0) 
				da+=CV_PI;

			da = da*180/CV_PI;
			da = abs(da);
			
			if(da >= 85 && da <= 90){
				cvLine(m_result_img, point[0], point[1], CV_RGB(255, 255, 255), 3, 4, 0);
			}
		
		}
		
		cvCvtColor(m_result_img, m_gray_temp, CV_RGB2GRAY);

		cvClearMemStorage(storage);

		cvSmooth(m_gray_temp, m_gray_temp, CV_MEDIAN, 5);

		// ���� ����
		cvDilate(m_gray_temp, m_gray_temp, 0 , 2);
		cvErode(m_gray_temp, m_gray_temp, 0, 2);

		CvSeq* c_mask = cvSegmentFGMask(m_gray_temp, 1, 8, storage);

		CvPoint points[256];
		int index = 0;

		cvCopy(m_orig_img, m_point_img, NULL);

		// ���ο��� �׸���
		for(int i = 0; c_mask != NULL; c_mask = c_mask->h_next, i++){
		
			if(i < 256){
			
				cvDrawContours(m_gray_temp, c_mask, 
					CV_RGB(255,255,255), CV_RGB(255,255,255),
					-1, CV_FILLED, 8);

				CvRect r = cvBoundingRect(c_mask, 0);

				cvRectangle(m_point_img, cvPoint(r.x, r.y),
						cvPoint(r.x + r.width, r.y + r.height),
						cvScalar(255, 0, 255), CV_FILLED, 8, 0);
				
				cvCircle(m_point_img, 
					cvPoint((r.x + r.x + r.width) / 2, r.y + r.height),
					3, CV_RGB(255,255,255), CV_FILLED, 8, 0);

				points[index].x = (r.x + r.x + r.width) / 2;
				points[index].y = (r.y + r.height);

				index++;

			}
		}

		// ���� ���� ����
		for(int i = 0; i < index - 1; i++){
			for(int j = i + 1; j < index; j++){
				if(points[i].x > points[j].x){
					swap(&points[i], &points[j]);
				}
			}
		}

		//����Ʈ ��ǥ ��Ʈ�� ����

		std::vector<PositionDataType> data;

		for(int i = 0; i < index; i++){
		
			if(i == 0){
			
				cvLine(m_point_img, cvPoint(1, m_in_height-1),
					cvPoint(points[i].x, points[i].y),
					CV_RGB(0, 0, 255), 2, 8, 0);

				// ��Ʈ�� ���
				PositionDataType base;

				base.setX(1);
				base.setY(m_in_height - 1);
					
				base.setName("");
				base.setHeight(NULL);
				base.setWidth(NULL);
				base.setRadian(NULL);

				data.push_back(base);
			
			}

			if(points[i+1].x != 0 && points[i+1].y != 0){
			
				if(points[i+1].x > 0 && points[i+1].y > 0){

					cvLine(m_point_img, cvPoint(points[i].x, points[i].y),
						cvPoint(points[i+1].x, points[i+1].y),
						CV_RGB(0, 0, 255), 2, 8, 0);

				}
				
				// ��Ʈ�� ���
				PositionDataType base;

				base.setX(points[i].x);
				base.setY(points[i].y);

				base.setName("");
				base.setHeight(NULL);
				base.setWidth(NULL);
				base.setRadian(NULL);

				data.push_back(base);
			}

			if(i == index - 1){
			
				cvLine(m_point_img, cvPoint(points[i].x, points[i].y),
					cvPoint(m_in_width - 1, m_in_height -1 ),
					CV_RGB(0, 0, 255), 2, 8, 0);

				// ��Ʈ�� ���
				PositionDataType base;

				base.setX(m_in_width -1);
				base.setY(m_in_height - 1);

				base.setName("");
				base.setHeight(NULL);
				base.setWidth(NULL);
				base.setRadian(NULL);

				data.push_back(base);
			
			}
		}

		

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_point_img->width, m_point_img->height, m_point_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_point_img->width * m_point_img->height * m_point_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_point_img->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����
		PositionDataOut.push(data);

		memset(points, 0, sizeof(points));

		delete pData;
	}

	return OPROS_SUCCESS;
}

void AreaDetection::swap(CvPoint *n1, CvPoint *n2){

	CvPoint temp = *n1;
	*n1 = *n2;
	*n2 = temp;

}
	
ReturnType AreaDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onPeriodChanged()
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
	return new AreaDetection();
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
	return new AreaDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

