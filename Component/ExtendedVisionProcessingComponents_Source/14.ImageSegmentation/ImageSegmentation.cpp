
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

#include "ImageSegmentation.h"

//
// constructor declaration
//

int m_in_height;                // ó���� �̹����� Height
int m_in_width;                 // ������ �̹�����  Width
int segPort_width;				// ���ҿ��� ��� ��Ʈ �̹��� ����
int segPort_height;				// ���ҿ��� ��� ��Ʈ �̹��� ����

IplImage* m_image_buff;			// Original Image
IplImage* m_image_dest;         // OutPut Image ���� ��� 
IplImage* m_image_curr;			// �ӽ� ���
IplImage* m_resize;				// ���ҿ��� ���

CvRect* m_rec_seg;				//���ɿ������� ������ ��Ʈ

std::string m_segment;			// ������ �޾Ƽ� ����
int m_number;					// ������ �޾Ƽ� ���� 

int div_value;					//���μ��� ������ ����
int selected ;					//���� ����

std::string outString;			//������ ��Ʈ�� ���� ����

ImageSegmentation::ImageSegmentation()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ImageSegmentation::ImageSegmentation(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ImageSegmentation::~ImageSegmentation() {
}

void ImageSegmentation::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageOut2", &ImageOut2);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ImageSegmentation::onInitialize()
{
	// �̹����� �޸� �ʱ�ȭ
    m_image_buff		= NULL; 
	m_image_dest		= NULL;
	m_image_curr		= NULL;
	m_resize			= NULL;

	//���� �ʱ�ȭ
	m_in_width			= 0;             
    m_in_height			= 0;

	outString			= "";

	//���� ��Ʈ����
	m_rec_seg = NULL;

	//���ҵ� �̹��� ũ�� ����
	segPort_width = 0;
	segPort_height = 0;

	//������ �б�
	m_segment =  getProperty("Segmentation");
	m_number =   atoi(getProperty("Number").c_str());

	//�ǽð� ������ �ƴϹǷ� ó���� �Ǻ��ϱ�
	if(m_segment == "1*1")
	{
		div_value = 1;	
	}
	else if(m_segment == "2*2")
	{
		div_value = 2;
	}
	else if(m_segment == "3*3")
	{
		div_value = 3;
	}
	else
	{
		div_value = 0;

	}
		
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ImageSegmentation::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onDestroy()
{
	// �̹����� �޸��� ����
    cvReleaseImage(&m_image_buff);
	cvReleaseImage(&m_image_dest);
	cvReleaseImage(&m_image_curr);
    cvReleaseImage(&m_resize);

	 if(m_rec_seg != NULL)
		delete m_rec_seg;

	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;
	RawImage result2;

	if(pData != NULL){
		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();
		std::vector<PositionDataType> data;

		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// ���������� �̹������� Ȯ��
		if(m_image_buff == NULL){
			m_image_buff = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_image_dest == NULL){
			m_image_dest = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_image_curr == NULL){
			m_image_curr = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		}
		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());
		
		cvCopy(m_image_buff, m_image_dest, 0);	//��¿�

		//���������� ����
		cvSetZero(m_image_curr);

		//������ ������ ����� �Ǿ�����
		if(div_value > 0)
		{
			//�ش� ���������� �̹��� �����ϱ�
			image_segmentation(div_value);
			
		}
		else
		{
			//������ ����� ���� �ʾ��� ����� ����ó��
			if(m_resize != NULL)
			{
				 cvReleaseImage(&m_resize);
			}
			
			//���� �̹��� ũ�� �ݿ�
			m_resize = cvCreateImage(cvGetSize(m_image_curr), IPL_DEPTH_8U, 3);
			
			//�⺻ ȭ�� ���
			cvCopy(m_image_curr, m_resize);

		}

//// DataOut		
		if(outString != "")
		{
			std::string data_string;

			data_string = outString;

			PositionDataType base;
			
			base.setName(data_string);

			base.setX(NULL);
			base.setY(NULL);	
			base.setHeight(NULL);
			base.setWidth(NULL);
			base.setRadian(NULL);

			data.push_back(base);
	
			PositionOut.push(data);

		}

//// ImageOut 1
		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_image_dest->width, m_image_dest->height, m_image_dest->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_image_dest->width * m_image_dest->height * m_image_dest->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_image_dest->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����
		
//// ImageOut 2
		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage2 = result2.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage2->resize(m_resize->width, m_resize->height, m_resize->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size2 = m_resize->width * m_resize->height * m_resize->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata2 = pimage2->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata2, m_resize->imageData, size2);

		// ��Ʈ�ƿ�
		opros_any mdata2 = result2;
		ImageOut2.push(result2);//����
		
		delete pData;

	}
	return OPROS_SUCCESS;
}
	
ReturnType ImageSegmentation::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageSegmentation::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}



