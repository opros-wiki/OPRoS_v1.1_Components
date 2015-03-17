#ifdef TEST_BUILD

#include <iostream>
#include <device/Property.h>
#include <device/OprosMath.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "MicroSoftKinect.h"
#include "debug_macro.h"

OPRoS::Property GetProperty()
{
	OPRoS::Property property;

	property.SetValue("ColorWidth", "640");
	property.SetValue("ColorHeight", "480");
	property.SetValue("DepthWidth", "640");
	property.SetValue("DepthHeight", "480");

	property.SetValue("KinectIndex", "0");
	property.SetValue("NearMode", "true");
	property.SetValue("SkeletonTrackingMode", "false");

	return property;
}


int main()
{	
	MicrosoftKinect kinect;

	if (kinect.Initialize(GetProperty()) != API_SUCCESS)
	{
		std::cerr << DEBUG_MESSAGE("Can't initialize MicrosoftKinect");
		kinect.Finalize();
		return 0;
	}

	if (kinect.Enable() != API_SUCCESS )
	{
		std::cerr << DEBUG_MESSAGE("Can't initialize MicrosoftKinect");
		kinect.Finalize();
		return 0;
	}

	kinect.SetCameraAngle(20);
	kinect.SetCameraAngle(-20);
	kinect.SetCameraAngle(0);

	ImageFrame imageFrame;
	DepthFrame depthFrame;
	std::vector<Skeleton> skeletons;

	while(kinect.GetImage(imageFrame) != API_SUCCESS || kinect.GetDepthImage(depthFrame) != API_SUCCESS)
	{
		if (cvWaitKey(10) == VK_ESCAPE)
		{
			kinect.Finalize();
			return 0;
		}
	}

	IplImage* pImage = cvCreateImage(cvSize(imageFrame.width, imageFrame.height), 8, 3);
	IplImage* pDepthImage = cvCreateImage(cvSize(depthFrame.width, depthFrame.height), 8, 1);
	IplImage* pDepthImageHeader = cvCreateImageHeader(cvSize(depthFrame.width, depthFrame.height), 16, 1);

	for (; cvWaitKey(10) != VK_ESCAPE;)
	{
		if (kinect.GetImage(imageFrame) != API_SUCCESS || kinect.GetDepthImage(depthFrame) != API_SUCCESS)
			continue;
		kinect.GetSkeleton(skeletons);

		memcpy(pImage->imageData, &(*imageFrame.data)[0], pImage->imageSize);

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
					cvCircle(pImage, cvPoint((int)skeleton.position.x, (int)skeleton.position.y), 5, color, 1);
					resultMessage = "Position Only";
					break;
				case Skeleton::TRACKED:
					for(int index = 0; index < skeleton.JOINT_COUNT; index++)
					{
						cvCircle(pImage, cvPoint((int)skeleton.joints[index].x
							, (int)skeleton.joints[index].y), 5, color);
					}
					resultMessage = "Tracked";
					break;
				default:
					resultMessage = "Unknown";
					break;
				}

				std::cout << skeleton.userID << " : " << resultMessage << std::endl;
			}
		}

		pDepthImageHeader->imageData = (char*)&(*depthFrame.data)[0];

		cvNormalize(pDepthImageHeader, pDepthImage, 0, 0xFF, CV_MINMAX);

		cvShowImage("Kinect RGB", pImage);
		cvShowImage("Kinect Depth", pDepthImage);
	}

	cvReleaseImage(&pImage);
	cvReleaseImage(&pDepthImage);
	cvReleaseImageHeader(&pDepthImageHeader);

	kinect.Finalize();

	return 0;
}

#endif