
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
IplImage	*m_image_buff;			// 입력이미지 저장 버퍼
IplImage	*m_image_gray;			// 흑백이미지 저장 버퍼

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
	// 버퍼초기화
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
	// 버퍼해제
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
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;

	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// 원본영상의 이미지영역 확보
		if(m_image_buff == NULL){
			m_image_buff = cvCreateImage(cvSize(RawImage->getWidth(), RawImage->getHeight()), IPL_DEPTH_8U, 3);
		}
		// 그레이영상의 이미지영역 확보
		if(m_image_gray == NULL)
			m_image_gray = cvCreateImage(cvSize(RawImage->getWidth(), RawImage->getHeight()), IPL_DEPTH_8U, 1);

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// 컬러영상을 그레이스케일로 변환
		cvCvtColor( m_image_buff, m_image_gray, CV_BGR2GRAY );

		// 이진화
		cvThreshold(m_image_gray, m_image_gray, 0.0, 255.0, CV_THRESH_OTSU);

		// 레이블링
		CBlobResult blobs;
		blobs = CBlobResult(m_image_gray, NULL, 255);
		blobs.Filter(blobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, MIN_BLOB_AREA, MAX_BLOB_AREA);
		
		CvPoint rectP1, rectP2;
		int rectCount = 0;
		// 레이블링영역에서 심볼추출
		for (int i = 0; i < blobs.GetNumBlobs(); i++) {
			CBlob blob;
			blob = blobs.GetBlob(i);

			// 레이블링된 영역이 심볼일경우
			if (isSymbol(blob, m_image_gray)) {
				CvPoint p1, p2;
				p1.x = (int)blob.MinX ();
				p1.y = (int)blob.MinY ();
				p2.x = (int)blob.MaxX ();
				p2.y = (int)blob.MaxY ();
				cvRectangle(m_image_buff, p1, p2, CV_RGB(255, 0, 0), 2, 8, 0);

				// QR코드 전체좌표 설정
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

		// 심볼이 3개 검출된 경우
		if(rectCount == 3){
			rectP1.x -= 10;
			rectP1.y -= 5;
			rectP2.x += 10;
			rectP2.y += 5;
			cvRectangle(m_image_buff, rectP1, rectP2, CV_RGB(255, 0, 0), 2, 8, 0);
			// QR코드영역정보			
			PositionDataType base("", NULL, NULL, NULL, NULL, NULL);
			base.setX(rectP1.x);
			base.setY(rectP1.y);
			base.setWidth(rectP2.x - rectP1.x);
			base.setHeight(rectP2.y - rectP1.y);

			data.push_back(base);
		}
		
		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();

		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_image_buff->width, m_image_buff->height, m_image_buff->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_image_buff->width * m_image_buff->height * m_image_buff->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_image_buff->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		// 데이터포트아웃(QR코드인식정보)
		if(data.size() > 0)
			PositionOut.push(data);

		delete pData;
	}

	return OPROS_SUCCESS;
}

// 레이블영역이 QR코드의 심볼여부 체크
int isSymbol(CBlob blob, IplImage *img) {
	int array[5];
	int index;
	uchar previousPixel;
	double target;
	bool symbolFlg1 = false;
	bool symbolFlg2 = false;

	// 가로방향 조사
	memset(array, 0, sizeof(array));
	index = -1;
	previousPixel = 255;
	for (int x = (int)blob.MinX(); x < (int)blob.MaxX(); x++) {
		uchar pixel;
		int y;
		y = ((int)blob.MaxY() + (int)blob.MinY()) / 2;	// 해당영역의 중간지점중심
		pixel = img->imageData[img->widthStep * y + x * img->nChannels];

		// 첫픽셀이 흰색일경우 제외
		if (index == -1 && pixel == 255) {
			continue;
		} else {
			if (previousPixel != pixel) {
				index++;
				previousPixel = pixel;
				// 흑백흑백흑 순으로 검출되면break
				if (index >= 5){
					break;
				}
			}
			array[index]++;
		}
	}

	// 1:1:3:1:1의 비율인지 체크
	target = 0.25 * (array[0] + array[1] + array[3] + array[4]);
	if ((array[2] > (target*2.5)) && (array[2] < (target*3.5))) {		// 0.5의 편차
		symbolFlg1 = true;
	}

	// 세로방향조사
	memset(array, 0, sizeof(array));
	index = -1;
	previousPixel = 255;
	for (int y = (int)blob.MinY(); y < (int)blob.MaxY(); y++) {
		uchar pixel;
		int x;
		x = ((int)blob.MaxX() + (int)blob.MinX()) / 2;
		pixel = img->imageData[img->widthStep * y + x * img->nChannels];

		// 첫픽셀이 흰색일경우 제외
		if (index == -1 && pixel == 255) {
			continue;
		} else {
			if (previousPixel != pixel) {
				index++;
				previousPixel = pixel;
				// 흑백흑백흑 순으로 검출되면break
				if (index >= 5){
					break;
				}
			}
			array[index]++;
		}
	}

	// 1:1:3:1:1의 비율인지 체크
	target = 0.25 * (array[0] + array[1] + array[3] + array[4]);
	if (((double)array[2] > (target*2.5)) && ((double)array[2] < (target*3.5))) {
		symbolFlg2 = true;
	}
	
	// 가로세로 모두 만족할때
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

