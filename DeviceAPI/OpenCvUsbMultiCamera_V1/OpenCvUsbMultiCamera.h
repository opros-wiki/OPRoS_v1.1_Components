#pragma once

#include "MultiCamera.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"

/**
 *	OpenCvUsbMultiCamera
 *	OpenCV ���̺귯������ �ν��� �� �ִ� �������� ī�޶� ��ġ�� �����մϴ�.
 *	ī�޶��� ����̹��� ��ġ�� �� ������ Ž���⿡�� ������ Ȯ���� �� �ִٸ�
 *	�� OpenCvUsbMultiCamera�� ���� ī�޶� ��ġ�� ������ �� �ֽ��ϴ�.
 */
class OpenCvUsbMultiCamera : public MultiCamera
{
public:
	OpenCvUsbMultiCamera(void);
	virtual ~OpenCvUsbMultiCamera(void);

public:
	/**
	* bool Initialize(Property parameter)
	* ī�޶� ��ġ�� �����ϸ� ȣ��˴ϴ�.
	* return - true : ī�޶� ��ġ ���� ����
	*		 - false : ī�޶� ��ġ ���� ����
	*/
	virtual int Initialize(Property parameter);

	/**
	* bool Finalize(void)
	* ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	*/
	virtual int Finalize(void);

	/**
	* bool Enable(void)
	* ī�޶� ��ġ�� Ȱ��ȭ �մϴ�.
	* return - true : ī�޶� Ȱ��ȭ ����
	*		 - false : ī�޶� Ȱ��ȭ ����
	*/
	virtual int Enable(void);

	/**
	* bool Disable(void)
	* ī�޶� ��ġ�� ��Ȱ��ȭ �մϴ�.
	* return - true : ī�޶� ��Ȱ��ȭ ����
	*		 - false : ī�޶� ��Ȱ��ȭ ����
	*/
	virtual int Disable(void);

	/**
	* bool SetParameter(Property parameter)
	* ī�޶��� parameter(ID, width, height, pixelByte, flip)�� �����մϴ�.
	* return - true : ī�޶� parameter ���� ����
	*		 - false : ī�޶� parameter ���� ����
	*/
	virtual int SetParameter(Property parameter);

	/**
	* int GetParameter(Property &parameter)
	* ī�޶�κ��� parameter(ID, width, height, pixelByte, flip) ������ �����ɴϴ�.
	* return - true : ī�޶� parameter ���� ȹ�� ����
	*		 - false : ī�޶� parameter ���� ȹ�� ����
	*/
	virtual int GetParameter(Property &parameter);

	/**
	* int GetImage(unsigned char *image)
	* ī�޶󿡼� ������ ĸ���Ͽ� ��� ������ image�� �����մϴ�.
	* image : ������ ������ �迭�� ������
	* return - true : ���� ĸ�� ����
	*		 - false : ���� ĸ�� ����
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
