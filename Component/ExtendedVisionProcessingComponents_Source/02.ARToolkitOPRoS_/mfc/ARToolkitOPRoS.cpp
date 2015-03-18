
#include "stdafx.h"
	
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

#include "ARToolkitOPRoS.h"

int m_in_width = 0;
int m_in_height = 0;

HANDLE m_mutex;				// 별도 스레드용 핸들
int m_state;				// 스레드 실행 플러그

char *cparam_name;
std::string sparam_name;
std::string patt_name;
std::string vrml_name;

int patt_id;
int thresh;

double patt_trans[3][4];
double patt_center[2] = {0.0, 0.0};
double patt_width = 80.0;

ARUint8* ar_image;

//기본
IplImage* m_image_buff;			// Original Image
IplImage* m_image_temp;			// Convert Image
IplImage* m_image_dest;			// Convert Image
IplImage* marker_img;			// 4채널 이미지

bool m_compStopped = false;
bool m_threadStopped = false;

bool marker_flag = true;

GLuint texture_name;
GLuint renderbuffer_name;
GLuint framebuffer_name;

// 내보낼 마크 중심 데이터
double outPoint_x, outPoint_y, outPoint_z;

// vrml
int		vrml_id;

//
// constructor declaration
//
ARToolkitOPRoS::ARToolkitOPRoS()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ARToolkitOPRoS::ARToolkitOPRoS(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ARToolkitOPRoS::~ARToolkitOPRoS() {
}

void ARToolkitOPRoS::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);


	// export variable setup
}

UINT ImageFunc( LPVOID pParam )
{
	// 스레드 실행
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// 스레드의 종료 지연 선언
	::WaitForSingleObject(m_mutex, INFINITE);

	// 스레드 기동 플래그 일 경우
	if(m_state == 1){
		// 파라메터 변수
		ARParam cparam;
		ARParam wparam;

		// 카메라 파라메터의 설정
		if(arParamLoad(cparam_name, 1, &wparam) < 0) {
			std::cout<<"Camera Parameter file not found!!"<<std::endl;
			
			// 스레드 종료 플래그 세우기
			m_state = 2;

		}

		if(m_state == 1){		
			// 초기화
			arParamChangeSize(&wparam, wparam.xsize, wparam.ysize, &cparam);
			arInitCparam(&cparam);

			// 파라메터 파일의 로드
			if((patt_id = arLoadPatt(patt_name.c_str())) < 0 )
			{	
				std::cout<<"Pattern file not found!!"<<std::endl;
				
				// 스레드 종료 플래그 세우기
				m_state = 2;
			}

			//vrml 로드
			/*
			if((vrml_id = arVrmlLoadFile(vrml_name.c_str())) < 0)
			{
				std::cout<<"VRML file not found!!"<<std::endl;			
				m_state = 2;
			}
			*/

			if(m_state == 1){		
				//  뷰잉 윈도우 설정
				argInit(&cparam, 1.0, 0, 0, 0, 0);

				glewInit();
				InitTexture();
				InitRenderbuffer();
				InitFramebuffer();

				// 메인루프 개시
				MainLoop();
				std::cout<<"argMainLoop exit"<<std::endl;
			}
		}	
	}

	
	// 스레드 중지 플래그 일 경우
	if(m_state == 2){
		::ReleaseMutex(m_mutex);
	}

	::WaitForSingleObject(m_mutex, INFINITE);

	m_state = 3; // thread stopped

	::ReleaseMutex(m_mutex);

	return 0;

}

