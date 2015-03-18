#include "BBox.h"

BBox::BBox(float _perimScale){

	storage = cvCreateMemStorage(0);
	num = 0;
	perimScale = _perimScale;

}

BBox::~BBox(void){

	cvReleaseMemStorage(&storage);
	
}

void BBox::update(IplImage *foreground_clone){

	IplImage* fgc = foreground_clone;

	cvClearMemStorage(storage);

	cvSmooth(fgc, fgc, CV_MEDIAN, 5);

	cvDilate(fgc, fgc, 0, 2);
	cvErode(fgc, fgc, 0, 2);
	
	//get contours (level 0)
	CvSeq* contours = cvSegmentFGMask(fgc, 1, perimScale, storage);

	cvSetZero(fgc);

	// inital setting
	num = MAX_BBOX_SIZE;

	int numFilled = 0;
	CvSeq* c = contours;

	for(int i = 0; c != NULL; c = c->h_next, i++){
	
		if(i < num){
		
			cvDrawContours(fgc, c, CV_RGB(255,255,255), CV_RGB(255,255,255),
				-1, CV_FILLED, 8);

			//get bounding boxes
			bbs[i] = cvBoundingRect(c, 0);

			//post conds
			cvSetZero(fgc);
			numFilled++;
		
		}
	
	} // for end

	num = numFilled;

}

int BBox::length(void){

	return num;

}

CvRect& BBox::operator [](int idx){

	return bbs[idx];

}

void BBox::drawBBox(IplImage *showimg){

	for(int i = 0; i < num; i++){
	
		cvRectangle(showimg,
			cvPoint(bbs[i].x, bbs[i].y),
			cvPoint(bbs[i].x + bbs[i].width, bbs[i].y + bbs[i].height),
			CV_RGB(255, 255, 255),
			2);
	
	}
	
}