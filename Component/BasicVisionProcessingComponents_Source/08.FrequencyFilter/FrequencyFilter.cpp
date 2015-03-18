
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

#include "FrequencyFilter.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_gray_img;
IplImage* m_result_img;
IplImage* m_final_img;
IplImage* m_image_Re;
IplImage* m_image_Im;
IplImage* m_pDFT_A;

int m_in_width;
int m_in_height;

std::string m_low_Pass_Filtering;
std::string m_high_Pass_Filtering;
float m_cutoff_Frequency;

FrequencyFilter::FrequencyFilter()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FrequencyFilter::FrequencyFilter(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FrequencyFilter::~FrequencyFilter() {
}

void FrequencyFilter::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType FrequencyFilter::onInitialize()
{

	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_gray_img != NULL)
		m_gray_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;
	if(m_final_img != NULL)
		m_final_img = NULL;
	if(m_image_Re != NULL)
		m_image_Re = NULL;
	if(m_image_Im != NULL)
		m_image_Im = NULL;
	if(m_pDFT_A != NULL)
		m_pDFT_A = NULL;

	m_in_width = 0;
	m_in_height = 0;

	m_low_Pass_Filtering = getProperty("LowPassFiltering");
	m_high_Pass_Filtering = getProperty("HighPassFiltering");
	m_cutoff_Frequency = atof(getProperty("CutOffFrequency").c_str());

	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FrequencyFilter::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onDestroy()
{
	// ����� ��� �̹������� �޸� ���� ����
	if(m_orig_img != NULL)
        cvReleaseImage(&m_orig_img);
	if(m_gray_img != NULL)
		cvReleaseImage(&m_gray_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);
	if(m_final_img != NULL)
		cvReleaseImage(&m_final_img);
	if(m_image_Re != NULL)
		cvReleaseImage(&m_image_Re);
	if(m_image_Im != NULL)
		cvReleaseImage(&m_image_Im);
	if(m_pDFT_A != NULL)
		cvReleaseImage(&m_pDFT_A);

	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onExecute()
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

		// ���� ������ 2�� �¼����� Ȯ��
		if(!Check2Square(m_in_width) || !Check2Square(m_in_height)){

			std::cout << "This image is not a multifplier of 2" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;
		
		}

		// ���� ������ ���� ���� ����� ������ Ȯ��
		if(m_in_width != m_in_height){
		
			std::cout << "Size(width and height) of Image is not equal" << std::endl;

			return OPROS_BAD_INPUT_PARAMETER;

		}

		// ���������� �̹������� Ȯ��
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// ���̳ʸ� ���󿵿��� Ȯ��
		if(m_gray_img == NULL){
			m_gray_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		// ������ ���󿵿��� Ȯ��
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		}
		// ��°�� ���󿵿��� Ȯ��
		if(m_final_img == NULL){
			m_final_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}
		// Re���� ���󿵿��� Ȯ��(�Ǽ�)
		if(m_image_Re == NULL){
			m_image_Re = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 1);
		}
		// Im���� ���󿵿��� Ȯ��(���)
		if(m_image_Im == NULL){
			m_image_Im = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 1);
		}
		// ���ļ� ��ȯ ���󿵿��� Ȯ��.
		if(m_pDFT_A == NULL){
			m_pDFT_A = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 2);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// �Ѵ� none �� �ƴϰų�, ���߿� �ϳ��� none �ϰ��
		if((m_low_Pass_Filtering != "none" || m_high_Pass_Filtering != "none") &&  
			(m_low_Pass_Filtering == "none" || m_high_Pass_Filtering == "none")){

			// �Է� ���� ������ ����ȭ ��Ŵ
			cvCvtColor( m_orig_img, m_gray_img, CV_BGR2GRAY );

			// ���ļ����������� �۾��� ���� ���� ���� ����
			cvConvertScale(m_gray_img, m_image_Re);		// 8U -> 32F
			// m_image_Im�� �ʱ�ȭ
			cvZero(m_image_Im);

			// shift center
			// �Է¿����� �Ǽ��η� ��ȯ�� �̹����� Ȧ���� ȭ���� ��ȣ�� �����Ͽ�
			// Ǫ������ȯ�� ���� ���ļ� ������ ������ �߽����� �̵���Ű�� ����
			ChangePosition(m_image_Re);

			cvMerge(m_image_Re, m_image_Im, NULL, NULL, m_pDFT_A);

			// m_pDFT_A�� ���� Ǫ���� ��ȯ�� ����
			cvDFT(m_pDFT_A, m_pDFT_A, CV_DXT_FORWARD);

			// �̻��� ������ ��� ���͸� ����
			if(m_low_Pass_Filtering == "ideal" && m_high_Pass_Filtering == "none"){
				
				IdealLowPassFiltering(m_pDFT_A, m_cutoff_Frequency);
				
			}
			// ���Ϳ��� ������ ��� ���͸� ����
			else if(m_low_Pass_Filtering == "butterworth" && m_high_Pass_Filtering == "none"){

				ButterworthLowPassFiltering(m_pDFT_A, m_cutoff_Frequency, 2);
				
			}
			// ����þ� ������ ��� ���͸� ����
			else if(m_low_Pass_Filtering == "gaussian" && m_high_Pass_Filtering == "none"){

				GaussianLowPassFiltering(m_pDFT_A, m_cutoff_Frequency);
				
			}
			// �̻��� ������ ��� ���͸� ����
			else if(m_high_Pass_Filtering == "ideal" && m_low_Pass_Filtering == "none"){
				
				IdealHighPassFiltering(m_pDFT_A, m_cutoff_Frequency);
				
			}
			// ���Ϳ��� ������ ��� ���͸� ����
			else if(m_high_Pass_Filtering == "butterworth" && m_low_Pass_Filtering == "none"){
				
				ButterworthHighPassFiltering(m_pDFT_A, m_cutoff_Frequency, 2);
				
			}
			// ����þ� ������ ��� ���͸� ����
			else if(m_high_Pass_Filtering == "gaussian" && m_low_Pass_Filtering == "none"){
				
				GaussianHighpassFiltering(m_pDFT_A, m_cutoff_Frequency);
				

			}
			else{
				//none
			}

			// ǻ���� ����ȯ ����
			cvDFT(m_pDFT_A, m_pDFT_A, CV_DXT_INV_SCALE);

			// ���� ä���� ����� ���� ä�� ��ķ� ����(Re, Im����)
			cvSplit(m_pDFT_A, m_image_Re, m_image_Im, NULL, NULL);
			
			// �������϶��� ����
			if((m_low_Pass_Filtering == "ideal" || m_low_Pass_Filtering == "butterworth" || m_low_Pass_Filtering == "gaussian")
				&& m_high_Pass_Filtering == "none"){
				ChangePosition(m_image_Re);
				cvScale(m_image_Re, m_result_img, 1);
			}

			// �������϶��� ����
			if((m_high_Pass_Filtering == "ideal" || m_high_Pass_Filtering == "butterworth" || m_high_Pass_Filtering == "gaussian")
				&& m_low_Pass_Filtering == "none"){
				
				// ����Ʈ���� ������ ��� Mag=sqrt(Re^2 + Im^2)
				cvPow(m_image_Re, m_image_Re, 2.0);
				cvPow(m_image_Im, m_image_Im, 2.0);
				cvAdd(m_image_Re, m_image_Re, m_image_Re);
				cvPow(m_image_Re, m_image_Re, 0.5); 
				
				// ���� ȭ���� �ȼ�ġ�� min�� max���̿� �����Ϸε� �����ϸ�
				double min_val, max_val;
				cvMinMaxLoc(m_image_Re, &min_val, &max_val, NULL, NULL);
				cvScale(m_image_Re, m_result_img, 255.0/max_val);
			}
			
			// 1ä�� ������ 3ä�� ���������� ��ȯ
			cvMerge(m_result_img, m_result_img, m_result_img, NULL, m_final_img);

			// �ƿ�ǲ push
			// RawImage�� �̹��� ������ ���� �Ҵ�
			RawImageData *pimage = result.getImage();
			
			// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
			pimage->resize(m_final_img->width, m_final_img->height, m_final_img->nChannels);
			
			// ������ �� ũ��(pixels��) ���
			int size = m_final_img->width * m_final_img->height * m_final_img->nChannels;
			
			// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
			unsigned char *ptrdata = pimage->getData();
			
			// ���� ������ ������ ������ ��ŭ memcpy
			memcpy(ptrdata, m_final_img->imageData, size);

			// ��Ʈ�ƿ�
			opros_any mdata = result;
			ImageOut.push(result);//����

			delete pData;

		}else{

			// �ƿ�ǲ push
			// �ƿ�ǲ push
			// RawImage�� �̹��� ������ ���� �Ҵ�
			RawImageData *pimage = result.getImage();
			
			// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
			pimage->resize(m_orig_img->width, m_orig_img->height, m_orig_img->nChannels);
			
			// ������ �� ũ��(pixels��) ���
			int size = m_orig_img->width * m_orig_img->height * m_orig_img->nChannels;
			
			// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
			unsigned char *ptrdata = pimage->getData();
			
			// ���� ������ ������ ������ ��ŭ memcpy
			memcpy(ptrdata, m_orig_img->imageData, size);

			// ��Ʈ�ƿ�
			opros_any mdata = result;
			ImageOut.push(result);//����

			delete pData;

		}
		
	}

	return OPROS_SUCCESS;
}

