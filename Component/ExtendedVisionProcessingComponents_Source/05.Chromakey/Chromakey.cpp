
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



#include "Chromakey.h"

//
// constructor declaration
//
int m_in_height;                 // ��������Height
int m_in_width;                  // ��������Width
int m_in2_height;                // Background����Height
int m_in2_width;                 // Background����Width

int m_low_h;
int m_high_h;
int m_polly_hull;
int m_perimScale;
int m_num;
int m_approx_level;
int m_morpology_repeat;

// �������� �̹�������
IplImage* m_image_buff;			 // Original Image
IplImage* m_image_extracted;     // Extracted Image
// ����ũ����
IplImage* m_image_mask;			 // Mask Image
IplImage* m_image_inverseMask;   // Inverse Mask Image
IplImage* m_image_mask_temp;	 // Mask temp Image
// HSV����
IplImage* m_image_hsv;
IplImage* m_image_h;
// ��濵�� �̹�������
IplImage* m_image_BG_in;         // Background Input image
IplImage* m_image_BG;            // Background Converted Image(Resized to Camera Image)
IplImage* m_image_extractedBG;   // Extracted Background Image
// ���IplImage
IplImage* m_image_destination;	 

void find_connected_components( IplImage* mask, int poly1_hull0, float perimScale, int* num, CvRect* bbs, CvPoint* centers);


