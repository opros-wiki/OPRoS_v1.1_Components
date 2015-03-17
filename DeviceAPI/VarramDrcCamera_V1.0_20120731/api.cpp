#include "drc.h"

BYTE robotdata1[64]; //명령데이터를 위한 배열
BYTE robotdata2[64]; //명령데이터를 위한 배열
double drcBright; //밝기조절을 위해 영상에 곱해지는 변수 ( 0 ~ 2 )
int servo[12];
int dcL, dcR;
int quality=90;


BYTE labelmap[IMG_W*IMG_H];
int labelnum=0;
CvRect selection;
int track_object=1;
CvPoint origin;
int ReadyToTrack = 0;
int select_object = 0;
int drcError; //영상수신오류마다 증가하는 변수
CvHaarClassifierCascade* cascade=(CvHaarClassifierCascade*) cvLoad(".\\data\\haarcascades\\haarcascade_frontalface_alt.xml");
int drcFaceNo;
CvRect drcFaceSize[5];
BYTE rgb[3][IMG_W*IMG_H];
BYTE YCrCb[3][IMG_W*IMG_H];
BYTE BnW[IMG_W*IMG_H];
int stackx[IMG_H*IMG_W],stacky[IMG_H*IMG_W];

int HistY[256]={0};
int HistCb[256]={0};
int HistCr[256]={0};
int HPos[3][3]={0};
CvScalar lab_col_s[5];
int move_en=0;
Area rgb_area;

Region reg[MAXREGION];



/*drc library*/
//extern "C" __declspec(dllexport) int InitUSB(BYTE Freg);
//extern "C" __declspec(dllexport) DRCIMG MJPEGVIEW(BYTE *command);


PDEVICE listOfDevice1,listOfDevice2;
PDEVICE ptDevice1,ptDevice2;
CFCPipeUSB pipe1,pipe2;
unsigned int deviceNumber;
unsigned int nbConnectedDevices=0, nbActiveDevice, i;
char ** strConnectedDevices1;
char ** strConnectedDevices2;





 int InitUSB1(BYTE Freg) 
{
	unsigned long  msgWritten;
	BYTE txMsg[128]={0};
	BYTE rxMsg[10000]={0};
	int con_flag=1;    
		
	
	
	nbConnectedDevices = GetUsbDeviceListName(&strConnectedDevices1); //연결된 USB디바이스의 이름을 얻어온다.
	if(!nbConnectedDevices)
	{
		printf("1번 카메라에 초록색 LED가 켜진 후 버튼을 누르세요.");
		return 0;
	}
	ptDevice1 = (PDEVICE) malloc(sizeof(DEVICE));
	ptDevice1->name = _strdup(strConnectedDevices1[0]);
	ptDevice1->txrate = 0;
	ptDevice1->number = deviceNumber++;
	ptDevice1->next = listOfDevice1;
	ptDevice1->active = 'Y';
	

	
	
	// open the device
	if (pipe1.Open(ptDevice1->name)){
		printf(">> Err : Can not open device %d\n%s \r\n", ptDevice1->number, ptDevice1->name);
		return 0;
	}
	else {
		printf(">> 새로 연결된 디비이스는 %d \r\n", ptDevice1->number);		
	}


	txMsg[0]='A';	txMsg[1]='S';	txMsg[2]='K';  txMsg[63]=Freg;
	if (pipe1.WritePipe(&txMsg[0], 64, &msgWritten) || (64 != msgWritten)) 
		{
		printf("DRC station을 찾을 수 없습니다.[100]"); 
		return 0;
		}				
	printf("msgWritten1=%d \r\n", msgWritten);
	if (pipe1.ReadPipe(&rxMsg[0], 64)||(rxMsg[0]!='A')||(rxMsg[1]!='C')||(rxMsg[2]!='K'))
		{
		printf("DRC1 station을 찾을 수 없습니다.[101]"); 
		return 0;
		}

	return 1;
}

  int InitUSB2(BYTE Freg) 
{
	unsigned long  msgWritten;
	BYTE txMsg[128]={0};
	BYTE rxMsg[10000]={0};
	int con_flag=1;    
		
	
	
	nbConnectedDevices = GetUsbDeviceListName(&strConnectedDevices2); //연결된 USB디바이스의 이름을 얻어온다.
	if(!nbConnectedDevices)
	{
		printf("1번 카메라에 초록색 LED가 켜진 후 버튼을 누르세요.");
		return 0;
	}
	ptDevice2 = (PDEVICE) malloc(sizeof(DEVICE));
	ptDevice2->name = _strdup(strConnectedDevices2[0]);
	ptDevice2->txrate = 0;
	ptDevice2->number = deviceNumber++;
	ptDevice2->next = listOfDevice2;
	ptDevice2->active = 'Y';
	

	
	
	// open the device
	if (pipe2.Open(ptDevice2->name)){
		printf(">> Err : Can not open device %d\n%s \r\n", ptDevice2->number, ptDevice2->name);
		return 0;
	}
	else {
		printf(">> 새로 연결된 디비이스는 %d \r\n", ptDevice2->number);		
	}


	txMsg[0]='A';	txMsg[1]='S';	txMsg[2]='K';  txMsg[63]=Freg;
	if (pipe2.WritePipe(&txMsg[0], 64, &msgWritten) || (64 != msgWritten)) 
		{
		printf("DRC station을 찾을 수 없습니다.[100]"); 
		return 0;
		}				
	printf("msgWritten2=%d \r\n", msgWritten);
	if (pipe2.ReadPipe(&rxMsg[0], 64)||(rxMsg[0]!='A')||(rxMsg[1]!='C')||(rxMsg[2]!='K'))
		{
		printf("DRC2 station을 찾을 수 없습니다.[101]"); 
		return 0;
		}
	return 1;
}
DRCIMG MJPEGVIEW1(BYTE *command){

	//IplImage* image = 0;		// image 선언
	DRCIMG img;
	unsigned int mjpeg_size=0,packet_size;
	unsigned long  msgWritten=0;
	HANDLE hFile;
	DWORD dwWritten;

	BYTE txusbMsg[64]={0};
	BYTE rxusbMsg[10000]={0};
	
	static int errortot=0; 
	int error=0;
	static int cnt=0;


	img.image=0;
	memset(img.sensor,0,21);

	hFile = CreateFile("pic1.jpg",GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);

		if (pipe1.ReadPipe (&rxusbMsg[0], 64)) {
			error++;
			errortot++;
			}
		if(error==0)
		{
			packet_size =(unsigned int)rxusbMsg[0];
			mjpeg_size=(rxusbMsg[1]<<8)|(rxusbMsg[2]);
			for(int i=0;i<command[33];i++)
					img.sensor[i]=rxusbMsg[10+i];

			memcpy(rxusbMsg,command,64);
			if (pipe1.WritePipe(&rxusbMsg[0], 64, &msgWritten) || (64 != msgWritten)) 
					{errortot++; error++;}
			if(pipe1.ReadPipe (&rxusbMsg[0], mjpeg_size)) {
							errortot++;
							error++;
							}
			if(error==0)
				WriteFile(hFile,&rxusbMsg[0],(DWORD)mjpeg_size,&dwWritten,NULL);
		}

		CloseHandle(hFile);
		
		img.image=cvLoadImage("pic1.jpg",1);
		DeleteFile("pic1.jpg");
		return img;

}


DRCIMG MJPEGVIEW2(BYTE *command){

	//IplImage* image = 0;		// image 선언
	DRCIMG img;
	unsigned int mjpeg_size=0,packet_size;
	unsigned long  msgWritten=0;
	HANDLE hFile;
	DWORD dwWritten;

	BYTE txusbMsg[64]={0};
	BYTE rxusbMsg[10000]={0};
	
	static int errortot=0; 
	int error=0;
	static int cnt=0;


	img.image=0;
	memset(img.sensor,0,21);

	hFile = CreateFile("pic2.jpg",GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);

		if (pipe2.ReadPipe (&rxusbMsg[0], 64)) {
			error++;
			errortot++;
			}
		if(error==0)
		{
			packet_size =(unsigned int)rxusbMsg[0];
			mjpeg_size=(rxusbMsg[1]<<8)|(rxusbMsg[2]);
			for(int i=0;i<command[33];i++)
					img.sensor[i]=rxusbMsg[10+i];

			memcpy(rxusbMsg,command,64);
			if (pipe2.WritePipe(&rxusbMsg[0], 64, &msgWritten) || (64 != msgWritten)) 
					{errortot++; error++;}
			if(pipe2.ReadPipe (&rxusbMsg[0], mjpeg_size)) {
							errortot++;
							error++;
							}
			if(error==0)
				WriteFile(hFile,&rxusbMsg[0],(DWORD)mjpeg_size,&dwWritten,NULL);
		}

		CloseHandle(hFile);
		
		img.image=cvLoadImage("pic2.jpg",1);
		DeleteFile("pic2.jpg");
		return img;

}






