#ifndef _WINDLL

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <device/Property.h>
#include <device/OprosMath.h>

#include "KinectComp.h"

OPRoS::Property GetProperty()
{
	OPRoS::Property property;

	property.SetValue("APIName", "MicrosoftKinect");

	property.SetValue("Width", "1280");
	property.SetValue("Height", "960");

	property.SetValue("KinectIndex", "0");
	property.SetValue("NearMode", "true");
	property.SetValue("SkeletonTrackingMode", "false");

	return property;
}

void SetPorperty(PropertyInterface& comp, OPRoS::Property& property)
{
	typedef std::map<std::string, std::string> PropertyMap;
	PropertyMap& propertyMap = property.GetProperty();

	for (PropertyMap::const_iterator itor = propertyMap.begin()
		; itor != propertyMap.end(); ++itor)
	{
		comp.setProperty(itor->first, itor->second);
	}
}

int main()
{	
	ImageFrame imageFrame;
	DepthFrame depthFrame;
	KinectComp kinect;

	SetPorperty(kinect, GetProperty());

	if (kinect.initialize() != OPROS_SUCCESS)
	{
		return 0;
	}

	if (kinect.start() != OPROS_SUCCESS)
	{
		return 0;
	}

	KinectServiceProvided& kinectService 
		= dynamic_cast<KinectServiceProvided&>(*kinect.getPort("KinectService"));

	kinectService.SetCameraAngle(20);
	kinectService.SetCameraAngle(-20);
	kinectService.SetCameraAngle(10);



	std::vector<Skeleton> skeletons;
	imageFrame = kinectService.GetImage();
	depthFrame = kinectService.GetDepthImage();	
	
	if (!imageFrame.isValid() || !depthFrame.isValid())
	{
		return 0;
	}

	IplImage* image = cvCreateImageHeader(cvSize(imageFrame.width, imageFrame.height), 8, 3);
	IplImage* depthImage = cvCreateImage(cvSize(depthFrame.width, depthFrame.height), 8, 1);
	IplImage* depthImageHeader = cvCreateImageHeader(cvSize(depthFrame.width, depthFrame.height), 16, 1);
	for (;;)
	{
		imageFrame = kinectService.GetImage();
		depthFrame = kinectService.GetDepthImage();
		skeletons = kinectService.GetSkeletonAll();

		if(skeletons.size() != 0)
		{
			for(size_t i = 0; i < skeletons.size(); i++)
			{
				Skeleton& skeleton = skeletons[i];
				char* resultMessage = "";
				CvScalar color = CV_RGB(255, skeleton.userID * 2, (skeleton.userID >> 8) * 2);

				switch(skeleton.result)
				{
				case Skeleton::NOT_TRACKED:
					resultMessage = "Not Tracked";
					break;
				case Skeleton::POSITION_ONLY:
					cvCircle(image, cvPoint((int)skeleton.position.x
						, (int)skeleton.position.y), 5, color, 1);
					resultMessage = "Position Only";
					break;
				case Skeleton::TRACKED:
					for(int index = 0; index < skeleton.JOINT_COUNT; index++)
					{
						cvCircle(image, cvPoint((int)skeleton.joints[index].x
							, (int)skeleton.joints[index].y), 5, color);
					}
					resultMessage = "Tracked";
					break;
				default:
					resultMessage = "Unknown";
					break;
				}

				printf("%d : %s\r\n", skeleton.userID, resultMessage);				
			}
		}

		if (imageFrame.isValid())
			image->imageData = (char*)&imageFrame.data->operator[](0);

		if (depthFrame.isValid())
			depthImageHeader->imageData = (char*)&depthFrame.data->operator[](0);

		cvNormalize(depthImageHeader, depthImage, 0, 0xFF, CV_MINMAX);

		cvShowImage("Kinect Camera", image);
		cvShowImage("Kinect Depth", depthImage);
		if (cvWaitKey(1) == 27)
		{
			break;
		}
	}

	cvReleaseImage(&image);
	cvReleaseImage(&depthImage);
	cvReleaseImageHeader(&depthImageHeader);

	kinect.stop();
	kinect.destroy();

	return 0;
}

#endif