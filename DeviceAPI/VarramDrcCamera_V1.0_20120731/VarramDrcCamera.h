#pragma once

#include "Camera.h"

/**
 *	VarramDrcCamera
 *	Varram���� ������ DRC ī�޶� ��ġ�� �����մϴ�.
 */
class VarramDrcCamera : public Camera
{
public:
	VarramDrcCamera(void);
	virtual ~VarramDrcCamera(void);

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
	virtual int GetImage(unsigned char *image);

private:
	int _id;
	int _width;
	int _height;
	int _pixelByte;
	int	_flip;

	unsigned char	_robotdata[64];

	Property parameter;
};
