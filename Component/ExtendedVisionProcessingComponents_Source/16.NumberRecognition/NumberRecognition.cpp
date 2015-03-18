
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



#include "NumberRecognition.h"
#include "BlobLabeling.h"

//
// constructor declaration
//
IplImage* m_image_buff;			// �Է��̹��� ���� ����
IplImage* m_image_buff_temp;
IplImage* m_image_buff_temp2;
IplImage* m_image_buff_diff;


IplImage* templateImage[10];
IplImage* templateGrayImage[10];
IplImage* templateBinaryImage[10];

int m_in_width;
int m_in_height;
int m_option_num;

const char* number[] = {"0","1","2","3","4","5","6","7","8","9"};

NumberRecognition::NumberRecognition()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
NumberRecognition::NumberRecognition(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

NumberRecognition::~NumberRecognition() {
}

void NumberRecognition::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);



	// export variable setup


}

// Call back Declaration
ReturnType NumberRecognition::onInitialize()
{
	// �����ʱ�ȭ
	if(m_image_buff != NULL)
		m_image_buff = NULL;
	if(m_image_buff_temp != NULL)
		m_image_buff_temp = NULL;
	if(m_image_buff_temp2 != NULL)
		m_image_buff_temp2 = NULL;
	if(m_image_buff_diff != NULL)
		m_image_buff_diff = NULL;

	for(int i=0; i<10; i++)
	{
		if(templateImage[i] != NULL)
			templateImage[i] = NULL;
		if(templateGrayImage[i] != NULL)
			templateGrayImage[i] = NULL;
		if(templateBinaryImage[i] != NULL)
			templateBinaryImage[i] = NULL;
	}

	std::string cpath = getProperty("opros.component.dir");
	std::string bmp0 = cpath + "0.bmp";
	std::string bmp1 = cpath + "1.bmp";
	std::string bmp2 = cpath + "2.bmp";
	std::string bmp3 = cpath + "3.bmp";
	std::string bmp4 = cpath + "4.bmp";
	std::string bmp5 = cpath + "5.bmp";
	std::string bmp6 = cpath + "6.bmp";
	std::string bmp7 = cpath + "7.bmp";
	std::string bmp8 = cpath + "8.bmp";
	std::string bmp9 = cpath + "9.bmp";

	// �������ø���������
	templateImage[0] = cvLoadImage( bmp0.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[1] = cvLoadImage( bmp1.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[2] = cvLoadImage( bmp2.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[3] = cvLoadImage( bmp3.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[4] = cvLoadImage( bmp4.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[5] = cvLoadImage( bmp5.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[6] = cvLoadImage( bmp6.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[7] = cvLoadImage( bmp7.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[8] = cvLoadImage( bmp8.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
	templateImage[9] = cvLoadImage( bmp9.c_str(), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );	

	for(int i=0; i<10; i++){
		if(templateImage[i] != NULL){
			if(templateGrayImage[i] != NULL)
				cvReleaseImage(&templateGrayImage[i]);			 
			if(templateBinaryImage[i] != NULL)
				cvReleaseImage(&templateBinaryImage[i]);

			templateGrayImage[i] = cvCreateImage( cvSize(templateImage[i]->width, templateImage[i]->height), IPL_DEPTH_8U, 1 );
			templateBinaryImage[i] = cvCreateImage( cvSize(templateImage[i]->width, templateImage[i]->height), IPL_DEPTH_8U, 1 );

			//  RGB���� ������� ��ȯ
			cvCvtColor( templateImage[i], templateGrayImage[i], CV_RGB2GRAY );
			cvNot(templateGrayImage[i], templateGrayImage[i]);

			//  ��鿵���� ����ȭ
			cvThreshold( templateGrayImage[i], templateBinaryImage[i], 70, 255, CV_THRESH_BINARY );			
			
		}
	}

	m_in_width = 0;
	m_in_height = 0;

	m_option_num = atoi(getProperty("option_num").c_str());

	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType NumberRecognition::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onDestroy()
{
	if(m_image_buff != NULL)
		cvReleaseImage(&m_image_buff);
	if(m_image_buff_temp != NULL)
		cvReleaseImage(&m_image_buff_temp);
	if(m_image_buff_temp2 != NULL)
		cvReleaseImage(&m_image_buff_temp2);
	if(m_image_buff_diff != NULL)
		cvReleaseImage(&m_image_buff_diff);

	for(int i=0; i<10; i++)
	{
		if(templateImage[i] != NULL)
			cvReleaseImage(&templateImage[i]);
		if(templateGrayImage[i] != NULL)
			cvReleaseImage(&templateGrayImage[i]);
		if(templateBinaryImage[i] != NULL)
			cvReleaseImage(&templateBinaryImage[i]);
	}

	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onExecute()
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

		// �̹����������&InPort�̹����� ������ ���۰� ��������ʴٸ� �޸𸮹�ȯ
		if(m_image_buff != NULL)
			cvReleaseImage(&m_image_buff);
		if(m_image_buff_temp != NULL)
			cvReleaseImage(&m_image_buff_temp);
		if(m_image_buff_temp2 != NULL)
			cvReleaseImage(&m_image_buff_temp2);
		if(m_image_buff_diff != NULL)
			cvReleaseImage(&m_image_buff_diff);

		//********* Inport�̹����� ����(IplImage*)�� �޸��Ҵ�*********
		// cvCreateImage�� IplImage����ü�� �޸𸮸� �����ϰ� �����͸� �Ѱ��ش�
		// IplImage* img = cvCreateImage(size, depth, chennel);
		// depth = �̹����� �� �ȼ��� ǥ���ϴ� ��Ʈũ��. ��κ� 1����Ʈ�� ǥ�� 
		//			-> IPL_DEPTH_8U : unsigned 8-bit integer
		// chennel = �̹����� ǥ���ϴ� ä�ΰ��� �׷��̴�1ä��, RGB 3ä��, RGBA 4ä��
		m_image_buff = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		m_image_buff_temp = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		m_image_buff_temp2 = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		m_image_buff_diff = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// �÷������� �׷��̽����Ϸ� ��ȯ
		cvCvtColor( m_image_buff, m_image_buff_temp, CV_BGR2GRAY );

		cvMorphologyEx(m_image_buff_temp, m_image_buff_temp2, NULL, NULL, CV_MOP_CLOSE, 2);
		cvAbsDiff(m_image_buff_temp, m_image_buff_temp2, m_image_buff_diff);
		cvThreshold(m_image_buff_diff, m_image_buff_temp, 0.0, 255.0, CV_THRESH_OTSU);

		// ���̺�
		CBlobLabeling blob;
		blob.SetParam(m_image_buff_temp, 200); // ���̺� �� �̹����� �ּ��ȼ��� ����
		blob.DoLabeling();

		float templateScale = 0.5f;
		double minVal, maxVal;
		CvPoint minLoc, maxLoc;
		CvPoint tempLoc;

		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0);

		std::vector<PositionDataType> data;

		// �ɼǰ��� ����ã��
		if(templateBinaryImage[m_option_num] != NULL)
		{
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			int templateHeight = templateBinaryImage[m_option_num]->height;
			int templateWidth = templateBinaryImage[m_option_num]->width;
			
			// ���ø� ��Ī ����κ� 
			for(int i = 1; i <= 3; i++)  // ������(50%���� 150%���� 50%�������� ��� �� Ȯ��) ,i �� templateScale �� �����Ͽ� ���� ����
			{
				int tempTemplateHeight = (int)(templateWidth * (i * templateScale));
				int tempTemplateWidth = (int)(templateHeight * (i * templateScale));
				IplImage *tempBinaryTemplateImage = cvCreateImage(cvSize(tempTemplateWidth, tempTemplateHeight), IPL_DEPTH_8U, 1);
				// W - w + 1, H - h + 1
				IplImage *result = cvCreateImage(cvSize(m_image_buff_temp->width - tempTemplateWidth + 1, m_image_buff_temp->height - tempTemplateHeight + 1), IPL_DEPTH_32F, 1);
				cvResize(templateBinaryImage[m_option_num], tempBinaryTemplateImage, CV_INTER_LINEAR);

				cvMatchTemplate(m_image_buff_temp, tempBinaryTemplateImage, result, CV_TM_CCOEFF_NORMED); // �ۺ���Ī
				cvMinMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, NULL);

				if(maxVal >= 0.68) // 65% �̻� ������ ��� Ž���Ͽ��ٰ� ����
				{
					tempLoc.x = maxLoc.x + tempTemplateWidth;
					tempLoc.y = maxLoc.y + tempTemplateHeight;
					cvRectangle(m_image_buff_temp, maxLoc, tempLoc, CV_RGB(255, 0, 0), 1);
					cvPutText(m_image_buff, number[m_option_num], cvPoint(maxLoc.x, maxLoc.y - 5), &font, CV_RGB(255,0,0));
	
					// �ν��� ��ǥ����
					PositionDataType base("", NULL, NULL, NULL, NULL, NULL);
					base.setName(number[m_option_num]);
					base.setX(maxLoc.x);
					base.setY(maxLoc.y);
					base.setWidth(tempLoc.x - maxLoc.x);
					base.setHeight(tempLoc.y - maxLoc.y);

					data.push_back(base);
				}

				cvReleaseImage(&tempBinaryTemplateImage);
				cvReleaseImage(&result);
			}
		}

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_image_buff->width, m_image_buff->height, m_image_buff->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_image_buff->width * m_image_buff->height * m_image_buff->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_image_buff->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		// ��������Ʈ�ƿ�(�����ν�����)
		if(data.size() > 0)
			PositionOut.push(data);

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType NumberRecognition::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onPeriodChanged()
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
	return new NumberRecognition();
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
	return new NumberRecognition();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

