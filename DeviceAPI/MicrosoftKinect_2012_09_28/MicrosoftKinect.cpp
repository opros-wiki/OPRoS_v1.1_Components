#include "MicrosoftKinect.h"

#include <iostream>
#include <emmintrin.h>
#include <limits>

#include <OprosPrintMessage.h>
#include <OprosMath.h>

#include "debug_macro.h"
#include "OprosLockHelper.h"

//Define Required Profile Name List
#define KINECT_IMAGE_WIDTH "Width"
#define KINECT_IMAGE_HEIGHT "Height"
#define KINECT_NEAR_MODE "NearMode"
#define KINECT_SKELETON_TRACKING_MODE "SkeletonTrackingMode"
#define KINECT_INDEX "KinectIndex"
//End of Define Required Profile Name List

MicrosoftKinect::MicrosoftKinect()
{
	mpNuiSensor = NULL;
	mHandleDepthStream = NULL;
	mHandleColorStream = NULL;
	mNuiColorResoultion = NUI_IMAGE_RESOLUTION_640x480;
	mNuiDepthResoultion = NUI_IMAGE_RESOLUTION_640x480;
	mIsNearMode = false;
	mIsSkeletonTrackingMode = false;

	mNuiIndex = 0;

	InitDepthConverter();
}

MicrosoftKinect::~MicrosoftKinect()
{
	Finalize();
}


int MicrosoftKinect::Initialize( Property parameter )
{	
	if (mpNuiSensor != NULL)
	{
		mpNuiSensor->NuiShutdown();
		mpNuiSensor->Release();
		mpNuiSensor = NULL;

		mHandleDepthStream = NULL;
		mHandleColorStream = NULL;
	}

	if (SetKinectProfile(parameter) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Can't set kinect profile.").c_str());
		return API_ERROR;
	}

	return API_SUCCESS;
}

int MicrosoftKinect::Finalize( void )
{
	if (mpNuiSensor != NULL)
	{
		mpNuiSensor->NuiShutdown();
		mpNuiSensor->Release();
		mpNuiSensor = NULL;

		mHandleDepthStream = NULL;
		mHandleColorStream = NULL;
	}	

	return API_SUCCESS;
}

int MicrosoftKinect::Enable( void )
{
	if (mpNuiSensor != NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Already kinect enabled.").c_str());
		return API_SUCCESS;
	}

	if (FAILED(NuiCreateSensorByIndex(mNuiIndex, &mpNuiSensor)))
	{
		PrintMessage(DEBUG_MESSAGE("Can't create Kinect").c_str());
		return API_ERROR;
	}

	if (FAILED(mpNuiSensor->NuiInitialize((mIsSkeletonTrackingMode 
		? NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX : NUI_INITIALIZE_FLAG_USES_DEPTH)
		| NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON)))
	{
		PrintMessage(DEBUG_MESSAGE("Can't initialize Kinect").c_str());
		return API_ERROR;
	}

	if(mIsSkeletonTrackingMode == true)
	{
		if (FAILED(mpNuiSensor->NuiSkeletonTrackingEnable(NULL
			, NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE)))
		{
			PrintMessage(DEBUG_MESSAGE("Can't enable sekelton tracking mode").c_str());
			return API_ERROR;
		}
	}
	else
	{
		if (FAILED(mpNuiSensor->NuiSkeletonTrackingDisable()))
		{
			PrintMessage(DEBUG_MESSAGE("Can't disable sekelton tracking mode").c_str());
			return API_ERROR;
		}
	}

	if (FAILED(mpNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, mNuiColorResoultion
		, 0, 2, NULL, &mHandleColorStream)))
	{
		PrintMessage(DEBUG_MESSAGE("Can't open corlor stream.").c_str());
		return API_ERROR;
	}

	if (FAILED(mpNuiSensor->NuiImageStreamOpen(mIsSkeletonTrackingMode
		? NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX : NUI_IMAGE_TYPE_DEPTH, mNuiDepthResoultion
		, mIsNearMode ? NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE : 0, 2, NULL, &mHandleDepthStream)))
	{
		PrintMessage(DEBUG_MESSAGE("Can't open depth stream.").c_str());
		return API_ERROR;
	}

	mLockTempImageFrame.Lock();
	mTempImageFrame.width = 0;
	mTempImageFrame.height = 0;
	mTempImageFrame.data = NULL;
	mLockTempImageFrame.Unlock();

	mLockTempDepthFrame.Lock();
	mTempDepthFrame.width = 0;
	mTempDepthFrame.height = 0;
	mTempDepthFrame.data = NULL;
	mLockTempDepthFrame.Unlock();

	return API_SUCCESS;
}

