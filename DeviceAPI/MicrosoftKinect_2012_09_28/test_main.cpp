#ifndef _WINDLL

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <Property.h>
#include <OprosMath.h>

#include "MicroSoftKinect.h"

Property GetProperty()
{
	Property property;

	property.SetValue("Width", "640");
	property.SetValue("Height", "480");

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
		return 0;
	}

	if (kinect.Enable())
	{
		return 0;
	}

	kinect.SetCameraAngle(20);
	kinect.SetCameraAngle(-20);
	kinect.SetCameraAngle(0);
	
	ImageFrame imageFrame;
	DepthFrame depthFrame;

	vector<Skeleton> skeletons;
	kinect.GetImage(imageFrame);
	kinect.GetDepthImage(depthFrame);	

	IplImage* image = cvCreateImageHeader(cvSize(imageFrame.width, imageFrame.height), 8, 3);
	IplImage* depthImage = cvCreateImage(cvSize(depthFrame.width, depthFrame.height), 8, 1);
	IplImage* depthImageHeader = cvCreateImageHeader(cvSize(depthFrame.width, depthFrame.height), 16, 1);
	for (;;)
	{
		kinect.GetImage(imageFrame);
		kinect.GetDepthImage(depthFrame);
		kinect.GetSkeleton(skeletons);

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
					cvCircle(image, cvPoint((int)skeleton.position.x, (int)skeleton.position.y), 5, color, 1);
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

		image->imageData = (char*)&imageFrame.data->operator[](0);
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

	kinect.Disable();

	return 0;
}

#endif