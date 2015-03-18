#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

/*

	바운딩 박스 객체

	CvRect 배열 형태구조이다

	void update(IplImage* foreground_clone)
			: 내부적으로 이미지를 변형하기에 반드시 clone 이미지
	
	int length();
	operator[] : 배열처럼 접근한다

*/

class BBox{

private:
	enum{MAX_BBOX_SIZE=256};

	CvMemStorage* storage;
	int num;
	float perimScale;
	CvRect bbs[MAX_BBOX_SIZE];

public:
	BBox(float _perimScale = 12);
	~BBox(void);
	void update(IplImage* foreground_clone);
	int length(void);
	CvRect& operator[](int idx);
	void drawBBox(IplImage* showimg);
};
