#include "OpenCvUsbCamera.h"
#include "OprosPrintMessage.h"

#if defined(WIN32)
#pragma warning(disable : 4996)
#endif

#define DISPLAY_POPUP 0

OpenCvUsbCamera::OpenCvUsbCamera(void)
: _capture(0), _id(0), _width(320), _height(240), _pixelByte(3), _flip(0)
{
#if(DISPLAY_POPUP==1)
	cvNamedWindow("OpenCvUsbCamera");
#endif
}

OpenCvUsbCamera::~OpenCvUsbCamera(void)
{
#if(DISPLAY_POPUP==1)
	cvDestroyWindow("OpenCvUsbCamera");
#endif
}

int OpenCvUsbCamera::Initialize(Property parameter)
{
	if( SetParameter(parameter) == API_ERROR ) {
		return API_ERROR;
	}
	else {
		_capture = cvCaptureFromCAM(_id);
		if(!_capture) {
			PrintMessage("ERROR : OpenCvUsbCamera::Initialize() -> Can't init camera device.\n");
			return API_ERROR;
		}

		cvSetCaptureProperty( _capture, CV_CAP_PROP_FRAME_WIDTH, (double)_width );
		cvSetCaptureProperty( _capture, CV_CAP_PROP_FRAME_HEIGHT, (double)_height );
		cvSetCaptureProperty( _capture, CV_CAP_PROP_FPS, (double)_frameRate );

//		PrintMessage("SUCCESS : OpenCvUsbCamera::Initialize() -> OpenCvUsbCamera initialized.\n");
		return API_SUCCESS;
	}
	return API_SUCCESS;
}

int OpenCvUsbCamera::Enable(void)
{
	return API_SUCCESS;
}

int OpenCvUsbCamera::Disable(void)
{
	return API_SUCCESS;
}

int OpenCvUsbCamera::Finalize(void)
{
	if (_capture) {
		cvReleaseCapture( &_capture );
		_capture = 0;
		PrintMessage("SUCCESS : OpenCvUsbCamera::Disable()\n");
	}
	return API_SUCCESS;
}

int OpenCvUsbCamera::SetParameter(Property parameter)
{
	if (!parameter.FindName("CameraID") || 
		!parameter.FindName("Width") || 
		!parameter.FindName("Height") ||
		!parameter.FindName("PixelBytes") ||
		!parameter.FindName("FrameRate") ||
		!parameter.FindName("Flip") )
	{
		if( !parameter.FindName("CameraID") )
			PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter ID.\n");
		if( !parameter.FindName("Width") )
			PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter Width.\n");
		if( !parameter.FindName("Height") )
			PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter Height.\n");
		if( !parameter.FindName("PixelBytes") )
			PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter PixelBytes.\n");
		if( !parameter.FindName("FrameRate") )
			PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter FrameRate.\n");
		if( !parameter.FindName("Flip") )
			PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter Flip.\n");

		PrintMessage("Error:OpenCvUsbCamera::SetParameter()->Can't find parameter(s) name.\n");
		return API_ERROR;
	}

	_id = atoi( (parameter.GetValue("CameraID")).c_str() );
	_width = atoi( (parameter.GetValue("Width")).c_str() );
	_height = atoi( (parameter.GetValue("Height")).c_str() );
	_pixelByte = atoi( (parameter.GetValue("PixelBytes")).c_str() );
	_frameRate = atoi( (parameter.GetValue("FrameRate")).c_str() );
	_flip = atoi( (parameter.GetValue("Flip")).c_str() );

	this->parameter = parameter;

	return API_SUCCESS;
}

int OpenCvUsbCamera::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int OpenCvUsbCamera::GetImage(unsigned char *image)
{
//	PrintMessage("INIFO:OpenCvUsbCamera::GetImage() Start.\n");
	if(_capture) {
		_cvImage = cvQueryFrame(_capture);
		if(_cvImage) {

			if( _flip  == 1 ) {
				cvFlip(_cvImage, NULL, 0);	// x-axis
			}
			else if( _flip == 2 ) {
				cvFlip(_cvImage, NULL, 1);	// y-axis
			}
			else if( _flip == 3 ) {
				cvFlip(_cvImage, NULL, -1);	// xy-axis
			}

			if( _cvImage->imageSize != _width*_height*_pixelByte ) {
				IplImage *resizeImage = cvCreateImage( cvSize(_width, _height), 8, _pixelByte );
				cvResize(_cvImage, resizeImage );
				memcpy(image, resizeImage->imageData, _width*_height*_pixelByte);
				cvReleaseImage( &resizeImage );
			} else {
				memcpy(image, _cvImage->imageData, _width*_height*_pixelByte);
			}

#if(DISPLAY_POPUP==1)
			cvShowImage("OpenCvUsbCamera", _cvImage);
#endif
//			cvSaveImage("OpenCvUsbCamera.jpg", _cvImage);

//			PrintMessage("SUCCESS:OpenCvUsbCamera::GetImage() End.\n");
			return API_SUCCESS;
		}
		else {
			PrintMessage("ERROR:OpenCvUsbCamera::GetImage() -> Can't capture camera.\n");
			return API_ERROR;
		}
		//cvReleaseImage(&_cvImage);
	}
	else {
		PrintMessage("ERROR:OpenCvUsbCamera::GetImage() -> There is no camera.\n");
		return API_ERROR;
	}
}

#if defined(WIN32)
extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
__declspec(dllexport) void ReleaseAPI(OprosApi *api);
}

OprosApi *GetAPI()
{
	return new OpenCvUsbCamera();
}

void ReleaseAPI(OprosApi *api)
{
	delete api;
}
#endif
