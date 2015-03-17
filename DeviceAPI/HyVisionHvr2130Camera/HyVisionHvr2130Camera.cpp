#include "HyVisionHvr2130Camera.h"

#include "OprosPrintMessage.h"
#include "HVRLibEx.h"

#include "opencv/cv.h"
//#include "opencv/highgui.h"

HyVisionHvr2130Camera::HyVisionHvr2130Camera(void)
: _id(0), _width(320), _height(240), _pixelByte(3), _flip(0), _cameraWidth(640), _cameraHeight(480), _cameraBuf(0), _cameraBufBMP(0)
{
}

HyVisionHvr2130Camera::~HyVisionHvr2130Camera(void)
{
}

int HyVisionHvr2130Camera::Initialize(Property parameter)
{
	if( SetParameter(parameter) == API_ERROR ) {
		return API_ERROR;
	}
	PrintMessage("SUCCESS : HyVisionHvr2130Camera::Initialize()\n");
	return API_SUCCESS;
}

int HyVisionHvr2130Camera::Enable(void)
{
	if(! cameraInit(_cameraWidth, _cameraHeight) ) {
		PrintMessage("ERROR : HyVisionHvr2130Camera::Enable() -> Can't init camera");
		return API_ERROR;
	}

	if( !_cameraBuf ) {
		_cameraBuf = new unsigned char[_cameraWidth*_cameraHeight*2];
	}

	if( !_cameraBufBMP ) {
		_cameraBufBMP = new unsigned char[_cameraWidth*_cameraHeight*3];
	}

	PrintMessage("SUCCESS : HyVisionHvr2130Camera::Enable()\n");
	return API_SUCCESS;
}

int HyVisionHvr2130Camera::Disable(void)
{
	if( _cameraBufBMP ) {
		delete [] _cameraBufBMP;
		_cameraBufBMP = 0;
	}

	if( _cameraBuf ) {
		delete [] _cameraBuf;
		_cameraBuf = 0;
	}

	return API_SUCCESS;
}

int HyVisionHvr2130Camera::Finalize(void)
{
	return API_SUCCESS;
}

int HyVisionHvr2130Camera::SetParameter(Property parameter)
{
	if (!parameter.FindName("CameraID") || 
		!parameter.FindName("Width") || 
		!parameter.FindName("Height") ||
		!parameter.FindName("PixelBytes") ||
		!parameter.FindName("Flip") )
	{
		if( !parameter.FindName("CameraID") )
			PrintMessage("Error:HyVisionHvr2130Camera::SetParameter()->Can't find parameter CameraID.\n");
		if( !parameter.FindName("Width") )
			PrintMessage("Error:HyVisionHvr2130Camera::SetParameter()->Can't find parameter Width.\n");
		if( !parameter.FindName("Height") )
			PrintMessage("Error:HyVisionHvr2130Camera::SetParameter()->Can't find parameter Height.\n");
		if( !parameter.FindName("PixelBytes") )
			PrintMessage("Error:HyVisionHvr2130Camera::SetParameter()->Can't find parameter PixelBytes.\n");
		if( !parameter.FindName("Flip") )
			PrintMessage("Error:HyVisionHvr2130Camera::SetParameter()->Can't find parameter Flip.\n");

		PrintMessage("Error:HyVisionHvr2130Camera::SetParameter()->Can't find parameter(s) name.\n");
		return API_ERROR;
	}

	_id = atoi( (parameter.GetValue("CameraID")).c_str() );
	_width = atoi( (parameter.GetValue("Width")).c_str() );
	_height = atoi( (parameter.GetValue("Height")).c_str() );
	_pixelByte = atoi( (parameter.GetValue("PixelBytes")).c_str() );
	_flip = atoi( (parameter.GetValue("Flip")).c_str() );
	PrintMessage("SUCCESS:HyVisionHvr2130Camera::SetParameter()->CameraID = %d, Width = %d, Height = %d.\n", _id, _width, _height);

	this->parameter = parameter;

	return API_SUCCESS;
}

int HyVisionHvr2130Camera::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int HyVisionHvr2130Camera::GetImage(unsigned char *image)
{
	if(1 != HVR_camGetImageData(_cameraWidth*_cameraHeight*2, _cameraBuf, _id))
	{
		PrintMessage("ERROR : HyVisionHvr2130Camera::GetImage() -> Buffer fail...\r\n");
		return API_ERROR;
	}

	HVR_ispYCbCrToBmp(YCbYCr, _cameraWidth, _cameraHeight, _cameraBuf, _cameraBufBMP, HVR2130P);

	IplImage *cvImage = cvCreateImageHeader( cvSize(_cameraWidth, _cameraHeight), 8, 3 );
	cvImage->imageData = (char *)_cameraBufBMP;
	cvFlip(cvImage, NULL, 0);

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

	cvReleaseImageHeader( &cvImage );

	return API_SUCCESS;
}

bool HyVisionHvr2130Camera::cameraInit(int width, int height)
{
	int res = VGA;
	if( width == 1280 && height == 1024 ) {
		res = SXGA;
		_cameraWidth = 1280;
		_cameraHeight = 1024;
	}
	else if( width == 1024 && height == 768 ) {
		res = SXGA;
		_cameraWidth = 1280;
		_cameraHeight = 1024;
	}
	else {
		res = VGA;
		_cameraWidth = 640;
		_cameraHeight = 480;
	}

	int DrvInfo[MAX_DEVICE];
	int	ConnectNo;

	// 드라이버 확인 및 카메라 정보 얻어오기...
	ConnectNo = HVR_camDrvInit(&DrvInfo[0]);
	Sleep(800);

	PrintMessage("Camera connect number : %d \r\n", ConnectNo);

	if(ConnectNo==0) return false;
	else			 _id = 0;  // 1개 연결시, 0번째 카메라를 초기화함...

	// 카메라 초기화...
	if(!HVR_camSensorInit(HVR2130P, _id)) {
		PrintMessage("ERROR : Product No.을 확인하세요. (Product No. HVR2130P) \r\n");
		return false;
	}

	// 해상도 세팅...
	if(!HVR_ispResolutionCmd(res, HVR2130P, _id)) return false;
	
	return true;
}

extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
__declspec(dllexport) void ReleaseAPI(OprosApi *api);
}

OprosApi *GetAPI()
{
	return new HyVisionHvr2130Camera();
}

void ReleaseAPI(OprosApi *api)
{
	delete api;
}