void FrequencyFilter::ChangePosition(IplImage *pImage)
{
	int x, y;
	float fValue;

	for(y = 0; y < pImage->height; y++)
		for(x = 0; x < pImage->width; x++)
		{
			fValue = (float)cvGetReal2D(pImage, y, x);
			if((x+y)%2 == 1)
				fValue = - fValue;
			cvSetReal2D(pImage, y, x, fValue);
		}
	
}

// �̻��� ������ ���� ��� ���͸�
void FrequencyFilter::IdealHighPassFiltering(IplImage *pDFT, double D0)
{
	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u <pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			if(D < D0)
				H = 0.0;
			else
				H = 1.0;

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}
}

// �̻��� ������ ���� ��� ���͸�
void FrequencyFilter::IdealLowPassFiltering(IplImage *pDFT, double D0)
{
	int u, v;
	double D; // �Ÿ�����
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));

			if(D <= D0)
				H = 1.0;
			else
				H = 0.0;
			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}
}

// ���Ϳ��� ������ ���� ��� ���͸�
void FrequencyFilter::ButterworthHighPassFiltering(IplImage *pDFT, double D0, int nCh)
{

	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			if(D == 0)
				D = 0.1;

			H = 1.0 / (1.0 + pow(D0 / D, 2*nCh));
			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}


}

