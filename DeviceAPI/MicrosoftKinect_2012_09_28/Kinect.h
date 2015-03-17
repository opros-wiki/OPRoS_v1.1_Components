#ifndef _KINECT_H_
#define  _KINECT_H_

#include <Device.h>

#include "KinectDataType.h"

class Kinect : public Device
{
public:
	Kinect() {}
	virtual ~Kinect(void) {}
 
public:
	virtual int SetCameraAngle(float degree) { return API_NOT_SUPPORTED; }
	virtual int GetCameraAngle(float& degree)  { return API_NOT_SUPPORTED; }
	
	virtual int GetSkeleton(vector<Skeleton>& output) { return API_NOT_SUPPORTED; }
	virtual int GetSkeleton(Skeleton& output, unsigned long id)  { return API_NOT_SUPPORTED; }
	virtual int GetSkeletonID(vector<unsigned long>& output)  { return API_NOT_SUPPORTED; }

	virtual int GetImage(ImageFrame& image)  { return API_NOT_SUPPORTED; }
	virtual int GetDepthImage(DepthFrame& depth)  { return API_NOT_SUPPORTED; }
};

#endif



