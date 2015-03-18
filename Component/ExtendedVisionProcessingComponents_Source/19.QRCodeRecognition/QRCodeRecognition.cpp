
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



#include "QRCodeRecognition.h"
#include "BlobResult.h"

#define MIN_BLOB_AREA  1000
#define MAX_BLOB_AREA 50000

//
// constructor declaration
//
IplImage	*m_image_buff;			// �Է��̹��� ���� ����
IplImage	*m_image_gray;			// ����̹��� ���� ����

int isSymbol(CBlob blob, IplImage *img);

QRCodeRecognition::QRCodeRecognition()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
QRCodeRecognition::QRCodeRecognition(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

QRCodeRecognition::~QRCodeRecognition() {
}

void QRCodeRecognition::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);



	// export variable setup


}

// Call back Declaration
ReturnType QRCodeRecognition::onInitialize()
{
	// �����ʱ�ȭ
	if(m_image_buff != NULL)
		m_image_buff = NULL;
	if(m_image_gray != NULL)
		m_image_gray = NULL;

	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType QRCodeRecognition::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onDestroy()
{
	// ��������
	if(m_image_buff != NULL)
		cvReleaseImage(&m_image_buff);
	if(m_image_gray != NULL)			
		cvReleaseImage(&m_image_gray);

	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;

	if(pData != NULL){
		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���������� �̹������� Ȯ��
		if(m_image_buff == NULL){
			m_image_buff = cvCreateImage(cvSize(RawImage->getWidth(), RawImage->getHeight()), IPL_DEPTH_8U, 3);
		}
		// �׷��̿����� �̹������� Ȯ��
		if(m_image_gray == NULL)
			m_image_gray = cvCreateImage(cvSize(RawImage->getWidth(), RawImage->getHeight()), IPL_DEPTH_8U, 1);

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// �÷������� �׷��̽����Ϸ� ��ȯ
		cvCvtColor( m_image_buff, m_image_gray, CV_BGR2GRAY );

		// ����ȭ
		cvThreshold(m_image_gray, m_image_gray, 0.0, 255.0, CV_THRESH_OTSU);

		// ���̺�
		CBlobResult blobs;
		blobs = CBlobResult(m_image_gray, NULL, 255);
		blobs.Filter(blobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, MIN_BLOB_AREA, MAX_BLOB_AREA);
		
		CvPoint rectP1, rectP2;
		int rectCount = 0;
		// ���̺��������� �ɺ�����
		for (int i = 0; i < blobs.GetNumBlobs(); i++) {
			CBlob blob;
			blob = blobs.GetBlob(i);

			// ���̺��� ������ �ɺ��ϰ��
			if (isSymbol(blob, m_image_gray)) {
				CvPoint p1, p2;
				p1.x = (int)blob.MinX ();
				p1.y = (int)blob.MinY ();
				p2.x = (int)blob.MaxX ();
				p2.y = (int)blob.MaxY ();
				cvRectangle(m_image_buff, p1, p2, CV_RGB(255, 0, 0), 2, 8, 0);

				// QR�ڵ� ��ü��ǥ ����
				if(rectCount == 0) {
					rectP1.x = (int)blob.MinX ();
					rectP1.y = (int)blob.MinY ();
					rectP2.x = (int)blob.MaxX ();
					rectP2.y = (int)blob.MaxY ();
					rectCount++;
				}else {
					if( rectP1.x > p1.x)
						rectP1.x = p1.x;
					if( rectP1.y > p1.y)
						rectP1.y = p1.y;
					if( rectP2.x < p2.x)
						rectP2.x = p2.x;
					if( rectP2.y < p2.y)
						rectP2.y = p2.y;
					rectCount++;
				}
			}
		}

		std::vector<PositionDataType> data;

		// �ɺ��� 3�� ����� ���
		if(rectCount == 3){
			rectP1.x -= 10;
			rectP1.y -= 5;
			rectP2.x += 10;
			rectP2.y += 5;
			cvRectangle(m_image_buff, rectP1, rectP2, CV_RGB(255, 0, 0), 2, 8, 0);
			// QR�ڵ念������			
			PositionDataType base("", NULL, NULL, NULL, NULL, NULL);
			base.setX(rectP1.x);
			base.setY(rectP1.y);
			base.setWidth(rectP2.x - rectP1.x);
			base.setHeight(rectP2.y - rectP1.y);

			data.push_back(base);
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

		// ��������Ʈ�ƿ�(QR�ڵ��ν�����)
		if(data.size() > 0)
			PositionOut.push(data);

		delete pData;
	}

	return OPROS_SUCCESS;
}

// ���̺����� QR�ڵ��� �ɺ����� üũ
int isSymbol(CBlob blob, IplImage *img) {
	int array[5];
	int index;
	uchar previousPixel;
	double target;
	bool symbolFlg1 = false;
	bool symbolFlg2 = false;

	// ���ι��� ����
	memset(array, 0, sizeof(array));
	index = -1;
	previousPixel = 255;
	for (int x = (int)blob.MinX(); x < (int)blob.MaxX(); x++) {
		uchar pixel;
		int y;
		y = ((int)blob.MaxY() + (int)blob.MinY()) / 2;	// �ش翵���� �߰������߽�
		pixel = img->imageData[img->widthStep * y + x * img->nChannels];

		// ù�ȼ��� ����ϰ�� ����
		if (index == -1 && pixel == 255) {
			continue;
		} else {
			if (previousPixel != pixel) {
				index++;
				previousPixel = pixel;
				// �������� ������ ����Ǹ�break
				if (index >= 5){
					break;
				}
			}
			array[index]++;
		}
	}

	// 1:1:3:1:1�� �������� üũ
	target = 0.25 * (array[0] + array[1] + array[3] + array[4]);
	if ((array[2] > (target*2.5)) && (array[2] < (target*3.5))) {		// 0.5�� ����
		symbolFlg1 = true;
	}

	// ���ι�������
	memset(array, 0, sizeof(array));
	index = -1;
	previousPixel = 255;
	for (int y = (int)blob.MinY(); y < (int)blob.MaxY(); y++) {
		uchar pixel;
		int x;
		x = ((int)blob.MaxX() + (int)blob.MinX()) / 2;
		pixel = img->imageData[img->widthStep * y + x * img->nChannels];

		// ù�ȼ��� ����ϰ�� ����
		if (index == -1 && pixel == 255) {
			continue;
		} else {
			if (previousPixel != pixel) {
				index++;
				previousPixel = pixel;
				// �������� ������ ����Ǹ�break
				if (index >= 5){
					break;
				}
			}
			array[index]++;
		}
	}

	// 1:1:3:1:1�� �������� üũ
	target = 0.25 * (array[0] + array[1] + array[3] + array[4]);
	if (((double)array[2] > (target*2.5)) && ((double)array[2] < (target*3.5))) {
		symbolFlg2 = true;
	}
	
	// ���μ��� ��� �����Ҷ�
	if(symbolFlg1 && symbolFlg2)
		return 1;
	else
		return 0;
}

ReturnType QRCodeRecognition::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onPeriodChanged()
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
	return new QRCodeRecognition();
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
	return new QRCodeRecognition();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

