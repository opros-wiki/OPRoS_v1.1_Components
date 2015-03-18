
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



#include "HandsMotionTracking.h"

#define _DEF_MAX_BLOBS		100
#define _DEF_MAX_LABEL		 20

//
// constructor declaration
//

IplImage* m_image_buff;			// 원본 영상
IplImage* m_image_dest;			// 출력 영상 
IplImage* m_image_dest2;		// 임시 3채널 영상 
 
IplImage* m_image_th;			// 이진영상
IplImage* m_image_th2;			// 이진영상
IplImage* temp_mask;			// 마스크
IplImage* temp_mask2;			// 마스크
IplImage* circle_mask;			// 마스크
IplImage* in_mask;				// 마스크
	
CvCapture* m_video;				// 동영상 프레임 저장

bool video_flag;				// 비디오 재생 컨트롤 플래그

CvMemStorage* storage1;			// 메모리 할당
CvMemStorage* storage2;			// 메모리 할당

CvPoint		max_pt;             // 중심점과의 최대거리에 있는 좌표 저장
CvPoint		max_pt_in;			// 손끝 영역내의 중심점과 최대거리에 있는 좌표 저장
CvPoint     finger_pt[2];		// 동영상 와핑이 사용할 손끝 두점 

int m_in_width;
int m_in_height;

CvPoint outData[3];				// 내보낼 데이터 변수 - 중심점과 동영상 와핑될 두 점
std::string outString[3];		// 출력 데이터 보기좋게 스트링도 함께

/***** 레이블링 관련 변수 *******/	

	IplImage*	m_Image;			// 레이블링을 위한 이미지
	int			m_nThreshold;		// 레이블링 스레스홀드 값
	Visited*	m_vPoint_out;		// 레이블링시 방문정보
	int			m_nBlobs_out;		// 레이블의 갯수
	CvRect*		m_rec_out;			// 각 레이블 정보
	
	
	int			m_nThreshold_in;	// 레이블링 스레스홀드 값
	Visited*	m_vPoint_in;		// 레이블링시 방문정보
	int			m_nBlobs_in;		// 레이블의 갯수
	CvRect*		m_rec_in;			// 각 레이블 정보


	

HandsMotionTracking::HandsMotionTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
HandsMotionTracking::HandsMotionTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

HandsMotionTracking::~HandsMotionTracking() {
}

void HandsMotionTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);

	// export variable setup


}