Chromakey::Chromakey()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Chromakey::Chromakey(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

Chromakey::~Chromakey() {
}

void Chromakey::portSetup() {

	// data port setup

	addPort("ImageIn1", &ImageIn1);

	addPort("ImageIn2", &ImageIn2);

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType Chromakey::onInitialize()
{
	// �������� �̹�������
	if(m_image_buff        != NULL)
		m_image_buff        = NULL;
    if(m_image_extracted   != NULL)
		m_image_extracted   = NULL;

    // ����ũ����
	if(m_image_mask   != NULL)
		m_image_mask        = NULL;
	if(m_image_inverseMask   != NULL)
		m_image_inverseMask = NULL;
	if(m_image_mask_temp  != NULL)
		m_image_mask_temp = NULL;
    
    // ��濵�����
	if(m_image_BG_in   != NULL)
		m_image_BG_in       = NULL;
    if(m_image_BG   != NULL)
		m_image_BG          = NULL;
    if(m_image_extractedBG   != NULL)
		m_image_extractedBG = NULL;

    // ���
	if(m_image_destination   != NULL)
		m_image_destination = NULL;

	// HSV����
	if(m_image_h   != NULL)
		m_image_h = NULL;
	if(m_image_hsv   != NULL)
		m_image_hsv = NULL;

	m_low_h			= atoi(getProperty("low_h").c_str());
	m_high_h		= atoi(getProperty("high_h").c_str());
	m_polly_hull	= atoi(getProperty("polly_hull").c_str());
	m_perimScale	= atoi(getProperty("perimScale").c_str());
	m_num			= atoi(getProperty("num").c_str());
	m_approx_level	= atoi(getProperty("approx_level").c_str());
	m_morpology_repeat = atoi(getProperty("morpology_repeat").c_str());

	m_in_height     = 0;
    m_in_width      = 0;
    m_in2_height    = 0;
    m_in2_width     = 0;

	return OPROS_SUCCESS;
}

ReturnType Chromakey::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Chromakey::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onDestroy()
{
	// �������� �̹�������
	if(m_image_buff        != NULL)
        cvReleaseImage(&m_image_buff);
    if(m_image_extracted   != NULL)
        cvReleaseImage(&m_image_extracted);
    // ����ũ����
	if(m_image_mask        != NULL)
        cvReleaseImage(&m_image_mask);
    if(m_image_inverseMask != NULL)
        cvReleaseImage(&m_image_inverseMask);
	if(m_image_mask_temp != NULL)
        cvReleaseImage(&m_image_mask_temp);
    // ��濵�� �̹�������
	if(m_image_BG_in       != NULL)
        cvReleaseImage(&m_image_BG_in);     
	if(m_image_BG          != NULL)
        cvReleaseImage(&m_image_BG);
    if(m_image_extractedBG != NULL)
        cvReleaseImage(&m_image_extractedBG);
	// ���
    if(m_image_destination != NULL)
        cvReleaseImage(&m_image_destination);
	// HSV����
	if(m_image_hsv != NULL)
        cvReleaseImage(&m_image_hsv);
	if(m_image_h != NULL)
        cvReleaseImage(&m_image_h);

	return OPROS_SUCCESS;
}

ReturnType Chromakey::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData1 = ImageIn1.pop();
	opros_any *pData2 = ImageIn2.pop();

	RawImage result;

	RawImage Image1;
	RawImage Image2;
	
	// Port for Background image
    if (pData2 != NULL) 
    {
        // ��Ʈ�� ���� �̹��� ���
		Image2 = ImageIn2.getContent(*pData2);
		RawImageData *RawImage2 = Image2.getImage();

		// ���翵���� ũ�⸦ ���
		m_in2_width = RawImage2->getWidth();
		m_in2_height = RawImage2->getHeight();

		// ���������� �̹������� Ȯ��
		if(m_image_BG_in == NULL){
			m_image_BG_in = cvCreateImage(cvSize(m_in2_width, m_in2_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_BG_in->imageData, RawImage2->getData(), RawImage2->getSize());

		delete pData2;

		if(pData1 != NULL)
		{
			Image1 = ImageIn1.getContent(*pData1);

			RawImageData *RawImage1 = Image1.getImage();

			// ���翵���� ũ�⸦ ���
			m_in_width = RawImage1->getWidth();
			m_in_height = RawImage1->getHeight();

			 // �������� �̹�������
			if(m_image_buff == NULL)
				m_image_buff		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			if(m_image_extracted == NULL)
				m_image_extracted   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );

			// ����ũ����
			if(m_image_mask == NULL)
				m_image_mask        = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
			if(m_image_inverseMask == NULL)
				m_image_inverseMask = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
			if(m_image_mask_temp == NULL)
				m_image_mask_temp   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );
	            
			// HSV����
			if(m_image_hsv == NULL)
				m_image_hsv			= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
			if(m_image_h == NULL)
				m_image_h			= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1 );

			// ��濵�� �̹�������
			if(m_image_BG == NULL)
				m_image_BG          = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );
			if(m_image_extractedBG == NULL)
				m_image_extractedBG = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );

			// ���
			if(m_image_destination == NULL)
				m_image_destination = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3 );

			// ��濵�� ��������
			if(m_image_BG_in != NULL)
				cvResize(m_image_BG_in, m_image_BG, CV_INTER_LINEAR);

			// ���� ���� ������ Ȯ��!memcpy
			memcpy(m_image_buff->imageData, RawImage1->getData(), RawImage1->getSize());

			// ����ȯ BGR->HSV
			cvCvtColor(m_image_buff, m_image_hsv, CV_BGR2HSV);

			// HSVä�κи�
			cvSplit(m_image_hsv, m_image_h, NULL, NULL, NULL);

			// ���������� H���� ���(������) ����
			cvInRangeS( m_image_h, cvScalar(m_low_h), cvScalar(m_high_h), m_image_mask );	// m_image_mask : ������(���� ����)
			cvNot( m_image_mask, m_image_inverseMask );										// m_image_inverseMask : ��������(����������)

			// ������ IplImage(1ä��)
			cvSet(m_image_mask_temp, cvScalar(0), m_image_mask);							// ���κ��� ������
			cvSet(m_image_mask_temp, cvScalar(255), m_image_inverseMask);					// �׿ܿ��� ���		

			// ����������
			find_connected_components(m_image_mask_temp, m_polly_hull, m_perimScale, NULL, NULL, NULL);

			// ���������ŵ� ����ũ���� �������и�
			cvInRangeS( m_image_mask_temp, cvScalar(0), cvScalar(1), m_image_mask );	
			cvInRangeS( m_image_mask_temp, cvScalar(255), cvScalar(256), m_image_inverseMask );

			// ����ũ�� �̿��Ͽ� �������� �� ���� ����
			cvSetZero( m_image_extracted );
			cvCopy( m_image_buff, m_image_extracted, m_image_inverseMask );					// ��������(��������� ����κ�)

			// ����ũ�� �̿��Ͽ� ������� �� ���� ����
			cvSetZero( m_image_extractedBG );
			cvCopy( m_image_BG, m_image_extractedBG, m_image_mask );						// ��濵��(�������κ�)
			
			
			// ��� + ����
			cvAdd( m_image_extractedBG, m_image_extracted, m_image_destination, NULL);

			// RawImage�� �̹��� ������ ���� �Ҵ�
			RawImageData *pimage = result.getImage();
			
			// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
			pimage->resize(m_in_width, m_in_height, m_image_destination->nChannels);
			
			// ������ �� ũ��(pixels��) ���
			int size = m_in_width * m_in_height * m_image_destination->nChannels;
			
			// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
			unsigned char *ptrdata = pimage->getData();
			
			// ���� ������ ������ ������ ��ŭ memcpy
			memcpy(ptrdata, m_image_destination->imageData, size);

			// ��Ʈ�ƿ�
			opros_any mdata = result;
			ImageOut.push(result);//����

			delete pData1;
		}
    }	

	return OPROS_SUCCESS;
}

