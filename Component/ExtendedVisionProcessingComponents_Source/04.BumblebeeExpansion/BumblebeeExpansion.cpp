
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



#include "BumblebeeExpansion.h"
#include "pgrflycapture.h"
#include "triclops.h"
#include "pgrflycapturestereo.h"

//
// constructor declaration
//
#define DISPARITY_LUT_SHIFT_BITS    4
#define DISPARITY_LUT_BITS	    (16 - DISPARITY_LUT_SHIFT_BITS)
#define DISPARITY_VALID_LUT_ENTRIES ( 1 << DISPARITY_LUT_BITS )
// there are a maximum of 256 invalid mappings available
#define DISPARITY_INVALID_LUT_ENTRIES 256

FlyCaptureContext		pContext;
FlyCaptureImage			pImage;
FlyCaptureImage			pImageBGR;
FlyCaptureInfoEx		pInfo;

TriclopsContext			triclops;
TriclopsInput			stereoData;
TriclopsImage16			depthImage16;
TriclopsImage			monoImage;

IplImage *imgDisparity;

unsigned char* rowIntMono;
unsigned char* redMono;
unsigned char* greenMono;
unsigned char* blueMono;

unsigned char m_ucValidDisparityMapLUT[ DISPARITY_VALID_LUT_ENTRIES ][3];
unsigned char m_ucInvalidDisparityMapLUT[ DISPARITY_INVALID_LUT_ENTRIES ][3];

int iMaxCols;
int iMaxRows;

int m_nLUTMinDisp;
int m_nLUTMaxDisp;

void generateLookupTables();
void makeColoredInvalidLUT(unsigned char ucLUT[DISPARITY_INVALID_LUT_ENTRIES][3],
			unsigned char ucInvalidTexture,
			unsigned char ucInvalidUniqueness,
			unsigned char ucInvalidSurface,
			unsigned char ucInvalidBackForth,
			unsigned char ucInvalidSubpixel );
  void disparityToTemperature(TriclopsContext,
			unsigned int uiDisp16Bit, 
			double dMinDisp,
			double dMaxDisp,
			double *pdRed,
			double *pdGreen,
			double *pdBlue );