// Call back Declaration
ReturnType HandsMotionTracking::onInitialize()
{
	// 이미지용 메모리 초기화
	m_image_buff		= NULL;           
	m_image_dest		= NULL;
	m_image_dest2		= NULL;
	m_image_th			= NULL;
	m_image_th2			= NULL;
	temp_mask			= NULL;
	temp_mask2			= NULL;
	circle_mask			= NULL;
	in_mask				= NULL;
	m_video				= NULL;

	//변수 초기화
	m_in_width			= 0;
    m_in_height			= 0;

	storage1			= NULL;
	storage2			= NULL;

	m_rec_out			= NULL;
	m_rec_in			= NULL;
	m_nBlobs_out		= 0;
	m_nBlobs_in			= 0;
	m_vPoint_out		= NULL;
	m_vPoint_in 		= NULL;

	m_nThreshold		= 0;
	m_nThreshold_in		= 0;

	video_flag			= FALSE;

	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HandsMotionTracking::onStop()
{
	//스탑 되었다 스타트 시킬때 새로 재생 되도록
	video_flag			= false;
	

	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onDestroy()
{
	// 이미지용 메모리의 해제	
	cvReleaseImage(&m_image_buff);
    cvReleaseImage(&m_image_dest);
	cvReleaseImage(&m_image_dest2);
    cvReleaseImage(&m_image_th);
	cvReleaseImage(&m_image_th2);
    cvReleaseImage(&temp_mask);
    cvReleaseImage(&temp_mask2);
	cvReleaseImage(&circle_mask);
    cvReleaseImage(&in_mask);

	// 메모리 해제
	cvReleaseMemStorage(&storage1);
	cvReleaseMemStorage(&storage2);
	cvReleaseCapture(&m_video);
	
	return OPROS_SUCCESS;

}

ReturnType HandsMotionTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onExecute()
{
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;

	//아웃 데이터
	std::vector<PositionDataType> data;


	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();


		// 현재영상의 크기를 취득
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// 메모리 한번 해제해주고
		if(m_image_buff			!= NULL)
			cvReleaseImage(&m_image_buff);
		if(m_image_dest         != NULL)
			cvReleaseImage(&m_image_dest);
		if(m_image_dest2        != NULL)
			cvReleaseImage(&m_image_dest2);

		if(m_image_th			!= NULL)
			cvReleaseImage(&m_image_th);
		if(m_image_th2			!= NULL)
			cvReleaseImage(&m_image_th2);

		// 이미지용 메모리 할당
        m_image_buff   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);//원본 이미지
		m_image_dest   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		m_image_dest2  = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		m_image_th     = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);//영역 추출 이미지
		m_image_th2    = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);//영역 추출 이미지
		
		
		if(!video_flag)
		{
			std::string cpath = getProperty("opros.component.dir");
			std::string file = getProperty("VideoFile");
			if (file == "") file = "sample.avi";

			std::string path = cpath + file;

			m_video	= NULL;
			m_video = cvCreateFileCapture(path.c_str()); //비디오
			video_flag = true;// 비디오가 계속 새로 재생됨을 방지
			
		}

		// 영상에 대한 정보를 확보!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// 출력용
		cvCopy(m_image_buff, m_image_dest, 0);

		// 색상 분리용 이미지
		IplImage* m_image_YCrCb = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 3);
		IplImage* m_Y  = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);
		IplImage* m_Cr = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);
		IplImage* m_Cb = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);

		cvCvtColor(m_image_buff, m_image_YCrCb, CV_RGB2YCrCb);   //RGB - > YCrCV 변환

		cvSplit(m_image_YCrCb, m_Y, m_Cr, m_Cb, NULL);   //채널 분리

		//추출이 필요한 영역 픽셀 데이터 저장 변수
		unsigned char m_Cr_val = 0;			
		unsigned char m_Cb_val = 0;

		// 살색추출
		for(int i=0;i<m_image_buff->height;i++)            
		{
			for(int j=0;j<m_image_buff->width;j++)
			{  
				//Cr 영역과 Cb 영역 추출
				m_Cr_val = (unsigned char)m_Cr->imageData[i*m_Cr->widthStep+j];
				m_Cb_val = (unsigned char)m_Cb->imageData[i*m_Cb->widthStep+j];

				//살색에 해당하는 영역인지 검사
				if( (77 <= m_Cr_val) && (m_Cr_val <= 127) && (133 <= m_Cb_val) && (m_Cb_val <= 173) )
				{
					// 살색부분은 하얀색
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+0] = (unsigned char)255; 
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+1] = (unsigned char)255;
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+2] = (unsigned char)255; 
				}
				else
				{ 
					// 나머지는 검정색
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+0]= 0;
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+1]= 0;
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+2]= 0;
				}
			}
		}
		
		//살색 추출한 영상을 이진화
		cvCvtColor(m_image_buff, m_image_th, CV_RGB2GRAY); 

		//잡영 제거를 위한 연산
		cvDilate (m_image_th, m_image_th, NULL, 2);//팽창
		cvErode  (m_image_th, m_image_th, NULL, 2);//침식	

		//변수 및 이미지 메모리 초기화
		int temp_num = 0;
		int StartX , StartY, EndX , EndY;
		int nNumber = 0;
		m_nThreshold	= 100;

		if( m_rec_out != NULL )
		{
			delete m_rec_out;

			m_rec_out	= NULL;
			m_nBlobs_out	= _DEF_MAX_BLOBS;
		}
		else
		{
			m_rec_out	= NULL;
			m_nBlobs_out	= _DEF_MAX_BLOBS;
		}
	
		if( m_image_th2 != NULL )	
			cvReleaseImage( &m_image_th2 );

		//레이블링 할 영상 따로 생성
		m_image_th2			= cvCloneImage( m_image_th );

		//레이블링 할 이미지의 크기 저장
		int nWidth	= m_image_th2->width;
		int nHeight = m_image_th2->height;

		//해당 영상 크기만큼 버프 설정
		unsigned char* tmpBuf = new unsigned char [nWidth * nHeight];

		for(int j=0; j<nHeight ;j++)	
			for(int i=0; i<nWidth ;i++)	
				//전 픽셀 순회
				tmpBuf[j*nWidth+i] = (unsigned char)m_image_th2->imageData[j*m_image_th2->widthStep+i];
		
////// 레이블링을 위한 포인트 초기화

		m_vPoint_out = new Visited [nWidth * nHeight];
		
		for(int nY = 0; nY < nHeight; nY++)
		{
			for(int nX = 0; nX < nWidth; nX++)
			{
				m_vPoint_out[nY * nWidth + nX].bVisitedFlag		= FALSE;
				m_vPoint_out[nY * nWidth + nX].ptReturnPoint.x	= nX;
				m_vPoint_out[nY * nWidth + nX].ptReturnPoint.y	= nY;
			}
		}

