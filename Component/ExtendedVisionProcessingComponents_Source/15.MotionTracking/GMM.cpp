#include "GMM.h"

GMM::GMM(int _learnCnt){

	learnCnt = _learnCnt;
	frameCnt = 0;
	bgModel = NULL;
	fgclone = NULL;

}

GMM::~GMM(void){

	if(bgModel != NULL){
		cvReleaseBGStatModel(&bgModel);
	}
	if(fgclone != NULL){
		cvReleaseImage(&fgclone);
	}

}

// update Gaussian Mixture Model
void GMM::update(IplImage* curr){

	if(bgModel == NULL){
	
		bgModel = cvCreateGaussianBGModel(curr, NULL);

	}else{
	
		// check channels
		IplImage* curr_gray;
		if(curr->nChannels == 3){
		
			curr_gray = cvCreateImage(cvGetSize(curr), IPL_DEPTH_8U, 1);
			cvCvtColor(curr, curr_gray, CV_RGB2GRAY);
			cvEqualizeHist(curr_gray, curr_gray);
		
		}else if(curr->nChannels == 1){
		
			curr_gray = curr;
			cvEqualizeHist(curr_gray, curr_gray);
		
		}else{
		
			// exception
		
		}

		//learning rate
		if(frameCnt++ <= learnCnt){
		
			cvUpdateBGStatModel(curr_gray, bgModel, -1); // learn
		
		}else{
		
			cvUpdateBGStatModel(curr_gray, bgModel, 0);
		
		}

		if(curr->nChannels == 3){
		
			cvReleaseImage(&curr_gray);
		
		}
	
	}

	if(fgclone){
	
		cvCopy(bgModel->foreground, fgclone, NULL);

		// remove salt & pepper noise
		cvSmooth(fgclone, fgclone, CV_MEDIAN, 5);
	
	}else{
	
		fgclone = cvCloneImage(bgModel->foreground);

		//remove salt & pepper noise
		cvSmooth(fgclone, fgclone, CV_MEDIAN, 5);
	
	}

}

// getter method, foreground image
IplImage* GMM::foreground(void){

	return fgclone;

}

IplImage* GMM::foregroundOri(void){

	return bgModel->foreground;

}