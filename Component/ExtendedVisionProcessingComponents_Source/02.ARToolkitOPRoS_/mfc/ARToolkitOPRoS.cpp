
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

HANDLE m_mutex;				// ���� ������� �ڵ�
int m_state;				// ������ ���� �÷���

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

//�⺻
IplImage* m_image_buff;			// Original Image
IplImage* m_image_temp;			// Convert Image
IplImage* m_image_dest;			// Convert Image
IplImage* marker_img;			// 4ä�� �̹���

bool m_compStopped = false;
bool m_threadStopped = false;

bool marker_flag = true;

GLuint texture_name;
GLuint renderbuffer_name;
GLuint framebuffer_name;

// ������ ��ũ �߽� ������
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
	// ������ ����
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// �������� ���� ���� ����
	::WaitForSingleObject(m_mutex, INFINITE);

	// ������ �⵿ �÷��� �� ���
	if(m_state == 1){
		// �Ķ���� ����
		ARParam cparam;
		ARParam wparam;

		// ī�޶� �Ķ������ ����
		if(arParamLoad(cparam_name, 1, &wparam) < 0) {
			std::cout<<"Camera Parameter file not found!!"<<std::endl;
			
			// ������ ���� �÷��� �����
			m_state = 2;

		}

		if(m_state == 1){		
			// �ʱ�ȭ
			arParamChangeSize(&wparam, wparam.xsize, wparam.ysize, &cparam);
			arInitCparam(&cparam);

			// �Ķ���� ������ �ε�
			if((patt_id = arLoadPatt(patt_name.c_str())) < 0 )
			{	
				std::cout<<"Pattern file not found!!"<<std::endl;
				
				// ������ ���� �÷��� �����
				m_state = 2;
			}

			//vrml �ε�
			/*
			if((vrml_id = arVrmlLoadFile(vrml_name.c_str())) < 0)
			{
				std::cout<<"VRML file not found!!"<<std::endl;			
				m_state = 2;
			}
			*/

			if(m_state == 1){		
				//  ���� ������ ����
				argInit(&cparam, 1.0, 0, 0, 0, 0);

				glewInit();
				InitTexture();
				InitRenderbuffer();
				InitFramebuffer();

				// ���η��� ����
				MainLoop();
				std::cout<<"argMainLoop exit"<<std::endl;
			}
		}	
	}

	
	// ������ ���� �÷��� �� ���
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

	// �̹����� �޸� �ʱ�ȭ
	m_image_buff = NULL;
	marker_img = NULL;
	ar_image = NULL;

	// ī�޶� �Ķ����
	sparam_name = m_cpath + "camera/camera_para.dat";
	cparam_name = (char *) sparam_name.c_str();

	std::string p_patt_name =  getProperty("PattName");
	patt_name = m_cpath + p_patt_name;

	std::string p_vrml_name =  getProperty("VrmlName");
	vrml_name = m_cpath + p_vrml_name;

	thresh = atoi(getProperty("Thresh").c_str());

	//m_state = 0; // ready state
	m_state = 1;
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);	//Mutex �ڵ� ����

	// �ڵ� ����ó��
	if (m_mutex == 0) {
		std::cout << "[ERROR-ARToolKit::onInit()] Mutex is NULL ============================ " << std::endl;

		return OPROS_INTERNAL_FAULT;
	}

	m_compStopped = false;

	// ImageFunc ������ ����
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

	// �޸� ����
	cvReleaseImage(&m_image_buff);
	cvReleaseImage(&marker_img);

	ar_image = NULL;

	// ar ���� ����
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
		if(m_image_buff == NULL){

			// �̹����� �޸� �Ҵ�
			m_image_buff	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);//���� �̹���
			marker_img		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 4);
			m_image_temp	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			m_image_dest	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// 4ä�� ��ȯ
		cvCvtColor(m_image_buff, marker_img, CV_BGR2BGRA);

		// ar�̹��� ����
		ar_image = (ARUint8*)(marker_img->imageData);

		if(m_image_temp->imageData != NULL){
			
			cvCvtColor(m_image_temp, m_image_dest, CV_RGB2BGR);

			cvFlip(m_image_dest, m_image_dest);

		}
//// DataOut
		//�߽����� ������ �����͸� ��������
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
		
			// ī�޶� ���� ����
			argDrawMode2D();
			argDispImage(ar_image, 0, 0);

			// ��Ŀ�� ����� �ν�
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
				// ��Ŀ�� ��ġ�� ��
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

				// ��Ŀ�� ��ġ �� ��ǥ��ȯ��� ����
				if(k != -1) 
				{
					arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);

					//��Ŀ �߽� ���� ����
					arUtilMatInv(patt_trans, cam_trans);

					//������ ������ ����
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

	// 3D ������Ʈ�� �����ϱ����� �غ�
	argDrawMode3D();
	argDraw3dCamera(0, 0);

	// ��ȯ����� ����
	argConvGlpara(patt_trans, gl_para);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(gl_para);

	// 3D ������Ʈ�� ����
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