/********************************************************************************************************************
함수명 : drcInit
기   능 : drc와 station간의 연결 및 기타 환경 설정 
전달인자: 총 6 개 
	1.	double frequency : 주파수를 설정한다. 2.400...2.525 [0.05단위로 변화 가능]
	2.	int Pc2Drc_DataLen : PC->DRC로 보내는 데이터 길이   (0...29)
	3.	int Drc2Pc_DataLen : DRC->PC로 받는 데이터 길이		(0...20) (현재 지원 안됨)
	4.	int baudrate : DRC<->사용자MCU와의 시리얼 통신 속도 (115200, 38400, 19200, 9200 bps지원)
	5.	int quality_num : 영상의 화질 선택 (0...100) : 숫자가 클수록 화질은 좋아지나 프레임전송 속도가 저하됨
	6.	double bright : 최초의 영상 밝기 (0.0...2.0) : ex)1.5배로 밝기를 밝게하고 싶으면 1.5 
반환값 : 없음.
*******************************************************************************************************************/
void drcInit(int id, double frequency,int Pc2Drc_DataLen ,int Drc2Pc_DataLen , int baudrate , int quality_num, double bright)
{
	/* Init data structure... */
	if(id==0)
		{
		memset(robotdata1,0,sizeof(robotdata1));
		drcError=0;
		dcL=0;	
		dcR=0;	
		memset(servo,0,sizeof(servo));
		drcBright=bright;
		if(quality_num>100)
			quality_num=100;
		if(quality_num<0)
			quality_num=0;
		quality = 100-quality_num;
		robotdata1[IMGQUALITY]=quality;
		robotdata1[FREQUENCY]=(BYTE)((frequency-2.4)*1000);//주파수 설정: 2.47GHz
		robotdata1[PC2DRC_DATA]=Pc2Drc_DataLen; //명령데이터의 길이 : 5 Bytes
		robotdata1[DRC2PC_DATA]=0;
		switch(baudrate)	// drc & robot serial baudrate 
			{
			case 115200:	robotdata1[BAUDRATE]=3; break;
			case 38400:		robotdata1[BAUDRATE]=2; break;
			case 19200:		robotdata1[BAUDRATE]=1; break;
			case 9600:		robotdata1[BAUDRATE]=0; break;
			default :		robotdata1[BAUDRATE]=3; break;
			}
		// 사용자가 변경할 수 잇는 부분
		robotdata1[HEADER]='V'; 	//명령데이터 0 : 헤더로사용
		robotdata1[RMOTOR]=127; 	//명령데이터 1 : 오른쪽 모터로 사용
		robotdata1[LMOTOR]=127; 	//명령데이터 2 : 왼쪽 모터로 사용
		robotdata1[LRSERVO]=90; 	//명령데이터 3 : 수평 서보모터로 사용
		robotdata1[UDSERVO]=90; 	//명령데이터 4 : 수직 서보모터로 사용
	
		if(InitUSB1(robotdata1[FREQUENCY])==0) 
			{printf("drc 연결 실패");exit(1);}
		}
	else if(id==1)
		{
		memset(robotdata2,0,sizeof(robotdata2));
		drcError=0;
		dcL=0;	
		dcR=0;	
		memset(servo,0,sizeof(servo));
		drcBright=bright;
		if(quality_num>100)
			quality_num=100;
		if(quality_num<0)
			quality_num=0;
		quality = 100-quality_num;
		robotdata1[IMGQUALITY]=quality;
		robotdata2[FREQUENCY]=(BYTE)((frequency-2.4)*1000);//주파수 설정: 2.47GHz
		robotdata2[PC2DRC_DATA]=Pc2Drc_DataLen; //명령데이터의 길이 : 5 Bytes
		robotdata2[DRC2PC_DATA]=0;

		switch(baudrate)	// drc & robot serial baudrate 
			{
			case 115200:	robotdata2[BAUDRATE]=3; break;
			case 38400:		robotdata2[BAUDRATE]=2; break;
			case 19200:		robotdata2[BAUDRATE]=1; break;
			case 9600:		robotdata2[BAUDRATE]=0; break;
			default :		robotdata2[BAUDRATE]=3; break;
			}
		// 사용자가 변경할 수 잇는 부분
		robotdata2[HEADER]='V'; 	//명령데이터 0 : 헤더로사용
		robotdata2[RMOTOR]=127; 	//명령데이터 1 : 오른쪽 모터로 사용
		robotdata2[LMOTOR]=127; 	//명령데이터 2 : 왼쪽 모터로 사용
		robotdata2[LRSERVO]=90; 	//명령데이터 3 : 수평 서보모터로 사용
		robotdata2[UDSERVO]=90; 	//명령데이터 4 : 수직 서보모터로 사용
	
		if(InitUSB2(robotdata2[FREQUENCY])==0) 
			{printf("drc 연결 실패");exit(1);}
		}
	
}

/********************************************************************************************************************
함수명 : CheckKey
기   능 : 키보드의 눌린 값에 따른 전역변수의 변경, 사용자들은 이 부분을 수정함
전달인자: 총 1 개 
	1.	BYTE key : 키의 아스키값 
반환값 : 없음.
*******************************************************************************************************************/
void CheckKey(BYTE key)
{
	int MaxSpeed = 127;
	int servo1=0;
	int servo2=0;
	switch(key)
		{
		case 'Q': 	 						//SERVO ORG
			servo[0]=0;	servo[1]=0;
			break;
		case 'W': 							//SERVO UP
			servo[1]-=2;
			break;	
		case 'S': 							//SERVO DOWN
		 	servo[1]+=2;
			break;
		case 'D':  							//SERVO RIGHT
		 	servo[0]-=2;
			break;
		case 'A':  							//SERVO LEFT
		 	servo[0]+=2;
			break; 
		case 38 : 							// ROBOT GO !
		 	dcL = MaxSpeed;
			dcR = MaxSpeed;
		 	break; 
		case 40:  							// ROBOT  BACK !
		 	dcL = -MaxSpeed;
			dcR = -MaxSpeed;
			break; 
		case 37: 							// ROBOT  LEFT !
			dcL = -MaxSpeed;
			dcR = MaxSpeed;
			break; 
		case 39:							// ROBOT  RIGHT !
			dcL = MaxSpeed;
			dcR = -MaxSpeed;
			break; 
		case 1 :			//키를 떼는 순간 1이 들어온다.여기서는 DC모터를 정지
			dcL=0; 
			dcR=0; 	
			break;
		case '1': 
			move_en=1;	
			break;	 
		case '3': 
			move_en=0;	
			break;	
		}

}