void find_connected_components( IplImage* mask, int poly1_hull0, float perimScale, int* num, CvRect* bbs, CvPoint* centers){
	static CvMemStorage* mem_storage = NULL;
	static CvSeq* contours = NULL;

	// ����, �ݱ�
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_OPEN, m_morpology_repeat);
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_CLOSE, m_morpology_repeat);

	// ū������ �ѷ��� �ܰ����� ã��
	if( mem_storage == NULL)
		mem_storage = cvCreateMemStorage(0);
	else
		cvClearMemStorage(mem_storage);

	CvContourScanner scanner = cvStartFindContours(
		mask, mem_storage, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	CvSeq* c;
	int numCont = 0;
	while( (c = cvFindNextContour( scanner)) != NULL)
	{
		double len = cvContourPerimeter( c );

		// �ܰ��� ������ �ּ� �Ӱ谪 ���
		double q = (mask->height + mask->width)/perimScale;

		// �ܰ����� �ʹ� ���� ������ ����
		if( len < q) {
			cvSubstituteContour( scanner, NULL );
		}else{
			// �ܰ��� ���̰� ����� ū ���, ������ �������Ѵ�
			CvSeq* c_new;

			if( poly1_hull0 ){
				// �ٰ��� �ٻ�ȭ
				c_new = cvApproxPoly( c, sizeof(CvContour), mem_storage, CV_POLY_APPROX_DP, m_approx_level, 0 );
			}else{
				// ������ �� �ٻ�ȭ
				c_new = cvConvexHull2(c, mem_storage, CV_CLOCKWISE, 1);
			}
			cvSubstituteContour( scanner, c_new);
			numCont++;
		}
	}
	contours = cvEndFindContours( &scanner );

	// ���Ǹ� ���� ���� ����
	const CvScalar CVX_WHITE = CV_RGB(0xff, 0xff, 0xff);
	const CvScalar CVX_BLACK = CV_RGB(0x00, 0x00, 0x00);

	// ã���� ������ ���� �׸���
	cvZero( mask );
	IplImage* maskTemp;

	// ������ �߽ɰ� �ٿ�� �ڽ��� ���Ѵ�
	if( num != NULL )
	{
		// ����ڰ� ����� ��ġ�� �����ϱ⸦ ���Ѵ�
		int N = *num, numFilled = 0, i = 0;
		CvMoments moments;
		double M00, M01, M10;
		maskTemp = cvCloneImage(mask);

		for( i=0, c = contours; c!=NULL; c=c->h_next, i++)
		{
			if(i < N)
			{
				//(*num)������ �۾�����
				cvDrawContours(maskTemp, c, CVX_WHITE, CVX_WHITE, -1, CV_FILLED, 8);

				//�� �ܰ����� �߽��� ã�´�
				if(centers != NULL)
				{
					cvMoments(maskTemp, &moments, 1);
					M00 = cvGetSpatialMoment(&moments, 0, 0);
					M10 = cvGetSpatialMoment(&moments, 1, 0);
					M01 = cvGetSpatialMoment(&moments, 0, 1);
					centers[i].x = (int)(M10/M00);
					centers[i].y = (int)(M01/M00);
				}

				// ������ ���δ� �ٿ�� �ڽ��� ���Ѵ�
				if(bbs != NULL){
					bbs[i] = cvBoundingRect(c);
				}
				cvZero(maskTemp);
				numFilled++;
			}

			// ä���� �ܰ����� mask���� �׸���
			cvDrawContours( mask, c, CVX_WHITE, CVX_WHITE, -1, CV_FILLED, 8);
		}// end of for
		*num = numFilled;
		cvReleaseImage( &maskTemp);
	}

	//�ƴϸ� �ܼ��� ����ũ�� �׸��⸸ �Ѵ�
	else
	{
		// ����ڰ� ����� ��ġ�� ����ϴ� ���, �ܰ����� �׸��⸦ ���Ѵ�
		for( c=contours; c!=NULL; c=c->h_next) {
			cvDrawContours( mask, c, CVX_WHITE, CVX_BLACK, -1, CV_FILLED, 8);
		}
	}
}

ReturnType Chromakey::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onPeriodChanged()
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
	return new Chromakey();
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
	return new Chromakey();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

