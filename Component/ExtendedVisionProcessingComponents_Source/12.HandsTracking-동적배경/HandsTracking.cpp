
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



#include "HandsTracking.h"

//
// constructor declaration
//
#define _DEF_MAX_BLOBS		200
#define _DEF_MAX_LABEL		 30


int m_in_height;                // 처리할 이미지의 Height
int m_in_width;                 // 처리할 이미지의  Width

int INIT_TIME;					// 누적할 배경수치

//기본
IplImage* m_image_buff;			// Original Image
IplImage* m_image_dest;			// Convert Image
IplImage* m_image_curr;
IplImage* m_image_th;
IplImage* m_image_th2;
IplImage* m_image_shift;

//동적배경 연산에 필요한 이미지들
IplImage* m_image_acc;
IplImage* m_image_sgm;
IplImage* m_image_tmp;

int			m_nThreshold;			// 레이블링 스레스홀드 값
Visited*	m_vPoint_out;				// 레이블링시 방문정보
int			m_nBlobs_out;				// 레이블의 갯수
CvRect*		m_rec_out;				// 각 레이블 정보

CvPoint outPoint;					//내보낼 중심점 데이터

HandsTracking::HandsTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
HandsTracking::HandsTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

HandsTracking::~HandsTracking() {
}

void HandsTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);

	addPort("ImageIn", &ImageIn);

	// export variable setup


}

