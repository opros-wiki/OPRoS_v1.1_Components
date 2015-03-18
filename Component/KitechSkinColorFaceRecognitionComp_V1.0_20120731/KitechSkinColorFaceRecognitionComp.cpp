
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
#include "OprosPrintMessage.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "BlobLabeling.h"



#include "KitechSkinColorFaceRecognitionComp.h"

//
// constructor declaration
//
KitechSkinColorFaceRecognitionComp::KitechSkinColorFaceRecognitionComp()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechSkinColorFaceRecognitionComp::KitechSkinColorFaceRecognitionComp(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

KitechSkinColorFaceRecognitionComp::~KitechSkinColorFaceRecognitionComp() {
}
vector<VisionRecognitionResult> KitechSkinColorFaceRecognitionComp::Recognize(vector<unsigned char> image,int width,int height,int pixelBytes)
{
	vector<VisionRecognitionResult> _recognitionResult(0);

	IplImage *cvImage = cvCreateImageHeader( cvSize(width, height), 8, pixelBytes );
	cvImage->imageData = (char *)&(image)[0];

	IplImage *ycrcbImage = cvCreateImage( cvGetSize(cvImage), 8, pixelBytes );
	cvCvtColor(cvImage, ycrcbImage, CV_BGR2YCrCb);

	IplImage *cvMask= cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );

	cvInRangeS( ycrcbImage, cvScalar(0,133,77), cvScalar(255,173,127), cvMask );

	CBlobLabeling blob;
	blob.SetParam( cvMask, width*height/100 );
	blob.DoLabeling();

	_recognitionResult.resize(blob.m_nBlobs);
	for( int i = 0  ;  i < blob.m_nBlobs  ;  i++ ) {
		_recognitionResult[i].name = "Face";

		_recognitionResult[i].point1X = blob.m_recBlobs[i].x;
		_recognitionResult[i].point1Y = blob.m_recBlobs[i].y;
		_recognitionResult[i].point2X = blob.m_recBlobs[i].x + blob.m_recBlobs[i].width;
		_recognitionResult[i].point2Y = blob.m_recBlobs[i].y;
		_recognitionResult[i].point3X = blob.m_recBlobs[i].x + blob.m_recBlobs[i].width;
		_recognitionResult[i].point3Y = blob.m_recBlobs[i].y + blob.m_recBlobs[i].height;
		_recognitionResult[i].point4X = blob.m_recBlobs[i].x;
		_recognitionResult[i].point4Y = blob.m_recBlobs[i].y + blob.m_recBlobs[i].height;
		//PrintMessage("SUCCESS:KitechSkinColorFaceRecognitionComp::recognize() -> I found %d data.(%d=%s)\n", blob.m_nBlobs, i, _recognitionResult[i].name.c_str());
	}

	//cvSaveImage("KitechSkinColorFaceRecognitionComp.jpg", cvImage);

	cvReleaseImage(&cvMask);
	cvReleaseImage(&ycrcbImage);
	cvReleaseImageHeader(&cvImage);

	return _recognitionResult;
}

ReturnType KitechSkinColorFaceRecognitionComp::GetError()
{
	return OPROS_SUCCESS;
}

Property KitechSkinColorFaceRecognitionComp::GetParameter()
{
	return this->parameter;
}

ReturnType KitechSkinColorFaceRecognitionComp::SetParameter(Property parameter)
{
	this->parameter = parameter;
	return OPROS_SUCCESS;
}


void KitechSkinColorFaceRecognitionComp::portSetup() {
	ProvidedServicePort *pa1;
	pa1=new VisionRecognitionServiceProvided(this);
	addPort("VisionRecognitionService",pa1);




	// export variable setup


}

// Call back Declaration
ReturnType KitechSkinColorFaceRecognitionComp::onInitialize()
{
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onStart()
{
	return OPROS_SUCCESS;
}
	
ReturnType KitechSkinColorFaceRecognitionComp::onStop()
{
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechSkinColorFaceRecognitionComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechSkinColorFaceRecognitionComp::onPeriodChanged()
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
	return new KitechSkinColorFaceRecognitionComp();
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
	return new KitechSkinColorFaceRecognitionComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