////// 레이블링 수행
		for(int nY = 0; nY < nHeight; nY++)
		{
			for(int nX = 0; nX < nWidth; nX++)
			{
				if(tmpBuf[nY * nWidth + nX] == 255)		// Is this a new component?, 255 == Object
				{
					temp_num++;

					tmpBuf[nY * nWidth + nX] = temp_num;
					
					StartX = nX, StartY = nY, EndX = nX, EndY= nY;

					__NRFIndNeighbor(tmpBuf, nWidth, nHeight, nX, nY, &StartX, &StartY, &EndX, &EndY, m_vPoint_out);

					if(__Area(tmpBuf, StartX, StartY, EndX, EndY, nWidth, temp_num) < m_nThreshold)
					{
		 				for(int k = StartY; k <= EndY; k++)
						{
							for(int l = StartX; l <= EndX; l++)
							{
								if(tmpBuf[k * nWidth + l] == temp_num)
									tmpBuf[k * nWidth + l] = 0;
							}
						}
						--temp_num;

						if(temp_num > 250)
							temp_num = 0;
					}
				}
			}
		}
		// 포인트 메모리 해제
		delete m_vPoint_out;

		//결과 보존
		nNumber = temp_num;
		
		//레이블링 수만큼 렉트 생성
		if( nNumber != _DEF_MAX_BLOBS )		
			m_rec_out = new CvRect [nNumber];
	
		//렉트 만들기
			if( nNumber != 0 )	
				DetectLabelingRegion(nNumber, tmpBuf, nWidth, nHeight,m_rec_out);

				for(int j=0; j<nHeight; j++)
					for(int i=0; i<nWidth ; i++)
						m_image_th2->imageData[j*m_image_th2->widthStep+i] = tmpBuf[j*nWidth+i];
		
				delete tmpBuf;
	
		//레이블링 수 보존		
		m_nBlobs_out = nNumber;
	
		//레이블링 영역 거르기
		int nMaxWidth	= m_in_height * 9 / 10;	   // 영상 가로 전체 크기의 90% 이상인 레이블은 제거
		int nMaxHeight	= m_in_width  * 9 / 10;	   // 영상 세로 전체 크기의 90% 이상인 레이블은 제거

		//최소영역과 최대영역 지정- 화면 크기에 영향 받음..
		_BlobSmallSizeConstraint( 5, 150, m_rec_out, &m_nBlobs_out);
		_BlobBigSizeConstraint(nMaxWidth, nMaxHeight,m_rec_out, &m_nBlobs_out);

		//앞으로 쓸 메모리 등록
		storage1 = cvCreateMemStorage(0);
		storage2 = cvCreateMemStorage(0);

		//변수 초기화
		CvPoint point;
		CvSeq* seq[10];
		CvSeq* hull;
		CvPoint end_pt;
		CvPoint center;

		//내보낼 데이터 초기화
		outData[0].x = 0, outData[0].y  = 0;
		outData[1].x = 0, outData[1].y  = 0;
		outData[2].x = 0, outData[2].y  = 0;

		int num = 0;
		int temp_x = 0;
		int temp_y = 0;
		int rect = 0;
		
		//만일을 대비하여 준비한 시퀸스 배열의 크기를 초과하지 않도록 조절
		//일단 한곳에서만 영상이 나오도록 조절..
		if(m_nBlobs_out > 1)
		{
			m_nBlobs_out = 1;
		}

		//레이블링 영역 내의 처리 시작 
		for( int i=0; i <  m_nBlobs_out; i++ )
		{
			//사각형 그리기에 필요한 두점 저장
			CvPoint	pt1 = cvPoint(	m_rec_out[i].x, m_rec_out[i].y );
			CvPoint pt2 = cvPoint(	pt1.x + m_rec_out[i].width,pt1.y + m_rec_out[i].height );

			// 컬러값 설정
			CvScalar color	= cvScalar( 0, 0, 255 );

			//레이블 사각형 그리기 - 확인용
			//cvDrawRect( m_image_dest, pt1, pt2, color);
			
			//레이블을 관심영역으로 지정할 이미지 생성
			temp_mask = cvCreateImage(cvSize(m_rec_out[i].width, m_rec_out[i].height),8,1);
			temp_mask2 = cvCreateImage(cvSize(m_rec_out[i].width, m_rec_out[i].height),8,1);
			
			//관심영역 지정
			cvSetImageROI(m_image_th, m_rec_out[i]);
				
			//관심영역 추출
			cvCopy(m_image_th, temp_mask, 0);

			//관심영역 해제
			cvResetImageROI(m_image_th);

			
			//관심영역 내의 오브젝트 처리를 위한 시퀸스 생성
			seq[i] = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2,sizeof(CvContour),sizeof(CvPoint), storage1);
			
			//관심영역에서 추출한이미지의 흰색 픽셀값으로 시퀸스 생성
			for(int j =0; j < temp_mask ->height ; j++)
			{
				for(int k = 0; k < temp_mask ->width; k++)
				{				
					if((unsigned char)temp_mask->imageData[j*temp_mask->widthStep+k] == 255)
					{
						point.x = k;		//흰색 픽셀 x좌표 저장
						point.y = j;		//흰색 픽셀 y좌표 저장
						cvSeqPush(seq[i], &point);	//시퀸스 구조체에 해당 좌표 삽입
						temp_x += point.x; //좌표 누적
						temp_y += point.y; //좌표 누적
						num++;             //픽셀 수 카운트

					}	
				}
			} 
			
			//좌표 초기화
			point.x				= 0;
			point.y				= 0;
			end_pt.x			= 0;
			end_pt.y			= 0;
			center.x			= 0;
			center.y			= 0;
			
			CvPoint dist_pt;			//중심점과의 최대거리를 찾을 컨백스헐 저장
			double fMaxDist		= 0;    //중심점과의 최대거리 저장
			double fDist		= 0;	//거리계산에 사용
		
	
			//중심점 찾기 - 픽셀의  평균값 찾기
			if(num != 0)
			{
				center.x			= (int)temp_x/num; //평균 좌표값 구하기
				center.y			= (int)temp_y/num; //평균 좌표값 구하기
			}

			//관심영역 설정
			cvSetImageROI(m_image_dest, m_rec_out[i]);

