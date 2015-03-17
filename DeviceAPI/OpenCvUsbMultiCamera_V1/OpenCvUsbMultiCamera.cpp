#include "OpenCvUsbMultiCamera.h"
#include "OprosPrintMessage.h"

#if defined(WIN32)
#pragma warning(disable : 4996)
#endif

OpenCvUsbMultiCamera::OpenCvUsbMultiCamera(void)
: _capture(0), _id(0), _width(0), _height(0), _pixelByte(0), _flip(0)
{
//	cvNamedWindow("OpenCvUsbMultiCamera");
}

OpenCvUsbMultiCamera::~OpenCvUsbMultiCamera(void)
{
//	cvDestroyWindow("OpenCvUsbMultiCamera");
}

int OpenCvUsbMultiCamera::Initialize(Property parameter)
{
	if( SetParameter(parameter) == API_ERROR ) {
		return API_ERROR;
	}
	if( 0 < _nCameraCount ) {
		if( _capture ) {
			free(_capture); _capture = 0;
		}

		_capture = (CvCapture **)malloc(_nCameraCount*sizeof(CvCapture *));

		for( int i = 0  ;  i < _nCameraCount  ;  i++ ) {
			_capture[i] = cvCaptureFromCAM(_id[i]);
			if(!_capture[i]) {
				PrintMessage("ERROR : OpenCvUsbMultiCamera::Initialize() -> Can't init camera device. %d\n", i);
				return API_ERROR;
			}

			cvSetCaptureProperty( _capture[i], CV_CAP_PROP_FRAME_WIDTH, (double)_width[i] );
			cvSetCaptureProperty( _capture[i], CV_CAP_PROP_FRAME_HEIGHT, (double)_height[i] );
		}

		PrintMessage("SUCCESS : OpenCvUsbMultiCamera::Initialize() -> OpenCvUsbMultiCamera initialized.\n");
		return API_SUCCESS;
	}
	else {
		return API_ERROR;
	}
}

int OpenCvUsbMultiCamera::Enable(void)
{
	return API_SUCCESS;
}

int OpenCvUsbMultiCamera::Disable(void)
{
	return API_SUCCESS;
}

int OpenCvUsbMultiCamera::Finalize(void)
{
	if( 0 < _nCameraCount ) {
		if( _id ) {
			free(_id);			_id = 0;
		}
		if( _width ) {
			free(_width);		_width = 0;
		}
		if( _height ) {
			free(_height);		_height = 0;
		}
		if( _pixelByte ) {
			free(_pixelByte);	_pixelByte = 0;
		}
		if( _flip ) {
			free(_flip);		_flip = 0;
		}

		for( int i = 0  ;  i < _nCameraCount  ;  i++ ) {
			if (_capture[i]) {
				cvReleaseCapture( &_capture[i] );
				_capture[i] = 0;
			}
		}
		if( _capture ) {
			free(_capture); _capture = 0;
		}
	}
	PrintMessage("SUCCESS : OpenCvUsbMultiCamera::Finalize()\n");
	return API_SUCCESS;
}

int OpenCvUsbMultiCamera::SetParameter(Property parameter)
{
	if (!parameter.FindName("Count") ) {
		PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Can't find parameter(s) name.\n");
		return API_ERROR;
	}

	_nCameraCount = atoi( (parameter.GetValue("Count")).c_str() );
	PrintMessage("INFO:OpenCvUsbMultiCamera::SetParameter()->parameter Count is %d.\n", _nCameraCount);
	if( 0 < _nCameraCount ) {
		if( _id ) {
			free(_id);			_id = 0;
		}
		if( _width ) {
			free(_width);		_width = 0;
		}
		if( _height ) {
			free(_height);		_height = 0;
		}
		if( _pixelByte ) {
			free(_pixelByte);	_pixelByte = 0;
		}
		if( _flip ) {
			free(_flip);		_flip = 0;
		}

		_id = (int *)malloc(_nCameraCount*sizeof(int));
		_width = (int *)malloc(_nCameraCount*sizeof(int));
		_height = (int *)malloc(_nCameraCount*sizeof(int));
		_pixelByte = (int *)malloc(_nCameraCount*sizeof(int));
		_flip = (int *)malloc(_nCameraCount*sizeof(int));

		char str[256];
		for( int i = 0  ;  i < _nCameraCount  ;  i++ ) {
			sprintf(str, "CameraID%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_id[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "Width%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_width[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "Height%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_height[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "PixelBytes%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_pixelByte[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "Flip%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_flip[i] = atoi( (parameter.GetValue(string(str))).c_str() );
		}

		this->parameter = parameter;

		return API_SUCCESS;
	}
	else {
		PrintMessage("Error:OpenCvUsbMultiCamera::SetParameter()->Camera count is 0.\n");
		return API_ERROR;
	}
}

int OpenCvUsbMultiCamera::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int OpenCvUsbMultiCamera::GetImage(int index, unsigned char *image)
{
	if( 0 <= index && index < _nCameraCount ) {
		if(_capture[index]) {
			IplImage *cvImage = cvQueryFrame(_capture[index]);
			if(cvImage) {

				if( _flip[index]  == 1 ) {
					cvFlip(cvImage, NULL, 0);	// x-axis
				}
				else if( _flip[index] == 2 ) {
					cvFlip(cvImage, NULL, 1);	// y-axis
				}
				else if( _flip[index] == 3 ) {
					cvFlip(cvImage, NULL, -1);	// xy-axis
				}

				if( cvImage->imageSize != _width[index]*_height[index]*_pixelByte[index] ) {
					IplImage *resizeImage = cvCreateImage( cvSize(_width[index], _height[index]), 8, _pixelByte[index] );
					cvResize(cvImage, resizeImage );
					memcpy(image, resizeImage->imageData, _width[index]*_height[index]*_pixelByte[index]);
					cvReleaseImage( &resizeImage );
				} else {
					memcpy(image, cvImage->imageData, _width[index]*_height[index]*_pixelByte[index]);
				}

				char str[256];
				sprintf(str, "OpenCvUsbMultiCamera%d.jpg", index);
				cvSaveImage(str, cvImage);

				return API_SUCCESS;
			}
			else {
				PrintMessage("ERROR:OpenCvUsbMultiCamera::GetImage() -> Can't capture camera.\n");
				return API_ERROR;
			}
			//cvReleaseImage(&_cvImage);
		}
		return API_SUCCESS;
	}
	else {
		PrintMessage("ERROR:OpenCvUsbMultiCamera::GetImage() -> Camera Index is not valid.(index/CameraCount) = (%d/%d)\n", index, _nCameraCount);
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
	return new OpenCvUsbMultiCamera();
}

void ReleaseAPI(OprosApi *api)
{
	delete api;
}
#endif