int MicrosoftKinect::Disable( void )
{
	if (mpNuiSensor != NULL)
	{
		mpNuiSensor->NuiShutdown();
		mpNuiSensor->Release();
		mpNuiSensor = NULL;

		mHandleDepthStream = NULL;
		mHandleColorStream = NULL;
	}	

	return API_SUCCESS;
}

int MicrosoftKinect::SetParameter( Property parameter )
{
	if (Disable() != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't disable.").c_str());
		return API_ERROR;
	}

	if (SetKinectProfile(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't set kinect profile.").c_str());
		return API_ERROR;
	}

	if (Enable() != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't enable.").c_str());
		return API_ERROR;
	}

	return API_SUCCESS;
}

int MicrosoftKinect::GetParameter( Property &parameter )
{
	std::ostringstream stringConverter;

	DWORD width, height;
	NuiImageResolutionToSize(mNuiColorResoultion, width, height);

	stringConverter << width;
	parameter.SetValue(KINECT_IMAGE_WIDTH, stringConverter.str());
	stringConverter.str("");

	stringConverter << height;
	parameter.SetValue(KINECT_IMAGE_HEIGHT, stringConverter.str());
	stringConverter.str("");

	stringConverter << mNuiIndex;
	parameter.SetValue(KINECT_INDEX, stringConverter.str());
	stringConverter.str("");

	parameter.SetValue(KINECT_NEAR_MODE, mIsNearMode ? "true" : "false");
	parameter.SetValue(KINECT_SKELETON_TRACKING_MODE, mIsSkeletonTrackingMode ? "true" : "false");

	return API_SUCCESS;
}

bool MicrosoftKinect::SetKinectProfile( Property& parameter )
{
	int width = 640;
	int height = 480;
	mIsNearMode = false;
	mIsSkeletonTrackingMode = false;
	mNuiIndex = 0;

	mLockSkeletonList.Lock();
	mSkeletonList.clear();
	mLockSkeletonList.Unlock();

	PrintMessage("--------------------------------\r\n");
	PrintMessage("MicrosoftKinect Profiles\r\n");
	PrintMessage("--------------------------------\r\n");

	PrintMessage("%s : ", KINECT_IMAGE_WIDTH);
	if (parameter.FindName(KINECT_IMAGE_WIDTH) == true)
		width = atoi(parameter.GetValue(KINECT_IMAGE_WIDTH).c_str());
	PrintMessage("%d\r\n", width);

	PrintMessage("%s : ", KINECT_IMAGE_HEIGHT);
	if (parameter.FindName(KINECT_IMAGE_HEIGHT) == true)
		height = atoi(parameter.GetValue(KINECT_IMAGE_HEIGHT).c_str());
	PrintMessage("%d\r\n", height);

	PrintMessage("%s : ", KINECT_INDEX);
	if (parameter.FindName(KINECT_INDEX) == true)
		mNuiIndex = atoi(parameter.GetValue(KINECT_INDEX).c_str());
	PrintMessage("%d\r\n", mNuiIndex);

	PrintMessage("%s : ", KINECT_NEAR_MODE);
	if (parameter.FindName(KINECT_NEAR_MODE) == true)
		mIsNearMode = strcmp(strupr(&parameter.GetValue(KINECT_NEAR_MODE)[0]), "TRUE") == 0;
	PrintMessage("%s\r\n", mIsNearMode ? "true" : "false");

	PrintMessage("%s : ", KINECT_SKELETON_TRACKING_MODE);
	if (parameter.FindName(KINECT_SKELETON_TRACKING_MODE) == true)
		mIsSkeletonTrackingMode = strcmp(strupr(&parameter.GetValue(KINECT_SKELETON_TRACKING_MODE)[0]), "TRUE") == 0;
	PrintMessage("%s\r\n", mIsSkeletonTrackingMode ? "true" : "false");


	if (width == 640 && height == 480)
	{
		mNuiColorResoultion = NUI_IMAGE_RESOLUTION_640x480;
		mNuiDepthResoultion = NUI_IMAGE_RESOLUTION_640x480;
	}
	else if(width == 1280 && height == 960)
	{
		mNuiColorResoultion = NUI_IMAGE_RESOLUTION_1280x960;
		mNuiDepthResoultion = NUI_IMAGE_RESOLUTION_640x480;
	}
	else
	{
		PrintMessage("Width and Height is invalid value.\r\n");
		PrintMessage("Valid values are 640x480 and 1280x960.\r\n");
		return false;
	}

	PrintMessage("--------------------------------\r\n");

	return true; //초기화 성공한 경우
}

void MicrosoftKinect::TransformSkeletonToDepthImage(Position& result, const Vector4& skeletonPosition )
{
	NuiTransformSkeletonToDepthImage(skeletonPosition, &result.x, &result.y, mNuiDepthResoultion);
	result.z = skeletonPosition.z * 1000.0f; //m단위를 mm단위로 변경
}

bool MicrosoftKinect::GetSKeletonNextFrame()
{
	NUI_SKELETON_FRAME skeletonFrame;

	//스켈레톤 정보 받아오기
	if (mIsSkeletonTrackingMode == false
		|| FAILED(mpNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
	{
		return false;
	}

	//스켈레톤의 각 조인트 정보를 구조체에
	vector<Skeleton> tempSkeleton;
	tempSkeleton.reserve(NUI_SKELETON_COUNT);

	mpNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);

	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		NUI_SKELETON_DATA& skel = skeletonFrame.SkeletonData[i];

		if (skel.eTrackingState == NUI_SKELETON_TRACKED)
		{
			Skeleton skeleton ;

			for (int index = 0; index < Skeleton::JOINT_COUNT; index++)
			{
				if(skel.eSkeletonPositionTrackingState[index] == NUI_SKELETON_POSITION_NOT_TRACKED)
				{
					skeleton.joints[index].x = std::numeric_limits<float>::quiet_NaN();
					skeleton.joints[index].y = std::numeric_limits<float>::quiet_NaN();
					skeleton.joints[index].z = std::numeric_limits<float>::quiet_NaN();
				}
				else
				{
					TransformSkeletonToDepthImage(skeleton.joints[index], skel.SkeletonPositions[index]);
				}
			}

			skeleton.userID = skel.dwTrackingID; 
			skeleton.result = Skeleton::TRACKED;
			TransformSkeletonToDepthImage(skeleton.position, skel.Position);

			tempSkeleton.push_back(skeleton);
		}
		else if(skel.eTrackingState == NUI_SKELETON_POSITION_ONLY)
		{			
			Skeleton skeleton;

			skeleton.userID = skel.dwTrackingID; 
			skeleton.result = Skeleton::POSITION_ONLY;
			TransformSkeletonToDepthImage(skeleton.position, skel.Position);

			for (int index = 0; index < Skeleton::JOINT_COUNT; index++)
			{
				skeleton.joints[index].x = std::numeric_limits<float>::quiet_NaN();
				skeleton.joints[index].y = std::numeric_limits<float>::quiet_NaN();
				skeleton.joints[index].z = std::numeric_limits<float>::quiet_NaN();
			}

			tempSkeleton.push_back(skeleton);
		}
	}

	mLockSkeletonList.Lock();
	mSkeletonList = tempSkeleton;
	mLockSkeletonList.Unlock();

	return true;
}

int MicrosoftKinect::SetCameraAngle( float degree )
{
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	LONG elevationAngleValue = bound(INTEGER(degree)
		, NUI_CAMERA_ELEVATION_MINIMUM, NUI_CAMERA_ELEVATION_MAXIMUM);

	//카메라의 각도 바꿔주는 함수
	if (FAILED(mpNuiSensor->NuiCameraElevationSetAngle(elevationAngleValue)))
	{
		PrintMessage(DEBUG_MESSAGE("Can't set camera angle.").c_str());
		return API_ERROR;
	}

	return API_SUCCESS;
}

int MicrosoftKinect::GetCameraAngle(float& degree ) 
{
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	LONG elevationAngleValue = 0;

	if(FAILED(mpNuiSensor->NuiCameraElevationGetAngle(&elevationAngleValue)))
	{		
		PrintMessage(DEBUG_MESSAGE("Can't get camera angle.").c_str());
		return API_ERROR;
	}

	degree = (float)elevationAngleValue;

	return API_SUCCESS;
}

int MicrosoftKinect::GetSkeleton( vector<Skeleton>& output )
{
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	GetSKeletonNextFrame();

	mLockSkeletonList.Lock();

	output = mSkeletonList;

	mLockSkeletonList.Unlock();

	return API_SUCCESS;
}

int MicrosoftKinect::GetSkeleton( Skeleton& output, unsigned long id )
{
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	GetSKeletonNextFrame();

	mLockSkeletonList.Lock();

	output.result = Skeleton::NOT_TRACKED;

	for (size_t i = 0; i < mSkeletonList.size(); i++)
	{
		if (mSkeletonList[i].userID == id)
		{
			output = mSkeletonList[i];
			break;
		}
	}

	mLockSkeletonList.Unlock();

	if (output.result == Skeleton::NOT_TRACKED)
		return API_ERROR;
	return API_SUCCESS;
}

int MicrosoftKinect::GetSkeletonID( vector<unsigned long>& output )
{
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	GetSKeletonNextFrame();

	mLockSkeletonList.Lock();

	output.resize(mSkeletonList.size());

	for (size_t i = 0; i < mSkeletonList.size(); i++)
	{
		output[i] = mSkeletonList[i].userID;
	}

	mLockSkeletonList.Unlock();

	return API_SUCCESS;	
}

int MicrosoftKinect::GetImage( ImageFrame& image )
{
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	NUI_IMAGE_FRAME imageFrame;
	if (FAILED(mpNuiSensor->NuiImageStreamGetNextFrame(mHandleColorStream, 0
		, &imageFrame)))
	{
		//PrintMessage(DEBUG_MESSAGE("Can't get color stream image.").c_str());
		OprosLockHelper lock(mLockTempImageFrame);
		if (mTempImageFrame.width == 0 || mTempImageFrame.height == 0
			|| mTempImageFrame.data.isNULL())
		{
			return API_ERROR;
		}

		image = mTempImageFrame; 

		return API_SUCCESS;
	}

	INuiFrameTexture* pFrameTexture = imageFrame.pFrameTexture;
	NUI_LOCKED_RECT lockedRect;

	pFrameTexture->LockRect(0, &lockedRect, NULL, 0);

	if (lockedRect.Pitch == 0)
	{
		PrintMessage(DEBUG_MESSAGE("lockRect.Pitch is 0").c_str());

		pFrameTexture->UnlockRect(0);
		mpNuiSensor->NuiImageStreamReleaseFrame(mHandleColorStream, &imageFrame);

		OprosLockHelper lock(mLockTempImageFrame);
		if (mTempImageFrame.width == 0 || mTempImageFrame.height == 0
			|| mTempImageFrame.data.isNULL())
		{
			return API_ERROR;
		}

		image = mTempImageFrame; 

		return API_SUCCESS;		
	}

	DWORD width = 0;
	DWORD height = 0;
	NuiImageResolutionToSize(imageFrame.eResolution, width, height);

	image.width = width;
	image.height = height;
	image.type =ImageFrame::BGR8;
	image.data = new vector<uint8_t>();

	const size_t outImagePixelByte = 3;
	const size_t outImageSize = width * height * outImagePixelByte;
	vector<uint8_t>& outImage = const_cast<vector<uint8_t>&>(*image.data);

	//1바이트는 복사속도향상을 위한 방법에서 발생할 수 있는
	//Out Of Index 오류를 막기 위한 공간으로 나중에 이 1바이트를 삭제한다.
	outImage.resize(outImageSize + 1); 

	//Kinect로 부터 나오는 이미지의 한 픽셀의 크기는 4
	//[0] = B, [1] = G, [2] = R, [3] = A 이다.
	const size_t inImagePixeByte = 4;
	BYTE* pImageBuffer = lockedRect.pBits;	

	for (uint8_t* outImagePixel = &outImage[0], *lastPixelAddress = outImagePixel + outImageSize
		; outImagePixel < lastPixelAddress
		; outImagePixel += outImagePixelByte, pImageBuffer += inImagePixeByte)
	{
		*((uint32_t*)outImagePixel) = *((uint32_t*)pImageBuffer);
	}

	outImage.resize(outImageSize); 

	pFrameTexture->UnlockRect(0);
	mpNuiSensor->NuiImageStreamReleaseFrame(mHandleColorStream, &imageFrame);

	mLockTempImageFrame.Lock();
	mTempImageFrame = image;
	mLockTempImageFrame.Unlock();

	return API_SUCCESS;
}

int MicrosoftKinect::GetDepthImage( DepthFrame& depth )
{   
	if (mpNuiSensor == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	NUI_IMAGE_FRAME imageFrame;
	if (FAILED(mpNuiSensor->NuiImageStreamGetNextFrame(mHandleDepthStream, 0
		, &imageFrame)))
	{
		//PrintMessage(DEBUG_MESSAGE("Can't get depth stream image.").c_str());

		OprosLockHelper lock(mLockTempDepthFrame);
		if (mTempDepthFrame.width == 0 || mTempDepthFrame.height == 0
			|| mTempDepthFrame.data.isNULL())
		{
			return API_ERROR;
		}

		depth = mTempDepthFrame;

		return API_SUCCESS;
	}

	INuiFrameTexture* pFrameTexture = imageFrame.pFrameTexture;
	NUI_LOCKED_RECT lockedRect;

	pFrameTexture->LockRect(0, &lockedRect, NULL, 0);

	if (lockedRect.Pitch == 0)
	{
		PrintMessage(DEBUG_MESSAGE("lockRect.Pitch is 0").c_str());

		pFrameTexture->UnlockRect(0);
		mpNuiSensor->NuiImageStreamReleaseFrame(mHandleDepthStream, &imageFrame);

		OprosLockHelper lock(mLockTempDepthFrame);
		if (mTempDepthFrame.width == 0 || mTempDepthFrame.height == 0
			|| mTempDepthFrame.data.isNULL())
		{
			return API_ERROR;
		}

		depth = mTempDepthFrame;

		return API_SUCCESS;	
	}

	DWORD width = 0;
	DWORD height = 0;
	NuiImageResolutionToSize(imageFrame.eResolution, width, height);

	depth.width = width;
	depth.height = height;
	depth.data = new vector<uint16_t>();

	vector<uint16_t>& outImage = const_cast<vector<uint16_t>&>(*depth.data);

	(this->*depthConverter)(outImage, (USHORT*)lockedRect.pBits, width * height);

	pFrameTexture->UnlockRect(0);
	mpNuiSensor->NuiImageStreamReleaseFrame(mHandleDepthStream, &imageFrame);

	mLockTempDepthFrame.Lock();
	mTempDepthFrame = depth;
	mLockTempDepthFrame.Unlock();

	return API_SUCCESS;
}

void MicrosoftKinect::ConvertNuiDepthToDepth( vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount )
{
	output.resize(maxCount);

	USHORT* pNuiDeptPixel = pNuiDepth;
	uint16_t* pOutputDepthPixel = &output[0];
	for (const uint16_t* pLastPixelAddress = &output[0] + maxCount
		; pOutputDepthPixel < pLastPixelAddress
		; pNuiDeptPixel++, pOutputDepthPixel++)
	{
		*pOutputDepthPixel = NuiDepthPixelToDepth(*pNuiDeptPixel);
	}
}

void MicrosoftKinect::ConvertNuiDepthToDepthUsingSSE( vector<uint16_t>& output, USHORT* pNuiDepth, const size_t maxCount )
{
	output.resize(maxCount);

	const size_t processElemetSize = sizeof(__m128i) / sizeof(uint16_t);
	const size_t remainElementCount = maxCount % processElemetSize;

	USHORT* pNuiDeptPixel = pNuiDepth;
	uint16_t* pOutputDepthPixel = &output[0];
	for (const uint16_t* pLastSseProcessDepthPixelAddress = pOutputDepthPixel + maxCount - remainElementCount
		; pOutputDepthPixel < pLastSseProcessDepthPixelAddress
		; pNuiDeptPixel += processElemetSize, pOutputDepthPixel += processElemetSize)
	{
		__m128i shiftInput = _mm_loadu_si128((__m128i*)pNuiDeptPixel);
		__m128i shiftOutput = _mm_srli_epi16(shiftInput, NUI_IMAGE_PLAYER_INDEX_SHIFT);
		_mm_storeu_si128((__m128i*)pOutputDepthPixel, shiftOutput);
	}

	for (const uint16_t* pLastPixelAddress = &output[0] + maxCount
		; pOutputDepthPixel < pLastPixelAddress
		; pNuiDeptPixel++, pOutputDepthPixel++)
	{
		*pOutputDepthPixel = *pNuiDeptPixel >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
	}
}

void MicrosoftKinect::InitDepthConverter()
{
	bool isUsableSSE2 = false;

	__asm
	{
		mov eax, 1;				//프로세스 정보 및 기능
		CPUID					//CPUID 호출

		mov [isUsableSSE2], 0
		test edx, 04000000h		//반환값이 SSE2를 지원하는지 확인
		jz CANT_USE_SSE2
		mov [isUsableSSE2], 1

CANT_USE_SSE2:
	}

	if (isUsableSSE2)
	{
		depthConverter = &MicrosoftKinect::ConvertNuiDepthToDepthUsingSSE;
	}
	else
	{
		depthConverter = &MicrosoftKinect::ConvertNuiDepthToDepth;
	}
}

extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new MicrosoftKinect();
}

//Undefine Required Profile Name List
#undef KINECT_IMAGE_WIDTH
#undef KINECT_IMAGE_HEIGHT
#undef KINECT_INDEX
#undef KINECT_NEAR_MODE
#undef KINECT_SKELETON_TRACKING_MODE
//End of Undefine Required Profile Name List