/////////컨백스헐 그리기////////
			if(seq[i]->total !=0)
			{	
				//컨백스헐 구하기
				hull = cvConvexHull2(seq[i], 0, CV_COUNTER_CLOCKWISE, 0);	
				point = **CV_GET_SEQ_ELEM(CvPoint*, hull,hull->total-1);

				//구한 컨백스헐 라인으로 그리기
				for(int x = 0; x < hull->total; x++)
				{
					CvPoint hull_pt = **CV_GET_SEQ_ELEM(CvPoint*, hull,x);

					//컨백스헐 라인 그리기
					//cvLine(m_image_dest, point, hull_pt, CV_RGB(255, 255, 0 ),2, 8);
					point = hull_pt;

					//최대 거리 구하기
					dist_pt =  **CV_GET_SEQ_ELEM(CvPoint*, hull,x);

					fDist = sqrt((double)((center.x - dist_pt.x) * (center.x - dist_pt.x) 
						+ (center.y - dist_pt.y) * (center.y - dist_pt.y)));

					if(fDist > fMaxDist)
					{
						max_pt = dist_pt;
						fMaxDist = fDist;
					}
				}
			}
			

			//중심점그리기
			cvCircle(m_image_dest,center,5, CV_RGB(0,0,255), 5);

			//내보낼 중심점 데이터 저장
			outData[0].x = center.x;
			outData[0].y = center.y;
	
			
////////마스크 만들기///////

			//중심점을 기준으로 그릴 마스크 이미지 생성
			circle_mask = cvCreateImage(cvGetSize(temp_mask), 8, 1);
			
			//바탕은 검은색으로
			cvSetZero(circle_mask);
			
			//흰색 원 - 손 영상과의 연산을 위해 바이너리 이미지에 그리기
			int radi = (int)m_rec_out[i].height/2.9; // 원 크기 수동조절..

			//흰색 원과 흰색 네모로 구성된 마스크 영상 생성을 위한 그리기
			cvCircle(circle_mask, center, radi, CV_RGB(255,255,255),CV_FILLED);
			cvDrawRect(circle_mask, cvPoint(center.x - radi, center.y),cvPoint(center.x + radi, pt2.y),
				 CV_RGB(255,255,255),CV_FILLED);

			//마스크 추출
			cvSub(temp_mask, circle_mask, temp_mask, 0);

	