// Call back Declaration
ReturnType ARToolkitOPRoS::onInitialize()
{
	m_cpath = getProperty("opros.component.dir");

	m_in_height = 0;
	m_in_width = 0;

	outPoint_x = 0;
	outPoint_y = 0;
	outPoint_z = 0;

	// 이미지용 메모리 초기화
	m_image_buff = NULL;
	marker_img = NULL;
	ar_image = NULL;

	// 카메라 파라메터
	sparam_name = m_cpath + "camera/camera_para.dat";
	cparam_name = (char *) sparam_name.c_str();

	std::string p_patt_name =  getProperty("PattName");
	patt_name = m_cpath + p_patt_name;

	std::string p_vrml_name =  getProperty("VrmlName");
	vrml_name = m_cpath + p_vrml_name;

	thresh = atoi(getProperty("Thresh").c_str());

	//m_state = 0; // ready state
	m_state = 1;
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex 핸들 생성

	// 핸들 예외처리
	if (m_mutex == 0) {
		std::cout << "[ERROR-ARToolKit::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}

	m_compStopped = false;

	// ImageFunc 스레드 실행
	AfxBeginThread(ImageFunc, (LPVOID) this);

	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ARToolkitOPRoS::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onDestroy()
{
	m_compStopped = true;
	while (!m_threadStopped) {
		std::cout<<"--------------------- WAIT thread stopping"<<std::endl;
		arUtilSleep(100);
	}

	if (m_mutex != 0)
		::CloseHandle(m_mutex);

	// 메모리 해제
	cvReleaseImage(&m_image_buff);
	cvReleaseImage(&marker_img);

	ar_image = NULL;

	// ar 변수 해제
	arFreePatt(patt_id);

	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onExecute()
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
		if(m_image_buff == NULL){

			// 이미지용 메모리 할당
			m_image_buff	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);//원본 이미지
			marker_img		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 4);
			m_image_temp	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			m_image_dest	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// 4채널 변환
		cvCvtColor(m_image_buff, marker_img, CV_BGR2BGRA);

		// ar이미지 생성
		ar_image = (ARUint8*)(marker_img->imageData);

		if(m_image_temp->imageData != NULL){
			
			cvCvtColor(m_image_temp, m_image_dest, CV_RGB2BGR);

			cvFlip(m_image_dest, m_image_dest);

		}
//// DataOut
		//중심점이 있으면 데이터를 내보낸다
		if(outPoint_x != 0 && outPoint_y !=0 && outPoint_z !=0)
		{
			double data_x, data_y, data_z;
			
			data_x = outPoint_x;
			data_y = outPoint_y;
            data_z = outPoint_z;

			Coodinates base;
			
			base.setX(data_x);
			base.setY(data_y);
			base.setZ(data_z);
	
			PositionOut.push(base);
		}

		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_image_dest->width, m_image_dest->height, m_image_dest->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_image_dest->width * m_image_dest->height * m_image_dest->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_image_dest->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;

	}

	return OPROS_SUCCESS;
}
	
ReturnType ARToolkitOPRoS::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

void Display(void)
{
	std::cout<<"display"<<std::endl;
	glutHideWindow();
	MainLoop();
}

void MainLoop(void){

	m_threadStopped = false;

		std::cout<<"--------------------- thread START mainLoop"<<std::endl;

	for (;;)
	{
		if(m_compStopped)
		{
//			arUtilSleep(500);
			std::cout<<"--------------------- thread stopped"<<std::endl;
			break;
		}

		ARMarkerInfo *marker_info;
		int marker_num;
		int j , k;
		double			cam_trans[3][4];

		if(ar_image != NULL)
		{
			// Framebuffer
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );

			// clear buffer
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
			// 카메라 영상 묘사
			argDrawMode2D();
			argDispImage(ar_image, 0, 0);

			// 마커의 검출과 인식
			if(arDetectMarker(ar_image, thresh, &marker_info, &marker_num) < 0) 
			{
				//return;
				marker_flag = false;
			}
			else
			{
				marker_flag = true;
			}
			
			if(marker_flag == true)
			{
				// 마커의 일치도 비교
				k = -1;
				for(j = 0; j < marker_num; j++)
				{
					if(patt_id == marker_info[j].id )
					{
						if(k == -1) 
						{
							k = j;
						}
						else if(marker_info[k].cf < marker_info[j].cf ) 
						{
							k = j;
						}
					}
				}

				// 마커의 위치 및 좌표변환행렬 실행
				if(k != -1) 
				{
					arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);

					//마커 중심 정보 추출
					arUtilMatInv(patt_trans, cam_trans);

					//내보낼 데이터 보존
					outPoint_x = cam_trans[0][3];
					outPoint_y = cam_trans[1][3];
					outPoint_z = cam_trans[2][3];

					DrawObject();
				}
				else
				{
					outPoint_x = 0;
					outPoint_y = 0;
					outPoint_z = 0;
				}

				//argSwapBuffers();
				glReadPixels(0, 0, m_in_width, m_in_height, GL_RGB, GL_UNSIGNED_BYTE, 
					m_image_temp->imageData);
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			}
		}
	}

	m_threadStopped = true;
}

void DrawObject(void)
{
	double	gl_para[16];

	// 3D 오브젝트를 묘사하기위한 준비
	argDrawMode3D();
	argDraw3dCamera(0, 0);

	// 변환행렬의 적용
	argConvGlpara(patt_trans, gl_para);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(gl_para);

	// 3D 오브젝트의 묘사
	glTranslatef(0.0, 0.0, 20.0);
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(3.0);
	glutWireCube(40.0);
	
}

void InitTexture( void )
{
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &texture_name );
	glBindTexture( GL_TEXTURE_2D, texture_name );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT,	
		0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
}

void InitRenderbuffer( void )
{
	glGenRenderbuffersEXT( 1, &renderbuffer_name );
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderbuffer_name );
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		RENDERBUFFER_WIDTH, RENDERBUFFER_HEIGHT );
}

void InitFramebuffer( void )
{
	glGenFramebuffersEXT( 1, &framebuffer_name );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );

	// Texture Object  FBO (attach a texture to FBO color attachement point)
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture_name, 0 );

	// RBO  FBO (attach a renderbuffer to depth attachment point)
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, renderbuffer_name );

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
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
	return new ARToolkitOPRoS();
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
	return new ARToolkitOPRoS();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

