#include "PointGreyFlyCaptureCamera.h"
#include "OprosPrintMessage.h"

#if defined(WIN32)
#pragma warning(disable : 4996)
#endif

#include "pgrflycapture.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"

class FlyCapture {
public:
	FlyCaptureContext pContext;
	FlyCaptureImage pImage;
	FlyCaptureImage pImageBGR;
};

PointGreyFlyCaptureCamera::PointGreyFlyCaptureCamera(void)
: _id(0), _width(320), _height(240), _pixelByte(3), _flip(0), _initialize(false)
{
}

PointGreyFlyCaptureCamera::~PointGreyFlyCaptureCamera(void)
{
}

int PointGreyFlyCaptureCamera::Initialize(Property parameter)
{
	if( SetParameter(parameter) == API_ERROR ) {
		return API_ERROR;
	}
	return API_SUCCESS;
}

int PointGreyFlyCaptureCamera::Enable(void)
{
	_flyCapture = new FlyCapture();

	FlyCaptureError error;
	error = flycaptureCreateContext(&_flyCapture->pContext);
	if( error != FLYCAPTURE_OK ) {
		return API_ERROR;
	}
	error = flycaptureInitialize(_flyCapture->pContext, 0);
	if( error != FLYCAPTURE_OK ) {
		return API_ERROR;
	}
	error = flycaptureStart(_flyCapture->pContext,FLYCAPTURE_VIDEOMODE_ANY,FLYCAPTURE_FRAMERATE_ANY); 
	if( error != FLYCAPTURE_OK ) {
		return API_ERROR;
	}
	//error=flycaptureStartCustomImage(_flyCapture->pContext,3,0,0,width,height,100.0f,FLYCAPTURE_RAW16);
	
	_flyCapture->pImageBGR.pixelFormat = FLYCAPTURE_BGR;
	_flyCapture->pImageBGR.pData = new unsigned char[2248*2048*3];

	if( error == FLYCAPTURE_OK ) {
		_initialize = true;
	}
	return API_SUCCESS;
}

int PointGreyFlyCaptureCamera::Disable(void)
{
	_initialize = false;

	flycaptureStop(_flyCapture->pContext);
	flycaptureDestroyContext(_flyCapture->pContext);

	delete[] _flyCapture->pImageBGR.pData;
	delete _flyCapture;

	return API_SUCCESS;
}

int PointGreyFlyCaptureCamera::Finalize(void)
{
	return API_SUCCESS;
}

int PointGreyFlyCaptureCamera::SetParameter(Property parameter)
{
	if (!parameter.FindName("CameraID") || 
		!parameter.FindName("Width") || 
		!parameter.FindName("Height") ||
		!parameter.FindName("PixelBytes") ||
		!parameter.FindName("Flip") )
	{
		PrintMessage("ERROR : PointGreyFlyCaptureCamera::SetParameter() -> Can't find parameter name.\n");
		return API_ERROR;
	}

	_id = atoi( (parameter.GetValue("CameraID")).c_str() );
	_width = atoi( (parameter.GetValue("Width")).c_str() );
	_height = atoi( (parameter.GetValue("Height")).c_str() );
	_pixelByte = atoi( (parameter.GetValue("PixelBytes")).c_str() );
	_flip = atoi( (parameter.GetValue("Flip")).c_str() );

	this->parameter = parameter;

	return API_SUCCESS;
}

int PointGreyFlyCaptureCamera::GetParameter(Property &parameter)
{
	//if (!parameter.FindName("ID") || 
	//	!parameter.FindName("Width") || 
	//	!parameter.FindName("Height") ||
	//	!parameter.FindName("PixelBytes") ||
	//	!parameter.FindName("Flip") )
	//{
	//	PrintMessage("ERROR : PointGreyFlyCaptureCamera::GetParameter() -> Can't find parameter name.\n");
	//	return API_ERROR;
	//}

	//std::string str;
	//str = _id;
	//parameter.SetValue("ID", str);
	//str = _width;
	//parameter.SetValue("Width", str);
	//str = _height;
	//parameter.SetValue("Height", str);
	//str = _pixelByte;
	//parameter.SetValue("PixelBytes", str);
	//str = _flip;
	//parameter.SetValue("Flip", str);
	parameter = this->parameter;

	return API_SUCCESS;
}

int PointGreyFlyCaptureCamera::GetImage(unsigned char *image)
{
	if( _initialize ) {
		flycaptureGrabImage2(_flyCapture->pContext,&_flyCapture->pImage);
		flycaptureConvertImage(_flyCapture->pContext,&_flyCapture->pImage,&_flyCapture->pImageBGR);

		IplImage* cvImage = cvCreateImageHeader(cvSize(_flyCapture->pImageBGR.iCols,_flyCapture->pImageBGR.iRows),8,3);
		cvImage->imageData = (char *)_flyCapture->pImageBGR.pData;

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
		}
		else {
			memcpy(image, cvImage->imageData, _width*_height*_pixelByte);
		}

		cvReleaseImageHeader(&cvImage);

		return API_SUCCESS;
	}
	else {
		return API_ERROR;
	}
}

extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
__declspec(dllexport) void ReleaseAPI(OprosApi *api);
}

OprosApi *GetAPI()
{
	return new PointGreyFlyCaptureCamera();
}

void ReleaseAPI(OprosApi *api)
{
	delete api;
}