/********************************************************************************************************************
함수명 : GetMousePosWin
기   능 : 콘솔창의 키보드 이벤트 발생을 체크하는 함수.
전달인자: 없음.
반환값 : 해당키의 아스키값
평소에는 0이 반환되며 누르면 해당 키의 아스키값이 반환된다.
주의할 점은 손을 때는 순간 1이 반환 
	 ex)----------|_______________|------------
	    0000000000X000XXXXXXXXXXXX100000000000
*******************************************************************************************************************/
BYTE GetMousePosWin(void)
{
	HANDLE OutputH;
	INPUT_RECORD Inrec;
	DWORD evRead;
	HANDLE hStdIn=NULL;
	DWORD dwMode;
	BOOL Captured=FALSE;
	BYTE key=0;
	 
	GetConsoleMode(hStdIn, &dwMode);
	 
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdIn, &dwMode);
	 
	if(TRUE != SetConsoleMode( hStdIn, dwMode | ENABLE_MOUSE_INPUT))
	{
		return FALSE;
	}
	OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
	PeekConsoleInput(hStdIn, &Inrec, 1, &evRead);
		  
	if( evRead>0) 
	{ 
		ReadConsoleInput(hStdIn, &Inrec, 1, &evRead);
		switch (Inrec.EventType )
		{
		   
			case MOUSE_EVENT:
						Captured = true;
						break; 
		  case KEY_EVENT:
		   				Captured = true;
						key=(BYTE)(Inrec.Event.KeyEvent.wVirtualKeyCode);
						break; 

		}
	}
	if( Inrec.Event.MouseEvent.dwMousePosition.X )
	 	return key;
	else
	 	return 1;
 return key;
}

/********************************************************************************************************************
함수명 : drcUpdateCommand
기   능 : 실제로 명령데이터를 갱신하는 함수.MJPEGVIEW함수를 호출하면 DRC로 전송됨.
 int servo[12]	: [-90 ~ 90]
 int dcL,dcR	: [-127 ~ 127]
전달인자: 없음.
반환값 : 없음.
*******************************************************************************************************************/
void drcUpdateCommand(void)
{	
	if(dcL>DCMAX)	dcL = DCMAX;
	if(dcL<DCMIN)	dcL = DCMIN;
	if(dcR>DCMAX)	dcR = DCMAX;
	if(dcR<DCMIN)	dcR = DCMIN;

	robotdata1[LMOTOR]=128+dcL;	
	robotdata1[RMOTOR]=128+dcR;
	
	if(servo[0]>90)	servo[0] =90;
	if(servo[0]<-90)	servo[0] =-90;
	if(servo[1]>90)	servo[1] =90;
	if(servo[1]<-40)	servo[1] =-40;
	robotdata1[LRSERVO]=90+(BYTE)servo[0];
	robotdata1[UDSERVO]=90+(BYTE)servo[1];	
}

/********************************************************************************************************************
함수명 : drcCheckKeyboard
기   능 : 콘솔창에서 어떤키가 눌렸는지 체크하는 함수 
	눌린 키에 해당하는 명령데이터를 만들어 놓는 기능.
	이 명령데이터는 다음 영상을 읽는 순간 DRC에게 무선으로 전송됨.
전달인자: 없음
반환값 : 없음.
*******************************************************************************************************************/
void drcCheckKeyboard(void)
{
	
	BYTE key=0;
	static BYTE oldkey=0;
	
	key = GetMousePosWin();  
	// 키보드를 누르지 않으면 0이 반환
	// 누르는 동안 해당 아스키코드가 반환.
	// 손을 때는 순간 1이 반환 
	// ex)----------|_______________|------------
	//    0000000000X000XXXXXXXXXXXX100000000000
	//              -----
	// 따라서 X000X부분을 XXXXX로 바꾸어야만 된다.
	// 이를 위해 현재값이 0이면 이전 값으로 바꾸어주면 해결된다. 
	if(key==0)
		key=oldkey;
	
	if(key!=1&&key!=0)
		printf("['%c'=%d]키가 눌렸습니다.\r\n",key,key);
		
	CheckKey(key);
	oldkey=key;
	drcUpdateCommand();
	cvWaitKey(1);
}


/********************************************************************************************************************
함수명 : drcGetImage
기   능 : 영상을 읽는 동시에 명령데이터를 drc로 전송하는 함수. 결국 MJPEGVIEW함수를 호출한다.
		물론 로봇으로부터 온 데이터도 받는 부분이기도 하다.
전달인자: 없음
반환값 : 영상 IplImage 포인터 
*******************************************************************************************************************/
IplImage *drcGetImage(int id)
{
	DRCIMG FromRobot;
	if(id==0)
		{
		FromRobot = MJPEGVIEW1(robotdata1);
		}
	else if(id==1)
		{
		FromRobot = MJPEGVIEW2(robotdata2);
		}
	
	for(int i=0;i<robotdata1[DRC2PC_DATA];i++)
				printf("%d,",FromRobot.sensor[i]); 
	
	if(robotdata1[DRC2PC_DATA])
		printf("\r\n");

	
	return FromRobot.image;
}






