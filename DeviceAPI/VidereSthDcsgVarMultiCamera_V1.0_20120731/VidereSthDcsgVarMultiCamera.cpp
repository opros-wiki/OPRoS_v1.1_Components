#if defined(WIN32)
#pragma warning(disable : 4996)
#endif


#include "VidereSthDcsgVarMultiCamera.h"
#include "OprosPrintMessage.h"
#include "svs.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"


VidereSthDcsgVarMultiCamera::VidereSthDcsgVarMultiCamera(void)
: _id(0), _width(0), _height(0), _pixelByte(0), _flip(0), _isEnabledCamera(false), videoObject(NULL),processObject(0),si(0)
{

}

VidereSthDcsgVarMultiCamera::~VidereSthDcsgVarMultiCamera(void)
{
	videoObject->Close();

}

int VidereSthDcsgVarMultiCamera::Initialize(Property parameter)
{
	if( SetParameter(parameter) == API_ERROR ) {
		return API_ERROR;
	}
	return API_SUCCESS;
}

int VidereSthDcsgVarMultiCamera::Enable(void)
{
	videoObject = getVideoObject();

	bool ret;
	ret = videoObject->Open();
	if (!ret)
	{
		PrintMessage("ERROR: VidereSthDcsgVarMultiCamera::Enable() ->Can't open stereo device\n");
		return API_ERROR;
	}

	// We can set the frame size and sampling modes here
	// Each device has a default mode
	// Always set parameters *after* opening the device
	//  videoObject->SetSample(0,1);	// decimation and binning
	//  videoObject->SetFrameDiv(1);	// Full frame
	//PrintMessage("INFO: VidereSthDcsgVarMultiCamera::Enable() -> videoObject->SetSize(%d, %d)\n",_width[0],_height[0]);
	videoObject->SetSize(640, 480); // 320x240 image

	// Set up to get color, if we can, from the device
	videoObject->SetColor(true, true); // both left and right

	videoObject->SetFormat(svsRGB24);  // MONOCHROME, YUV, or RGB24
	videoObject->SetCapture(CAP_DUAL);

	// start
	ret = videoObject->Start();
	if (!ret)
    {
		PrintMessage("ERROR: VidereSthDcsgVarMultiCamera::Enable() ->Can't start continuous capture\n");
		return API_ERROR;
    }

	//PrintMessage("SUCCESS : VidereSthDcsgVarMultiCamera::Enable() -> VidereSthDcsgVarMultiCamera enabled. ->image size(%d,%d)\n",_width[0],_height[0]);

	_isEnabledCamera = true;

	return API_SUCCESS;
}

int VidereSthDcsgVarMultiCamera::Disable(void)
{
	_isEnabledCamera = false;
	
	int ret = videoObject->Stop();

	if (!ret)
    {
		PrintMessage("ERROR: VidereSthDcsgVarMultiCamera::Disable() ->Can't stop\n");
		return API_ERROR;
    }

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

	}

	//PrintMessage("SUCCESS : VidereSthDcsgVarMultiCamera::Disable()\n");

	return API_SUCCESS;
}

int VidereSthDcsgVarMultiCamera::Finalize(void)
{
	return API_SUCCESS;
}

