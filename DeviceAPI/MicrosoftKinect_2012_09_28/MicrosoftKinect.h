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
	int SetCameraAngle(float degree); //Ű��Ʈ ���� ���� ���� ���� �Լ�
	int GetCameraAngle(float& degree); //Ű��Ʈ ���� ���� ���� ��ġ ����
	int GetSkeleton(vector<Skeleton>& output); //��ü ��� ����
	int GetSkeleton(Skeleton& output, unsigned long id); //���̷��� ID�� Ư�� Skeleton ȣ��
    int GetSkeletonID(vector<unsigned long>& output); //���̷��� ID ��ȯ
    int GetImage(ImageFrame& image); //�÷� �̹��� ��ȯ
    int GetDepthImage(DepthFrame& depth); //���� ������ ��ȯ
	
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