// ���Ϳ��� ������ ���� ��� ���͸�
void FrequencyFilter::ButterworthLowPassFiltering(IplImage *pDFT, double D0, int nCh)
{

	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			H = 1.0 / (1.0 + pow(D / D0, 2*nCh));

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);

		}

}

// ����þ� ������ ���� ��� ���͸�
void FrequencyFilter::GaussianHighpassFiltering(IplImage *pDFT, double D0)
{

	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			H = 1.0 - exp(-D * D / (2.0 * D0 * D0));

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);
		
		}

}

// ����þ� ������ ���� ��� ���͸�
void FrequencyFilter::GaussianLowPassFiltering(IplImage *pDFT, double D0)
{
	int u, v;
	double D;
	double H;
	double centerU = pDFT->width/2;
	double centerV = pDFT->height/2;
	CvScalar cmplxValue;

	for(v = 0; v < pDFT->height; v++)
		for(u = 0; u < pDFT->width; u++){
		
			D = sqrt((u - centerU)*(u - centerU) + (v - centerV)*(v - centerV));
			H = exp(-D * D / (2.0 * D0 * D0));

			cmplxValue = cvGet2D(pDFT, v, u);
			cmplxValue.val[0]*= H;
			cmplxValue.val[1]*= H;

			cvSet2D(pDFT, v, u, cmplxValue);
		
		}
}

// 2�� �¼� üũ �Լ�
bool FrequencyFilter::Check2Square(int number)
{
	
	int ref = 1;

	while(ref < number)
		ref <<= 1;

	if(ref == number)
		return true;
	else
		return false;

}

	
ReturnType FrequencyFilter::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onPeriodChanged()
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
	return new FrequencyFilter();
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
	return new FrequencyFilter();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