/********************************************************************************************************************
함수명 : cvtCV2rgb
기   능 : openCV에서 제공되는 영상구조체(IplImage)형 포인터에서 
             raw데이터를 읽어 전역변수인 rgb 2차원 배열에 복사하는 기능.
             주로 RGB영상을 이용해서 영상처리를 시작하기 위해 사용한다.
전달인자: 영상구조체(IplImage)형 포인터
반환값 : 없음.
*******************************************************************************************************************/
void cvtCV2rgb(IplImage *img)
{
	CvScalar s;	
	for(int y=0; y<IMG_H; y++)
		for(int x=0; x<IMG_W; x++)
			{
			s=cvGet2D(img,y,x);
			rgb[0][y*IMG_W+x]= (BYTE)(s.val[2]);
			rgb[1][y*IMG_W+x]= (BYTE)(s.val[1]);
			rgb[2][y*IMG_W+x]= (BYTE)(s.val[0]);
			}
}
/********************************************************************************************************************
함수명 : cvtrgb2CV
기   능 : cvtCV2rgb함수와는 반대의 개념으로...openCV에서 제공되는 영상구조체(IplImage)형 포인터를 인자로 받아 
             전역변수인 rgb 2차원 배열을 IplImage 구조체로 복사하는 기능이다.
             주로 영상처리를 완료한 후 RGB영상을 화면에 표시하기 위해 사용한다.
전달인자: 영상구조체(IplImage)형 포인터
반환값 : 없음.
*******************************************************************************************************************/
void cvtrgb2CV(IplImage *img)
{
	CvScalar s;	
	for(int y=0; y<IMG_H; y++)
		for(int x=0; x<IMG_W; x++)
			{
			s.val[2]=(double)rgb[0][y*160+x];
			s.val[1]=(double)rgb[1][y*160+x];
			s.val[0]=(double)rgb[2][y*160+x];
			cvSet2D(img,y,x,s);
			}
}
/********************************************************************************************************************
함수명 : cvtCV2YCrCb
기   능 : openCV에서 제공되는 영상구조체(IplImage)형 포인터에서 
             raw데이터를 읽어 전역변수인 YCrCb 2차원 배열에 복사하는 기능.
             주로 YCrCb영상을 이용해서 영상처리를 시작하기 위해 사용한다.
전달인자: 영상구조체(IplImage)형 포인터
반환값 : 없음.
*******************************************************************************************************************/
void cvtCV2YCrCb(IplImage *img)
{
	CvScalar s;	
	for(int y=0; y<IMG_H; y++)
		for(int x=0; x<IMG_W; x++)
			{
			s=cvGet2D(img,y,x);
			YCrCb[0][y*IMG_W+x]= (BYTE)(s.val[0]);
			YCrCb[1][y*IMG_W+x]= (BYTE)(s.val[1]);
			YCrCb[2][y*IMG_W+x]= (BYTE)(s.val[2]);
			}
}
/********************************************************************************************************************
함수명 : cvtYCrCb2CV
기   능 : cvtCV2YCrCb함수와는 반대의 개념으로...openCV에서 제공되는 영상구조체(IplImage)형 포인터를 인자로 받아 
             전역변수인 YCrCb 2차원 배열을 IplImage 구조체로 복사하는 기능이다.
             주로 영상처리를 완료한 후 YCrCb영상을 화면에 표시하기 위해 사용한다.
전달인자: 영상구조체(IplImage)형 포인터
반환값 : 없음.
*******************************************************************************************************************/
void cvtYCrCb2CV(IplImage *img)
{
	CvScalar s;	
	for(int y=0; y<IMG_H; y++)
		for(int x=0; x<IMG_W; x++)
			{
			s.val[0]=(double)YCrCb[0][y*160+x];
			s.val[1]=(double)YCrCb[1][y*160+x];
			s.val[2]=(double)YCrCb[2][y*160+x];
			cvSet2D(img,y,x,s);
			}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////



/********************************************************************************************************************
함수명 : SetByte
기   능 : int형을 BYTE형으로 변환하면서 최대 255, 최소 0으로 설정하는 함수 
전달인자: int 변수 
반환값 :  BYTE 변수 (0.......255)
*******************************************************************************************************************/
BYTE SetByte(int val)
{
	if(val>255)
		val = 255;
	if(val<0)
		val = 0;
	return (BYTE)val;
}

/********************************************************************************************************************
함수명 : Event_GetRectangle
기   능 : 영상창의 이벤트함수로 ....cvSetMouseCallback함수로 등록을 하면 
		윈도우에 마우스 이벤트 발생시 자동으로 호출됨.

		영상에서 마우스로 사각형의 영역을 선택하는 기능을 수행
		
전달인자: 총 5개
	1.  int event : 윈도우에서 발생한 이벤트의 종류 : 좌측버튼 다운/ 업 등
	2.   int x, int y : 이벤트 발생시 x와 y 좌표.
반환값 : 없음.
*******************************************************************************************************************/
#define min(a, b) ((a<b)? a : b)
#define max(a, b) ((a>b)? a : b)
void Event_GetRectangle( int event, int x, int y, int flags, void* param )
{

    if( select_object )
    {
        selection.x = min(x,origin.x);
        selection.y = min(y,origin.y);
        selection.width = selection.x + abs(x - origin.x);
        selection.height = selection.y + abs(y - origin.y);
        //최대최소 마스킹
        selection.x = max( selection.x, 0 );
        selection.y = max( selection.y, 0 );
        selection.width = min( selection.width, 160 ); 
        selection.height = min( selection.height, 120 );
        selection.width -= selection.x;
        selection.height -= selection.y;
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = cvPoint(x,y);
        selection = cvRect(x,y,0,0);
        select_object = 1;
        break;
    case CV_EVENT_LBUTTONUP:
        select_object = 0;
        if( selection.width > 0 && selection.height > 0 )
            track_object = -1;
        break;
    }
}


/********************************************************************************************************************
함수명 : Event_SelectPixel
기   능 : 영상창의 이벤트함수로 ....cvSetMouseCallback함수로 등록을 하면 
		윈도우에 마우스 이벤트 발생시 자동으로 호출됨.

		영상에서 좌측버튼이 클릭된  픽셀의 값을 저장
		
전달인자: 총 5개
	1.  int event : 윈도우에서 발생한 이벤트의 종류 : 좌측버튼 다운/ 업 등
	2.   int x, int y : 이벤트 발생시 x와 y 좌표.
반환값 : 없음.
*******************************************************************************************************************/
void Event_SelectPixel( int event, int x, int y, int flags, void* param )
{
	BYTE Y,Cr,Cb;
	//CvScalar s;
	
	if(event==CV_EVENT_LBUTTONDOWN)
		{
		Y=YCrCb[0][y*160+x];
		Cr=YCrCb[1][y*160+x];
		Cb=YCrCb[2][y*160+x];
		printf("Y=%3d,Cr=%3d,Cb=%3d \r",Y,Cr,Cb);
		HPos[0][DCEN] = Y;
		HPos[1][DCEN] = Cr;
		HPos[2][DCEN] = Cb;
		if(Y<HPos[0][DMIN])
			HPos[0][DMIN]=Y;
		if(Y>HPos[0][DMAX])
			HPos[0][DMAX]=Y;

		if(Cr<HPos[1][DMIN])
			HPos[1][DMIN]=Cr;
		if(Cr>HPos[1][DMAX])
			HPos[1][DMAX]=Cr;

		if(Cb<HPos[2][DMIN])
			HPos[2][DMIN]=Cb;
		if(Cb>HPos[2][DMAX])
			HPos[2][DMAX]=Cb;
		}

}





/********************************************************************************************************************
함수명 : PanTiltControl
기   능 : 추적하기 원하는 물체의 중심좌표를 전달받아 pan-tilt 두개의 서보모터로 추적하는
		기능을 수행 PD제어를 사용함. 사용자가 자신의 모터에 맞게 변경해야함.
		
전달인자: 총 2개
	int cx, int cy : 추적하고자 하는 물체의  x와 y 좌표.
반환값 : 없음.
*******************************************************************************************************************/
void PanTiltControl(int cx, int cy)
{
	double errx=0.0; // x축 오차 저장 변수
	double erry=0.0; // y축 오차 저장 변수

	double Pgain=0.08;   // P gain 저장 변수
	double Dgain=0.08;   // D gain 저장 변수

	
	static double doubleservo_0 = 0;
	static double doubleservo_1 = 0;
	static double prev_errx=0;
	static double prev_erry=0;
	double dot_errx,dot_erry;
	errx = (double)cx - 80; // x축 오차 
	erry = (double)cy - 60 ; // y축 오차
if(abs((int)errx)>5||abs((int)erry)>5)
	{
		dot_errx = errx - prev_errx;
		dot_erry = erry - prev_erry;
		prev_errx = errx;
		prev_erry = erry;
		errx = errx*Pgain*(-1)+dot_errx*Dgain*(-1);
		erry = erry*Pgain+dot_erry*Dgain;
		//각 서보모터 변수에 저장
		doubleservo_0+=errx;
		doubleservo_1+=erry;
		if(doubleservo_0>90)	doubleservo_0 =90;
		if(doubleservo_0<-90)	doubleservo_0 =-90;
		if(doubleservo_1>90)	doubleservo_1 =90;
		if(doubleservo_1<-40)doubleservo_1=-40;
		servo[0]=(int)doubleservo_0; 
		servo[1]=(int)doubleservo_1; 
	}
	drcUpdateCommand();// 명령데이터 갱신함수 호출
}

/********************************************************************************************************************
함수명 : drcControlBrightness
기   능 : 주어진 영상을 drcBright배 만큼 밝게 한다.
		
전달인자: IplImage *img
반환값 : 없음.
*******************************************************************************************************************/
void drcControlBrightness(IplImage *img)
{
	//영상 밝기조절을 위해서 임시영상을 하나 만든다.
	IplImage *tmp_img = cvCreateImage(cvSize(IMG_W,IMG_H),IPL_DEPTH_8U,3);
	//임시영상을 모두 1로 채운다.
	cvSet(tmp_img, cvScalarAll(1),NULL); 	
	// (img*tmp_img)*scale = img로 영상을 drcBright배 만큼 밝게 조절한다. 
	cvMul(img, tmp_img, img,drcBright); 	
	cvReleaseImage(&tmp_img);// 임시영상을 지운다.
}

/********************************************************************************************************************
함수명 : drcErrorImage
기   능 : 영상이 연속적으로 50회 이상 오지않으면 프로그램을 종료하기 위한 함수이다.
		
전달인자: 없음.
반환값 : 없음.
*******************************************************************************************************************/
void drcErrorImage(void)
{
	if(drcError>50) //너무 많은 에러로 프로그램을 종료한다.
		{printf("drc communication error overflow");exit(1);}
	else
	{
		drcError++;//에러변수를 증가 시킨다.
		printf("drc communication error count=%d \r\n",drcError);
	}
}

/********************************************************************************************************************
함수명 : drcCreateWindow
기   능 : 영상 출력을 위해 윈도우를 만드는 함수.
				
전달인자: 총 4개
	1. char* name : 만들고자하는 윈도우의 고유한 이름.
	2. int width : 윈도우의 크기 폭.
	3. int height : 윈도우의 크기 폭.
	4. int flags : 1이면 영상 확대/축소 불가능 2이면 가능  
반환값 : 없음.
*******************************************************************************************************************/
void drcCreateWindow( char* name, int width, int height,  int flags)
{
	// 영상을 출력할 창을 생성한다.
	cvNamedWindow(name, flags);
	// 창의 크기를 widthxheight픽셀의 크기로 한다.
	cvResizeWindow(name, width, height);
}





////////////////////////////////////////////////////////////////////////////////////////////////////
// 1. 얼굴 찾기 관련//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************************************************
함수명 : FaceSort
기   능 : 찾은 얼굴들을 면적순으로 버블소트를 이용하여 크기대로 정렬하는 함수.
		
전달인자:없음.
반환값 : 없음.
*******************************************************************************************************************/
void FaceSort(void)
{
	
	int x,y;
	CvRect TmpTable;

	for(y=0 ; y < (5-1) ; y++)
		for(x=(y+1) ; x < 5 ; x++)
			{
			if((drcFaceSize[x].width*drcFaceSize[x].height) > (drcFaceSize[y].width*drcFaceSize[y].height))
				{
				TmpTable	= drcFaceSize[x];
				drcFaceSize[x] = drcFaceSize[y];
				drcFaceSize[y] = TmpTable;
				}
			}			
}

/********************************************************************************************************************
함수명 : cvFindFace
기   능 : 영상에서 얼굴을 찾는 기능
		
전달인자:영상 
반환값 : 없음.
*******************************************************************************************************************/
void cvFindFace(IplImage *image)
{

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* faces;
	int i, scale = 1; 
	drcFaceNo=0;
	// 실제 영상에서 얼굴을 찾는 OpenCV함수이다. 찾은 얼굴들에 대한 정보를 포인터로 반환 
	faces = cvHaarDetectObjects( image, cascade, storage, 1.1, 2,  
										   CV_HAAR_DO_CANNY_PRUNING, cvSize(10, 10) );//------------(1)
	drcFaceNo=0; 								// 찾은 얼굴 개수를 저장할 변수를 초기화한다.
	memset(drcFaceSize,0,sizeof(drcFaceSize));	//얼굴 좌표를 저장할 배열도 초기화한다
	for( i = 0; i < faces->total; i++ ){	//찾은 얼굴 최대 5개의 좌표정보를 배열에 저장한다. 
		if(i==5) break;
		drcFaceSize[i]= *(CvRect*)cvGetSeqElem(faces,i); 
		cvRectangle( image, cvPoint(drcFaceSize[i].x,drcFaceSize[i].y),
		cvPoint((drcFaceSize[i].x+drcFaceSize[i].width),(drcFaceSize[i].y+drcFaceSize[i].height)), CV_RGB(255*i,255,100), 1);
	}
	drcFaceNo=i; // 얼굴 개수를 facenum에 저장한다.
	cvReleaseMemStorage( &storage );
}	

/********************************************************************************************************************
함수명 : cvFindBigFace
기   능 : 영상에서 가장 큰 얼굴을 찾아내는 기능
		
전달인자:영상 
반환값 : face형 구조체
*******************************************************************************************************************/
face cvFindBigFace(IplImage *img)
{
	face pos;
	pos.flag=0;
	
	cvFindFace(img); /// action
	FaceSort();
	if(drcFaceNo)
		{
		pos.cx= (drcFaceSize[0].x+drcFaceSize[0].width/2);
		pos.cy= (drcFaceSize[0].y+drcFaceSize[0].height/2);
		pos.width = drcFaceSize[0].width;
		pos.flag=1;
		}
	return pos;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// 2. 칼라 인식 관련 /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************************************************
함수명 : ColorSort
기   능 : 찾은 칼라 객체들을 면적순으로 버블소트를 이용하여 크기대로 정렬하는 함수.
		
전달인자:없음.
반환값 : 없음.
*******************************************************************************************************************/
void ColorSort(void)
{
	
	int x,y;
	Region TmpTable;

	for(y=0 ; y < (MAXREGION-1) ; y++)
		for(x=(y+1) ; x < MAXREGION ; x++)
			{
			if((reg[x].area) > (reg[y].area))
				{
				TmpTable	= reg[x];
				reg[x] = reg[y];
				reg[y] = TmpTable;
				}
			}	

}

int pushstack(int cx, int cy, int *top)
{
	if(*top>=(IMG_H*IMG_W))
		return -1;
	(*top)++;
	stackx[*top]=cx;
	stacky[*top]=cy;
	return 1;
}
int popstack(int *cx, int *cy, int *top)
{
	if(*top==0) return -1;
	*cx=stackx[*top];
	*cy=stacky[*top];
	(*top)--;
	return 1;
}

void labeling(int xc, int yc, colmap col, CvScalar col_s, IplImage *img)
{
	int x,y,index;
	int top=0;
	int i=0;
	
	i=labelnum-1;
	reg[i].x1 = 160-1;
	reg[i].y1 = 120-1;
	reg[i].x2 = 0;
	reg[i].y2 = 0;
	
while(1){	
GRASSFIRE:
		for(y=yc-1 ; y<=yc+1 ; y++){
			for(x=xc-1 ; x<=xc+1 ; x++)
			{
				if(y<0 || x<0 || y>=IMG_H || x>=IMG_W)
					continue;
				index = y*IMG_W+x;
				if((YCrCb[0][index]>col.Y_min&&YCrCb[0][index]<=col.Y_max
				&&YCrCb[1][index]>col.Cr_min&&YCrCb[1][index]<=col.Cr_max
				&&YCrCb[2][index]>col.Cb_min&&YCrCb[2][index]<=col.Cb_max) &&labelmap[index]==0)
				{
					labelmap[index]=labelnum;
					 i=labelnum-1;
					reg[i].color = labelnum;
					reg[i].area ++;
					reg[i].x1 = min(x , reg[i].x1);
					reg[i].x2 = max(x , reg[i].x2);
					reg[i].y1 = min(y , reg[i].y1);
					reg[i].y2 = max(y , reg[i].y2);
					reg[i].cen_x += x;
					reg[i].cen_y += y;
					cvSet2D(img,y,x,col_s);
					if(pushstack(x,y,&top)==-1)
						continue;
					xc=x;
					yc=y;
					goto GRASSFIRE;
				}
			}
		}
		if(popstack(&xc,&yc,&top)==-1) break;
	}
	
	i=labelnum-1;
	reg[i].cen_x /= (double)reg[i].area;
	reg[i].cen_y /= (double)reg[i].area;
}


int MakeLimit( int data , int max, int min) 
{
	if(data>max)	data=max;
	if(data<min)	data=min;
	return data;
}


void HistoCbCallback( int event, int x, int y, int flags, void* param )
{

	static int ms_status=0;
	static int flag=0;

	x=(int)SetByte(x);
	y=(int)SetByte(y);
	if(HPos[2][DMAX]<HPos[2][DMIN])
		HPos[2][DMAX]=HPos[2][DMIN];
	if(event==CV_EVENT_LBUTTONDOWN)
		{
		if(abs(HPos[2][DMAX]-x)<abs(HPos[2][DMIN]-x))
			{ms_status = DMAX; }
		else
			{ms_status = DMIN; }
		
		flag=1;
		HPos[2][ms_status]=x;
		}
	else if(event==CV_EVENT_LBUTTONUP)
		{
			flag=0;
			OnCfgSave();
		}

	// mouse move
	if(flag)
		HPos[2][ms_status]=x;

}

void HistoCrCallback( int event, int x, int y, int flags, void* param )
{

	static int ms_status=0;
	static int flag=0;


	x=(int)SetByte(x);
	y=(int)SetByte(y);

	if(HPos[1][DMAX]<HPos[1][DMIN])
		HPos[1][DMAX]=HPos[1][DMIN];

	
	if(event==CV_EVENT_LBUTTONDOWN)
		{
		if(abs(HPos[1][DMAX]-x)<abs(HPos[1][DMIN]-x))
			{ms_status = DMAX; }
		else
			{ms_status = DMIN; }
		
		flag=1;
		HPos[1][ms_status]=x;
		}
	else if(event==CV_EVENT_LBUTTONUP)
		{
			flag=0;
			OnCfgSave();
		}

	// mouse move
	if(flag)
		HPos[1][ms_status]=x;

}

void HistoYCallback( int event, int x, int y, int flags, void* param )
{

	static int ms_status=0;
	static int flag=0;


	x=(int)SetByte(x);
	y=(int)SetByte(y);
	if(HPos[0][DMAX]<HPos[0][DMIN])
		HPos[0][DMAX]=HPos[0][DMIN];
	
	if(event==CV_EVENT_LBUTTONDOWN)
		{
		if(abs(HPos[0][DMAX]-x)<abs(HPos[0][DMIN]-x))
			{ms_status = DMAX; }
		else
			{ms_status = DMIN; }
		
		flag=1;
		HPos[0][ms_status]=x;
		}
	else if(event==CV_EVENT_LBUTTONUP)
		{
			flag=0;
			OnCfgSave();
		}

	// mouse move
	if(flag)
		HPos[0][ms_status]=x;

}

void OnCfgLoad(void) 
{
	// TODO: Add your command handler code here

	HANDLE hFile;
	DWORD dwWritten;
	//LPCWSTR lpFileName =(LPCWSTR);
	
	hFile = CreateFile("color.txt",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	ReadFile(hFile,HPos,sizeof(HPos),&dwWritten,NULL);
	
	if(dwWritten!=sizeof(HPos)){
		HPos[0][DMIN]=HPos[1][DMIN]=HPos[2][DMIN]=0;
		HPos[0][DCEN]=HPos[1][DCEN]=HPos[2][DCEN]=150;
		HPos[0][DMAX]=HPos[1][DMAX]=HPos[2][DMAX]=255;
		}
	CloseHandle(hFile);

}

void OnCfgSave(void) 
{
	// TODO: Add your command handler code here

	HANDLE hFile;
	DWORD dwWritten;
	LPCWSTR lpFileName =(LPCWSTR)"color.txt";

	hFile = CreateFile("color.txt",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteFile(hFile,HPos,	sizeof(HPos),&dwWritten,NULL);
	CloseHandle(hFile);
}

void MakeHistogram(void)
{
	static int flag = 0;
	int x,y;
	CvScalar s;
	
	if(flag==0)
		{
		drcCreateWindow("Cb", HIST_W, HIST_H, 1);
		drcCreateWindow("Cr", HIST_W, HIST_H, 1);
		drcCreateWindow("Y", HIST_W, HIST_H, 1);
		//drcCreateWindow("TRACK", 320, 240, 2);
		cvSetMouseCallback("Cb" ,HistoCbCallback );
		cvSetMouseCallback("Cr" ,HistoCrCallback );
		cvSetMouseCallback("Y" ,HistoYCallback );
		flag=1;
		}
	IplImage *Yimg = cvCreateImage(cvSize(HIST_W,HIST_H),IPL_DEPTH_8U,3);
	IplImage *Crimg = cvCreateImage(cvSize(HIST_W,HIST_H),IPL_DEPTH_8U,3);
	IplImage *Cbimg = cvCreateImage(cvSize(HIST_W,HIST_H),IPL_DEPTH_8U,3);
	cvSet(Yimg, cvScalarAll(0),NULL); 
	cvSet(Crimg, cvScalarAll(0),NULL); 
	cvSet(Cbimg, cvScalarAll(0),NULL); 
	// 1) Set select min & max area color and Draw Area -------------------
	s.val[0]= 100;	//blue
	s.val[1]= 120;	//green
	s.val[2]= 100; 	//red
	
	for(y=0; y<100; y++)
		{
			for(x=HPos[0][DMIN]; x<HPos[0][DMAX]; x++)
					cvSet2D(Yimg,	(HIST_H-1)-y,	SetByte(x),	s);
			for(x=HPos[1][DMIN]; x<HPos[1][DMAX]; x++)
					cvSet2D(Crimg,	(HIST_H-1)-y,	SetByte(x),	s);		
			for(x=HPos[2][DMIN]; x<HPos[2][DMAX]; x++)
					cvSet2D(Cbimg,	(HIST_H-1)-y,	SetByte(x),	s);	
		}
	//  2) make histogram------------------------------------------------
	memset(HistY,0,sizeof(HistY));
	memset(HistCb,0,sizeof(HistCb));
	memset(HistCr,0,sizeof(HistCr));
	
	for(y=0; y<IMG_H; y++)
			for(x=0; x<IMG_W; x++)
				{
				HistY[YCrCb[0][y*IMG_W+x]]++;
				HistCb[YCrCb[1][y*IMG_W+x]]++;
				HistCr[YCrCb[2][y*IMG_W+x]]++;
				}	

	// 3)  draw histo------------------------------------------------------
	
	s.val[0]=s.val[1]=s.val[2]=200;	
	int Ymax=0,Crmax=0,Cbmax=0;
		for(x=0; x<255 ; x++)
			{
			if(HistY[x]>Ymax) Ymax=HistY[x];
			if(HistCb[x]>Crmax) Crmax=HistCb[x];
			if(HistCr[x]>Cbmax) Cbmax=HistCr[x];
			}
 	Ymax=MakeLimit(Ymax, 10000, 1);
	Crmax=MakeLimit(Crmax, 10000, 1);
	Cbmax=MakeLimit(Cbmax, 10000, 1);
	for(x=0; x<255 ; x++)
		{
			if(HistY[x]>0) HistY[x]=(int)((float)HistY[x]*(99/(float)Ymax));
			if(HistCb[x]>0) HistCb[x]=(int)((float)HistCb[x]*(99/(float)Crmax));
			if(HistCr[x]>0) HistCr[x]=(int)((float)HistCr[x]*(99/(float)Cbmax));
			
			if(HistY[x]>99)		HistY[x]=99;
			if(HistCb[x]>99)		HistCb[x]=99;
			if(HistCr[x]>99)		HistCr[x]=99;
				
			for(y=1; y<=HistY[x]; y++)
				cvSet2D(Yimg,(HIST_H-1)-y,x,s);
			for(y=1; y<=HistCb[x]; y++)
				cvSet2D(Crimg,(HIST_H-1)-y,x,s);
			for(y=1; y<=HistCr[x]; y++)
				cvSet2D(Cbimg,(HIST_H-1)-y,x,s);
		}

	// 4) draw active line...-----------------------------------------------
	
	s.val[0]=s.val[2]=100;	s.val[1]=255; // green
	for(y=0; y<100; y++)
		cvSet2D(Yimg,(HIST_H-1)-y,HPos[0][DCEN],s);
	for(y=0; y<100; y++)
		cvSet2D(Crimg,(HIST_H-1)-y,HPos[1][DCEN],s);
	for(y=0; y<100; y++)
		cvSet2D(Cbimg,(HIST_H-1)-y,HPos[2][DCEN],s);

	char str[100]={0};
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 0.5f, 0.5f, 0, 1, 8);
	memset(str,0,sizeof(str));
	sprintf_s(str,"%d",HPos[0][DMAX]);
	cvPutText(Yimg, str, cvPoint(60, 20), &font, CV_RGB(255,80,80));
	sprintf_s(str,"%d",HPos[0][DMIN]);
	cvPutText(Yimg, str, cvPoint(20, 20), &font, CV_RGB(255,80,80));

	sprintf_s(str,"%d",HPos[1][DMAX]);
	cvPutText(Crimg, str, cvPoint(60, 20), &font, CV_RGB(80,255,80));
	sprintf_s(str,"%d",HPos[1][DMIN]);
	cvPutText(Crimg, str, cvPoint(20, 20), &font, CV_RGB(80,255,80));

	sprintf_s(str,"%d",HPos[2][DMAX]);
	cvPutText(Cbimg, str, cvPoint(60, 20), &font, CV_RGB(80,80,255));
	sprintf_s(str,"%d",HPos[2][DMIN]);
	cvPutText(Cbimg, str, cvPoint(20, 20), &font, CV_RGB(80,80,255));
	
	cvShowImage("Y", Yimg);	
	cvShowImage("Cr", Crimg);	
	cvShowImage("Cb", Cbimg);	
	cvReleaseImage( &Yimg);
	cvReleaseImage( &Crimg);
	cvReleaseImage( &Cbimg);
	
}

void InsertSort(int* d, int n)
{
	int i, j;
	int temp;

	for( i = 1 ; i < n ; i++ )
	{
		temp = d[i];
		j = i - 1;

		while( j > -1 && d[j] > temp )
		{
			d[j+1] = d[j];
			j--;
		}

		d[j+1] = temp;
	}
}

void MedeanFilter(void) 
{
	int j,i;
	int m[9];
	int kk=0,first=1;
	BYTE tmp[3][IMG_W*IMG_H];
	
	
	for(kk=0;kk<3;kk++)
		for( j = 1 ; j < 120-1 ; j++ )
			for( i = 1 ; i < 160-1 ; i++ )
			{
				m[0] = rgb[kk][(j-1)*160+(i-1)]; 
				m[1] = rgb[kk][(j-1)*160+(i)]; 
				m[2] = rgb[kk][(j-1)*160+(i+1)];	
				m[3] = rgb[kk][(j)*160+(i-1)]; 
				m[4] = rgb[kk][(j)*160+(i)]; 
				m[5] = rgb[kk][(j)*160+(i+1)];
				m[6] = rgb[kk][(j+1)*160+(i-1)]; 
				m[7] = rgb[kk][(j+1)*160+(i)]; 
				m[8] = rgb[kk][(j+1)*160+(i+1)];

				// Insertion sort
				InsertSort(m, 9);
				
				tmp[kk][j*160+i] = (BYTE)m[4];
			}

	memcpy(rgb,tmp,sizeof(tmp));

}


void drcDefineColor(BYTE rl, BYTE ru, BYTE gl, BYTE gu, BYTE bl, BYTE bu)
{
	HPos[0][DMIN] = (int)rl;
	HPos[0][DMAX] = (int)ru;
	HPos[1][DMIN] = (int)gl;
	HPos[1][DMAX] = (int)gu;
	HPos[2][DMIN] = (int)bl;
	HPos[2][DMAX] = (int)bu;
	lab_col_s[4]=CV_RGB(255,0,0);
	lab_col_s[3]=CV_RGB(255,255,0);
	lab_col_s[2]=CV_RGB(0,255,0);
	lab_col_s[1]=CV_RGB(0,255,255);
	lab_col_s[0]=CV_RGB(0,0,255);
}
int CheckCirRatio(IplImage *img, int regid){

	const int maxcnt=MAXREGION; 			// maximun boundry number
	drcBound bnd[MAXREGION];			// boundry position arrary

	int boundcnt[MAXREGION] ={0}; 	// boundry pixel number
	int diagonal[MAXREGION]={0};
	int cnt=0;
	BYTE visited[IMG_H*IMG_W]={0};
	/*
	이웃한 픽셀들...
	________________
	|				|
	|	3	4	5	|
	|	2	X	6	|
	|	1	0	7	|	
	|_______________|
	*/
	CvPoint npix[8]={{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
	memset(visited,0,sizeof(visited));
	memset(bnd,0,sizeof(bnd));
	BYTE p0,p1;
	int x0,y0;
	int x,y,n,k;
	cnt=0;
	
	for(y=reg[regid].y1 ;y<reg[regid].y2;y++ )
		for(x=reg[regid].x1 ;x<reg[regid].x2;x++ )
			{
			p0=labelmap[y*IMG_W+x-1];
			p1=labelmap[y*IMG_W+x]; // save labelnum
			if((p0!=p1)&&p1!=0&&visited[y*IMG_W+x]==0)
				{
				boundcnt[cnt]=0;
				// save start pixel
				x0=x;
				y0=y;
				n=4;
				
				do{
					for(k=0 ;k<8;k++ )
						{
							int xx= x+npix[n].x;
							int yy= y+npix[n].y;
							// check outside
							if(xx<reg[regid].x1||xx>reg[regid].x2||yy<reg[regid].y1||yy>reg[regid].y2)
								{}
							else{
							// check next boundry
							if(labelmap[yy*IMG_W+xx]==p1)
								break;}
							n=((n+1)&7);				
							
						}
					if(k==8)	// check isolated pixel
						break;
					
					visited[y*IMG_W+x]=255; // mark visited pixel
					bnd[cnt].xchain[boundcnt[cnt]]=x;
					bnd[cnt].ychain[boundcnt[cnt]++]=y;
					if(boundcnt[cnt]>BOUNDRY_MAXNUM)
						break;
					x= x+npix[n].x;
					y= y+npix[n].y;
					if(n%2)diagonal[cnt]++;
					n=((n+5)&7);		
					}
				while(!(x==x0 && y==y0));// is here start pixel??
				
				// check isolated pixel & too small boundary
				if(k==8)	
					continue;
				if(boundcnt[cnt]<5)
					continue;
				//cnt++ & check maxcnt
				cnt++;
				
				
				if(cnt>=5)
					goto TTT;
				
				}

			}
		
TTT:
	// find max bnd
	
	int idx=0;
	int max=0;
	for(n=0 ;n<=cnt;n++ )
	{
		if(boundcnt[n]>max)
			{idx=n;max=boundcnt[n];}
	}
		
	// save reg
	reg[regid].boundcnt = boundcnt[idx];
	reg[regid].diagoncnt = diagonal[idx];
	for(n=0 ; n<reg[regid].boundcnt ; n++)
	{
		reg[regid].boundx[n]	=	bnd[idx].xchain[n];
		reg[regid].boundy[n]	=	bnd[idx].ychain[n];
	}
	CvScalar s;
	s=CV_RGB(0,0,255);
	for(n=0 ; n<reg[regid].boundcnt; n++)
	{
		cvSet2D(img, reg[regid].boundy[n], reg[regid].boundx[n],s);
	}
		//printf("[%d]",reg[regid].boundcnt);
	double e=0;
	double l=0;
	l=(reg[regid].boundcnt-reg[regid].diagoncnt)+(reg[regid].diagoncnt)*(1.4142)+0.5;
	e=4*3.14*(double)reg[regid].area;
	e=e/(double)(l*l);
	reg[regid].cir_ratio = (int)(e*100);
	
	if(reg[regid].cir_ratio>100)
		reg[regid].cir_ratio = 100;
	if(reg[regid].cir_ratio<0)
		reg[regid].cir_ratio = 0;
	char str[100]={0};
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 0.5f, 0.5f, 0, 1, 8);
	memset(str,0,sizeof(str));
	sprintf_s(str,"%d",reg[regid].cir_ratio);
	cvPutText(img, str, cvPoint((int)reg[regid].cen_x, (int)reg[regid].cen_y), &font, CV_RGB(0,0,0));

	return reg[regid].cir_ratio;
}

int DrawBoundary(IplImage *img, int regid){

	CvScalar s;
	int n;
	
	double per=0.0;
	

	if(reg[regid].boundcnt<30)
		return -1;

	for(n=0; n<reg[regid].boundcnt; n++)
		{
		reg[regid].bnd_dst[n]=(int)sqrt((reg[regid].cen_x-reg[regid].boundx[n])*(reg[regid].cen_x-reg[regid].boundx[n])+(reg[regid].cen_y-reg[regid].boundy[n])*(reg[regid].cen_y-reg[regid].boundy[n]));
		}
	//filtering
	int p1,p2,p3,p4,p5;
	int tmp[BOUNDRY_MAXNUM];
	int cnt =reg[regid].boundcnt;
	for(int i=0;i<1;i++)
		{
		memset(tmp,0,sizeof(tmp));
		for(n=0; n<cnt; n++)
			{
			p1=reg[regid].bnd_dst[((n-2)+cnt)%cnt];
			p2=reg[regid].bnd_dst[((n-1)+cnt)%cnt];
			p3=reg[regid].bnd_dst[((n-0)+cnt)%cnt];
			p4=reg[regid].bnd_dst[((n+1)+cnt)%cnt];
			p5=reg[regid].bnd_dst[((n+2)+cnt)%cnt];
			tmp[n]=(int)((p1+p2*2+p3*4+p4*2+p5)/10+0.5);
			}
		for(n=0; n<cnt; n++)
			{
			reg[regid].bnd_dst[n]=tmp[n];
			}
		}
	
	s=CV_RGB(255,0,0);
	int pulse=0;
	int edge=0;
	int cont=0;
	for(n=0; n<cnt; n++)
		{
		//
		if(   (reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n-1)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n+1)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n-2)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n+2)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n-3)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n+3)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n-4)+cnt)%cnt])>=0&&
			(reg[regid].bnd_dst[((n-0)+cnt)%cnt]-reg[regid].bnd_dst[((n+4)+cnt)%cnt])>=0)
			{
			if(pulse==0)
				{pulse=1;edge++;}		
			cont++;
			//cvSet2D(img, 120-20*regid-reg[regid].bnd_dst[n], n,CV_RGB(0,0,255));
			}
		else{
				pulse=0;
			//cvSet2D(img, 120-20*regid-reg[regid].bnd_dst[n], n,s);
			}
		}
	//per=((double)edge)/((double)cnt);

	// get circular ratio...-----------------------------------------------------------
	double e=0;
	double l=0;
	l=(reg[regid].boundcnt-reg[regid].diagoncnt)+(reg[regid].diagoncnt)*(1.4142)+0.5;
	e=4*3.14*(double)reg[regid].area;
	e=e/(double)(l*l);
	reg[regid].cir_ratio = (int)(e*100);
	printf("[%d:c=%d]",regid,reg[regid].cir_ratio);
	//----------------------------------------------------------------------------

	/*
	if(cont>(cnt/3))
		printf("[%d=원]",regid);
	else if(edge<6)
		printf("[%d=삼각,%d]",regid,edge);
	else
		printf("[%d=사각,%d]",regid,edge);
	*/
	return 1;
}

