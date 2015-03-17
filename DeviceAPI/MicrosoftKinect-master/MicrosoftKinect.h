#ifndef __MICROSOFT_KINECT_H__
#define __MICROSOFT_KINECT_H__

#include <windows.h>
#include <NuiApi.h>
#include <device/OprosLock.h>

#include "Kinect.h"

class MicrosoftKinect : public Kinect
{
public:
	MicrosoftKinect();
	virtual ~MicrosoftKinect();

public:
	int32_t Initialize(OPRoS::Property parameter);
	int32_t Finalize(void);
	int32_t Enable(void);
	int32_t Disable(void);
	int32_t SetProperty(OPRoS::Property parameter);
	int32_t GetProperty(OPRoS::Property &parameter);

public:
	int32_t SetCameraAngle(float degree);
	int32_t GetCameraAngle(float& degree);
	int32_t GetSkeleton(std::vector<Skeleton>& output);
	int32_t GetSkeleton(Skeleton& output, uint32_t id);
	int32_t GetSkeletonID(std::vector<uint32_t>& output);
	int32_t GetImage(ImageFrame& image);
	int32_t GetDepthImage(DepthFrame& depth);

private:
	bool SetKinectProfile(OPRoS::Property& property);
	void TransformSkeletonToDepthImage(Position& result, const Vector4& skeletonPosition);
	bool GetSKeletonNextFrame();
	void InitDepthConverter();

	static void ConvertNuiDepthToDepth(std::vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount);
	static void ConvertNuiDepthToDepthUsingSSE(std::vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount);

private:
	void (*mDepthConverter)(std::vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount);

	OprosLock mLockSkeletonList;
	std::vector<Skeleton> mSkeletonList;

	INuiSensor* mpNuiSensor;

	HANDLE mHandleDepthStream;
	HANDLE mHandleColorStream;

	NUI_IMAGE_RESOLUTION mNuiColorResoultion;
	NUI_IMAGE_RESOLUTION mNuiDepthResoultion;
	bool mIsNearMode;
	bool mIsSkeletonTrackingMode;

	int mNuiIndex;

	OprosLock mLockTempImageFrame;
	ImageFrame mTempImageFrame;

	OprosLock mLockTempDepthFrame;
	DepthFrame mTempDepthFrame;
};

#endif