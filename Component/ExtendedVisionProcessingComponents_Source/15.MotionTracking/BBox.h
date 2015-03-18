#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

/*

	�ٿ�� �ڽ� ��ü

	CvRect �迭 ���±����̴�

	void update(IplImage* foreground_clone)
			: ���������� �̹����� �����ϱ⿡ �ݵ�� clone �̹���
	
	int length();
	operator[] : �迭ó�� �����Ѵ�

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
