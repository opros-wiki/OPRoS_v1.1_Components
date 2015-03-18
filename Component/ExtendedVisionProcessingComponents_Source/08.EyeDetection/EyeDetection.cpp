
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

#include "EyeDetection.h"

IplImage* m_orig_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

CvHaarClassifierCascade *cascade_f;
CvHaarClassifierCascade *cascade_e;
CvMemStorage *storage;

CvPoint outPoint[256];
int rect_width[256], rect_height[256];

std::string hfpath, hepath;

//
// constructor declaration
//
EyeDetection::EyeDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
EyeDetection::EyeDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

EyeDetection::~EyeDetection() {
}

void EyeDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionDataOut", &PositionDataOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType EyeDetection::onInitialize()
{
	std::string cpath = getProperty("opros.component.dir");

	hfpath = cpath + "haarcascade_frontalface_alt.xml";
	hepath = cpath + "haarcascade_eye.xml";

	char *file1 = (char *) hfpath.c_str();
	char *file2 = (char *) hepath.c_str();

	if(m_orig_img != NULL)
		m_orig_img = NULL;

	m_in_width = 0;
	m_in_height = 0;

	// face classifier 의 로드
	cascade_f = (CvHaarClassifierCascade*)cvLoad(file1, 0, 0, 0);
	if(cascade_f == NULL){
		std::cout << " There is no haarcascade face file" << std::endl;

		return OPROS_INPUT_NULL;

	}

	// eyes classifier 의 로드
	cascade_e = (CvHaarClassifierCascade*)cvLoad(file2, 0, 0, 0);
	if(cascade_e == NULL){
		std::cout << " There is no haarcascade eyes file" << std::endl;

		return OPROS_INPUT_NULL;

	}

	storage = cvCreateMemStorage(0);

	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EyeDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);

	cvReleaseMemStorage(&storage);

	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;

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

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		detectEyes();

		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_orig_img->width, m_orig_img->height, m_orig_img->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_orig_img->width * m_orig_img->height * m_orig_img->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_orig_img->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;
	}

	return OPROS_SUCCESS;
}
	
ReturnType EyeDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}


void EyeDetection::detectEyes(){
	
	int i;

	CvSeq *faces = cvHaarDetectObjects(m_orig_img, cascade_f, storage,
		1.1, 3, 0, cvSize(40, 40));

	if(faces->total != 0){
		
		CvRect *r = (CvRect*)cvGetSeqElem(faces, 0);
		
		cvClearMemStorage(storage);

		cvSetImageROI(m_orig_img, 
						cvRect(r->x,
						r->y + (r->height/5.5),
						r->width,
						r->height/3.0));

		// detect eyes
		CvSeq* eyes = cvHaarDetectObjects( 
			m_orig_img, cascade_e, storage,
			1.15, 3, 0, cvSize(25, 15));
	
		CvRect *eye_r;
		std::vector<PositionDataType> data;

		if(eyes->total <= 2)
		{

			for( i = 0; i < (eyes ? eyes->total : 0); i++ ) {
				eye_r = (CvRect*)cvGetSeqElem( eyes, i );
				cvRectangle(m_orig_img, 
							cvPoint(eye_r->x, eye_r->y), 
							cvPoint(eye_r->x + eye_r->width, eye_r->y + eye_r->height),
							CV_RGB(255, 0, 0), 1, 8, 0);

				outPoint[i].x = eye_r->x;
				outPoint[i].y = eye_r->y;

				rect_width[i] = eye_r->width;
				rect_height[i] = eye_r->height;

				PositionDataType base;

				base.setName("");
				base.setX(outPoint[i].x);
				base.setY(outPoint[i].y);
				base.setHeight(rect_height[i]);
				base.setWidth(rect_width[i]);

				base.setRadian(NULL);
				
				data.push_back(base);
				PositionDataOut.push(data);

			}
			
		}

		cvResetImageROI(m_orig_img);

	}

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
	return new EyeDetection();
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
	return new EyeDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

