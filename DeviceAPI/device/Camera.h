#ifndef	_CAMERA_H_
#define	_CAMERA_H_

#include "OprosApi.h"

class Camera : public OprosApi
{
public:
	Camera(void) { };
	virtual ~Camera(void) { };

public:
	virtual int GetImage(unsigned char *image) { return API_NOT_SUPPORTED; };
	virtual int StartPushImage(void *callback) { return API_NOT_SUPPORTED; };
	virtual int EndPushImage(void) { return API_NOT_SUPPORTED; };
};


#endif	//	_CAMERA_H_