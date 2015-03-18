#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

/* 
	����þ� �ͽ��� �� Wrapping ��ü

	void update(IplImage* img)
		
		IplImage* foreground()

*/

class GMM
{
private:
	
public:
	GMM(int _learnCnt = 20);
	~GMM(void);
	void update(IplImage* curr);
	IplImage* foreground(void);
	IplImage* foregroundOri(void);

	CvBGStatModel* bgModel;
	int learnCnt;
	int frameCnt;

	IplImage* fgclone;
};