///////관심영역 레이블링 - 손가락 끝 추출//////

			//변수 및 이미지 메모리 초기화
			int temp_num_in = 0;
			int StartX_in , StartY_in, EndX_in , EndY_in;
			int nNumber_in = 0;
			m_nThreshold_in	= 10;

			if( m_rec_in != NULL )
			{
				delete m_rec_in;

				m_rec_in	= NULL;
				m_nBlobs_in	= _DEF_MAX_BLOBS;
			}
			else
			{
				m_rec_in	= NULL;
				m_nBlobs_in	= _DEF_MAX_BLOBS;
			}

			if( temp_mask2 != NULL )	
				cvReleaseImage( &temp_mask2 );

			temp_mask2			= cvCloneImage( temp_mask );	

			//들어온 이미지의 크기 저장
			int nWidth	= temp_mask2->width;
			int nHeight = temp_mask2->height;
		
			//영상 크기만큼 버프 설정
			unsigned char* tmpBuf_in = new unsigned char [nWidth * nHeight];

			for(int j=0; j<nHeight ;j++)	
				for(int i=0; i<nWidth ;i++)
					//전 픽셀 순회
					tmpBuf_in[j*nWidth+i] = (unsigned char)temp_mask2->imageData[j*temp_mask2->widthStep+i];
				
	
		/////// 레이블링을 위한 포인트 초기화 ////////
			
			m_vPoint_in = new Visited [nWidth * nHeight];

			for(int nY = 0; nY < nHeight; nY++)
			{
				for(int nX = 0; nX < nWidth; nX++)
				{
					m_vPoint_in[nY * nWidth + nX].bVisitedFlag		= FALSE;
					m_vPoint_in[nY * nWidth + nX].ptReturnPoint.x	= nX;
					m_vPoint_in[nY * nWidth + nX].ptReturnPoint.y	= nY;
				}
			}

			////레이블링 수행
			for(int nY = 0; nY < nHeight; nY++)
			{
				for(int nX = 0; nX < nWidth; nX++)
				{
					if(tmpBuf_in[nY * nWidth + nX] == 255)		// Is this a new component?, 255 == Object
					{
						temp_num_in++;

						tmpBuf_in[nY * nWidth + nX] = temp_num_in;
						
						StartX_in = nX, StartY_in = nY, EndX_in = nX, EndY_in= nY;

						__NRFIndNeighbor(tmpBuf_in, nWidth, nHeight, nX, nY, 
							&StartX_in, &StartY_in, &EndX_in, &EndY_in,m_vPoint_in);

						if(__Area(tmpBuf_in, StartX_in, StartY_in, EndX_in, EndY_in, nWidth, temp_num_in) < m_nThreshold_in)
						{
		 					for(int k = StartY_in; k <= EndY_in; k++)
							{
								for(int l = StartX_in; l <= EndX_in; l++)
								{
									if(tmpBuf_in[k * nWidth + l] == temp_num_in)
										tmpBuf_in[k * nWidth + l] = 0;
								}
							}
							--temp_num_in;

							if(temp_num_in > 250)
								temp_num_in = 0;
						}
					}
				}
			}
			// 포인트 메모리 해제
			delete m_vPoint_in;

			//레이블링 수 보존
			nNumber_in = temp_num_in;

			if( nNumber_in != _DEF_MAX_BLOBS )		
				m_rec_in = new CvRect [nNumber_in];
		
				if( nNumber_in != 0 )	
					DetectLabelingRegion(nNumber_in, tmpBuf_in, nWidth, nHeight,m_rec_in);
				
				for(int j=0; j<nHeight; j++)
					for(int i=0; i<nWidth ; i++)
						temp_mask2->imageData[j*temp_mask2->widthStep+i] = tmpBuf_in[j*nWidth+i];
			
					delete tmpBuf_in;
				
			m_nBlobs_in = nNumber_in;

			//최소영역과 최대영역 설정
			_BlobSmallSizeConstraint( 5, 5, m_rec_in, &m_nBlobs_in);
			_BlobBigSizeConstraint( temp_mask2->width, temp_mask2->height,m_rec_in, &m_nBlobs_in);

			//선언 및 초기화
			CvPoint center_in;
			CvPoint point_in;
			
			point_in.x = 0;
			point_in.y = 0;
			center_in.x = 0;
			center_in.x = 0;
			CvSeq* seq_in[20];

			//준비한 시퀸스 배열크기를 초과하지 않도록 조절
			if(m_nBlobs_in > 20)
			{
				m_nBlobs_in =20;
			}

			for( int ni =0; ni <  m_nBlobs_in; ni++ )
			{		
				//사각형 그리기에 필요한 두 점 저장
				CvPoint	pt1 = cvPoint(	m_rec_in[ni].x, m_rec_in[ni].y );
				CvPoint pt2 = cvPoint(	pt1.x + m_rec_in[ni].width,pt1.y + m_rec_in[ni].height );

				//색상값 설정
				CvScalar color	= cvScalar( 255,0 , 255 );
				
				//레이블 사각형 그리기
				//cvDrawRect( m_image_dest, pt1, pt2, color);
				
				//처리할 손끝 마스크 생성할 메모리 할당
				in_mask = cvCreateImage(cvSize(m_rec_in[ni].width, m_rec_in[ni].height),8,1);

				//관심영역 설정
				cvSetImageROI(temp_mask, m_rec_in[ni]);
				
				//필요한 영역 복사
				cvCopy(temp_mask, in_mask, 0);

				//관심영역 해제
				cvResetImageROI(temp_mask);

				//관심영역 내의 오브젝트 처리를 위한 시퀸스 생성
				seq_in[ni] = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2,sizeof(CvContour),sizeof(CvPoint), storage2);

				//초기화
				int temp_x_in = 0;
				int temp_y_in = 0;	
				int num_in = 0;
				
				//관심영역에서 추출한이미지의 흰색 픽셀값으로 시퀸스 생성
				for(int j =0; j < in_mask ->height ; j++)
				{
					for(int k = 0; k < in_mask ->width; k++)
					{				
						if((unsigned char)in_mask->imageData[j*in_mask->widthStep+k] == 255)
						{
							point_in.x = k;		//흰색 픽셀 x좌표 저장
							point_in.y = j;		//흰색 픽셀 y좌표 저장
							cvSeqPush(seq_in[ni], &point_in);	//시퀸스 구조체에 해당 좌표 삽입
							temp_x_in += point_in.x; //좌표 누적
							temp_y_in += point_in.y; //좌표 누적
							num_in++;             //픽셀 수 카운트

						}

					}
				}
				
				//초기화
				max_pt_in.x = 0;
				max_pt_in.y = 0;					
				double fMaxDist_in	= 0;
				double fDist_in	= 0;
			
				//중심점 찾기 - 픽셀의  평균값 찾기
				if(num_in != 0)
				{
					center_in.x			= (int)temp_x_in/num_in + pt1.x; //평균 좌표값 구하기
					center_in.y			= (int)temp_y_in/num_in + pt1.y; //평균 좌표값 구하기
					
				}

				//우선 끝점이 2개일때만..
				if(m_nBlobs_in == 2)  
				{	
					//초기화
					finger_pt[ni].x = NULL;
					finger_pt[ni].y = NULL;
					finger_pt[ni].x = NULL;
					finger_pt[ni].y = NULL;

					if(seq_in[ni]->total !=0)
					{	
						//컨백스헐 구하기 - 윤곽선의 좌표 정보 겟
						CvSeq* hull_in = cvConvexHull2(seq_in[ni], 0, CV_COUNTER_CLOCKWISE, 0);	
						//point_in = **CV_GET_SEQ_ELEM(CvPoint*, hull_in,hull_in->total-1);


						//구한 컨백스헐 라인으로 그리기
						for(int nx = 0; nx < hull_in->total; nx++)
						{
							CvPoint hull_pt_in = **CV_GET_SEQ_ELEM(CvPoint*, hull_in,nx);
							hull_pt_in.x = hull_pt_in.x  + pt1.x;
							hull_pt_in.y = hull_pt_in.y + pt1.y;

							//중심점과 해당영역의 컨백스 헐 지점간의 거리 계산
							fDist_in = sqrt((double)((center.x - hull_pt_in.x) * (center.x - hull_pt_in.x) 
								+ (center.y - hull_pt_in.y) * (center.y - hull_pt_in.y)));

							//거리가 먼 점 찾기
							if(fDist_in > fMaxDist_in)
							{
								max_pt_in = hull_pt_in;
								fMaxDist_in = fDist_in;
								
							}
						}
					}				
				
					//최대점 보존
					finger_pt[ni].x = max_pt_in.x ;
					finger_pt[ni].y = max_pt_in.y ;
						
					//관심영역 해제할 경우의 값으로 보정
					finger_pt[ni].x = finger_pt[ni].x + m_rec_out[i].x;
					finger_pt[ni].y = finger_pt[ni].y + m_rec_out[i].y;		
				}
			}

			//관심영역 해제
			cvResetImageROI(m_image_dest);

			//와핑될 동영상이 참고할 두 점
			CvPoint Left;
			CvPoint Right;

			//오른쪽-왼쪽 정하기
			if(finger_pt[0].x != NULL && finger_pt[1].x != NULL 
				&&finger_pt[0].y != NULL && finger_pt[1].y != NULL)

			{
				//좌표 크기를 보고 방향을 정한다.
				if(finger_pt[0].x >= finger_pt[1].x)
				{
					Right = finger_pt[0];
					Left = finger_pt[1];
				}
				else
				{
					Right = finger_pt[1];
					Left = finger_pt[0];
					
				}
				//다시써야 하므로 초기화..
				finger_pt[0].x = NULL;
				finger_pt[0].y = NULL;
				finger_pt[1].x = NULL;
				finger_pt[1].y = NULL;

				//화면상의 위치 표시
				cvCircle(m_image_dest,Right, 3, CV_RGB(0,255,0), -1, 8, 0); // 오른쪽 - 그린
				cvCircle(m_image_dest,Left, 3, CV_RGB(255,0,0), -1, 8, 0); // 왼쪽 - 레드

				//내보낼 손 끝 두점의 데이터 저장
				outData[1].x = Right.x;
				outData[1].y = Right.y;

				outData[2].x = Left.x;
				outData[2].y = Left.y;

			}

			///동영상 와핑
			// 점이 두개 생길때만 동영상이 나타나므로 손끝 찾기가 원활이 안될 시에는 깜박거리는 현상 발생.
			// 주먹을 쥘 경우에도 원의 크기가 작아짐에 따라 손끝이라고 추정되는 점들이 생기는 경우 발생 
			// -별도의 제거 알고리즘이 필요할 듯..
			if(cvGrabFrame( m_video ))
			{	

				//캡처한 동영상을 이미지로 전환
				IplImage* m_image_video = cvRetrieveFrame( m_video);

				CvPoint2D32f ptSource[4], ptPrespective[4];

				//원래 영상 기본 좌표
				ptSource[0] = cvPoint2D32f(0, 0);
				ptSource[1] = cvPoint2D32f(m_image_video->width, 0);
				ptSource[2] = cvPoint2D32f(m_image_video->width, m_image_video->height);
				ptSource[3] = cvPoint2D32f(0, m_image_video->height);

				// 변환될 네 점의 좌표를 정렬 (LEFT-TOP - RIGHT-TOP - RIGHT-BOTTOM - LEFT_BOTTOM 순)	
				//손가락 사이에 따라서 영상 크기 확대 축소를 위한 가로/세로길이 계산
				//현재 3:2
				double temp_h = 0.0, temp_w = 0.0;

				temp_w = abs(Right.x - Left.x);
				temp_h = 2 * temp_w / 3;

				//대입할 좌표
				ptPrespective[0] = cvPoint2D32f(Right.x ,	(double)(Right.y - temp_h));
				ptPrespective[1] = cvPoint2D32f(Left.x ,	(double)(Left.y - temp_h));
				ptPrespective[2] = cvPoint2D32f(Left.x ,	Left.y );
				ptPrespective[3] = cvPoint2D32f(Right.x ,	Right.y	 );

				float newm[9];

				CvMat mat = cvMat(3, 3, CV_32F, newm); //변환행렬 초기화

				cvWarpPerspectiveQMatrix(ptSource, ptPrespective, &mat);//행렬 변환

				//반투명 처리
				cvSubS(m_image_video,cvScalarAll(100),m_image_video);
				cvZero(m_image_dest2);
				cvWarpPerspective(m_image_video, m_image_dest2, &mat, CV_INTER_LINEAR);
				cvAdd(m_image_dest2,m_image_dest, m_image_dest);

			}
			
			//사용이 끝난 메모리의 해제
			cvReleaseImage(&temp_mask);
			cvReleaseImage(&temp_mask2);
			cvReleaseImage(&circle_mask);
			cvReleaseImage(&in_mask);
			cvClearSeq(seq[i]);
			cvClearSeq(seq_in[i]);
		}

		// 보기 좋게 하기 위한 스트링 
		outString[0] = "Center";
		outString[1] = "Right";
		outString[2] = "Left";