// Call back Declaration
ReturnType HandsTracking::onInitialize()
{
	// 이미지용 메모리 초기화
	m_image_buff		= NULL;     // 원본 이미지 - 동적배경 누적에 사용
	m_image_dest		= NULL;		// 출력용
	m_image_curr		= NULL;		// 원본이미지 - 업데이트 등
	m_image_th			= NULL;
	m_image_th2			= NULL;
	m_image_shift		= NULL;

	m_image_acc			= NULL;		// 누적	
	m_image_sgm			= NULL;		// 차연산
	m_image_tmp			= NULL;		// 차연산

	//변수 초기화
	m_in_width         = 0;
    m_in_height        = 0;

	INIT_TIME		   = 200;

	m_rec_out = NULL;
	m_nBlobs_out = 0;	
	m_nThreshold	= 0;

	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HandsTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onDestroy()
{
	// 이미지용 메모리의 해제
	//cvReleaseImage 는 IPlImage구조체를 삭제하고 메모리를 반환한다.
	cvReleaseImage(&m_image_buff);
	cvReleaseImage(&m_image_dest);
	cvReleaseImage(&m_image_curr);
    cvReleaseImage(&m_image_th);
    cvReleaseImage(&m_image_th2);
	cvReleaseImage(&m_image_shift);

	cvReleaseImage(&m_image_acc);
	cvReleaseImage(&m_image_sgm);
	cvReleaseImage(&m_image_tmp);
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onExecute()
{
	
	// 영상을 Inport로부터 취득
	opros_any *pData = ImageIn.pop();
	RawImage result;
	// 데이터 포트로 보낼 백터 선언
	std::vector<PositionDataType> data;

	if(pData != NULL){
		
		// 포트로 부터 이미지 취득
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();


		if(m_in_width != RawImage->getWidth() || m_in_height != RawImage->getHeight())
        {
			
			// 현재영상의 크기를 취득
			m_in_width = RawImage->getWidth();
			m_in_height = RawImage->getHeight();
			
			// 메모리 한번 해제해주고
			if(m_image_buff         != NULL)
				cvReleaseImage(&m_image_buff);
			if(m_image_dest         != NULL)
				cvReleaseImage(&m_image_dest);
			if(m_image_curr         != NULL)
				cvReleaseImage(&m_image_curr);
			if(m_image_th		    != NULL)
				cvReleaseImage(&m_image_th);
			if(m_image_th2		    != NULL)
				cvReleaseImage(&m_image_th2);
			if(m_image_shift           != NULL)
				cvReleaseImage(&m_image_shift);
			 
			if(m_image_acc          != NULL)
				cvReleaseImage(&m_image_acc);
			if(m_image_sgm          != NULL)
				cvReleaseImage(&m_image_sgm);
			if(m_image_tmp          != NULL)
				cvReleaseImage(&m_image_tmp);


			// 이미지용 메모리 할당
	        m_image_buff	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			m_image_curr	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			m_image_dest	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
			m_image_th		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
			m_image_th2		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);//영역 추출 이미지
			m_image_shift	= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

			m_image_acc		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 3);
			m_image_sgm		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 3);
			m_image_tmp		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 3);

			cvSetZero(m_image_acc);

			// 캠 영상 누적
			for(int i =0; i < INIT_TIME; i++)
			{
				memcpy(m_image_buff->imageData,RawImage->getData(), RawImage->getSize());
	
				//이미지 누적
				cvAcc(m_image_buff, m_image_acc);
			}
			//누적된 데이터 횟수만큼 나누어 평균값을 구함.
			cvConvertScale(m_image_acc, m_image_acc, 1.0/ INIT_TIME);

			std::cout <<"1 done" << std::endl;

			// 현재 영상과 평균값의 차연산
			cvSetZero(m_image_sgm);

			for(int i =0; i< INIT_TIME; i++)
			{
				memcpy(m_image_buff->imageData,RawImage->getData(), RawImage->getSize());
				
				//32F로 변환
				cvConvert(m_image_buff, m_image_tmp);
				
				// 평균값 acc와 현재영상과의 차연산 실행. 평균값과의 차이.
				cvSub(m_image_tmp, m_image_acc, m_image_tmp);

				// 제곱 후 로트 - 차연산 실행시 음수가 되는 부분을 양수로 바꾸기 위함.
				cvPow(m_image_tmp, m_image_tmp, 2.0); //제곱
				cvConvertScale(m_image_tmp, m_image_tmp, 2.0);
				cvPow(m_image_tmp, m_image_tmp, 0.5);	//루트
				
				//계산된 결과를 누적
				cvAcc(m_image_tmp, m_image_sgm);
			}
			
			cvConvertScale(m_image_sgm, m_image_sgm, 1.0/ INIT_TIME);
			
			std::cout <<"2 done" << std::endl;

		}
	//화상이미지 복사
		memcpy(m_image_curr->imageData,RawImage->getData(), RawImage->getSize());

		cvCopy(m_image_curr, m_image_dest);

///동적배경 업데이트 및 마스크 영상 획득

		//동적배경 업데이트에 필요한 이미지 생성
		IplImage* m_image_lower		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 3);
		IplImage* m_image_upper		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_32F, 3);
		IplImage* m_image_mask		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);
		IplImage* m_image_hand		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);	
		IplImage* m_image_avg		= cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);

		//파라메터 설정
		double B_PARAM = 1.0 / 40.0;
		double T_PARAM = 1.0 / 120.0;
		double zeta = 10.0;

		//배경과 물체 인식
		cvConvert(m_image_curr, m_image_tmp);

		//범위 만들기
		cvSub(m_image_acc, m_image_sgm, m_image_lower);
		cvSubS(m_image_lower, cvScalarAll(zeta), m_image_lower);
		
		cvAdd(m_image_acc, m_image_sgm, m_image_upper);
		cvAddS(m_image_upper, cvScalarAll(zeta), m_image_upper);
		cvInRange(m_image_tmp, m_image_lower, m_image_upper, m_image_mask); //마스크 영상 만들기
		

		//배경 영상의 업데이트
		cvSub(m_image_tmp, m_image_acc, m_image_tmp);
		cvPow(m_image_tmp, m_image_tmp, 2.0);
		cvConvertScale(m_image_tmp, m_image_tmp, 2.0);
		cvPow(m_image_tmp, m_image_tmp, 0.5);

		cvRunningAvg(m_image_curr, m_image_acc, B_PARAM, m_image_mask);
		cvRunningAvg(m_image_tmp, m_image_sgm, B_PARAM, m_image_mask);

		cvNot(m_image_mask, m_image_mask);

		//배경에 물체가 검출 되었을 경우, 이 물체가 시간이 지나도 움직임이 없으면 배경으로 처리.
		cvRunningAvg(m_image_tmp, m_image_sgm, T_PARAM, m_image_mask);
		
		//보존
		cvCopy(m_image_mask, m_image_avg, 0 );

		cvErode  (m_image_avg, m_image_avg, NULL, 2);//침식	
		cvDilate (m_image_avg, m_image_avg, NULL, 2);//팽창

