#ifndef __KINECT_H__
#define __KINECT_H__

#include <device/OprosDevice.h>

#include "KinectDataType.h"

class Kinect : public OprosDevice
{
public:
	Kinect() {}
	virtual ~Kinect(void) {}

public:
	virtual int32_t SetCameraAngle(float degree) { return API_NOT_SUPPORTED; }
	virtual int32_t GetCameraAngle(float& degree)  { return API_NOT_SUPPORTED; }

	virtual int32_t GetSkeleton(std::vector<Skeleton>& output) { return API_NOT_SUPPORTED; }
	virtual int32_t GetSkeleton(Skeleton& output, uint32_t id)  { return API_NOT_SUPPORTED; }
	virtual int32_t GetSkeletonID(std::vector<uint32_t>& output)  { return API_NOT_SUPPORTED; }

	virtual int32_t GetImage(ImageFrame& image)  { return API_NOT_SUPPORTED; }
	virtual int32_t GetDepthImage(DepthFrame& depth)  { return API_NOT_SUPPORTED; }
};

#endif