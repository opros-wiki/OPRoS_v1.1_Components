
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

#include "KitechHaarFaceRecognitionComp.h"

const char* cascade_name = "haarcascade_frontalface_alt2.xml";

//
// constructor declaration
//
KitechHaarFaceRecognitionComp::KitechHaarFaceRecognitionComp()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechHaarFaceRecognitionComp::KitechHaarFaceRecognitionComp(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

KitechHaarFaceRecognitionComp::~KitechHaarFaceRecognitionComp() {
}
vector<VisionRecognitionResult> KitechHaarFaceRecognitionComp::Recognize(vector<unsigned char> image,int width,int height,int pixelBytes)
{
	vector<VisionRecognitionResult> _recognitionResult(0);

	IplImage *cvImage = cvCreateImageHeader( cvSize(width, height), 8, pixelBytes );
	cvImage->imageData = (char *)&image[0];

	double scale = 1.5;
	IplImage* gray = cvCreateImage( cvSize(cvImage->width,cvImage->height), 8, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (cvImage->width/scale), cvRound (cvImage->height/scale)), 8, 1 );

	cvCvtColor( cvImage, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	if( _storage ) {
		cvClearMemStorage( _storage );
	}

	if( _cascade ) {
		CvSeq* faces = cvHaarDetectObjects( small_img, _cascade, _storage,
							1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
							cvSize(30, 30) );
		if( faces->total ) {
			_recognitionResult.resize (faces->total);

			for( int i = 0; i < (faces ? faces->total : 0); i++ ) {
				CvRect* rect = (CvRect*)cvGetSeqElem( faces, i );

				//rec.type = 1;
				_recognitionResult[i].name = "Face";
				/*- Get Upper left rectangle corner coordinate -*/
				_recognitionResult[i].point1X = (int)((rect->x) * scale + 0.5);
				_recognitionResult[i].point1Y = (int)((rect->y) * scale + 0.5);
				/*- Get Upper right rectangle corner coordinate -*/
				_recognitionResult[i].point2X = (int)((rect->x + rect->width) * scale + 0.5);
				_recognitionResult[i].point2Y = (int)((rect->y) * scale + 0.5);
				/*- Get Lower right rectangle corner coordinate -*/
				_recognitionResult[i].point3X = (int)((rect->x + rect->width) * scale + 0.5);
				_recognitionResult[i].point3Y = (int)((rect->y + rect->height) * scale + 0.5);
				/*- Get Lower left rectangle corner coordinate -*/
				_recognitionResult[i].point4X = (int)((rect->x) * scale + 0.5);
				_recognitionResult[i].point4Y = (int)((rect->y + rect->height) * scale + 0.5);
			}
			//cvSaveImage("faces.jpg", cvImage);
			//PrintMessage("KitechHaarFaceRecognitionComp::Recognize() -> I found %d faces.\n", faces->total);
		}
	}
	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );
	cvReleaseImageHeader( &cvImage );

	return _recognitionResult;
}

ReturnType KitechHaarFaceRecognitionComp::GetError()
{
	return OPROS_SUCCESS;
}

Property KitechHaarFaceRecognitionComp::GetParameter()
{
	return this->parameter;
}

ReturnType KitechHaarFaceRecognitionComp::SetParameter(Property parameter)
{
	this->parameter = parameter;
	return OPROS_SUCCESS;
}


void KitechHaarFaceRecognitionComp::portSetup() {
	ProvidedServicePort *pa1;
	pa1=new VisionRecognitionServiceProvided(this);
	addPort("VisionRecognitionService",pa1);




	// export variable setup


}

// Call back Declaration
ReturnType KitechHaarFaceRecognitionComp::onInitialize()
{
	parameter.SetProperty(getPropertyMap());

	_cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
	_storage = cvCreateMemStorage(0);
	if( !_cascade ) {
		PrintMessage ("ERROR : KitechHaarFaceRecognitionComp::onInitialize() -> Can't initialize cascade!\n");
		return OPROS_CALLER_ERROR;
	}

	PrintMessage ("SUCCESS : KitechHaarFaceRecognitionComp::onInitialize()\n");
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onStart()
{
	return OPROS_SUCCESS;
}
	
ReturnType KitechHaarFaceRecognitionComp::onStop()
{
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onDestroy()
{
#if defined(WIN32)
	Sleep(100);
#endif
	if( _storage ) {
		cvReleaseMemStorage( &_storage );
		_storage = 0;
		_cascade = 0;
	}
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

#define TEST 0
ReturnType KitechHaarFaceRecognitionComp::onExecute()
{
#if(TEST==1)
	vector<unsigned char> image;
	vector<VisionRecognitionResult> recResult;
	IplImage *cvImage = cvLoadImage("friend.jpg");
	if( cvImage ) {
		image.resize(cvImage->imageSize);
		memcpy(&image[0], cvImage->imageData, cvImage->imageSize);
		recResult = Recognize(image, cvImage->width, cvImage->height, cvImage->nChannels);
		PrintMessage ("INFO : KitechHaarFaceRecognitionComp::onExecute() -> Find %d faces\n", recResult.size());
	}
	else {
		PrintMessage ("ERROR : KitechHaarFaceRecognitionComp::onExecute() -> Failt to load friend.jpg\n");
	}
#endif
	return OPROS_SUCCESS;
}
	
ReturnType KitechHaarFaceRecognitionComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechHaarFaceRecognitionComp::onPeriodChanged()
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
	return new KitechHaarFaceRecognitionComp();
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
	return new KitechHaarFaceRecognitionComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