///스킨추출 영상 획득		

		//살색 추출에 필요한 이미지 생성
		IplImage* m_image_YCrCb = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 3);
		IplImage* m_Y  = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);
		IplImage* m_Cr = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);
		IplImage* m_Cb = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);

		//살색인지 비교할 이미지 픽셀 데이터 저장 변수
		unsigned char m_Cr_val;
		unsigned char m_Cb_val;

		cvCvtColor(m_image_curr, m_image_YCrCb, CV_RGB2YCrCb);   //RGB - > YCrCV 변환

		cvSplit(m_image_YCrCb, m_Y, m_Cr, m_Cb, NULL);   //채널 분리

		// 살색추출
		for(int i=0;i<m_image_curr->height;i++)            
		{
			for(int j=0;j<m_image_curr->width;j++)
			{  
				//픽셀 데이터 저장
				m_Cr_val = (unsigned char)m_Cr->imageData[i*m_Cr->widthStep+j];
				m_Cb_val = (unsigned char)m_Cb->imageData[i*m_Cb->widthStep+j];

				//살색영역인지 아닌지 판별
				if( (77 <= m_Cr_val) && (m_Cr_val <= 127) && (133 <= m_Cb_val) && (m_Cb_val <= 173) )
				{
					// 살색부분은 하얀색
					m_image_curr->imageData[i*m_image_curr->widthStep+j*3+0] = (unsigned char)255; 
					m_image_curr->imageData[i*m_image_curr->widthStep+j*3+1] = (unsigned char)255;
					m_image_curr->imageData[i*m_image_curr->widthStep+j*3+2] = (unsigned char)255; 
				}
				else
				{ 
					// 나머지는 검정색
					m_image_curr->imageData[i*m_image_curr->widthStep+j*3+0]= 0; 
					m_image_curr->imageData[i*m_image_curr->widthStep+j*3+1]= 0;
					m_image_curr->imageData[i*m_image_curr->widthStep+j*3+2]= 0;
				}
			}
		}

		//RGB -> GRAY
		cvCvtColor(m_image_curr, m_image_th, CV_RGB2GRAY); 

		cvDilate (m_image_th, m_image_th, NULL, 2);//팽창
		cvErode  (m_image_th, m_image_th, NULL, 2);//침식	

		//스킨컬러 영상과, 동적배경 영상 AND 연산
		cvAnd(m_image_th, m_image_avg,m_image_hand, 0);