bool captureStartFlg;
BumblebeeExpansion::BumblebeeExpansion()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
BumblebeeExpansion::BumblebeeExpansion(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

BumblebeeExpansion::~BumblebeeExpansion() {
}

void BumblebeeExpansion::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType BumblebeeExpansion::onInitialize()
{
	iMaxCols = 640;
	iMaxRows = 480;

	// 영상 획득을 위한 FlyCapture API 코드
	FlyCaptureError error;				// Error 코드 구조체

	error = flycaptureCreateContext(&pContext); // Context 생성
	if(error != FLYCAPTURE_OK){
		std::cout << "[ERROR-BumblebeeExpansion::onInitialize()] No Bumblebee Camera Context" << std::endl;
		// 비정상 리턴타입 반환
		return OPROS_INPUT_NULL;
	}
	flycaptureInitialize(pContext, 0);	// 카메라 초기화
	
	char* szCalFile;
	// 카메라 보정 파일을 저장하고 경로 반환
	flycaptureGetCalibrationFileFromCamera( pContext, &szCalFile );
	// 카메라 보정 파일에서 Triclops context 만들기
	triclopsGetDefaultContextFromFile( &triclops, szCalFile );
	// 카메라정보 취득
	flycaptureGetCameraInfo( pContext, &pInfo );

	unsigned long ulValue;
	flycaptureGetCameraRegister( pContext, 0x1F28, &ulValue );

	// 크기설정
	triclopsSetResolution( triclops, iMaxRows, iMaxCols );		
   
	// disparity 설정
	triclopsSetDisparity( triclops, 0, 100 );
	triclopsSetStereoMask( triclops, 23 ) ;//마스크 크기(Disparity 객체크기)

	// 서브픽셀및 surface를 제외한 유효성검사 해제
	triclopsSetTextureValidation( triclops, 0 );
	triclopsSetUniquenessValidation( triclops, 0 );

	// Turn on sub-pixel interpolation
	triclopsSetSubpixelInterpolation( triclops, 1 );

	// 매핑매개변수가 변하지 않으면 호출되는 메소드
	// without regenerating the tables
	generateLookupTables();
	
	rowIntMono = NULL;
	redMono = NULL;
	greenMono = NULL;
	blueMono = NULL;

	captureStartFlg = true;

	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType BumblebeeExpansion::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onDestroy()
{
	flycaptureStop(pContext);				// 카메라 동작 멈춤
    flycaptureDestroyContext(pContext);		// 생성된 Context 파괴
	triclopsDestroyContext( triclops );		// 생성된 Context 파괴	

	if(rowIntMono != NULL)
		delete rowIntMono;   
	redMono = NULL;
	greenMono = NULL;
	blueMono = NULL;

	if(imgDisparity != NULL)
		cvReleaseImage(&imgDisparity);

	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onExecute()
{
	RawImage result;

	// 범블비영상이미지가져오기
	if(captureStartFlg){
		FlyCaptureError error;
		error = flycaptureStartCustomImage(pContext,3,0,0,iMaxCols,iMaxRows,100.0f,FLYCAPTURE_RAW16);
		if(error != FLYCAPTURE_OK){
			std::cout << "[ERROR-BumblebeeExpansion::onExecute()] Capture was not started." << std::endl;
			// 비정상 리턴타입 반환
			return OPROS_INPUT_NULL;
		}
		captureStartFlg = false;
	}
	flycaptureGrabImage2( pContext, &pImage );

	// 이미지로부터 정보 취득
	int imageCols = pImage.iCols;
	int imageRows = pImage.iRows;
	int imageRowInc = pImage.iRowInc;
	int iSideBySideImages = pImage.iNumImages;
	unsigned long timeStampSeconds = pImage.timeStamp.ulSeconds;
	unsigned long timeStampMicroSeconds = pImage.timeStamp.ulMicroSeconds;

	// 모노이미지 저장할 버퍼생성
	if(rowIntMono != NULL)
		delete rowIntMono;
	rowIntMono = new unsigned char[ imageCols * imageRows * iSideBySideImages ];

	// 스테레오 이미지를 준비하기위한 임시 FlyCaptureImage 만들기
	FlyCaptureImage tempMonoImage;
	tempMonoImage.pData = rowIntMono;
   
	// row interleaved format -> pixel interleaved raw data 변환
	flycapturePrepareStereoImage( pContext, pImage, &tempMonoImage, NULL  );

	// 모노 버퍼의 위치에 대한 시작포인터 지정
	// of the red, green and blue sections
	redMono = rowIntMono;
	greenMono = redMono + imageCols;
	blueMono = redMono + imageCols;

	// RGB TriclopsInput를 구축하기 위해 row interleaved images사용  
	// RGB triclops 입력은 3 RAW 이미지를 포함 (1 from each camera).
	triclopsBuildRGBTriclopsInput(
		imageCols, 
		imageRows, 
		imageRowInc,  
		timeStampSeconds, 
		timeStampMicroSeconds, 
		redMono, 
		greenMono, 
		blueMono, 
		&stereoData);
 
	// 이미지 전처리
	triclopsRectify( triclops, &stereoData );

	// 스테레오 프로세싱
	triclopsStereo( triclops ) ;

	// 깊이이미지 검색
	triclopsGetImage16( triclops, TriImg16_DISPARITY, TriCam_REFERENCE, &depthImage16 );
   
	// 행 당 픽셀 간격의 수를 결정
	int iPixelinc = depthImage16.rowinc/2;
	unsigned short* pRow = depthImage16.data;

	// 결과이미지버퍼생성
	if(imgDisparity != NULL)
		cvReleaseImage(&imgDisparity);
	imgDisparity = cvCreateImage(cvSize(depthImage16.ncols, depthImage16.nrows), IPL_DEPTH_8U,3);	
	
	int i,k,j;
	for ( i = 0, k = 0; i < depthImage16.nrows; i++ )
	{
		for ( j = 0; j < depthImage16.ncols; j++, k++ )
		{
			if (pRow[j] >= 0xFF00)
			{
				// 유효 픽셀에 대해, 하단에 8 비트만을 받아
				int index = 0x00FF & pRow[j];

				imgDisparity->imageData[i*imgDisparity->widthStep+j*3+0] = m_ucInvalidDisparityMapLUT[index][0]; // blue
				imgDisparity->imageData[i*imgDisparity->widthStep+j*3+1] = m_ucInvalidDisparityMapLUT[index][1]; // green
				imgDisparity->imageData[i*imgDisparity->widthStep+j*3+2] = m_ucInvalidDisparityMapLUT[index][2]; // red
			}
			else
			{
				int index = pRow[j] >> DISPARITY_LUT_SHIFT_BITS;
				imgDisparity->imageData[i*imgDisparity->widthStep+j*3+0] = m_ucValidDisparityMapLUT[index][0]; // blue
				imgDisparity->imageData[i*imgDisparity->widthStep+j*3+1] = m_ucValidDisparityMapLUT[index][1]; // green
				imgDisparity->imageData[i*imgDisparity->widthStep+j*3+2] = m_ucValidDisparityMapLUT[index][2]; // red
			}
		}
		pRow   += iPixelinc;
	}

	redMono = NULL;
	greenMono = NULL;
	blueMono = NULL;

	// RawImage의 이미지 포인터 변수 할당
	RawImageData *pimage = result.getImage();
	
	// 입력된 이미지 사이즈 및 채널수로 재 설정
	pimage->resize(imgDisparity->width, imgDisparity->height, imgDisparity->nChannels);
	
	// 영상의 총 크기(pixels수) 취득
	int size = imgDisparity->width * imgDisparity->height * imgDisparity->nChannels;
	
	// 영상 데이터로부터 영상값만을 할당하기 위한 변수
	unsigned char *ptrdata = pimage->getData();
	
	// 현재 프레임 영상을 사이즈 만큼 memcpy
	memcpy(ptrdata, imgDisparity->imageData, size);

	// 포트아웃
	opros_any mdata = result;
	ImageOut.push(result);//전달

	return OPROS_SUCCESS;
}
	
ReturnType BumblebeeExpansion::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

void makeColoredInvalidLUT(unsigned char ucLUT[DISPARITY_INVALID_LUT_ENTRIES][3],
   unsigned char ucInvalidTexture,
   unsigned char ucInvalidUniqueness,
   unsigned char ucInvalidSurface,
   unsigned char ucInvalidBackForth,
   unsigned char ucInvalidSubpixel )
{
   for ( int i = 0; i < DISPARITY_INVALID_LUT_ENTRIES; i++)
   {
	   if ( i == ucInvalidTexture )
	   {
		   // red
		   ucLUT[i][0] = 0;
		   ucLUT[i][1] = 0;
		   ucLUT[i][2] = 255;
	   }
	   else if (i == ucInvalidUniqueness )
	   {
		   // green
		   ucLUT[i][0] = 0;
		   ucLUT[i][1] = 255;
		   ucLUT[i][2] = 0;
	   }
	   else if ( i == ucInvalidSurface )
	   {
		   // blue
		   ucLUT[i][0] = 255;
		   ucLUT[i][1] = 0;
		   ucLUT[i][2] = 0;
	   }
	   else if ( i == ucInvalidBackForth )
	   {
		   // yellow
		   ucLUT[i][0] = 0;
		   ucLUT[i][1] = 255;
		   ucLUT[i][2] = 255;
	   }
	   else if ( i == ucInvalidSubpixel )
	   {
		   // ugh, magenta
		   ucLUT[i][0] = 255;
		   ucLUT[i][1] = 0;
		   ucLUT[i][2] = 255;
	   }
	   else
	   {
		   // these pixel should never appear
		   // ugh, cyan
		   ucLUT[i][0] = 255;
		   ucLUT[i][1] = 255;
		   ucLUT[i][2] = 0;
	   }
   }
}
void disparityToTemperature(TriclopsContext,	  // may want to use this in the future...
					   unsigned int uiDisp16Bit, 
					   double dMinDisp,
					   double dMaxDisp,
					   double *pdRed,
					   double *pdGreen,
					   double *pdBlue )
{
	// note: context not used now but maybe later
	double dDisp = (double) uiDisp16Bit/256.0;

	if ( dDisp < dMinDisp )
	{
		dDisp = dMinDisp;
	}
	if ( dDisp > dMaxDisp )
	{
		dDisp = dMaxDisp;
	}

	double dRange = dMaxDisp - dMinDisp;


	// This code is a little more complicated than it should be, but it
	// does allow one to easily tweak the sizes of the different color
	// zones.
	// you can change the width of the red zone, blue zone and green zone 
	// by tweaking the dRed, dGreen and dBlue values
	double dRed	     = 1;
	double dGreen     = 1;
	double dBlue	     = 1;
	double dTotal     = dRed + dGreen + dBlue;
	double dNormDisp  = (dDisp-dMinDisp)/dRange;
	double dBGThresh  = dBlue/dTotal;
	double dGRThresh  = (dBlue+dGreen)/dTotal;

	if ( dNormDisp < dBGThresh  )
	{
		double dInBand = dNormDisp/dBGThresh;
		*pdRed   = 0;
		*pdGreen = 255*dInBand;
		*pdBlue  = 255;
	}
	else if ( dNormDisp < dGRThresh )
	{
		double dInBand = (dNormDisp-dBGThresh)/(dGRThresh-dBGThresh);
		*pdRed   = 255*dInBand;
		*pdGreen = 255;
		*pdBlue  = 255*(1-dInBand);
	}
	else
	{
		double dInBand = (dNormDisp-dGRThresh)/(1-dGRThresh);
		*pdRed   = 255;
		*pdGreen = 255*(1-dInBand);
		*pdBlue  = 0;
	}


	return;
}

void generateLookupTables()
{
	static bool	  bCalledBefore	   = false;
	static bool	  bLastPseudoColor = false;
	static bool	  bLastColorInvalid= false;

	int            nMinDisparity;
	int            nMaxDisparity;
	int            nDisparityOffset;
	int		  i;


	// get convenience variables from dialog
	// do we want to display as temperature?
	bool		  bPseudoColor	   = TRUE;
	bool		  bColorInvalid	   = TRUE;

	triclopsGetDisparity( triclops, &nMinDisparity, &nMaxDisparity );
	triclopsGetDisparityOffset( triclops, &nDisparityOffset );


	// do not generate table if nothing has changed.
	if(( nMinDisparity-nDisparityOffset == m_nLUTMinDisp ) &&
		( nMaxDisparity-nDisparityOffset == m_nLUTMaxDisp ) &&
		( bLastPseudoColor == bPseudoColor ) &&
		( bLastColorInvalid == bColorInvalid ) &&
		bCalledBefore )
	{
		return;
	}

	if ( bPseudoColor )
	{
		// we enforce that the coloring of invalid pixels doesn't happen 
		// if we are using the heatmap disparity display
		bColorInvalid = false;
	}

	bCalledBefore     = true;
	bLastPseudoColor  = bPseudoColor;
	bLastColorInvalid = bColorInvalid;


	// get the invalid pixel mappings
	unsigned char ucInvalidTexture;
	unsigned char ucInvalidUniqueness;
	unsigned char ucInvalidSurface;
	unsigned char ucInvalidBackForth;
	unsigned char ucInvalidSubpixel;

	triclopsGetTextureValidationMapping(    triclops, &ucInvalidTexture );
	triclopsGetUniquenessValidationMapping( triclops, &ucInvalidUniqueness );
	triclopsGetSurfaceValidationMapping(    triclops, &ucInvalidSurface );
	triclopsGetSubpixelValidationMapping(   triclops, &ucInvalidSubpixel );
	triclopsGetBackForthValidationMapping(  triclops, &ucInvalidBackForth );

	if ( bPseudoColor )
	{

		// store values that we're dealing with so we don't compute next time.
		m_nLUTMinDisp     = nMinDisparity-nDisparityOffset;
		m_nLUTMaxDisp     = nMaxDisparity-nDisparityOffset;


		// handle valid pixels
		for( i = 0; i < DISPARITY_VALID_LUT_ENTRIES; i++ )
		{
			unsigned int uiDisp16Bit = i << DISPARITY_LUT_SHIFT_BITS;
			if( uiDisp16Bit <= 0xFF00 )
			{

				double dRed, dGreen, dBlue;
				disparityToTemperature( triclops, 
					uiDisp16Bit, 
					(double) m_nLUTMinDisp,
					(double) m_nLUTMaxDisp,
					&dRed, 
					&dGreen, 
					&dBlue );
				m_ucValidDisparityMapLUT[i][2] = (unsigned char) dRed;
				m_ucValidDisparityMapLUT[i][1] = (unsigned char) dGreen;
				m_ucValidDisparityMapLUT[i][0] = (unsigned char) dBlue;
			}
		}

		if ( !bColorInvalid )
		{
			for (i = 0; i < DISPARITY_INVALID_LUT_ENTRIES; i++)
			{
				// make them all grey
				m_ucInvalidDisparityMapLUT[i][0] = 127;
				m_ucInvalidDisparityMapLUT[i][1] = 127;
				m_ucInvalidDisparityMapLUT[i][2] = 127;
			}
		}
		else
		{
			makeColoredInvalidLUT(	 m_ucInvalidDisparityMapLUT,
				ucInvalidTexture,
				ucInvalidUniqueness,
				ucInvalidSurface,
				ucInvalidBackForth,
				ucInvalidSubpixel );
		}
	}
	else // if ( bPseudoColor )
	{

		// store values that we're dealing with so we don't compute next time.
		m_nLUTMinDisp     = nMinDisparity-nDisparityOffset;
		m_nLUTMaxDisp     = nMaxDisparity-nDisparityOffset;



		// These parameters control the mapping of disparity values to
		// the palette of the disparity display window. We want to scale
		// the current disparity range between 127 and 255 for "easy viewing"
		const unsigned char g_ucLUTMinDispMap = 127;
		const unsigned char g_ucLUTMaxDispMap = 255;

		float fScale = ((float)(g_ucLUTMaxDispMap - g_ucLUTMinDispMap))/
			((float)(m_nLUTMaxDisp-m_nLUTMinDisp));

		// handle valid pixels
		for( i = 0; i < DISPARITY_VALID_LUT_ENTRIES; i++ )
		{
			unsigned int uiDisp16Bit = i << DISPARITY_LUT_SHIFT_BITS;
			if( uiDisp16Bit <= 0xFF00 )
			{
				double dGreyValue = (
					((double)( uiDisp16Bit - (m_nLUTMinDisp<<8)))
					*fScale + (double)(g_ucLUTMinDispMap<<8))/256.0;
				m_ucValidDisparityMapLUT[i][0] = (unsigned char) dGreyValue;
				m_ucValidDisparityMapLUT[i][1] = (unsigned char) dGreyValue;
				m_ucValidDisparityMapLUT[i][2] = (unsigned char) dGreyValue;
			}
		}

		if ( !bColorInvalid )
		{
			for (i = 0; i < DISPARITY_INVALID_LUT_ENTRIES; i++)
			{
				// make them all blue
				m_ucInvalidDisparityMapLUT[i][0] = 255;
				m_ucInvalidDisparityMapLUT[i][1] = 0;
				m_ucInvalidDisparityMapLUT[i][2] = 0;
			}
		}
		else
		{
			makeColoredInvalidLUT(	 m_ucInvalidDisparityMapLUT,
				ucInvalidTexture,
				ucInvalidUniqueness,
				ucInvalidSurface,
				ucInvalidBackForth,
				ucInvalidSubpixel );
		}
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
	return new BumblebeeExpansion();
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
	return new BumblebeeExpansion();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

