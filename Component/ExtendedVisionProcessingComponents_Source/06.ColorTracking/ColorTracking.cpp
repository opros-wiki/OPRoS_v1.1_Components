
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



#include "ColorTracking.h"

//
// constructor declaration
//

IplImage* m_orig_img;			// �Է� ����
IplImage* m_dest_img;			// ó�� ����
IplImage* m_hsv_img;			// hsvó��
IplImage* m_gray_img;			// 1ä�� ����

int m_in_width;
int m_in_height;

std::string m_color;			// ������ ����	
int circle_cnt, rect_cnt;		// �� ����� ���� ������ ��

//����� ���� ����
CvRect* rects;					// ����� �簢�� ������ �簢�� ����ü�� ��´�
CvSeq* circles;					// ����� �� ����
CvMemStorage* storage0 ;		// ������ ����� ���� ���� �޸� ���� �ϳ� �ʿ�


ColorTracking::ColorTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ColorTracking::ColorTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ColorTracking::~ColorTracking() {
}

void ColorTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);

	// export variable setup
}

// Call back Declaration
ReturnType ColorTracking::onInitialize()
{
	//�̹��� ���� �ʱ�ȭ
	m_orig_img		= NULL;
	m_dest_img		= NULL;
	m_hsv_img		= NULL;
	m_gray_img		= NULL;

	m_in_width		= 0;
	m_in_height		= 0;

	//������ �б�
	m_color =  getProperty("TrackingColor");

	// ������ �ƿ��� �ʿ��� ���� �ʱ�ȭ
	circle_cnt = 0;					// ����� ���� ����
	rect_cnt = 0;					// ����� �簢���� ����
	circles = NULL;					// ����� ���� ���� ������ ����ü 
	rects = NULL;					// ����� �簢���� ���� ��Ʈ ����ü


	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ColorTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onDestroy()
{
	// �̹����� �޸��� ����
    cvReleaseImage(&m_orig_img);
	cvReleaseImage(&m_dest_img);
	cvReleaseImage(&m_hsv_img);
	cvReleaseImage(&m_gray_img);

	cvReleaseMemStorage(&storage0);

	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;
	// ������ ��Ʈ ����
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
		if(m_dest_img == NULL){
			m_dest_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_hsv_img == NULL){
			m_hsv_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		}
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		//���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		//HSV��ȯ
		cvCvtColor(m_orig_img, m_hsv_img, CV_BGR2HSV);

		//hsv ���� ��� ��, �������� ���� �ش� ���� ���� ���� ����
		color_config(m_hsv_img, m_color);
		
		//��������
		image_filter(m_gray_img);

		//���� ���� ���� ���� �ʱ�ȭ
		circle_cnt = 0;
		rect_cnt = 0;
		
		//����� ���� ���� �޸� ���� �Ҵ�
		storage0 = cvCreateMemStorage(0);
		
		//�� �׸���
		draw_circle(m_gray_img);
		
		//�簢�� �׸���
		draw_square(m_gray_img);

//// DataOut
		//�Ѱ����� ����Ǹ� 
		if(circles || rects != NULL)
		{
			//�� �����Ͱ� ������
			if(circles)
			{
				//���� ������ŭ 
				for(int k = 0; k < circles->total; k++)
				{
					float* cir;
					int circle_x, circle_y;
					double radi;

					//����� ���� ������ circles���� ���� �Ķ���͸� cir�� ����
					//���� �߽� ��ǥ �� �������� �迭�� ������� �����
					cir = (float*)cvGetSeqElem(circles, k);
					//����� ���� ������ circles���� ���� �Ķ���͸� cir�� ����
					//���� �߽� ��ǥ �� �������� �迭�� ������� �����
					circle_x    = cvRound(cir[0]);     //�߽��� x ��ǥ
					circle_y    = cvRound(cir[1]);     //�߽��� y ��ǥ
					radi        = (double)cvRound(cir[2]);     //������

					PositionDataType base;
					base.setName("circle");
					base.setX(circle_x);
					base.setY(circle_y);
					base.setRadian(radi);

					base.setHeight(NULL);
					base.setWidth(NULL);

					data.push_back(base);
			
				}

			}

			//�簢�� �����Ͱ� ������
			if(rects != NULL)
			{
				for(int j = 0; j < rect_cnt; j++)
				{
					int rect_x, rect_y, rect_width, rect_height;

					rect_x = rects[j].x;
					rect_y = rects[j].y;
					rect_width = rects[j].width;
					rect_height = rects[j].height;

					PositionDataType base;
					base.setName("rectangle");
					base.setX(rect_x);
					base.setY(rect_y);
					base.setHeight(rect_height);
					base.setWidth(rect_width);

					base.setRadian(NULL);

					data.push_back(base);
				}
			}

			PositionOut.push(data);
		}
			
//// ImageOut
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
	
ReturnType ColorTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}




//���� ���� �� ����ȭ
void ColorTracking::color_config(IplImage* image, std::string config){
	
	//������ ���� ����
	CvScalar hsv_min, hsv_max, hsv_min2, hsv_max2;
	if(image != NULL)
	{
		IplImage* m_tem1_img = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);//���� ���� �̹���
		IplImage* m_tem2_img = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);//���� ���� �̹���
	
		//�ʿ��� ���� ���� ���� ���
		if(config == "Red")
		{
			//���� - ���� �ΰ��� ��ƺ�
			hsv_min = cvScalar(0, 85, 100, 0);
			hsv_max = cvScalar(10, 255, 255, 0);

			hsv_min2 = cvScalar(170, 85, 100, 0);
			hsv_max2 = cvScalar(220, 255, 255, 0);

		}
		else if(config =="Green")
		{
			//�ʷ�
			hsv_min = cvScalar(55, 80, 100, 0);
			hsv_max = cvScalar(75, 255, 255, 0);
		}
		else if(config == "Blue")
		{
			//�Ķ�
			hsv_min = cvScalar(100, 100,100 , 0);
			hsv_max = cvScalar(130, 255, 200, 0);
		}
		else if(config =="Yellow")
		{
			//���
			hsv_min = cvScalar(20, 100, 100, 0);
			hsv_max = cvScalar(35, 255, 255, 0);
		}
		
		if(config == "Red")
		{
			//�ϴ� ����� �� �������� ��ƺ�
			cvInRangeS(image, hsv_min, hsv_max, m_tem1_img);
			cvInRangeS(image, hsv_min2, hsv_max2, m_tem2_img);

			//���뿵�� ����
			cvOr(m_tem1_img, m_tem2_img , m_gray_img);
			
		}	
		else
		{
			//���尡 �ƴ� ���� �״��
			cvInRangeS(image, hsv_min, hsv_max, m_gray_img);
		}

		cvReleaseImage(&m_tem1_img);
		cvReleaseImage(&m_tem2_img);
	}
}