//변수 및 이미지 메모리 초기화
		int temp_num = 0;
		int StartX , StartY, EndX , EndY;
		int nNumber = 0;
		m_nBlobs_out = 0;
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

		m_image_th2			= cvCloneImage( m_image_hand );

		//해당 이미지의 크기 저장
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
		// nThreshold보다 작은 영역을 제외한 나머지를 blob으로 획득
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
						m_image_th2->imageData[j*m_image_th->widthStep+i] = tmpBuf[j*nWidth+i];
		
				delete tmpBuf;
	
		//레이블링 수 보존		
		m_nBlobs_out = nNumber;
	
		//레이블링 영역 거르기
		int nMaxWidth	= m_in_height * 8 / 10;	   // 영상 가로 전체 크기의 80% 이상인 레이블은 제거
		int nMaxHeight	= m_in_width  * 8 / 10;	   // 영상 세로 전체 크기의 80% 이상인 레이블은 제거

		//최소영역과 최대영역 지정- 화면 크기에 영향 받음..
		_BlobSmallSizeConstraint( 80, 120, m_rec_out, &m_nBlobs_out);
		_BlobBigSizeConstraint(nMaxWidth, nMaxHeight,m_rec_out, &m_nBlobs_out);

		cvCopy(m_image_th, m_image_shift,0);
		
		//변수 초기화
		CvPoint point;
		CvPoint temp_pt1;
		CvPoint temp_pt2;
							
		CvBox2D track_box;
		CvConnectedComp track_comp;
		CvRect track_window;

		//내보낼 중심점 데이터 담을 변수 초기화
		outPoint.x = 0;
		outPoint.y = 0;

		for( int i=0; i <  m_nBlobs_out; i++ )
		{

			CvPoint	pt1 = cvPoint(	m_rec_out[i].x, m_rec_out[i].y );
			CvPoint pt2 = cvPoint(	pt1.x + m_rec_out[i].width,pt1.y + m_rec_out[i].height );
			//레이블 사각형 그리기 -레이블링 사각형이나 캠시프트 사각영역이나 비슷하게 그려짐..
			//cvDrawRect( m_image_dest, pt1, pt2, CV_RGB(255,0,0),2);
	
			//레이블링으로 검출한 영역 보존
			track_window = m_rec_out[i];
			
			//추적... 제대로 되고있는지는..
			cvCamShift(m_image_shift, track_window , cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1),
				&track_comp, &track_box);
			
			//갱신
			track_window = track_comp.rect;

			//박스 중심점
			point.x = (int)(track_box.center.x);
			point.y = (int)(track_box.center.y); 

			//중심점 표시
			cvCircle(m_image_dest,point,5, CV_RGB(0,0,255), -1);

			//내보낼 중심 데이터 저장
			outPoint.x = point.x;
			outPoint.y = point.y;

			temp_pt1.x = track_window.x;
			temp_pt1.y = track_window.y;
			temp_pt2.x = track_window.x + track_window.width;
			temp_pt2.y = track_window.y + track_window.height;

			//추적 영역 그리기
			cvDrawRect( m_image_dest, temp_pt1, temp_pt2, CV_RGB(255,0,0), 2);
		
		}


//// DataOut
		//중심점이 있으면 데이터를 내보낸다
		if(outPoint.x != 0 && outPoint.y !=0 )
		{
			int data_x, data_y;

			data_x = outPoint.x;
			data_y = outPoint.y;

			PositionDataType base;

			base.setName("");
			base.setX(data_x);
			base.setY(data_y);

			base.setHeight(NULL);
			base.setWidth(NULL);
			base.setRadian(NULL);
			
			data.push_back(base);
	
			PositionOut.push(data);
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
	
		cvReleaseImage(&m_image_lower);
		cvReleaseImage(&m_image_upper);
		cvReleaseImage(&m_image_mask);
		cvReleaseImage(&m_image_hand);
		cvReleaseImage(&m_image_avg);

		cvReleaseImage(&m_image_YCrCb);
		cvReleaseImage(&m_Y);
		cvReleaseImage(&m_Cr);
		cvReleaseImage(&m_Cb);

	}

	return OPROS_SUCCESS;
}
	
ReturnType HandsTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onPeriodChanged()
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
void HandsTracking::DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight,  CvRect* rect)
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
int HandsTracking::__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY,Visited* Point)
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
int HandsTracking::__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel)
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


void HandsTracking::_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int *nRecNumber)
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


void HandsTracking::_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber)
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
	return new HandsTracking();
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
	return new HandsTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