int drcDebugColor(char* name, IplImage *img)
{
	int x,y,index;
	static int flag=0;
	colmap col;
	// 결과값을 출력하기 위한 윈도우를 생성하고 최근에 설정한 칼라범위값을 읽어온다.
	// 그리고 이벤트 함수도 등록한다.
	if(flag==0)
		{
		OnCfgLoad();
		drcCreateWindow("OUT", 320, 240, 2);
		cvSetMouseCallback(name ,Event_SelectPixel );
		flag=1;
		}
	//영상의 밝기를 적용한다.		
	drcControlBrightness(img);
	//원본 영상을 전달인자로 받은 윈도우에 출력한다.
	cvShowImage(name, img);
	//원본을 보존하기 위해 복사본의 영상을 하나 만든다.
	IplImage *cpimg = cvCreateImage(cvSize(IMG_W,IMG_H),IPL_DEPTH_8U,3);
	cvSet(cpimg, cvScalarAll(1),NULL); 
	cvCopyImage(img,cpimg);
	//원활한 칼라인식을 위해 RGB->YCrCb로 전환
	cvCvtColor(cpimg,cpimg,CV_BGR2YCrCb);
	//앞으로의 영상 처리를 위해서  2차원 배열 YCrCb[3][160*120]으로 영상을 복사한다.
	cvtCV2YCrCb(cpimg);
	//결과를 출력하기  위해 YCrCb->RGB로 전환
	cvCvtColor(cpimg,cpimg,CV_YCrCb2BGR);
	//히스토그램을 만든다.
	MakeHistogram();
	
	//라벨링을 위한 자료구조들을 초기화 한다.
	memset(stackx,0,sizeof(stackx));
	memset(stacky,0,sizeof(stacky));
	memset(labelmap,0,sizeof(labelmap));
	memset(reg,0,sizeof(reg));
	labelnum=0;
		
	col.Y_min = HPos[0][DMIN];
	col.Y_max = HPos[0][DMAX];
	col.Cr_min= HPos[1][DMIN];
	col.Cr_max= HPos[1][DMAX];
	col.Cb_min= HPos[2][DMIN];
	col.Cb_max= HPos[2][DMAX];
		
	for(y=0; y<IMG_H ; y++)
		for(x=0 ; x<IMG_W ; x++)
		{
			index = y*IMG_W+x;
			if(labelmap[index]==0)
			{
				if(YCrCb[0][index]>col.Y_min&&YCrCb[0][index]<=col.Y_max
					&&YCrCb[1][index]>col.Cr_min&&YCrCb[1][index]<=col.Cr_max
					&&YCrCb[2][index]>col.Cb_min&&YCrCb[2][index]<=col.Cb_max) 
				{
						labelnum++;
						if(labelnum<40) 
							labeling(x,y,col, CV_RGB(255,0,0), cpimg);
				}
			}
		}
	// 추출한 객체들을 크기 대로 정렬한다.
	ColorSort();
	int cx,cy;
	// 물체 중심점에 십자가를 출력하기 위한 루틴 
	if(labelnum)
		{
		cx = (int)(reg[0].cen_x);
		cy = (int)(reg[0].cen_y);
		char str[100]={0};
		CvFont font;
		cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 0.4f, 0.4f, 0, 1, 8);
		memset(str,0,sizeof(str));
		sprintf_s(str,"%3d , %3d",cx,cy);
		cvPutText(cpimg, str, cvPoint(20, 20), &font, CV_RGB(0,255,0));
		
		for(y=0; y<IMG_H; y++)
			cvSet2D(cpimg,y,cx,CV_RGB(80,255,80));
		for(x=0; x<IMG_W; x++)
			cvSet2D(cpimg,cy,x,CV_RGB(80,255,80));
		}
	// 출력을 위한 영상을 결과윈도우에 출력
	cvShowImage("OUT", cpimg);	
	cvReleaseImage( &cpimg);
	return labelnum;
}

