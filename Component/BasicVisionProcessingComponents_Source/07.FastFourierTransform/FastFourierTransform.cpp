
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

#include "FastFourierTransform.h"

//
// constructor declaration
//
const double PI = acos(-1.0);

IplImage* m_orig_img;	// 입력이미지 저장 버퍼
IplImage* m_result_img;	// 변환이미지 저장 버퍼
IplImage* m_gray_img1;	// 흑백이미지 저장 버퍼
IplImage* m_gray_img2;	// 흑백이미지 처리 버퍼

double** m_pRe;  // 실수부
double** m_pIm;  // 허수부

int m_in_width;
int m_in_height;

int m_nWidth, m_nHeight;

std::string m_Result_Type;

FastFourierTransform::FastFourierTransform()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FastFourierTransform::FastFourierTransform(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FastFourierTransform::~FastFourierTransform() {
}

void FastFourierTransform::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType FastFourierTransform::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_gray_img1 != NULL)
		m_gray_img1 = NULL;
	if(m_gray_img2 != NULL)
		m_gray_img2 = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_pRe = NULL;
	m_pIm = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_Result_Type = getProperty("ResultType");

	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FastFourierTransform::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);
	if(m_gray_img1 != NULL)
		cvReleaseImage(&m_gray_img1);
	if(m_gray_img2 != NULL)
		cvReleaseImage(&m_gray_img2);

	// 동적할당한 메모리해제처리
	Free();

	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onExecute()
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
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		if(m_gray_img1 == NULL){
			m_gray_img1 = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		if(m_gray_img2 == NULL){
			m_gray_img2 = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		if( !IsPowerOf2(m_orig_img->width) || !IsPowerOf2(m_orig_img->height) ) {
			// 2의 승수가 아닐경우
			std::cout << "This image is not a multifplier of 2" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;

		}else if(m_orig_img->width != m_orig_img->height){
			// 가로세로가 틀릴경우
			std::cout << "Width and height are difference." << std::endl;	

			return OPROS_BAD_INPUT_PARAMETER;

		}else{
			// 컬러영상을 그레이스케일로 변환
			cvCvtColor( m_orig_img, m_gray_img1, CV_RGB2GRAY );
			
			// 입력영상을 메모리에 복사(m_pRe)
			SetImage();
			
			// 고속이산푸리에
			FFT(1);
			
			// 결과 선택값에 따라
			if(m_Result_Type == "Spectrum"){
				GetSpectrumImage();					// 푸리에스팩트럼 이미지
			}else if(m_Result_Type == "PhaseAngle"){
				GetPhaseImage();					// 위상값 이미지
			}else if(m_Result_Type == "InverseTran"){
				FFT(-1);							// 고속이산푸리에역변환
				GetImage();							// 역변환한 이미지
			}

			// 그레이이미지(1채널)을 3채널로 변경
			cvMerge(m_gray_img2, m_gray_img2, m_gray_img2, NULL, m_result_img);
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

// 영상과 동일한 크기의 배열 메모리 할당: m_pRe, m_pIm
// 영상픽셀값을 m_pRe에 복사, m_pIm에 0지정
void FastFourierTransform::SetImage()
{
	//-------------------------------------------------------------------------
	// 멤버 변수 값 설정 
	//-------------------------------------------------------------------------

	Free();

	register int i, j;

	m_nWidth  = m_gray_img1->width;
	m_nHeight = m_gray_img1->height;

	//-------------------------------------------------------------------------
	// 푸리에 변환 결과 값을 저장할 메모리 공간 할당
	//-------------------------------------------------------------------------

	m_pRe = new double*[m_nHeight];
	m_pIm = new double*[m_nHeight];

	for( i = 0 ; i < m_nHeight ; i++ )
	{
		m_pRe[i] = new double[m_nWidth];
		m_pIm[i] = new double[m_nWidth];

		memset(m_pRe[i], 0, sizeof(double)*m_nWidth);
		memset(m_pIm[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 복소수 배열의 실수부에 영상의 그레이스케일 값을 저장
	//-------------------------------------------------------------------------
	BYTE* ptr = (BYTE *)m_gray_img1->imageData;

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		m_pRe[j][i] = (double)ptr[j*m_nHeight+i];
	}
}


// 고속이산푸리에 또는 역변환
void FastFourierTransform::FFT(int dir)
{
	if( m_pRe == NULL || m_pIm == NULL )
		return;

	if( !IsPowerOf2(m_nWidth) || !IsPowerOf2(m_nHeight) )
		return;

	register int i, j;

	//-------------------------------------------------------------------------
	// 행(Row) 단위 FFT (또는 IFFT)
	//-------------------------------------------------------------------------

	double* re = new double[m_nWidth];
	double* im = new double[m_nWidth];

	memset(re, 0, sizeof(double)*m_nWidth);
	memset(im, 0, sizeof(double)*m_nWidth);

	for( j = 0 ; j < m_nHeight ; j++ )
	{
		for( i = 0 ; i < m_nWidth ; i++ )
		{
			re[i] = m_pRe[j][i];
			im[i] = m_pIm[j][i];
		}

		FFT1d(re, im, m_nWidth, dir);

		for( i = 0 ; i < m_nWidth ; i++ )
		{
			m_pRe[j][i] = re[i];
			m_pIm[j][i] = im[i];
		}
	}

	delete [] re;
	delete [] im;

	//-------------------------------------------------------------------------
	// 열(Column) 단위 FFT (또는 IFFT)
	//-------------------------------------------------------------------------

	re = new double[m_nHeight];
	im = new double[m_nHeight];

	memset(re, 0, sizeof(double)*m_nHeight);
	memset(im, 0, sizeof(double)*m_nHeight);

	for( i = 0 ; i < m_nWidth ; i++ )
	{
		for( j = 0 ; j < m_nHeight ; j++ )
		{
			re[j] = m_pRe[j][i];
			im[j] = m_pIm[j][i];
		}

		FFT1d(re, im, m_nHeight, dir);

		for( j = 0 ; j < m_nHeight ; j++ )
		{
			m_pRe[j][i] = re[j];
			m_pIm[j][i] = im[j];
		}
	}

	delete [] re;
	delete [] im;
}


// 고속이산푸리에 또는 역변환
void FastFourierTransform::FFT1d(double* re, double* im, int N, int dir)
{
	register int i, j, k;

	//-------------------------------------------------------------------------
	// 입력 데이터의 순서 바꾸기
	//-------------------------------------------------------------------------

	int n2 = N >> 1;
	int nb = 0;

	while( N != ( 1 << nb ) )
		nb++;

	for( i = 0, j = 0 ; i < N-1 ; i++ )
	{
		if( i < j )
		{
			swap(re[i], re[j]);
			swap(im[i], im[j]);
		}

		k = n2;
		
		while( k <= j )
		{
			j -= k;
			k >>= 1;
		}

		j += k;
	}

	//-------------------------------------------------------------------------
	// 버터플라이(Butterfly) 알고리즘
	//-------------------------------------------------------------------------

	int i1, l, l1, l2;
	double c1, c2, t1, t2, u1, u2, z;

	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;

	for( l = 0 ; l < nb ; l++ ) 
	{
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;

		for( j = 0 ; j < l1 ; j++ )
		{
			for( i = j ; i < N ; i += l2 )
			{
				i1 = i + l1;
				t1 = u1 * re[i1] - u2 * im[i1];
				t2 = u1 * im[i1] + u2 * re[i1];
				re[i1] = re[i] - t1;
				im[i1] = im[i] - t2;
				re[i] += t1;
				im[i] += t2;
			}

			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}

		c2 = sqrt((1.0 - c1) / 2.0);
		
		if( dir == 1 ) // Forward
			c2 = -c2;
		
		c1 = sqrt((1.0 + c1) / 2.0);
	}

	if( dir == -1 ) // IDFT
	{
		for( i = 0 ; i < N ; i++ )
		{
			re[i] /= (double)N;
			im[i] /= (double)N;
		}
	}
}


// 푸리에 변환 후 호출, 푸리에 스펙트럼생성
void FastFourierTransform::GetSpectrumImage()
{
	register int i, j;

	// 결과버퍼
	BYTE * ptr = (BYTE *)m_gray_img2->imageData;

	//-------------------------------------------------------------------------
	// 스펙트럼 값을 저장할 임시 버퍼 메모리 공간 할당
	//-------------------------------------------------------------------------

	double** pSpectrum = new double*[m_nHeight];
	for( i = 0 ; i < m_nHeight ; i++ )
	{
		pSpectrum[i] = new double[m_nWidth];
		memset(pSpectrum[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 복소수 값의 절대값을 로그 변환하여 저장. 
	// 정규화된 결과 이미지 생성을 위하여 스펙트럼의 최대값 계산.
	//-------------------------------------------------------------------------

	double mag, max_value = 0;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		mag = sqrt(m_pRe[j][i]*m_pRe[j][i] + m_pIm[j][i]*m_pIm[j][i]);
		pSpectrum[j][i] = log(mag + 1);

		if( pSpectrum[j][i] > max_value ) 
			max_value = pSpectrum[j][i];
	}

	//-------------------------------------------------------------------------
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT 하고, 
	// 출력 영상의 최대값이 255가 되도록 그레이스케일 값을 정규화.
	//-------------------------------------------------------------------------

	int x, y;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + m_nWidth/2;
		y = j + m_nHeight/2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		ptr[j*m_nHeight+i] = (BYTE)limit(pSpectrum[y][x]*255/max_value);
	}

	//-------------------------------------------------------------------------
	// 임시 버퍼 메모리 공간 해제
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
		delete [] pSpectrum[i];
	delete [] pSpectrum;
}


// 푸리에 변환 후 호출, 위상값 영상생성
void FastFourierTransform::GetPhaseImage()
{
	register int i, j;

	// 결과버퍼
	BYTE * ptr = (BYTE *)m_gray_img2->imageData;

	//-------------------------------------------------------------------------
	// 위상각을 저장할 임시 버퍼 메모리 공간 할당
	//-------------------------------------------------------------------------

	double** pPhase = new double*[m_nHeight];
	for( i = 0 ; i < m_nHeight ; i++ )
	{
		pPhase[i] = new double[m_nWidth];
		memset(pPhase[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 복소수 값의 위상각을 저장. atan2 함수의 반환 값은 -PI ~ PI 이다.
	//-------------------------------------------------------------------------

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		pPhase[j][i] = atan2(m_pIm[j][i], m_pRe[j][i]); // 래디언 단위
	}

	//-------------------------------------------------------------------------
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT 하고, 
	// 출력 영상의 최대값이 255가 되도록 그레이스케일 값을 정규화.
	//-------------------------------------------------------------------------

	int x, y;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + m_nWidth/2;
		y = j + m_nHeight/2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		ptr[j*m_nHeight + i] = (BYTE)limit(pPhase[y][x]*128/PI + 128);
	}

	//-------------------------------------------------------------------------
	// 임시 버퍼 메모리 공간 해제
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
		delete [] pPhase[i];
	delete [] pPhase;
}


// m_pRe에 해당하는 배열값을 입력영상의 픽셀값으로 복사
void FastFourierTransform::GetImage()
{
	if( m_pRe == NULL )
		return;

	//-------------------------------------------------------------------------
	// 복소수의 실수부의 값을 이용하여 출력 영상을 생성한다. 
	//-------------------------------------------------------------------------

	register int i, j;

	// 결과버퍼
	BYTE * ptr = (BYTE *)m_gray_img2->imageData;

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		ptr[j*m_nHeight+i] = (BYTE)limit(m_pRe[j][i] + 0.5);
	}
}


// 가로,세로 2의 승수인지 체크
BOOL FastFourierTransform::IsPowerOf2(int n)
{
	int ref = 1;
	
	while( ref < n )
		ref <<= 1;
	
	if( ref == n ) 
		return TRUE;
	else
		return FALSE;
}

void FastFourierTransform::Free()
{
	register int i;

	//-------------------------------------------------------------------------
	// 동적 할당된 메모리가 존재한다면 해제한다. 
	//-------------------------------------------------------------------------

	if( m_pRe )
	{
		for( i = 0 ; i < m_nHeight ; i++ )
			delete [] m_pRe[i];
		delete [] m_pRe;
	}

	if( m_pIm )
	{
		for( i = 0 ; i < m_nHeight ; i++ )
			delete [] m_pIm[i];
		delete [] m_pIm;
	}
}

template<typename T>
void FastFourierTransform::swap(T &lha, T &rha){
	T tmp = lha;
	lha = rha;
	rha = tmp;
}

template<typename T>
T FastFourierTransform::limit(const T& value)
{
	return ( (value > 255) ? 255 : ((value < 0) ? 0 : value) );
}
	
ReturnType FastFourierTransform::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FastFourierTransform::onPeriodChanged()
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
	return new FastFourierTransform();
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
	return new FastFourierTransform();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

