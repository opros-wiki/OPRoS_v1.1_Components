
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

IplImage* m_orig_img;	// �Է��̹��� ���� ����
IplImage* m_result_img;	// ��ȯ�̹��� ���� ����
IplImage* m_gray_img1;	// ����̹��� ���� ����
IplImage* m_gray_img2;	// ����̹��� ó�� ����

double** m_pRe;  // �Ǽ���
double** m_pIm;  // �����

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

	// �����Ҵ��� �޸�����ó��
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

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		if( !IsPowerOf2(m_orig_img->width) || !IsPowerOf2(m_orig_img->height) ) {
			// 2�� �¼��� �ƴҰ��
			std::cout << "This image is not a multifplier of 2" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;

		}else if(m_orig_img->width != m_orig_img->height){
			// ���μ��ΰ� Ʋ�����
			std::cout << "Width and height are difference." << std::endl;	

			return OPROS_BAD_INPUT_PARAMETER;

		}else{
			// �÷������� �׷��̽����Ϸ� ��ȯ
			cvCvtColor( m_orig_img, m_gray_img1, CV_RGB2GRAY );
			
			// �Է¿����� �޸𸮿� ����(m_pRe)
			SetImage();
			
			// ����̻�Ǫ����
			FFT(1);
			
			// ��� ���ð��� ����
			if(m_Result_Type == "Spectrum"){
				GetSpectrumImage();					// Ǫ��������Ʈ�� �̹���
			}else if(m_Result_Type == "PhaseAngle"){
				GetPhaseImage();					// ���� �̹���
			}else if(m_Result_Type == "InverseTran"){
				FFT(-1);							// ����̻�Ǫ��������ȯ
				GetImage();							// ����ȯ�� �̹���
			}

			// �׷����̹���(1ä��)�� 3ä�η� ����
			cvMerge(m_gray_img2, m_gray_img2, m_gray_img2, NULL, m_result_img);
		}

		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_result_img->width, m_result_img->height, m_result_img->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_result_img->width * m_result_img->height * m_result_img->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_result_img->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;
	}

	return OPROS_SUCCESS;
}

// ����� ������ ũ���� �迭 �޸� �Ҵ�: m_pRe, m_pIm
// �����ȼ����� m_pRe�� ����, m_pIm�� 0����
void FastFourierTransform::SetImage()
{
	//-------------------------------------------------------------------------
	// ��� ���� �� ���� 
	//-------------------------------------------------------------------------

	Free();

	register int i, j;

	m_nWidth  = m_gray_img1->width;
	m_nHeight = m_gray_img1->height;

	//-------------------------------------------------------------------------
	// Ǫ���� ��ȯ ��� ���� ������ �޸� ���� �Ҵ�
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
	// ���Ҽ� �迭�� �Ǽ��ο� ������ �׷��̽����� ���� ����
	//-------------------------------------------------------------------------
	BYTE* ptr = (BYTE *)m_gray_img1->imageData;

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		m_pRe[j][i] = (double)ptr[j*m_nHeight+i];
	}
}


// ����̻�Ǫ���� �Ǵ� ����ȯ
void FastFourierTransform::FFT(int dir)
{
	if( m_pRe == NULL || m_pIm == NULL )
		return;

	if( !IsPowerOf2(m_nWidth) || !IsPowerOf2(m_nHeight) )
		return;

	register int i, j;

	//-------------------------------------------------------------------------
	// ��(Row) ���� FFT (�Ǵ� IFFT)
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
	// ��(Column) ���� FFT (�Ǵ� IFFT)
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


// ����̻�Ǫ���� �Ǵ� ����ȯ
void FastFourierTransform::FFT1d(double* re, double* im, int N, int dir)
{
	register int i, j, k;

	//-------------------------------------------------------------------------
	// �Է� �������� ���� �ٲٱ�
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
	// �����ö���(Butterfly) �˰���
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


// Ǫ���� ��ȯ �� ȣ��, Ǫ���� ����Ʈ������
void FastFourierTransform::GetSpectrumImage()
{
	register int i, j;

	// �������
	BYTE * ptr = (BYTE *)m_gray_img2->imageData;

	//-------------------------------------------------------------------------
	// ����Ʈ�� ���� ������ �ӽ� ���� �޸� ���� �Ҵ�
	//-------------------------------------------------------------------------

	double** pSpectrum = new double*[m_nHeight];
	for( i = 0 ; i < m_nHeight ; i++ )
	{
		pSpectrum[i] = new double[m_nWidth];
		memset(pSpectrum[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// ���Ҽ� ���� ���밪�� �α� ��ȯ�Ͽ� ����. 
	// ����ȭ�� ��� �̹��� ������ ���Ͽ� ����Ʈ���� �ִ밪 ���.
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
	// ��� ���� ����. Ǫ���� ��ȯ ����� SHIFT �ϰ�, 
	// ��� ������ �ִ밪�� 255�� �ǵ��� �׷��̽����� ���� ����ȭ.
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
	// �ӽ� ���� �޸� ���� ����
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
		delete [] pSpectrum[i];
	delete [] pSpectrum;
}


// Ǫ���� ��ȯ �� ȣ��, ���� �������
void FastFourierTransform::GetPhaseImage()
{
	register int i, j;

	// �������
	BYTE * ptr = (BYTE *)m_gray_img2->imageData;

	//-------------------------------------------------------------------------
	// ������ ������ �ӽ� ���� �޸� ���� �Ҵ�
	//-------------------------------------------------------------------------

	double** pPhase = new double*[m_nHeight];
	for( i = 0 ; i < m_nHeight ; i++ )
	{
		pPhase[i] = new double[m_nWidth];
		memset(pPhase[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// ���Ҽ� ���� ������ ����. atan2 �Լ��� ��ȯ ���� -PI ~ PI �̴�.
	//-------------------------------------------------------------------------

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		pPhase[j][i] = atan2(m_pIm[j][i], m_pRe[j][i]); // ����� ����
	}

	//-------------------------------------------------------------------------
	// ��� ���� ����. Ǫ���� ��ȯ ����� SHIFT �ϰ�, 
	// ��� ������ �ִ밪�� 255�� �ǵ��� �׷��̽����� ���� ����ȭ.
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
	// �ӽ� ���� �޸� ���� ����
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
		delete [] pPhase[i];
	delete [] pPhase;
}


// m_pRe�� �ش��ϴ� �迭���� �Է¿����� �ȼ������� ����
void FastFourierTransform::GetImage()
{
	if( m_pRe == NULL )
		return;

	//-------------------------------------------------------------------------
	// ���Ҽ��� �Ǽ����� ���� �̿��Ͽ� ��� ������ �����Ѵ�. 
	//-------------------------------------------------------------------------

	register int i, j;

	// �������
	BYTE * ptr = (BYTE *)m_gray_img2->imageData;

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		ptr[j*m_nHeight+i] = (BYTE)limit(m_pRe[j][i] + 0.5);
	}
}


// ����,���� 2�� �¼����� üũ
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
	// ���� �Ҵ�� �޸𸮰� �����Ѵٸ� �����Ѵ�. 
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