//// DataOut
		// 모든 점이 존재 하면 출력 함
		if(outData[0].x != 0 && outData[0].y != 0)
		{
			if(outData[1].x != 0 && outData[1].y != 0)
			{
				if(outData[2].x != 0 && outData[2].y != 0)
				{
					for(int i = 0; i< 3; i++)
					{
						int data_x, data_y;

						data_x = outData[i].x;
						data_y = outData[i].y;

						PositionDataType base;

						base.setName(outString[i]);
						base.setX(data_x);
						base.setY(data_y);	

						//사용하지 않는 자리는 NULL로 채워야 출력이 안된다.
						base.setHeight(NULL);
						base.setWidth(NULL);
						base.setRadian(NULL);
						
						data.push_back(base);
					}

					PositionOut.push(data);
				}
			}
		}

//// ImageOut 
		// RawImage의 이미지 포인터 변수 할당
		RawImageData *pimage = result.getImage();
		
		// 입력된 이미지 사이즈 및 채널수로 로 재 설정
		pimage->resize(m_image_dest->width, m_image_dest->height, m_image_dest->nChannels);
		
		// 영상의 총 크기(pixels수) 취득
		int size = m_image_dest->width * m_image_dest->height * m_image_dest->nChannels;
		
		// 영상 데이터로부터 영상값만을 할당하기 위한 변수
		unsigned char *ptrdata = pimage->getData();
		
		// 현재 프레임 영상을 사이즈 만큼 memcpy
		memcpy(ptrdata, m_image_dest->imageData, size);

		// 포트아웃
		opros_any mdata = result;
		ImageOut.push(result);//전달

		delete pData;

		//사용한 메모리 해제
		cvReleaseImage(&m_image_YCrCb);
		cvReleaseImage(&m_Y);
		cvReleaseImage(&m_Cr);
		cvReleaseImage(&m_Cb);

	}

	return OPROS_SUCCESS;
}
	
