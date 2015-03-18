
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

#include "KitechVerilookFaceRecognitionComp.h"

//
// constructor declaration
//
KitechVerilookFaceRecognitionComp::KitechVerilookFaceRecognitionComp()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechVerilookFaceRecognitionComp::KitechVerilookFaceRecognitionComp(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

KitechVerilookFaceRecognitionComp::~KitechVerilookFaceRecognitionComp() {
}
vector<VisionRecognitionResult> KitechVerilookFaceRecognitionComp::Recognize(vector<unsigned char> image,int width,int height,int pixelBytes)
{
	vector<VisionRecognitionResult> _recognitionResult(0);

	HNImage hImage;
	MakeGrayNImage (&hImage, (BYTE *)&image[0], width, height, pixelBytes);
	DetectFaceOnce (_hExtractor, hImage, _recognitionResult);
	NImageFree(hImage);

	return _recognitionResult;
}

ReturnType KitechVerilookFaceRecognitionComp::GetError()
{
	return OPROS_SUCCESS;
}

::Property KitechVerilookFaceRecognitionComp::GetParameter()
{
	return this->parameter;
}

ReturnType KitechVerilookFaceRecognitionComp::SetParameter(::Property parameter)
{
	this->parameter = parameter;
	return OPROS_SUCCESS;
}


/**
 *	카메라에서 받은 이미지를 OpenCV를 이용하여 Gray image로 변환 후 이를 다시 HNImage 형태로 변환한다.
 */
void KitechVerilookFaceRecognitionComp::MakeGrayNImage (HNImage *nImage, const BYTE *RGBimage, int width, int height, int pixelByte)
{
	IplImage *cvRGBImage = cvCreateImageHeader( cvSize(width, height), IPL_DEPTH_8U, pixelByte );
	cvRGBImage->imageData = (char *)RGBimage;

	IplImage *cvGrayImage = cvCreateImage( cvGetSize(cvRGBImage), IPL_DEPTH_8U, 1 );
	cvConvertImage( cvRGBImage, cvGrayImage, CV_BGR2GRAY );	// RGB이미지를 Gray이미지로 변환

	// Create HNImage from Grayscale Image data
	NResult result = NImageCreateFromData(npfGrayscale, (NUInt)width, (NUInt)height, (NSizeType)width, 0.0, 0.0, (NSizeType)width, cvGrayImage->imageData, nImage);

	// Release OpenCV variables
	cvReleaseImage(&cvGrayImage);
	cvReleaseImageHeader(&cvRGBImage);
}

/**
 * Detects face in single frame/image.
 * @param	image				image for face detection.
 */
NResult KitechVerilookFaceRecognitionComp::DetectFaceOnce(HNLExtractor _hExtractor, HNImage hImage, std::vector<VisionRecognitionResult> &faceResult)
{
	NInt faceCount = 0;
	NleFace *fd = NULL;
	NResult res = NleDetectFaces(_hExtractor, hImage, &faceCount, &fd);

	if (NSucceeded(res))
	{
		if (faceCount > 0)
		{
			NleDetectionDetails *details = (NleDetectionDetails *) malloc(sizeof(NleDetectionDetails) * faceCount);
			for (int i = 0; i < faceCount; ++i)
			{
				details[i].FaceAvailable = NTrue;
				details[i].Face = fd[i];
				if (i == 0)
				{
					details[i].EyesAvailable = NTrue;
					res = NleDetectFacialFeatures(_hExtractor, hImage, &fd[i], &details[i]);
					if (NFailed(res))
					{
						details[i].EyesAvailable = NFalse;
					}
				}
				else
				{
					details[i].EyesAvailable = NFalse;
				}
			}

			NFree(fd);
			faceResult.resize (faceCount);
			// 찾은 얼굴을 저장
			int n = 0;
			for (int i = 0; i < faceCount; ++i)
			{
				if( details[i].FaceAvailable )
				{
					faceResult[n].name = "Face";
					faceResult[n].point1X = details[i].Face.Rectangle.X;
					faceResult[n].point1Y = details[i].Face.Rectangle.Y;
					faceResult[n].point2X = details[i].Face.Rectangle.X+details[i].Face.Rectangle.Width;
					faceResult[n].point2Y = details[i].Face.Rectangle.Y;
					faceResult[n].point3X = details[i].Face.Rectangle.X+details[i].Face.Rectangle.Width;
					faceResult[n].point3Y = details[i].Face.Rectangle.Y+details[i].Face.Rectangle.Height;
					faceResult[n].point4X = details[i].Face.Rectangle.X;
					faceResult[n].point4Y = details[i].Face.Rectangle.Y+details[i].Face.Rectangle.Height;
					n++;
				}
			}
			free(details);
		}
		else
		{
			// - fail if no face was detected
			faceResult.resize(0);
			res = N_E_FAILED;
		}
	}

	return res;
}

void KitechVerilookFaceRecognitionComp::portSetup() {
	ProvidedServicePort *pa1;
	pa1=new VisionRecognitionServiceProvided(this);
	addPort("VisionRecognitionService",pa1);




	// export variable setup


}

// Call back Declaration
ReturnType KitechVerilookFaceRecognitionComp::onInitialize()
{
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onStart()
{
	NleCreate(&_hExtractor);
	return OPROS_SUCCESS;
}
	
ReturnType KitechVerilookFaceRecognitionComp::onStop()
{
#if defined(WIN32)
	Sleep(100);
#endif
	NleFree(_hExtractor);
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechVerilookFaceRecognitionComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechVerilookFaceRecognitionComp::onPeriodChanged()
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
	return new KitechVerilookFaceRecognitionComp();
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
	return new KitechVerilookFaceRecognitionComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

