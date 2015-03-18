
/*
*  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
*  
*/
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>

#include <device/OprosPrintMessage.h>
#include <algorithm>
#include <device/OprosMath.h>
#include <cmath>
#include <limits>

#include "KinectComp.h"
#include "debug_macro.h"

//
// constructor declaration
//
KinectComp::KinectComp()
{
	mhOprosAPI = NULL;
	mpKinect = NULL;
	portSetup();
}

//
// constructor declaration (with name)
//
KinectComp::KinectComp(const std::string &name):Component(name)
{
	mhOprosAPI = NULL;
	mpKinect = NULL;
	portSetup();
}

//
// destructor declaration
//

KinectComp::~KinectComp()
{
}

void KinectComp::portSetup() 
{
	ProvidedServicePort* pa1;
	pa1 = new KinectServiceProvided(this);
	addPort("KinectService",pa1);

	//event port setup
	addPort("KinectEvent", &mKinectOutputEvent);
}

// Call back Declaration
ReturnType KinectComp::onInitialize()
{
	//	XML에 저장된 프라퍼티를 parameter에 저장	
	std::map<std::string, std::string>& propertyMap = getPropertyMap();
	OPRoS::Property parameter;
	parameter.SetProperty(propertyMap);

	//	dll 파일이름을 확인하여 없으면 에러 리턴
	if (parameter.FindName("APIName") == false)
	{
		PrintMessage(DEBUG_MESSAGE("Can't find the APIName in property").c_str());
		return OPROS_FIND_PROPERTY_ERROR;
	}

	//DLL 로드
	mhOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("APIName").c_str());
	if (mhOprosAPI == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of GetAPI Funtion").c_str());
		PrintMessage(DEBUG_MESSAGE("Can't get a DLL").c_str());

		return OPROS_FIND_DLL_ERROR;
	}

	//API 로드
	GET_OPROS_DEVICE getOprosAPI = (GET_OPROS_DEVICE)GetProcAddress(mhOprosAPI, "GetAPI");
	if (getOprosAPI == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of GetAPI Funtion").c_str());
		PrintMessage(DEBUG_MESSAGE("Can't load API").c_str());

		FreeLibrary(mhOprosAPI);
		mhOprosAPI = NULL;

		return OPROS_LOAD_DLL_ERROR;
	}

	mpKinect = dynamic_cast<Kinect *>(getOprosAPI());
	if(mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		PrintMessage(DEBUG_MESSAGE("Can't get a Casting").c_str());

		FreeLibrary(mhOprosAPI);
		mhOprosAPI = NULL;

		return OPROS_LOAD_DLL_ERROR;
	}

	//API 초기화
	if(mpKinect->Initialize(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't initialize a Kinect API").c_str());

		delete mpKinect;
		mpKinect = NULL;

		FreeLibrary(mhOprosAPI);
		mhOprosAPI = NULL;

		return OPROS_INITIALIZE_API_ERROR;
	}

	return OPROS_SUCCESS;
}

ReturnType KinectComp::onStart()
{	
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return OPROS_PRECONDITION_NOT_MET;
	}

	if (mpKinect->Enable() == API_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE("Can't Enable  kinect API").c_str());
		return OPROS_ENABLE_API_ERROR;
	}

	mSaveID.clear();

	return OPROS_SUCCESS;
}

ReturnType KinectComp::onStop()
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return OPROS_PRECONDITION_NOT_MET;
	}

	if (mpKinect->Disable() == API_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE("Can't disable  kinect API").c_str());
		return OPROS_DISABLE_API_ERROR;
	}

	mSaveID.clear();

	return OPROS_SUCCESS;
}

ReturnType KinectComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KinectComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KinectComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KinectComp::onDestroy()
{
	//API 핸들 삭제
	if(mpKinect != NULL)
	{
		mpKinect->Finalize();
		delete mpKinect;
		mpKinect = NULL;
	}

	//DLL 핸들 삭제
	if (mhOprosAPI != NULL)
	{
		FreeLibrary(mhOprosAPI);
		mhOprosAPI = NULL;
	}

	return OPROS_SUCCESS;
}

ReturnType KinectComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}


ReturnType KinectComp::onExecute()
{
	std::vector<uint32_t> &userID = GetSkeletonID();
	if (mSaveID.size() != 0 && userID.size() == 0)
	{
		KinectEvent event(KinectEvent::NO_USER);
		SendEvent(event);
	}
	else
	{
		std::vector<uint32_t> deletedID;
		std::vector<uint32_t> addedID;
		for (unsigned int i = 0; i < mSaveID.size(); i++)
		{
			if (std::find(userID.begin(), userID.end(), mSaveID[i]) == userID.end()) 
			{
				deletedID.push_back(mSaveID[i]);
			}
		}

		for (unsigned int i = 0; i < userID.size(); i++) 
		{
			if (std::find(mSaveID.begin(), mSaveID.end(), userID[i]) == mSaveID.end())
			{
				addedID.push_back(userID[i]);
			}
		}

		if (deletedID.size() != 0)
		{
			KinectEvent event(KinectEvent::USER_OUT);
			event.userIDs = deletedID;
			SendEvent(event);
		}

		if (addedID.size() != 0)
		{
			KinectEvent event(KinectEvent::USER_IN);
			event.userIDs = addedID;
			SendEvent(event);
		}
	}

	mSaveID = userID;

	return OPROS_SUCCESS;
}

