
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

#include "FaceDetectionASM.h"

IplImage* m_orig_img;
IplImage* m_result_img;

int m_in_width;
int m_in_height;

char* m_model_name;
char* m_cascade_name;

asmfitting fit_asm;

CvMemStorage* __storage;
CvHaarClassifierCascade* __cascade;

asm_shape shape, detshape;	// ASM 변수
bool flag = false;			// 인식 플래그
bool check_flg = false;
int j = 0;

std::string s_model_path;	// 모델 파일
std::string s_cascade_path;	// cascade 파일

//
// constructor declaration
//
FaceDetectionASM::FaceDetectionASM()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FaceDetectionASM::FaceDetectionASM(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FaceDetectionASM::~FaceDetectionASM() {
}

void FaceDetectionASM::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);

	addPort("PositionDataOut", &PositionDataOut);



	// export variable setup


}

// Call back Declaration
ReturnType FaceDetectionASM::onInitialize()
{
	s_model_path = getProperty("opros.component.dir") + "my68-1d.amf";
	s_cascade_path = getProperty("opros.component.dir") + "haarcascade_frontalface_alt2.xml";

	m_model_name = (char *) s_model_path.c_str();	 // 모델 파일
	m_cascade_name = (char *) s_cascade_path.c_str(); // cascade 파일

	CvMemStorage* __storage = NULL;							// storage
	CvHaarClassifierCascade* __cascade = NULL;				// haar like

	// 모델 파일 읽기
	if(fit_asm.Read(m_model_name) == false){
	
		std::cout << "There is no 'Model file'" << std::endl;
		std::cout << "The Model file's name is 'my68-1d.amf'" << std::endl;

	}
	
	// cascade 파일 읽기
	if(InitDetectCascade(m_cascade_name) == false){
	
		std::cout << "There is no 'Cascade file" << std::endl;
		std::cout << "The Cascade file's name is 'haarcascade_frontalface_alt2.xml'" << std::endl;
	
	}

	// 영상 이미지의 초기화
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	// 영상 사이즈의 초기화
	m_in_width = 0;
	m_in_height = 0;

	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionASM::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onDestroy()
{
	// 영상의 릴리스
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	// storage의 릴리스
	cvReleaseMemStorage(&__storage);

	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onExecute()
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
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height),
				IPL_DEPTH_8U, 3);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// 얼굴 detection
		if(flag == false){
		
			flag = DetectOneFace(detshape, m_orig_img);

			if(flag){
			
				InitShapeFromDetBox(shape, detshape, 
					fit_asm.GetMappingDetShape(), fit_asm.GetMeanFaceWidth());

				j++;

				check_flg = true;
			}
			else{
			
				check_flg  = false;
			
			}
		
		}

		// 얼굴 fit정보 그리기
		if(check_flg){
		
			flag = fit_asm.ASMSeqSearch(shape, m_orig_img, j, true, 24);

			if(flag){
			
				fit_asm.Draw(m_orig_img, shape);

				// 왼 눈동자 좌표!
				cvCircle(m_orig_img, cvPoint(shape[31].x,shape[31].y), 3, CV_RGB(255, 0, 0), 2, 8, 0);

				PositionDataType base1;

				base1.setName("왼 눈동자 좌표");
				base1.setX(shape[31].x);
				base1.setY(shape[31].y);
				
				base1.setHeight(NULL);
				base1.setWidth(NULL);
				base1.setRadian(NULL);
				
				data.push_back(base1);

				// 오른 눈동자 좌표!
				cvCircle(m_orig_img, cvPoint(shape[36].x,shape[36].y), 3, CV_RGB(255, 0, 0), 2, 8, 0);

				PositionDataType base2;

				base2.setName("오른 눈동자 좌표");
				base2.setX(shape[36].x);
				base2.setY(shape[36].y);
				
				base2.setHeight(NULL);
				base2.setWidth(NULL);
				base2.setRadian(NULL);
				
				data.push_back(base2);

				// 입 중심 좌표
				cvCircle(m_orig_img, cvPoint(shape[66].x,shape[66].y), 3, CV_RGB(255, 255, 0), 2, 8, 0);

				PositionDataType base3;

				base3.setName("입 중심 좌표");
				base3.setX(shape[66].x);
				base3.setY(shape[66].y);
				
				base3.setHeight(NULL);
				base3.setWidth(NULL);
				base3.setRadian(NULL);
				
				data.push_back(base3);

				// 코끝 중심 좌표
				cvCircle(m_orig_img, cvPoint(shape[67].x,shape[67].y), 3, CV_RGB(255, 0, 255), 2, 8, 0);

				PositionDataType base4;

				base4.setName("코 끝 좌표");
				base4.setX(shape[67].x);
				base4.setY(shape[67].y);
				
				base4.setHeight(NULL);
				base4.setWidth(NULL);
				base4.setRadian(NULL);
				
				data.push_back(base4);

				PositionDataOut.push(data);

			}
		}

		// 작업 영상을 결과 영상에 복사
		cvCopy(m_orig_img, m_result_img, NULL);

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
	