//////////////////////
//
//    ���� ����
//
//////////////////////
void ColorTracking::image_filter(IplImage* image)
{

	cvDilate(image, image, NULL, 2);//��â
	cvErode(image, image, NULL, 2);//ħ��	
		
	//����þ� ���� ����
	cvSmooth( image, image, CV_GAUSSIAN, 9, 9 );

}


//////////////////////
//
//   �� ����
//
//////////////////////
void ColorTracking::draw_circle(IplImage* image)
{
	
	CvSeq* m_circle = NULL;					// �� ����
	CvMemStorage* storage1 = NULL;			// �޸� �Ҵ�

	//����� ���� ���� �޸� ���� �Ҵ�
	storage1 = cvCreateMemStorage(0);

	//�� ���� ���� ����
	circle_cnt = 0;

	//���� �߽����� �����ϱ� ���� ������� �ػ�
	// 1�̸� �Է¿���� ���� ũ��, 2�̸� �Է� ������ ����/������ ��ũ���� �����
	double dp = 1.5; 
	double min_dist = 300;				//����� ���� �߽� ������ �ּҰŸ�.���� ���� ���� ���� ���� ��-? 
	double cannyThreshold = 100;		//cvCanny �Լ� �Ӱ谪
	double accThreshold = 50;			//cvCanny �Լ� ��������� �Ӱ谪
	int min_radius = 50;				//�ּ� ������
	int max_radius = 150;				//�ִ� ������
	int cx, cy = 0;
		
	//�ҽ�����, �޸𸮽��丮�� ������, �޼ҵ�����, ������ �ػ�, ������ �� �������� �ּҰŸ�
	m_circle = cvHoughCircles(image, storage1, CV_HOUGH_GRADIENT,
		dp,min_dist, cannyThreshold, accThreshold, min_radius, max_radius);

		//���� 1�� �� ������
	if(m_circle->total >0 )
	{
		// �����͸� �������� ���� ���� ������ �������� ����
		circles = cvCloneSeq(m_circle, storage0);

		//�� �׸���
		for(int k = 0; k < m_circle->total; k++)
		{
			float* circle;
			int radius;

			//����� ���� ������ circles���� ���� �Ķ���͸� circle�� ����
			//���� �߽� ��ǥ �� �������� �迭�� ������� �����
			circle = (float*)cvGetSeqElem(m_circle, k);
			cx     = cvRound(circle[0]);     //�߽��� x ��ǥ
			cy     = cvRound(circle[1]);     //�߽��� y ��ǥ
			radius = cvRound(circle[2]);     //������

			//���׸���
			
			if(radius > min_radius && radius < max_radius)
			{
				//�߽���
				cvCircle(m_orig_img, cvPoint(cx, cy), 3, CV_RGB(240,0,255), -1, 8, 0);

				//�������
				cvCircle(m_orig_img, cvPoint(cx, cy), radius, CV_RGB(255,255,255), 3, 8, 0);

			}	
		}
	}
	else // ���� ������
	{
		circles = NULL;
		circle_cnt = 0;
	}

	cvReleaseMemStorage(&storage1);
}