void ImageSegmentation::image_segmentation(int d_v)
{
	int d_value = d_v;		//�Ѿ�� �� ����
	int line = 0;			//���� ���� �˷��� ����
	int seg = 0;			//���ҵ� ȭ���� ����

	int width = m_in_width;				//���� ȭ���� ����
	int height = m_in_height;			//���� ȭ���� ����
	int move_distX =0, move_distY = 0;	//������ ����

	CvRect m_rec_roi;					//���ҵ� ũ�⺰ �簢��
	CvPoint pt1,pt2;					//�簢�� ���� ����		
	
	IplImage* m_selected = NULL;		// ���ҵ� ���� ����
	IplImage* m_temp = NULL;			// ���� ��¿� �ӽ� ����

	std::string tempString;
	
	//�Ѿ�� �̹��� ũ��� ���� �̹��� ����
	m_temp   = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	
	//����
	cvCopy(m_image_dest, m_temp, 0 );

	//�ʱ�ȭ
	if(m_rec_seg != NULL)
	{
		delete m_rec_seg;
		m_rec_seg = NULL;

	}
	m_rec_roi.x = 0;
	m_rec_roi.y = 0;
	m_rec_roi.width = 0;
	m_rec_roi.height =0;

	// ���ҵ� ȭ�� ���� ���
	seg = d_value * d_value;

	//���ɿ��� ������ ���� �簢��
	m_rec_seg = new CvRect [seg];
	cvWaitKey(10);
	
	//�簢�� �׸��� ������ �Ÿ� (���ҵ� �� ������ ����/���ΰ� ��)
	move_distX = (int)(width/d_value);			
	move_distY = (int)(height/d_value);

	//ù��° �ٿ��� ����
	line = 1;
		
	//ù ���������� �ʱ�ȭ
	pt1 = cvPoint(0, 0);
	pt2 = cvPoint((int)(width/d_value), (int)(height/d_value));

	//���ҵ� ���� ó�� - �׸���, ��ȣ�ֱ�
	for(int i = 0; i < seg; i++)
	{
		//��ǥ �ֱ�
		m_rec_seg[i].x = pt1.x;
		m_rec_seg[i].y = pt1.y;
		m_rec_seg[i].width =  pt2.x;
		m_rec_seg[i].height =  pt2.y;		

		//�׸� ƴ�� ��� �ຽ
		cvWaitKey(10);

		// ��ǥ�� �簢�� �׸���
		cvDrawRect( m_image_dest, 
			cvPoint(m_rec_seg[i].x, m_rec_seg[i].y),
			cvPoint(m_rec_seg[i].width, m_rec_seg[i].height),
			CV_RGB(255 ,255, 0),1,8);

		//�ش� ������ ���������� ���õ� �������� �Ǻ�
		if(m_number - 1 == i)
		{	
			selected = m_number;
		}

		//���� �׸��⿡���� �ٹٲ㼭 �׸���(Y�� �̵� ����)
		if(line == (int)((i+1) / d_value))
		{
			pt1.x = 0;
			pt1.y = pt1.y + move_distY;
			pt2.x = pt1.x + move_distX;
			pt2.y = pt1.y + move_distY;

			line = line + 1; // ���� �ø���
		}
		else
		{
			//�ƴϸ� X���� ����
			pt1.x = pt1.x + move_distX;
			pt2.x = pt2.x + move_distX;
		
		}	
		
		//�ؽ�Ʈ ����
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, 8);
		char* text;
		
		//���� ���� ���ҵ� ĭ�� ���� ��ȣ�� �ű�
		text = new char [seg];
		
		//���ɿ��� ����
		cvSetImageROI(m_temp, m_rec_seg[i]);

		//���ڸ� �ؽ�Ʈ�� ��ȯ
		sprintf(&text[i], "%d",i+1);

		//�Է��� ����� ����
		cvPutText(m_image_dest, &text[i], cvPoint( m_rec_seg[i].x+10, m_rec_seg[i].y + 30), 
			&font, CV_RGB(255, 0, 0));
		
		//���ɿ��� ����
		cvResetImageROI(m_temp);
	
	}

	//�ùٸ��� ���õ� ������ ������
	if(selected > 0 && selected <= seg)
	{
		
		//�ʱ�ȭ �ѹ� ���ְ�
		if(m_selected != NULL)
		{
			m_selected = NULL;
		}

		//�ش� ������ ����
		m_rec_roi.x = m_rec_seg[selected-1].x;
		m_rec_roi.y = m_rec_seg[selected-1].y;
		m_rec_roi.width = m_rec_seg[selected-1].width -m_rec_roi.x;
		m_rec_roi.height = m_rec_seg[selected-1].height - m_rec_roi.y;

		//����� ���� ũ���� �̹��� ����
		m_selected = cvCreateImage(cvSize(m_rec_roi.width, m_rec_roi.height ),IPL_DEPTH_8U, 3);

		
		//�ѹ� �� üũ
		if(m_rec_roi.width > 0 && m_rec_roi.height > 0 )
		{		
			//���ɿ��� ����
			cvSetImageROI(m_temp, m_rec_roi);
			
			//�ش� ���ɿ����� ���� 
			cvCopy(m_temp, m_selected,0);

			cvWaitKey(10);
						
			//���ɿ��� ����
			cvResetImageROI(m_temp);

			//���õ� ���� ���� �簢������ ǥ��
			cvDrawRect( m_image_dest, cvPoint(m_rec_roi.x, m_rec_roi.y),
				cvPoint(m_rec_roi.x + m_rec_roi.width , m_rec_roi.y + m_rec_roi.height),
			CV_RGB(255 ,0, 0),3,8);

			//���ڸ� �ؽ�Ʈ�� ��ȯ
			std::stringstream sstr;
			sstr << selected;
			tempString = sstr.str();
		}
	}
	else
	{
		tempString = "";

	}
	outString  = "";
	//���õ� �̹����� �����ϸ�
	if(m_selected != NULL)
	{
		//�� ���õ� �̹��� ũ�� ���� - ����� �̹��� ũ�� ������ ���
		segPort_width = m_selected->width;
		segPort_height = m_selected->height;


		//�޸� �ѹ� ���� �ϰ�
		if(m_resize != NULL)
		{
			 cvReleaseImage(&m_resize);
		}
	
		//���õ� �̹����� ũ�⿡ �°� ��� �̹��� ����
		m_resize = cvCreateImage(cvSize(segPort_width,segPort_height), IPL_DEPTH_8U, 3);
		
		// �̹��� ����
		cvCopy(m_selected, m_resize);

		outString = tempString;

	}
	else //���õ� �̹����� �������� ������
	{		
		//����� �̹��� ũ�� ���� ����
		int resize_width, resize_height;

		//������ ������ �̹��� ����� ���ٸ�
		if(segPort_width == 0 && segPort_height == 0)
		{
			//�ʱ� ������� ����- ���� �̹��� ũ��
			resize_width = width;
			resize_height = height;

		}
		else
		{
			//������ ������ ������ ����
			resize_width = segPort_width;
			resize_height = segPort_height;
		}
		
		//�޸� ����
		if(m_resize != NULL)
		{
			 cvReleaseImage(&m_resize);
		}
		//���õ� �̹����� ������ ������ ũ�� ����
		m_resize = cvCreateImage(cvSize(resize_width, resize_height), IPL_DEPTH_8U, 3);

		//���õ� �̹����� ���� ��쿡�� ������ ȭ�� ���
		cvSetZero(m_resize);

	}

	//����� �޸� ����
	cvReleaseImage(&m_selected);
	cvReleaseImage(&m_temp);

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
	return new ImageSegmentation();
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
	return new ImageSegmentation();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

