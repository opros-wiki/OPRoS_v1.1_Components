#ifndef	_MULTI_CAMERA_H_
#define	_MULTI_CAMERA_H_

#include "Camera.h"

class MultiCamera : public Camera
{
public:
	MultiCamera(void) { };
	virtual ~MultiCamera(void) { };

public:
	virtual int GetImage(int index, unsigned char *image) { return API_NOT_SUPPORTED; };
};

#endif	//	_MULTI_CAMERA_H_