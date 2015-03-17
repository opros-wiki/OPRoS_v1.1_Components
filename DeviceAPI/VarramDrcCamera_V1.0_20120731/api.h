
///////////////////////////////////////////////////////
/*drc library*/
///////////////////////////////////////////////////////

void drcInit(int id, double frequency,int Pc2Drc_DataLen ,int Drc2Pc_DataLen , int baudrate , int quality_num, double bright);
void CheckKey(BYTE key);
BYTE GetMousePosWin(void);
void drcUpdateCommand(void);
void drcCheckKeyboard(void);
IplImage *drcGetImage(int id);


void cvtCV2rgb(IplImage *img);
void cvtrgb2CV(IplImage *img);
void cvtCV2YCrCb(IplImage *img);
void cvtYCrCb2CV(IplImage *img);


BYTE SetByte(int val);
void Event_GetRectangle( int event, int x, int y, int flags, void* param );
void Event_SelectPixel( int event, int x, int y, int flags, void* param );
void PanTiltControl(int cx, int cy);
void FaceSort(void);
void ColorSort(void);
void drcControlBrightness(IplImage *img);
void drcErrorImage(void);


///////////////////////////////////////////////////////
/*related OpenCV library...*/
///////////////////////////////////////////////////////
face cvFindBigFace(IplImage *img);

void drcCreateWindow( char* name, int width, int height,  int flags);









void drcSaveShape(char *name, IplImage *img);
void drcFindShape(char *name, IplImage *img);





void OnCfgLoad(void);
void OnCfgSave(void);






///////////////////////////////////////////////////////
/*Image processing library with opencv*/
///////////////////////////////////////////////////////
void cvFindFace(IplImage *image);
void FaceSort(void);
///////////////////////////////////////////////////////
/*Image processing library without opencv*/
///////////////////////////////////////////////////////

int drcFindColor(char *name, colmap col, CvScalar col_s, IplImage *img);
int drcDebugColor(char* name, IplImage *img);
void drcDefineColor(BYTE rl, BYTE ru, BYTE gl, BYTE gu, BYTE bl, BYTE bu);
int CheckCirRatio(IplImage *img, int regid);






void drcFindCorner( IplImage *img);

extern BYTE robotdata1[64]; //명령데이터를 위한 배열
extern BYTE robotdata2[64]; //명령데이터를 위한 배열
extern double drcBright;
extern Region reg[MAXREGION];
extern int drcFaceNo;
extern int servo[12];
extern int dcL, dcR;