ReturnType HandsMotionTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}





////////////////////////////////////////////////////////
//
//     
//	레이블링 관련
//
//
////////////////////////////////////////////////////////



// Blob labeling해서 얻어진 결과의 rec을 얻어냄 
void HandsMotionTracking::DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight,  CvRect* rect)
{
	int mX, mY;
	int nLabelIndex ;
	CvRect* m_recBlobs;
	
	m_recBlobs = rect;
	

	BOOL bFirstFlag[255] = {FALSE,};
	
	for(mY = 1; mY < nHeight - 1; mY++)
	{
		for(mX = 1; mX < nWidth - 1; mX++)
		{
			nLabelIndex = DataBuf[mY * nWidth + mX];

			if(nLabelIndex != 0)	// Is this a new component?, 255 == Object
			{
				if(bFirstFlag[nLabelIndex] == FALSE)
				{
					m_recBlobs[nLabelIndex-1].x			= mX;
					m_recBlobs[nLabelIndex-1].y			= mY;
					m_recBlobs[nLabelIndex-1].width		= 0;
					m_recBlobs[nLabelIndex-1].height	= 0;
				
					bFirstFlag[nLabelIndex] = TRUE;
				}
				else
				{
					int left	= m_recBlobs[nLabelIndex-1].x;
					int right	= left + m_recBlobs[nLabelIndex-1].width;
					int top		= m_recBlobs[nLabelIndex-1].y;
					int bottom	= top + m_recBlobs[nLabelIndex-1].height;

					if( left   >= mX )	left	= mX;
					if( right  <= mX )	right	= mX;
					if( top    >= mY )	top		= mY;
					if( bottom <= mY )	bottom	= mY;

					m_recBlobs[nLabelIndex-1].x			= left;
					m_recBlobs[nLabelIndex-1].y			= top;
					m_recBlobs[nLabelIndex-1].width		= right - left;
					m_recBlobs[nLabelIndex-1].height	= bottom - top;

				}
			}
				
		}
	}
	
}

