
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



#include "FaceDetectionHaarlike.h"

IplImage* m_orig_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

CvPoint outPoint;	// 내보낼 데이터
int rect_width, rect_height;

CvHaarClassifierCascade *cascade;

CvMemStorage *storage;

std::string s_ff_path;	// cascade 파일

//
// constructor declaration
//
FaceDetectionHaarlike::FaceDetectionHaarlike()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FaceDetectionHaarlike::FaceDetectionHaarlike(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FaceDetectionHaarlike::~FaceDetectionHaarlike() {
}

void FaceDetectionHaarlike::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);

	addPort("PositionDataOut", &PositionDataOut);



	// export variable setup


}

// Call back Declaration
ReturnType FaceDetectionHaarlike::onInitialize()
{
	s_ff_path = getProperty("opros.component.dir") + "haarcascade_frontalface_alt.xml";

	// cascade xml 파일(frontface)
	char *file = (char *) s_ff_path.c_str();

	// 사용할 이미지의 초기화
	if(m_orig_img != NULL)
		m_orig_img == NULL;
	if(m_result_img != NULL)
		m_result_img == NULL;

	// 사용할 이미지 사이즈의 초기화
	m_in_width = 0;
	m_in_height = 0;

	// cascade 파일의 로드
	cascade = (CvHaarClassifierCascade*)cvLoad(file, 0, 0, 0);
	if(cascade == NULL){
	
		std::cout << "There is no haarcascade face file" << std::endl;
		std::cout << "The file name is 'haarcascade_frontalface_alt.xml'" << std::endl;

	}

	// 메모리 storage의 초기화
	storage = cvCreateMemStorage(0);

	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionHaarlike::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onDestroy()
{
	// 사용한 이미지의 릴리스
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	// 사용한 storage의 릴리스
	cvReleaseMemStorage(&storage);

	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;
	std::vector<PositionDataType> data;

	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// 원본영상의 이미지영역 확보
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());
		
		// 입력 영상을 출력영상으로 복사
		cvCopy(m_orig_img, m_result_img, NULL);

		outPoint.x = 0;
		outPoint.y = 0;
		rect_width = 0; 
		rect_height = 0;

		// 얼굴의 검출
		detectFace();

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



		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_result_img->width, m_result_img->height, m_result_img->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_result_img->width * m_result_img->height * m_result_img->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_result_img->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionHaarlike::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}


void FaceDetectionHaarlike::detectFace(){

	int xROI;		// 관심영역 x
	int yROI;		// 관심영역 y
	int widthROI;	// 관심영역 x + width;
	int heightROI;	// 관심영역 y + height;

	xROI = (int)m_in_width * 0.25;
	yROI = (int)m_in_height * 0.25;
	widthROI = (int)m_in_width * 0.25 + (int)m_in_width * 0.25;
	heightROI = (int)m_in_height * 0.25 + (int)m_in_width * 0.25;

	// 관심 영역의 설정
	cvSetImageROI(m_orig_img, cvRect(xROI, yROI, widthROI, heightROI));

	// 출력 이미지에 관심영역의 사각형 묘사
	cvRectangle(m_result_img, cvPoint(xROI, yROI),
				cvPoint(xROI+widthROI, yROI+heightROI),
				CV_RGB(255, 255, 0), 1, 8, 0);

	CvSeq *faces = cvHaarDetectObjects(m_orig_img, cascade, storage,
					1.1, 3, 0, cvSize(40, 40));

	if(faces->total != 0){
	
		CvRect *r = (CvRect*)cvGetSeqElem(faces, 0);

		cvRectangle(m_result_img, cvPoint(r->x + xROI, r->y + yROI),
					cvPoint(r->x + r->width + xROI, r->y + r->height + yROI),
					CV_RGB(255, 0, 0), 1, 8, 0);

		//사각형 시작점 보존
		outPoint.x = r->x + xROI;
		outPoint.y = r->y + yROI;
		rect_width = r->width;
		rect_height = r->width;

	}
	else
	{

		outPoint.x = 0;
		outPoint.y = 0;
		rect_width = 0;
		rect_height = 0;

	}

	cvClearMemStorage(storage);
	cvResetImageROI(m_orig_img);

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
	return new FaceDetectionHaarlike();
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
	return new FaceDetectionHaarlike();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