ReturnType FaceDetectionASM::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

// 영상에서 얼굴 하나 detection
bool FaceDetectionASM::DetectOneFace(asm_shape &Shape, const IplImage *image){

	asm_shape* detShape;
	int nFaces;
	bool flag = DetectAllFace(&detShape, nFaces, image);

	if(!flag)
		return false;

	int iSelectedFace = 0;
	double x0 = image->width/2., y0 = image->height/2.;
	double x, y, d, D = 1e307;

	for(int i = 0; i < nFaces; i++){
	
		x = (detShape[i][0].x + detShape[i][1].x) / 2.;
		y = (detShape[i][0].y + detShape[i][1].y) / 2.;
		d = sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));

		if(d < D){
		
			D = d;
			iSelectedFace = i;
		
		}
	
	}

	Shape = detShape[iSelectedFace];
	delete []detShape;

	return true;

}

// 모든 얼굴 detection
bool FaceDetectionASM::DetectAllFace(asm_shape** Shapes, int& n_shapes, const IplImage* image){

	IplImage* pWork = cvCreateImage(cvSize(image->width/2, image->height/2),
		image->depth, image->nChannels);
	cvPyrDown(image, pWork, CV_GAUSSIAN_5x5);

	CvSeq* pFaces = cvHaarDetectObjects(pWork, __cascade, __storage,
		1.1, 3, CV_HAAR_DO_CANNY_PRUNING);

	cvReleaseImage(&pWork);

	n_shapes = 0;
	if(0 == pFaces->total)
		return false;

	*Shapes = new asm_shape[pFaces->total];
	n_shapes = pFaces->total;

	for(int iFace = 0; iFace < pFaces->total; iFace++){
	
		(*Shapes)[iFace].Resize(2);
		CvRect* r = (CvRect*)cvGetSeqElem(pFaces, iFace);

		CvPoint pt1, pt2;
		pt1.x = r->x * 2;
		pt2.x = (r->x + r->width) * 2;
		pt1.y = r->y * 2;
		pt2.y = (r->y + r->height) * 2;

		(*Shapes)[iFace][0].x = r->x * 2.0;
		(*Shapes)[iFace][0].y = r->y * 2.0;
		(*Shapes)[iFace][1].x = (*Shapes)[iFace][0].x + 2.0 * r->width;
		(*Shapes)[iFace][1].y = (*Shapes)[iFace][0].y + 2.0 * r->height;
	
	}

	return true;

}

// cascade 파일 읽기
bool FaceDetectionASM::InitDetectCascade(const char* cascade_name){

	__cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0);
	if(__cascade == 0){
		
		return false;

	}

	__storage = cvCreateMemStorage(0);
	
	return true;

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
	return new FaceDetectionASM();
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
	return new FaceDetectionASM();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

