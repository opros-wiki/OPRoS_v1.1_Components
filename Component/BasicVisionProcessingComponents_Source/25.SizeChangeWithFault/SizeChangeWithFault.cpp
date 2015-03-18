
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

#include "SizeChangeWithFault.h"

//
// constructor declaration
//

IplImage* m_orig_img;
IplImage* m_result_img;
bool m_is_changed;

int m_in_width;
int m_in_height;

int m_change_width;
int m_change_height;

SizeChange::SizeChange()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
SizeChange::SizeChange(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

SizeChange::~SizeChange() {
}

void SizeChange::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType SizeChange::onInitialize()
{
	if(m_orig_img != NULL)
		m_orig_img = NULL;
	if(m_result_img != NULL)
		m_result_img = NULL;

	m_is_changed = false;

	m_in_width = 0;
	m_in_height = 0;

	m_change_width = atoi(getProperty("Width").c_str());
	m_change_height = atoi(getProperty("Height").c_str());

	m_faultCount = 50;
	m_currentCount = 0;
	m_recoverCount = 1;
	m_exCount = 0;

	m_imgMinW = 100;
	m_imgMinH = 100;

	std::string strFaultCount = getProperty("FaultCount");
	if (strFaultCount != "") m_faultCount = atoi(strFaultCount.c_str());

	std::string strRecoverCount = getProperty("RecoverCount");
	if (strRecoverCount != "") m_recoverCount = atoi(strRecoverCount.c_str());

	std::string strImgMinW = getProperty("ImgMinW");
	if (strImgMinW != "") m_imgMinW = atoi(strImgMinW.c_str());

	std::string strImgMinH = getProperty("ImgMinH");
	if (strImgMinH != "") m_imgMinH = atoi(strImgMinH.c_str());
	
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType SizeChange::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onRecover()
{
	static int recoverCount = 1;

	if (recoverCount > m_recoverCount) return OPROS_SUCCESS;
	recoverCount++;

	int w, h;

	w = m_change_width;
	h = m_change_height;

	m_change_width /= 2;
	m_change_height /= 2;

	if (m_change_width < m_imgMinW) m_change_width = m_imgMinW;
	if (m_change_height < m_imgMinH) m_change_height = m_imgMinH;

	if (w == m_change_width && h == m_change_height) {
		std::cout << "        - No SizeChange : (" << w << "," << h << ")" << std::endl;
	}
	else {
		m_is_changed = true;
		std::cout << "        - SizeChange : (" << w << "," << h << ") ---> (" << \
			m_change_width << "," << m_change_height << ")" << std::endl;
	}

	return OPROS_SUCCESS;
}

ReturnType SizeChange::onDestroy()
{
	if(m_orig_img != NULL)
		cvReleaseImage(&m_orig_img);
	if(m_result_img != NULL)
		cvReleaseImage(&m_result_img);

	return OPROS_SUCCESS;
}

ReturnType SizeChange::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onExecute()
{
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;

	if(pData != NULL){

		if (m_exCount < m_recoverCount) {
			m_currentCount++;
			if (m_currentCount > m_faultCount) {
				delete pData;

				m_currentCount = 0;
				m_exCount++;

				throw std::exception("UserException : too slow");
			}
		}

		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();

		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		/*
		if (m_currentCount % 50 == 0) { 
			std::cout << "----- COUNT : " << m_currentCount << std::endl;
			std::cout << "     * (img_width, img_height) = (" << m_in_width << "," << m_in_height << ")" << std::endl;
			std::cout << "     * (new_width, new_height) = (" << m_change_width << "," << m_change_height << ")" << std::endl;
		}
		*/

		// ���������� �̹������� Ȯ��
		if(m_orig_img == NULL){
			m_orig_img = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_orig_img->imageData, RawImage->getData(), RawImage->getSize());

		// �̹��� ũ�� ����
		if(m_result_img == NULL){
			m_result_img = cvCreateImage(cvSize(m_change_width, m_change_height), IPL_DEPTH_8U, 3);

			//std::cout << "----- [Oringinal Image] COUNT : " << m_currentCount << std::endl;
			//std::cout << "     * (org_width, org_height) = (" << m_in_width << "," << m_in_height << ")" << std::endl;
			//std::cout << "     * (new_width, new_height) = (" << m_change_width << "," << m_change_height << ")" << std::endl;
		}
		else if (m_is_changed) {
			//std::cout << "----- [Size Changed Image] COUNT : " << m_currentCount << std::endl;
			//std::cout << "     * (org_width, org_height) = (" << m_in_width << "," << m_in_height << ")" << std::endl;
			//std::cout << "     * (new_width, new_height) = (" << m_change_width << "," << m_change_height << ")" << std::endl;

			m_is_changed = false;
			if (m_result_img != NULL) cvReleaseImage(&m_result_img);

			m_result_img = cvCreateImage(cvSize(m_change_width, m_change_height), IPL_DEPTH_8U, 3);
		}


		// �������
		cvResize(m_orig_img, m_result_img, CV_INTER_LINEAR);

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
	
ReturnType SizeChange::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onPeriodChanged()
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
	return new SizeChange();
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
	return new SizeChange();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