int VidereSthDcsgVarMultiCamera::SetParameter(Property parameter)
{
	if (!parameter.FindName("Count") ) {
		PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Can't find parameter(s) name.\n");
		return API_ERROR;
	}
	_nCameraCount = atoi( (parameter.GetValue("Count")).c_str() );

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

		//	CalibratedParameter parameter를 확인하여 없으면 에러 리턴
		if(parameter.FindName("CalibrationFilename") == false) {
			PrintMessage("WARNING : VidereSthDcsgVarMultiCamera::SetParameter() -> Can't find the Calibration file in property\n");
			//return API_ERROR;
			_calibrationFilename = parameter.GetValue("APIName");	// apiname으로 대체
		}
		else _calibrationFilename = parameter.GetValue("CalibrationFilename");

		char str[256];
		for( int i = 0  ;  i < _nCameraCount  ;  i++ ) {
			sprintf(str, "CameraID%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_id[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "Width%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_width[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "Height%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_height[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "PixelBytes%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_pixelByte[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			sprintf(str, "Flip%d", i);
			if( !parameter.FindName(str) ) {
				PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Can't find parameter(s) name %s\n", str);
				return API_ERROR;
			}
			_flip[i] = atoi( (parameter.GetValue(string(str))).c_str() );

			// check image size
			if(i>0)
			{
				if(_width[0]!=_width[i] || _height[0]!=_height[i] || _pixelByte[0]!=_pixelByte[i])
				{
					PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter() -> Each camera must have same parameters(width,height,pixelbytes)!\n");
					return API_ERROR;
				}
			}

		}

		PrintMessage("SUCCESS: VidereSthDcsgVarMultiCamera::SetParameter() OK!\n");
		PrintMessage("Count:%d, calibrationFilename:%s\n", _nCameraCount, _calibrationFilename.c_str() );
		for(int i=0; i<_nCameraCount;i++) 
			PrintMessage("CameraID:%d, width:%d, height:%d, pixelbytes:%d, flip:%d\n", _id[i],_width[i],_height[i],_pixelByte[i],_flip[i]);


		this->parameter = parameter;


		return API_SUCCESS;
	}
	else {
		PrintMessage("Error:VidereSthDcsgVarMultiCamera::SetParameter()->Camera count is 0.\n");
		return API_ERROR;
	}
}

int VidereSthDcsgVarMultiCamera::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int VidereSthDcsgVarMultiCamera::GetImage(int index, unsigned char *image)
{
	int i,j;

	if( 0 <= index && index < _nCameraCount ) 
	{
		if( _isEnabledCamera ) 
		{
			// index==0일때 왼쪽/오른쪽 영상을 동시에 얻는다. 
			// 따라서 동기화를 위해 index==1인 경우는 다시 카메라로부터 영상을 구하지 않고 
			// 기존 영상을 사용한다.
			if(index==0)
			{
				si = videoObject->GetImage(500); // 500 ms timeout
				if (si == NULL) 
				{
					PrintMessage("ERROR:VidereSthDcsgVarMultiCamera::GetImage() -> Fail to get an image from camera.\n");
					return API_ERROR;
				}
			}
	
			//PrintMessage("INFO: VidereSthDcsgVarMultiCamera::GetImage() -> width:%d, height:%d, pixelbytes:%d\n", _width[index],_height[index], _pixelByte[index]);
			//IplImage* cvImage = cvCreateImage(cvSize(_width[index],_height[index]),8, _pixelByte[index]);
			// videre 카메라 크기 고정
			IplImage* cvImage = cvCreateImage(cvSize(640,480),8, 3);

			if(cvImage) 
			{
				// Videre Cam의 영상 중 index에 따라 왼쪽/오른쪽 하나의 영상으로 만든다.
				unsigned char* pdata = (index==0)?si->color:si->color_right;
				for(i=0;i<_height[index];i++) {
					for(j=0;j<_width[index];j++) {
						cvImage->imageData[(i*_width[index]+j)*3]	= pdata[(i*_width[index]+j)*4+2 ];
						cvImage->imageData[(i*_width[index]+j)*3+1]	= pdata[(i*_width[index]+j)*4+1 ];
						cvImage->imageData[(i*_width[index]+j)*3+2]	= pdata[(i*_width[index]+j)*4+0 ];
					}
				}

				//PrintMessage("INFO: VidereSthDcsgVarMultiCamera::GetImage() -> GetImage() successed!");
				//char str[256];
				//sprintf(str, "VidereSthDcsgVarMultiCamera%d.jpg", index);
				//cvSaveImage(str, cvImage );
				
				// flip 변환
				if( _flip[index]  == 1 ) {
					cvFlip(cvImage, NULL, 0);	// x-axis
				}
				else if( _flip[index] == 2 ) {
					cvFlip(cvImage, NULL, 1);	// y-axis
				}
				else if( _flip[index] == 3 ) {
					cvFlip(cvImage, NULL, -1);	// xy-axis
				}

				// Videre Cam의 캡처 영상 크기를 구하는 영상크기로 변환
				if( cvImage->imageSize != _width[index]*_height[index]*_pixelByte[index] ) {
					IplImage *resizeImage = cvCreateImage( cvSize(_width[index], _height[index]), 8, _pixelByte[index] );
					cvResize(cvImage, resizeImage );
					memcpy(image, resizeImage->imageData, _width[index]*_height[index]*_pixelByte[index]);
					//test
					//PrintMessage("INFO: VidereSthDcsgVarMultiCamera::GetImage() -> resize w%d, h%d\n", resizeImage->width, resizeImage->height);
					cvReleaseImage( &resizeImage );
				} else {
					//PrintMessage("INFO: VidereSthDcsgVarMultiCamera::GetImage() -> size w%d, h%d\n", cvImage->width, cvImage->height);
					memcpy(image, cvImage->imageData, _width[index]*_height[index]*_pixelByte[index]);
				}

				//memcpy(image,cvImage->imageData,_width[index]*_height[index]*_pixelByte[index]);

				//cvReleaseImageHeader(&cvImage);
				cvReleaseImage(&cvImage);
				
				return API_SUCCESS;
			}
			else 
			{
				PrintMessage("ERROR:VidereSthDcsgVarMultiCamera::GetImage() -> Can't capture camera.\n");
				return API_ERROR;
			}

			return API_SUCCESS;
		}
		//else 
		//{
		//	PrintMessage("ERROR:VidereSthDcsgVarMultiCamera::GetImage() -> It is disabled Camera.\n");
		//	return API_ERROR;
		//}
	}
	else {
		PrintMessage("ERROR:VidereSthDcsgVarMultiCamera::GetImage() -> Camera Index is not valid.(index/CameraCount) = (%d/%d)\n", index, _nCameraCount);
		return API_ERROR;
	}
	return API_ERROR;
}

extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
__declspec(dllexport) void ReleaseAPI(OprosApi *api);
}

OprosApi *GetAPI()
{
	return new VidereSthDcsgVarMultiCamera();
}

void ReleaseAPI(OprosApi *api)
{
	delete api;
}
