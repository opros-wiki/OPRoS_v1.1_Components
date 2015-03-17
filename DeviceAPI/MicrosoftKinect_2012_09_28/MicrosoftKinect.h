#ifndef _MICRO_SOFT_KINECT_SDK_H_

#include <windows.h>
#include <NuiApi.h>
#include <OprosLock.h>

#include "Kinect.h"

class MicrosoftKinect : public Kinect
{
public:
	MicrosoftKinect();
	virtual ~MicrosoftKinect();

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

public:
	int SetCameraAngle(float degree); //키넥트 센서 상하 각도 조절 함수
	int GetCameraAngle(float& degree); //키넥트 센서 상하 각도 위치 리턴
	int GetSkeleton(vector<Skeleton>& output); //전체 골격 리턴
	int GetSkeleton(Skeleton& output, unsigned long id); //스켈레톤 ID로 특정 Skeleton 호출
    int GetSkeletonID(vector<unsigned long>& output); //스켈레톤 ID 반환
    int GetImage(ImageFrame& image); //컬러 이미지 반환
    int GetDepthImage(DepthFrame& depth); //뎁스 프레임 반환
	
private:
	bool SetKinectProfile(Property& property);
    void TransformSkeletonToDepthImage(Position& result, const Vector4& skeletonPosition);
    bool GetSKeletonNextFrame();
	void ConvertNuiDepthToDepth(vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount);
	void ConvertNuiDepthToDepthUsingSSE(vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount);
	void InitDepthConverter();

private:
	void (MicrosoftKinect::*depthConverter)(vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount);

	OprosLock mLockSkeletonList;
    vector<Skeleton> mSkeletonList;
	
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