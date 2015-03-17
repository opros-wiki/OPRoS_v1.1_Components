#include "MatroxImagingMilFrameGrabber.h"
#include "OprosPrintMessage.h"

#include "Mil.h"

MatroxImagingMilFrameGrabber::MatroxImagingMilFrameGrabber(void)
: _id(0), _width(320), _height(240), _pixelByte(4), _flip(0), _imageBuf(0)
{
}

MatroxImagingMilFrameGrabber::~MatroxImagingMilFrameGrabber(void)
{
}

int MatroxImagingMilFrameGrabber::Initialize(Property parameter)
{
	SetParameter(parameter);
	return true;
}

int MatroxImagingMilFrameGrabber::Enable(void)
{
   /* Allocate defaults. */
   MappAllocDefault(M_SETUP, &MilApplication, &MilSystem,
                             NULL, &MilDigitizer, &MilImage);

	return true;
}

int MatroxImagingMilFrameGrabber::Disable(void)
{
	MappFreeDefault(MilApplication, MilSystem, NULL, MilDigitizer, MilImage);

	return true;
}

int MatroxImagingMilFrameGrabber::Finalize(void)
{
	return true;
}

int MatroxImagingMilFrameGrabber::SetParameter(Property parameter)
{
	if (!parameter.FindName("CameraID") || 
		!parameter.FindName("Width") || 
		!parameter.FindName("Height") ||
		!parameter.FindName("PixelBytes") ||
		!parameter.FindName("Flip") )
	{
		if( !parameter.FindName("CameraID") )
			PrintMessage("Error:MatroxImagingMilFrameGrabber::SetParameter()->Can't find parameter ID.\n");
		if( !parameter.FindName("Width") )
			PrintMessage("Error:MatroxImagingMilFrameGrabber::SetParameter()->Can't find parameter Width.\n");
		if( !parameter.FindName("Height") )
			PrintMessage("Error:MatroxImagingMilFrameGrabber::SetParameter()->Can't find parameter Height.\n");
		if( !parameter.FindName("PixelBytes") )
			PrintMessage("Error:MatroxImagingMilFrameGrabber::SetParameter()->Can't find parameter PixelBytes.\n");
		if( !parameter.FindName("Flip") )
			PrintMessage("Error:MatroxImagingMilFrameGrabber::SetParameter()->Can't find parameter Flip.\n");

		PrintMessage("Error:MatroxImagingMilFrameGrabber::SetParameter()->Can't find parameter(s) name.\n");
		return API_ERROR;
	}

	_id = atoi( (parameter.GetValue("CameraID")).c_str() );
	_width = atoi( (parameter.GetValue("Width")).c_str() );
	_height = atoi( (parameter.GetValue("Height")).c_str() );
	_pixelByte = atoi( (parameter.GetValue("PixelBytes")).c_str() );
	_flip = atoi( (parameter.GetValue("Flip")).c_str() );

	this->parameter = parameter;

	return true;
}

int MatroxImagingMilFrameGrabber::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int MatroxImagingMilFrameGrabber::GetImage(unsigned char *image)
{
	MdigGrab(MilDigitizer, MilImage);

	IplImage *cvImage = cvCreateImageHeader( cvSize(640, 480), 8, _pixelByte );
	MbufInquire(MilImage, M_HOST_ADDRESS, &cvImage->imageData);

	if(cvImage) {
		if( _flip  == 1 ) {
			cvFlip(cvImage, NULL, 0);	// x-axis
		}
		else if( _flip == 2 ) {
			cvFlip(cvImage, NULL, 1);	// y-axis
		}
		else if( _flip == 3 ) {
			cvFlip(cvImage, NULL, -1);	// xy-axis
		}

		if( cvImage->imageSize != _width*_height*_pixelByte ) {
			IplImage *resizeImage = cvCreateImage( cvSize(_width, _height), 8, _pixelByte );
			cvResize(cvImage, resizeImage );
			memcpy(image, resizeImage->imageData, _width*_height*_pixelByte);
			cvReleaseImage( &resizeImage );
		} else {
			memcpy(image, cvImage->imageData, _width*_height*_pixelByte);
		}

		//cvSaveImage("MatroxImagingMilFrameGrabber.jpg", cvImage);
		cvReleaseImageHeader( &cvImage );
		return true;
	}
	else {
		return false;
	}
}

extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
__declspec(dllexport) void ReleaseAPI(OprosApi *api);
}

OprosApi *GetAPI()
{
	return new MatroxImagingMilFrameGrabber();
}

void ReleaseAPI(OprosApi *api)
{
	delete api;
}