//////////////////////
//
//   �簢�� ����
//
//////////////////////
void ColorTracking::draw_square(IplImage* image)
{
	CvMemStorage* storage2 = NULL;			// �޸� �Ҵ�
	CvMemStorage* storage3 = NULL;			// �޸� �Ҵ�
	storage2 = cvCreateMemStorage(0);		//�簢�� ������ ���� ������ ���� �޸�
	storage3 = cvCreateMemStorage(0);		//����� �簢���� ���� �޸�

	CvSeq* contours ;						//������						
	CvSeq* result ;							//������ �߿� �簢���� �ɸ��� ������ ����
	
	CvPoint corner[4];						//�簢���� �� �� ��

	rects = NULL;							//�簢���� �� �� ������ ���� �簢�� ����ü - ������ �������� ���� ���

	rects = new CvRect [100];				//�簢���� ������ �̸� �� �����⿡ ������ ��
		
	//����������
	cvFindContours(image, storage2, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	//�ܰ��� ���� ������ �簢�� �ձ�
	for(;contours !=0; contours = contours ->h_next)
	{
		//������ �ٻ�ȭ
		//cvApproxPoly �ܰ�������, ũ��, �޸�, �޼ҵ�, ���е�, 0�̸� �ش� �ܰ����� �˻�
		result = cvApproxPoly( contours, sizeof(CvContour), storage3, 
			CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );	

		//�������� 4��, �ܰ����ȼ����� 1000�� �̻� �϶� �簢������ ����
		//���齺 ���� �ϴ����� cvCheckContourConvexity(result)
		if( result->total == 4 &&
					cvContourArea(result,CV_WHOLE_SEQ,0) > 500)
		{

			//�ʱ� ��ġ ����
			CvPoint* st = (CvPoint*)cvGetSeqElem(result, 0);

			///////ù��° ������ ���� - ������ ������ ���� �� ��
			double fMaxDist = 0.0;

			for(int i = 1; i < result->total; i++)
			{
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, i);

				double fDist = sqrt((double)( ( st->x - pt->x) * (st->x - pt->x)
					+ (st->y - pt->y) * (st->y - pt->y) ));

				if(fDist > fMaxDist)
				{
					corner[0] = *pt;
					fMaxDist = fDist;
				}
			}

			///////�ι�° ������ ���� - ù��° ������ ���� �� ��
			fMaxDist = 0.0;

			for(int i = 1; i < result->total; i++)
			{
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, i);

				double fDist = sqrt((double)( ( corner[0].x - pt->x) * (corner[0].x - pt->x)
					+ (corner[0].y - pt->y) * (corner[0].y - pt->y) ));

				if(fDist > fMaxDist)
				{
					corner[1] = *pt;
					fMaxDist = fDist;
				}
			}

			////////����° ������ ���� - ù��° ���� �ι�° ������ ���� �� ��
			fMaxDist = 0.0;
			
			for(int i = 1; i < result->total; i++)
			{
				
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, i);

				int tempx;
				int tempy;

				//ù��° ���� �ι�° ������ �߰� �� ã�� 
				//��ǥ�� ������ ���� ���� �־ ū ���� �Ǻ��Ŀ� ��� ��
				
				//x��ǥ
				if(corner[0].x >= corner[1].x)
				{
					
					tempx = corner[0].x - (corner[0].x - corner[1].x) / 2;
				
				}
				else if(corner[0].x < corner[1].x)
				{
					
					tempx = corner[0].x + (corner[1].x - corner[0].x) / 2;
				
				}

				//y��ǥ
				if(corner[0].y >= corner[1].y)
				{

					tempy = corner[0].y - (corner[0].y - corner[1].y) / 2;

				}
				else if(corner[0].y < corner[1].y)
				{

					tempy = corner[0].y - (corner[1].y - corner[0].y) / 2;
		
				}

				//���� �߰������� ���� �� �� ã��
				double fDist = sqrt((double)( ( tempx - pt->x) * (tempx - pt->x)
					+ (tempy - pt->y) * (tempy - pt->y) ));

				if(fDist > fMaxDist)
				{
					corner[2] = *pt;
					fMaxDist = fDist;

				}
			}

			//////�׹�° ������ ����
			
			//���� ������ �̿��Ͽ� ��ǥ��鿡�� �簢���� �ʺ��� �ִ밪�� ���ϴ� ���� ���
			//�̹� ���� �� ���� �������� ������ ����, �� ������ ���� ��� ��(x,y)�� �� ����
			//�ﰢ�����γ��� �� �� ������ ���� �ִ밡 �Ǵ� ���� �� ��° ���������� ����.

			int x1 = corner[0].x;
			int y1 = corner[0].y;

			int x2 = corner[1].x;
			int y2 = corner[1].y;
			
			int x3 = corner[2].x;
			int y3 = corner[2].y;

			int nMaxDim = 0;

			for(int j = 0; j < result->total; j++ )
			{
				CvPoint* pt = (CvPoint *)cvGetSeqElem(result, j);

				int x = pt->x;
				int y = pt->y;

				int nDim = abs( ( x1 * y2 + x2 * y + x  * y1 ) - ( x2 * y1 + x  * y2 + x1 * y  ) )
						  +abs( ( x1 * y  + x  * y3+ x3 * y1 ) - ( x  * y1 + x3 * y  + x1 * y3 ) )
						  +abs( ( x  * y2 + x2 * y3+ x3 * y  ) - ( x2 * y  + x3 * y2 + x  * y3 ) );

				if(nDim > nMaxDim)
				{
					corner[3] = *pt;
					nMaxDim = nDim;
				}
			}

			//ù��° -> �ι�°
			cvLine(m_orig_img, cvPoint(corner[0].x, corner[0].y), cvPoint(corner[1].x, corner[1].y), CV_RGB(255,120,0), 3);
			//ù��° -> ����°
			cvLine(m_orig_img, cvPoint(corner[0].x, corner[0].y), cvPoint(corner[2].x, corner[2].y), CV_RGB(255,120,0), 3);
			//����° -> ù��°
			cvLine(m_orig_img, cvPoint(corner[3].x, corner[3].y), cvPoint(corner[1].x, corner[1].y), CV_RGB(255,120,0), 3);
			//����° -> �ι�°
			cvLine(m_orig_img, cvPoint(corner[3].x, corner[3].y), cvPoint(corner[2].x, corner[2].y), CV_RGB(255,120,0), 3);

			//������ �׸���
			cvCircle(m_orig_img, cvPoint(corner[0].x, corner[0].y), 5, CV_RGB(255,255,255), -1);
			cvCircle(m_orig_img, cvPoint(corner[1].x, corner[1].y), 5, CV_RGB(255,255,255), -1);
			cvCircle(m_orig_img, cvPoint(corner[2].x, corner[2].y), 5, CV_RGB(255,255,255), -1);
			cvCircle(m_orig_img, cvPoint(corner[3].x, corner[3].y), 5, CV_RGB(255,255,255), -1);
	
			//�簢�� ũ�� ���
			int mWidth = abs(corner[3].x - corner[0].x);
			int mHeight = abs(corner[3].y - corner[0].y);

			//��Ʈ ����ü�� �־ ����
			rects[rect_cnt].x = corner[0].x;
			rects[rect_cnt].y = corner[0].y;
			rects[rect_cnt].width = mWidth;
			rects[rect_cnt].height = mHeight;

			//�ε��� ����
			rect_cnt++;

			//�غ��� �迭���� ũ�� ����
			if(rect_cnt > 99 )
			{
				break;
			}
		}

		//����� �޸� ����
		cvClearSeq(contours);
		cvClearSeq(result);
	}
	//����� �޸� ����
	cvReleaseMemStorage(&storage2);
	cvReleaseMemStorage(&storage3);

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
	return new ColorTracking();
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
	return new ColorTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