// Blob Labeling을 실제 행하는 function
// 2000년 정보처리학회에 실린 논문 참조
int HandsMotionTracking::__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY,Visited* Point)
{
	CvPoint CurrentPoint;
	Visited* m_vPoint;

	m_vPoint = Point;
	CurrentPoint.x = nPosX;
	CurrentPoint.y = nPosY;

	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].bVisitedFlag    = TRUE;
	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].ptReturnPoint.x = nPosX;
	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].ptReturnPoint.y = nPosY;
			
	while(1)
	{
		if( (CurrentPoint.x != 0) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x - 1] == 255) )   // -X 방향
		{
			if( m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].bVisitedFlag == FALSE )
			{
				DataBuf[CurrentPoint.y  * nWidth + CurrentPoint.x  - 1]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].bVisitedFlag	= TRUE;
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].ptReturnPoint	= CurrentPoint;
				CurrentPoint.x--;
				
				if(CurrentPoint.x <= 0)
					CurrentPoint.x = 0;

				if(*StartX >= CurrentPoint.x)
					*StartX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.x != nWidth - 1) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1] == 255) )   // -X 방향
		{
			if( m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].bVisitedFlag == FALSE )
			{
				DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].bVisitedFlag	= TRUE;
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].ptReturnPoint	= CurrentPoint;
				CurrentPoint.x++;

				if(CurrentPoint.x >= nWidth - 1)
					CurrentPoint.x = nWidth - 1;
				
				if(*EndX <= CurrentPoint.x)
					*EndX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.y != 0) && (DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x] == 255) )   // -X 방향
		{
			if( m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].bVisitedFlag == FALSE )
			{
				DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].bVisitedFlag	= TRUE;
				m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].ptReturnPoint = CurrentPoint;
				CurrentPoint.y--;

				if(CurrentPoint.y <= 0)
					CurrentPoint.y = 0;

				if(*StartY >= CurrentPoint.y)
					*StartY = CurrentPoint.y;

				continue;
			}
		}
	
		if( (CurrentPoint.y != nHeight - 1) && (DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x] == 255) )   // -X 방향
		{
			if( m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].bVisitedFlag == FALSE )
			{
				DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].bVisitedFlag	= TRUE;
				m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].ptReturnPoint = CurrentPoint;
				CurrentPoint.y++;

				if(CurrentPoint.y >= nHeight - 1)
					CurrentPoint.y = nHeight - 1;

				if(*EndY <= CurrentPoint.y)
					*EndY = CurrentPoint.y;

				continue;
			}
		}
		
		if(		(CurrentPoint.x == m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint.x) 
			&&	(CurrentPoint.y == m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint.y) )
		{
			break;
		}
		else
		{
			CurrentPoint = m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint;
		}
	}

	return 0;
}

// 영역중 실제 blob의 칼라를 가진 영역의 크기를 획득
int HandsMotionTracking::__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel)
{
	int nArea = 0;
	int X, Y;

	for (Y = StartY; Y < EndY; Y++)
		for (X = StartX; X < EndX; X++)
			if (DataBuf[Y * nWidth + X] == nLevel)
				++nArea;

	return nArea;
}

/******************************************************************************************************/

// nWidth와 nHeight보다 작은 rec을 모두 삭제


void HandsMotionTracking::_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int *nRecNumber)
{
	if(*nRecNumber == 0)	return;

	int nX;
	int ntempRec = 0;

	CvRect* temp = new CvRect[*nRecNumber];

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		temp[nX] = rect[nX];
	}

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		if( (rect[nX].width > nWidth) && (rect[nX].height > nHeight) )
		{
			temp[ntempRec] = rect[nX];
			ntempRec++;
		}
	}

	*nRecNumber = ntempRec;

	for(nX = 0; nX < *nRecNumber; nX++)
		rect[nX] = temp[nX];

	delete temp;
}

// nWidth와 nHeight보다 큰 rec을 모두 삭제


void HandsMotionTracking::_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber)
{
	if(*nRecNumber == 0)	return;

	int nX;
	int ntempRec = 0;

	CvRect* temp = new CvRect [*nRecNumber];

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		temp[nX] = rect[nX];
	}

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		if( (rect[nX].width < nWidth) && (rect[nX].height < nHeight) )
		{
			temp[ntempRec] = rect[nX];
			ntempRec++;
		}
	}

	*nRecNumber = ntempRec;

	for(nX = 0; nX < *nRecNumber; nX++)
		rect[nX] = temp[nX];

	delete temp;
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
	return new HandsMotionTracking();
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
	return new HandsMotionTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