ReturnType KinectComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KinectComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

Position KinectComp::GetPixelDepth(int x, int y)
{	
	if (mpKinect == NULL)
	{		
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return Position((float)x, (float)y, std::numeric_limits<float>::quiet_NaN());
	}

	Position pixel((float)x, (float)y);
	DepthFrame depthImage;
	if (mpKinect->GetDepthImage(depthImage) == API_SUCCESS)
	{
		if (0 <= x && x < (int)depthImage.width
			&& 0 <= y && y < (int)depthImage.height)
		{
			pixel.z = (*depthImage.data)[x + y * depthImage.width];
		}
		else
		{
			PrintMessage(DEBUG_MESSAGE("x, y are invalid").c_str());
			pixel.z = std::numeric_limits<float>::quiet_NaN();
		}
	}
	else
	{
		PrintMessage(DEBUG_MESSAGE("Can't get depth image").c_str());
		pixel.z = std::numeric_limits<float>::quiet_NaN();
	}

	return pixel;
}

DepthFrame KinectComp::GetDepthImage()
{	
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return DepthFrame(0, 0);
	}

	DepthFrame depthImage; 
	if (mpKinect->GetDepthImage(depthImage) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get depth image").c_str());
		depthImage.width = 0;
		depthImage.height = 0;
		depthImage.data = NULL;
	}

	return depthImage;
}

ImageFrame KinectComp::GetImage()
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return ImageFrame(0, 0);
	}

	ImageFrame image;
	if (mpKinect->GetImage(image) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get image").c_str());
		image.width = 0;
		image.height = 0;
		image.data = NULL;
	}

	return image;
}

std::vector<uint32_t> KinectComp::GetSkeletonID()
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return std::vector<uint32_t>();
	}

	std::vector<uint32_t> skeletonID;
	if (mpKinect->GetSkeletonID(skeletonID) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get skeleton id").c_str());
		skeletonID.clear();
	}

	return skeletonID;
}

Skeleton KinectComp::GetSkeleton(uint32_t id)
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		Skeleton skel;
		const Position invalidPosition(std::numeric_limits<float>::quiet_NaN()
			, std::numeric_limits<float>::quiet_NaN()
			, std::numeric_limits<float>::quiet_NaN());
		skel.result = Skeleton::NOT_TRACKED;
		skel.position = invalidPosition;
		for (int i = 0; i < Skeleton::JOINT_COUNT; i++)
		{
			skel.joints[i] = invalidPosition;
		}
		return skel;
	}

	Skeleton skel(id);
	if (mpKinect->GetSkeleton(skel, id) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get skeleton").c_str());
		const Position invalidPosition(std::numeric_limits<float>::quiet_NaN()
			, std::numeric_limits<float>::quiet_NaN()
			, std::numeric_limits<float>::quiet_NaN());
		skel.userID = id;
		skel.result = Skeleton::NOT_TRACKED;
		skel.position = invalidPosition;
		for (int i = 0; i < Skeleton::JOINT_COUNT; i++)
		{
			skel.joints[i] = invalidPosition;
		}
	}

	return skel;
}

std::vector<Skeleton> KinectComp::GetSkeletonAll()
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return std::vector<Skeleton>();
	}

	std::vector<Skeleton> skel;
	if (mpKinect->GetSkeleton(skel) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get skeleton").c_str());
		skel.clear();
	}

	return skel;
}

float KinectComp::GetCameraAngle()
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return std::numeric_limits<float>::quiet_NaN();
	}

	float degree = 0.0f;
	if (mpKinect->GetCameraAngle(degree) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get camera angle").c_str());
		degree = std::numeric_limits<float>::quiet_NaN();
	}

	return degree;
}

bool KinectComp::SetCameraAngle(float degree)
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return false;
	}

	if (mpKinect->SetCameraAngle(degree) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't set camera angle").c_str());
		return false;
	}

	return true;
}

OPRoS::Property KinectComp::GetProperty()
{	
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return OPRoS::Property();
	}

	OPRoS::Property parameter;
	if (mpKinect->GetProperty(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get parameter").c_str());
		return OPRoS::Property();
	}

	return parameter;
}

bool KinectComp::SetProperty(OPRoS::Property parameter)
{
	if (mpKinect == NULL)
	{
		PrintMessage(DEBUG_MESSAGE("Can't get a handle of Kinect API").c_str());
		return false;
	}

	if (mpKinect->SetProperty(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can't set parameter").c_str());
		return false;
	}

	return true;
}

ReturnType KinectComp::SendEvent( const KinectEvent& event )
{
	EventData<KinectEvent> eventData;
	eventData.setId("KinectEvent");
	eventData.setContentData(event);

	return mKinectOutputEvent.push(&eventData);
}

#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
	__declspec(dllexport) Component *getComponent();
	__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new KinectComp();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new KinectComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

