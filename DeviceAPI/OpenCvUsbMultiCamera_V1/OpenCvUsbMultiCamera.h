#pragma once

#include "MultiCamera.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"

/**
 *	OpenCvUsbMultiCamera
 *	OpenCV 라이브러리에서 인식할 수 있는 여러개의 카메라 장치를 제어합니다.
 *	카메라의 드라이버를 설치한 후 윈도우 탐색기에서 영상을 확인할 수 있다면
 *	이 OpenCvUsbMultiCamera를 통해 카메라 장치를 제어할 수 있습니다.
 */
class OpenCvUsbMultiCamera : public MultiCamera
{
public:
	OpenCvUsbMultiCamera(void);
	virtual ~OpenCvUsbMultiCamera(void);

public:
	/**
	* bool Initialize(Property parameter)
	* 카메라 장치를 생성하면 호출됩니다.
	* return - true : 카메라 장치 생성 성공
	*		 - false : 카메라 장치 생성 실패
	*/
	virtual int Initialize(Property parameter);

	/**
	* bool Finalize(void)
	* 종료할 때 호출되는 함수입니다.
	*/
	virtual int Finalize(void);

	/**
	* bool Enable(void)
	* 카메라 장치를 활성화 합니다.
	* return - true : 카메라 활성화 성공
	*		 - false : 카메라 활성화 실패
	*/
	virtual int Enable(void);

	/**
	* bool Disable(void)
	* 카메라 장치를 비활성화 합니다.
	* return - true : 카메라 비활성화 성공
	*		 - false : 카메라 비활성화 실패
	*/
	virtual int Disable(void);

	/**
	* bool SetParameter(Property parameter)
	* 카메라의 parameter(ID, width, height, pixelByte, flip)를 설정합니다.
	* return - true : 카메라 parameter 변경 성공
	*		 - false : 카메라 parameter 변경 실패
	*/
	virtual int SetParameter(Property parameter);

	/**
	* int GetParameter(Property &parameter)
	* 카메라로부터 parameter(ID, width, height, pixelByte, flip) 정보를 가져옵니다.
	* return - true : 카메라 parameter 정보 획득 성공
	*		 - false : 카메라 parameter 정보 획득 실패
	*/
	virtual int GetParameter(Property &parameter);

	/**
	* int GetImage(unsigned char *image)
	* 카메라에서 영상을 캡춰하여 결과 영상을 image에 저장합니다.
	* image : 영상을 저장할 배열의 포인터
	* return - true : 영상 캡춰 성공
	*		 - false : 영상 캡춰 실패
	*/
	virtual int GetImage(int index, unsigned char *image);

private:
	int _nCameraCount;
	int *_id;
	int *_width;
	int *_height;
	int *_pixelByte;
	int	*_flip;

	CvCapture **_capture;
};