int drcFindColor(char *name, colmap col, CvScalar col_s, IplImage *img)
{
	int ret=0;
	int x,y,index;
	static int flag=0;
	// 결과값을 출력하기 위한 윈도우를 생성.
	if(flag==0)
		{
		drcCreateWindow("OUT", 320,240, 2);
		flag=1;
		}
	//영상의 밝기를 적용한다.		
	drcControlBrightness(img);	
	//원본 영상을 전달인자로 받은 윈도우에 출력한다.
	cvShowImage(name, img);
	//원본을 보존하기 위해 복사본의 영상을 하나 만든다.
	IplImage *cpimg = cvCreateImage(cvSize(IMG_W,IMG_H),IPL_DEPTH_8U,3);
	cvSet(cpimg, cvScalarAll(1),NULL); 
	cvCopyImage(img,cpimg);
	//원활한 칼라인식을 위해 RGB->YCrCb로 전환
	cvCvtColor(cpimg,cpimg,CV_BGR2YCrCb);
	//앞으로의 영상 처리를 위해서  2차원 배열 YCrCb[3][160*120]으로 영상을 복사한다.
	cvtCV2YCrCb(cpimg);
	//결과를 출력하기  위해 YCrCb->RGB로 전환
	cvCvtColor(cpimg,cpimg,CV_YCrCb2BGR);
	
	//라벨링을 위한 자료구조들을 초기화 한다.	
	memset(stackx,0,sizeof(stackx));
	memset(stacky,0,sizeof(stacky));
	memset(labelmap,0,sizeof(labelmap));
	memset(reg,0,sizeof(reg));
	labelnum=0;
		
	for(y=0; y<IMG_H ; y++)
		for(x=0 ; x<IMG_W ; x++)
			{
			index = y*IMG_W+x;
			if(labelmap[index]==0)
				{
				if(YCrCb[0][index]>col.Y_min&&YCrCb[0][index]<=col.Y_max
					&&YCrCb[1][index]>col.Cr_min&&YCrCb[1][index]<=col.Cr_max
					&&YCrCb[2][index]>col.Cb_min&&YCrCb[2][index]<=col.Cb_max) 
					{
					labelnum++;
					if(labelnum<40) 
						labeling(x,y,col, col_s, cpimg);
					}
				}
			}
	// 추출한 객체들을 크기 대로 정렬한다.
	ColorSort();
	//가장 큰 객체의 중심점을 구하고 move_en=1이라면 서보모터로 추적한다.
	int cx,cy;
	// 물체 중심점에 십자가를 출력하기 위한 루틴 
	if(labelnum)
		{
		cx = (int)(reg[0].cen_x);
		cy = (int)(reg[0].cen_y);
		char str[100]={0};
		CvFont font;
		cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 0.4f, 0.4f, 0, 1, 8);
		memset(str,0,sizeof(str));
		sprintf_s(str,"%3d , %3d",cx,cy);
		cvPutText(cpimg, str, cvPoint(20, 20), &font, CV_RGB(0,255,0));
		
		for(y=0; y<IMG_H; y++)
			cvSet2D(cpimg,y,cx,CV_RGB(80,255,80));
		for(x=0; x<IMG_W; x++)
			cvSet2D(cpimg,cy,x,CV_RGB(80,255,80));
		}
	// 출력을 위한 영상을 결과윈도우에 출력
	cvShowImage("OUT", cpimg);	
	cvReleaseImage( &cpimg);

	return labelnum;
}







