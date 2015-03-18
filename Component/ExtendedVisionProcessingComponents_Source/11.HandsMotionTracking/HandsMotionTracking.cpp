
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

IplImage* m_image_buff;			// ���� ����
IplImage* m_image_dest;			// ��� ���� 
IplImage* m_image_dest2;		// �ӽ� 3ä�� ���� 
 
IplImage* m_image_th;			// ��������
IplImage* m_image_th2;			// ��������
IplImage* temp_mask;			// ����ũ
IplImage* temp_mask2;			// ����ũ
IplImage* circle_mask;			// ����ũ
IplImage* in_mask;				// ����ũ
	
CvCapture* m_video;				// ������ ������ ����

bool video_flag;				// ���� ��� ��Ʈ�� �÷���

CvMemStorage* storage1;			// �޸� �Ҵ�
CvMemStorage* storage2;			// �޸� �Ҵ�

CvPoint		max_pt;             // �߽������� �ִ�Ÿ��� �ִ� ��ǥ ����
CvPoint		max_pt_in;			// �ճ� �������� �߽����� �ִ�Ÿ��� �ִ� ��ǥ ����
CvPoint     finger_pt[2];		// ������ ������ ����� �ճ� ���� 

int m_in_width;
int m_in_height;

CvPoint outData[3];				// ������ ������ ���� - �߽����� ������ ���ε� �� ��
std::string outString[3];		// ��� ������ �������� ��Ʈ���� �Բ�

/***** ���̺� ���� ���� *******/	

	IplImage*	m_Image;			// ���̺��� ���� �̹���
	int			m_nThreshold;		// ���̺� ������Ȧ�� ��
	Visited*	m_vPoint_out;		// ���̺��� �湮����
	int			m_nBlobs_out;		// ���̺��� ����
	CvRect*		m_rec_out;			// �� ���̺� ����
	
	
	int			m_nThreshold_in;	// ���̺� ������Ȧ�� ��
	Visited*	m_vPoint_in;		// ���̺��� �湮����
	int			m_nBlobs_in;		// ���̺��� ����
	CvRect*		m_rec_in;			// �� ���̺� ����


	

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
	// �̹����� �޸� �ʱ�ȭ
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

	//���� �ʱ�ȭ
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
	//��ž �Ǿ��� ��ŸƮ ��ų�� ���� ��� �ǵ���
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
	// �̹����� �޸��� ����	
	cvReleaseImage(&m_image_buff);
    cvReleaseImage(&m_image_dest);
	cvReleaseImage(&m_image_dest2);
    cvReleaseImage(&m_image_th);
	cvReleaseImage(&m_image_th2);
    cvReleaseImage(&temp_mask);
    cvReleaseImage(&temp_mask2);
	cvReleaseImage(&circle_mask);
    cvReleaseImage(&in_mask);

	// �޸� ����
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
	// ������ Inport�κ��� ���
	opros_any *pData = ImageIn.pop();
	RawImage result;

	//�ƿ� ������
	std::vector<PositionDataType> data;


	if(pData != NULL){
		
		// ��Ʈ�� ���� �̹��� ���
		RawImage Image = ImageIn.getContent(*pData);
		RawImageData *RawImage = Image.getImage();


		// ���翵���� ũ�⸦ ���
		m_in_width = RawImage->getWidth();
		m_in_height = RawImage->getHeight();

		// �޸� �ѹ� �������ְ�
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

		// �̹����� �޸� �Ҵ�
        m_image_buff   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);//���� �̹���
		m_image_dest   = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);
		m_image_dest2  = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 3);

		m_image_th     = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);//���� ���� �̹���
		m_image_th2    = cvCreateImage(cvSize(m_in_width, m_in_height), IPL_DEPTH_8U, 1);//���� ���� �̹���
		
		
		if(!video_flag)
		{
			std::string cpath = getProperty("opros.component.dir");
			std::string file = getProperty("VideoFile");
			if (file == "") file = "sample.avi";

			std::string path = cpath + file;

			m_video	= NULL;
			m_video = cvCreateFileCapture(path.c_str()); //����
			video_flag = true;// ������ ��� ���� ������� ����
			
		}

		// ���� ���� ������ Ȯ��!memcpy
		memcpy(m_image_buff->imageData, RawImage->getData(), RawImage->getSize());

		// ��¿�
		cvCopy(m_image_buff, m_image_dest, 0);

		// ���� �и��� �̹���
		IplImage* m_image_YCrCb = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 3);
		IplImage* m_Y  = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);
		IplImage* m_Cr = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);
		IplImage* m_Cb = cvCreateImage(cvGetSize(m_image_buff), IPL_DEPTH_8U, 1);

		cvCvtColor(m_image_buff, m_image_YCrCb, CV_RGB2YCrCb);   //RGB - > YCrCV ��ȯ

		cvSplit(m_image_YCrCb, m_Y, m_Cr, m_Cb, NULL);   //ä�� �и�

		//������ �ʿ��� ���� �ȼ� ������ ���� ����
		unsigned char m_Cr_val = 0;			
		unsigned char m_Cb_val = 0;

		// �������
		for(int i=0;i<m_image_buff->height;i++)            
		{
			for(int j=0;j<m_image_buff->width;j++)
			{  
				//Cr ������ Cb ���� ����
				m_Cr_val = (unsigned char)m_Cr->imageData[i*m_Cr->widthStep+j];
				m_Cb_val = (unsigned char)m_Cb->imageData[i*m_Cb->widthStep+j];

				//����� �ش��ϴ� �������� �˻�
				if( (77 <= m_Cr_val) && (m_Cr_val <= 127) && (133 <= m_Cb_val) && (m_Cb_val <= 173) )
				{
					// ����κ��� �Ͼ��
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+0] = (unsigned char)255; 
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+1] = (unsigned char)255;
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+2] = (unsigned char)255; 
				}
				else
				{ 
					// �������� ������
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+0]= 0;
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+1]= 0;
					m_image_buff->imageData[i*m_image_buff->widthStep+j*3+2]= 0;
				}
			}
		}
		
		//��� ������ ������ ����ȭ
		cvCvtColor(m_image_buff, m_image_th, CV_RGB2GRAY); 

		//�⿵ ���Ÿ� ���� ����
		cvDilate (m_image_th, m_image_th, NULL, 2);//��â
		cvErode  (m_image_th, m_image_th, NULL, 2);//ħ��	

		//���� �� �̹��� �޸� �ʱ�ȭ
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

		//���̺� �� ���� ���� ����
		m_image_th2			= cvCloneImage( m_image_th );

		//���̺� �� �̹����� ũ�� ����
		int nWidth	= m_image_th2->width;
		int nHeight = m_image_th2->height;

		//�ش� ���� ũ�⸸ŭ ���� ����
		unsigned char* tmpBuf = new unsigned char [nWidth * nHeight];

		for(int j=0; j<nHeight ;j++)	
			for(int i=0; i<nWidth ;i++)	
				//�� �ȼ� ��ȸ
				tmpBuf[j*nWidth+i] = (unsigned char)m_image_th2->imageData[j*m_image_th2->widthStep+i];
		
////// ���̺��� ���� ����Ʈ �ʱ�ȭ

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

////// ���̺� ����
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
		// ����Ʈ �޸� ����
		delete m_vPoint_out;

		//��� ����
		nNumber = temp_num;
		
		//���̺� ����ŭ ��Ʈ ����
		if( nNumber != _DEF_MAX_BLOBS )		
			m_rec_out = new CvRect [nNumber];
	
		//��Ʈ �����
			if( nNumber != 0 )	
				DetectLabelingRegion(nNumber, tmpBuf, nWidth, nHeight,m_rec_out);

				for(int j=0; j<nHeight; j++)
					for(int i=0; i<nWidth ; i++)
						m_image_th2->imageData[j*m_image_th2->widthStep+i] = tmpBuf[j*nWidth+i];
		
				delete tmpBuf;
	
		//���̺� �� ����		
		m_nBlobs_out = nNumber;
	
		//���̺� ���� �Ÿ���
		int nMaxWidth	= m_in_height * 9 / 10;	   // ���� ���� ��ü ũ���� 90% �̻��� ���̺��� ����
		int nMaxHeight	= m_in_width  * 9 / 10;	   // ���� ���� ��ü ũ���� 90% �̻��� ���̺��� ����

		//�ּҿ����� �ִ뿵�� ����- ȭ�� ũ�⿡ ���� ����..
		_BlobSmallSizeConstraint( 5, 150, m_rec_out, &m_nBlobs_out);
		_BlobBigSizeConstraint(nMaxWidth, nMaxHeight,m_rec_out, &m_nBlobs_out);

		//������ �� �޸� ���
		storage1 = cvCreateMemStorage(0);
		storage2 = cvCreateMemStorage(0);

		//���� �ʱ�ȭ
		CvPoint point;
		CvSeq* seq[10];
		CvSeq* hull;
		CvPoint end_pt;
		CvPoint center;

		//������ ������ �ʱ�ȭ
		outData[0].x = 0, outData[0].y  = 0;
		outData[1].x = 0, outData[1].y  = 0;
		outData[2].x = 0, outData[2].y  = 0;

		int num = 0;
		int temp_x = 0;
		int temp_y = 0;
		int rect = 0;
		
		//������ ����Ͽ� �غ��� ������ �迭�� ũ�⸦ �ʰ����� �ʵ��� ����
		//�ϴ� �Ѱ������� ������ �������� ����..
		if(m_nBlobs_out > 1)
		{
			m_nBlobs_out = 1;
		}

		//���̺� ���� ���� ó�� ���� 
		for( int i=0; i <  m_nBlobs_out; i++ )
		{
			//�簢�� �׸��⿡ �ʿ��� ���� ����
			CvPoint	pt1 = cvPoint(	m_rec_out[i].x, m_rec_out[i].y );
			CvPoint pt2 = cvPoint(	pt1.x + m_rec_out[i].width,pt1.y + m_rec_out[i].height );

			// �÷��� ����
			CvScalar color	= cvScalar( 0, 0, 255 );

			//���̺� �簢�� �׸��� - Ȯ�ο�
			//cvDrawRect( m_image_dest, pt1, pt2, color);
			
			//���̺��� ���ɿ������� ������ �̹��� ����
			temp_mask = cvCreateImage(cvSize(m_rec_out[i].width, m_rec_out[i].height),8,1);
			temp_mask2 = cvCreateImage(cvSize(m_rec_out[i].width, m_rec_out[i].height),8,1);
			
			//���ɿ��� ����
			cvSetImageROI(m_image_th, m_rec_out[i]);
				
			//���ɿ��� ����
			cvCopy(m_image_th, temp_mask, 0);

			//���ɿ��� ����
			cvResetImageROI(m_image_th);

			
			//���ɿ��� ���� ������Ʈ ó���� ���� ������ ����
			seq[i] = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2,sizeof(CvContour),sizeof(CvPoint), storage1);
			
			//���ɿ������� �������̹����� ��� �ȼ������� ������ ����
			for(int j =0; j < temp_mask ->height ; j++)
			{
				for(int k = 0; k < temp_mask ->width; k++)
				{				
					if((unsigned char)temp_mask->imageData[j*temp_mask->widthStep+k] == 255)
					{
						point.x = k;		//��� �ȼ� x��ǥ ����
						point.y = j;		//��� �ȼ� y��ǥ ����
						cvSeqPush(seq[i], &point);	//������ ����ü�� �ش� ��ǥ ����
						temp_x += point.x; //��ǥ ����
						temp_y += point.y; //��ǥ ����
						num++;             //�ȼ� �� ī��Ʈ

					}	
				}
			} 
			
			//��ǥ �ʱ�ȭ
			point.x				= 0;
			point.y				= 0;
			end_pt.x			= 0;
			end_pt.y			= 0;
			center.x			= 0;
			center.y			= 0;
			
			CvPoint dist_pt;			//�߽������� �ִ�Ÿ��� ã�� ���齺�� ����
			double fMaxDist		= 0;    //�߽������� �ִ�Ÿ� ����
			double fDist		= 0;	//�Ÿ���꿡 ���
		
	
			//�߽��� ã�� - �ȼ���  ��հ� ã��
			if(num != 0)
			{
				center.x			= (int)temp_x/num; //��� ��ǥ�� ���ϱ�
				center.y			= (int)temp_y/num; //��� ��ǥ�� ���ϱ�
			}

			//���ɿ��� ����
			cvSetImageROI(m_image_dest, m_rec_out[i]);

/////////���齺�� �׸���////////
			if(seq[i]->total !=0)
			{	
				//���齺�� ���ϱ�
				hull = cvConvexHull2(seq[i], 0, CV_COUNTER_CLOCKWISE, 0);	
				point = **CV_GET_SEQ_ELEM(CvPoint*, hull,hull->total-1);

				//���� ���齺�� �������� �׸���
				for(int x = 0; x < hull->total; x++)
				{
					CvPoint hull_pt = **CV_GET_SEQ_ELEM(CvPoint*, hull,x);

					//���齺�� ���� �׸���
					//cvLine(m_image_dest, point, hull_pt, CV_RGB(255, 255, 0 ),2, 8);
					point = hull_pt;

					//�ִ� �Ÿ� ���ϱ�
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
			

			//�߽����׸���
			cvCircle(m_image_dest,center,5, CV_RGB(0,0,255), 5);

			//������ �߽��� ������ ����
			outData[0].x = center.x;
			outData[0].y = center.y;
	
			
////////����ũ �����///////

			//�߽����� �������� �׸� ����ũ �̹��� ����
			circle_mask = cvCreateImage(cvGetSize(temp_mask), 8, 1);
			
			//������ ����������
			cvSetZero(circle_mask);
			
			//��� �� - �� ������� ������ ���� ���̳ʸ� �̹����� �׸���
			int radi = (int)m_rec_out[i].height/2.9; // �� ũ�� ��������..

			//��� ���� ��� �׸�� ������ ����ũ ���� ������ ���� �׸���
			cvCircle(circle_mask, center, radi, CV_RGB(255,255,255),CV_FILLED);
			cvDrawRect(circle_mask, cvPoint(center.x - radi, center.y),cvPoint(center.x + radi, pt2.y),
				 CV_RGB(255,255,255),CV_FILLED);

			//����ũ ����
			cvSub(temp_mask, circle_mask, temp_mask, 0);

	
///////���ɿ��� ���̺� - �հ��� �� ����//////

			//���� �� �̹��� �޸� �ʱ�ȭ
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

			//���� �̹����� ũ�� ����
			int nWidth	= temp_mask2->width;
			int nHeight = temp_mask2->height;
		
			//���� ũ�⸸ŭ ���� ����
			unsigned char* tmpBuf_in = new unsigned char [nWidth * nHeight];

			for(int j=0; j<nHeight ;j++)	
				for(int i=0; i<nWidth ;i++)
					//�� �ȼ� ��ȸ
					tmpBuf_in[j*nWidth+i] = (unsigned char)temp_mask2->imageData[j*temp_mask2->widthStep+i];
				
	
		/////// ���̺��� ���� ����Ʈ �ʱ�ȭ ////////
			
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

			////���̺� ����
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
			// ����Ʈ �޸� ����
			delete m_vPoint_in;

			//���̺� �� ����
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

			//�ּҿ����� �ִ뿵�� ����
			_BlobSmallSizeConstraint( 5, 5, m_rec_in, &m_nBlobs_in);
			_BlobBigSizeConstraint( temp_mask2->width, temp_mask2->height,m_rec_in, &m_nBlobs_in);

			//���� �� �ʱ�ȭ
			CvPoint center_in;
			CvPoint point_in;
			
			point_in.x = 0;
			point_in.y = 0;
			center_in.x = 0;
			center_in.x = 0;
			CvSeq* seq_in[20];

			//�غ��� ������ �迭ũ�⸦ �ʰ����� �ʵ��� ����
			if(m_nBlobs_in > 20)
			{
				m_nBlobs_in =20;
			}

			for( int ni =0; ni <  m_nBlobs_in; ni++ )
			{		
				//�簢�� �׸��⿡ �ʿ��� �� �� ����
				CvPoint	pt1 = cvPoint(	m_rec_in[ni].x, m_rec_in[ni].y );
				CvPoint pt2 = cvPoint(	pt1.x + m_rec_in[ni].width,pt1.y + m_rec_in[ni].height );

				//���� ����
				CvScalar color	= cvScalar( 255,0 , 255 );
				
				//���̺� �簢�� �׸���
				//cvDrawRect( m_image_dest, pt1, pt2, color);
				
				//ó���� �ճ� ����ũ ������ �޸� �Ҵ�
				in_mask = cvCreateImage(cvSize(m_rec_in[ni].width, m_rec_in[ni].height),8,1);

				//���ɿ��� ����
				cvSetImageROI(temp_mask, m_rec_in[ni]);
				
				//�ʿ��� ���� ����
				cvCopy(temp_mask, in_mask, 0);

				//���ɿ��� ����
				cvResetImageROI(temp_mask);

				//���ɿ��� ���� ������Ʈ ó���� ���� ������ ����
				seq_in[ni] = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2,sizeof(CvContour),sizeof(CvPoint), storage2);

				//�ʱ�ȭ
				int temp_x_in = 0;
				int temp_y_in = 0;	
				int num_in = 0;
				
				//���ɿ������� �������̹����� ��� �ȼ������� ������ ����
				for(int j =0; j < in_mask ->height ; j++)
				{
					for(int k = 0; k < in_mask ->width; k++)
					{				
						if((unsigned char)in_mask->imageData[j*in_mask->widthStep+k] == 255)
						{
							point_in.x = k;		//��� �ȼ� x��ǥ ����
							point_in.y = j;		//��� �ȼ� y��ǥ ����
							cvSeqPush(seq_in[ni], &point_in);	//������ ����ü�� �ش� ��ǥ ����
							temp_x_in += point_in.x; //��ǥ ����
							temp_y_in += point_in.y; //��ǥ ����
							num_in++;             //�ȼ� �� ī��Ʈ

						}

					}
				}
				
				//�ʱ�ȭ
				max_pt_in.x = 0;
				max_pt_in.y = 0;					
				double fMaxDist_in	= 0;
				double fDist_in	= 0;
			
				//�߽��� ã�� - �ȼ���  ��հ� ã��
				if(num_in != 0)
				{
					center_in.x			= (int)temp_x_in/num_in + pt1.x; //��� ��ǥ�� ���ϱ�
					center_in.y			= (int)temp_y_in/num_in + pt1.y; //��� ��ǥ�� ���ϱ�
					
				}

				//�켱 ������ 2���϶���..
				if(m_nBlobs_in == 2)  
				{	
					//�ʱ�ȭ
					finger_pt[ni].x = NULL;
					finger_pt[ni].y = NULL;
					finger_pt[ni].x = NULL;
					finger_pt[ni].y = NULL;

					if(seq_in[ni]->total !=0)
					{	
						//���齺�� ���ϱ� - �������� ��ǥ ���� ��
						CvSeq* hull_in = cvConvexHull2(seq_in[ni], 0, CV_COUNTER_CLOCKWISE, 0);	
						//point_in = **CV_GET_SEQ_ELEM(CvPoint*, hull_in,hull_in->total-1);


						//���� ���齺�� �������� �׸���
						for(int nx = 0; nx < hull_in->total; nx++)
						{
							CvPoint hull_pt_in = **CV_GET_SEQ_ELEM(CvPoint*, hull_in,nx);
							hull_pt_in.x = hull_pt_in.x  + pt1.x;
							hull_pt_in.y = hull_pt_in.y + pt1.y;

							//�߽����� �ش翵���� ���齺 �� �������� �Ÿ� ���
							fDist_in = sqrt((double)((center.x - hull_pt_in.x) * (center.x - hull_pt_in.x) 
								+ (center.y - hull_pt_in.y) * (center.y - hull_pt_in.y)));

							//�Ÿ��� �� �� ã��
							if(fDist_in > fMaxDist_in)
							{
								max_pt_in = hull_pt_in;
								fMaxDist_in = fDist_in;
								
							}
						}
					}				
				
					//�ִ��� ����
					finger_pt[ni].x = max_pt_in.x ;
					finger_pt[ni].y = max_pt_in.y ;
						
					//���ɿ��� ������ ����� ������ ����
					finger_pt[ni].x = finger_pt[ni].x + m_rec_out[i].x;
					finger_pt[ni].y = finger_pt[ni].y + m_rec_out[i].y;		
				}
			}

			//���ɿ��� ����
			cvResetImageROI(m_image_dest);

			//���ε� �������� ������ �� ��
			CvPoint Left;
			CvPoint Right;

			//������-���� ���ϱ�
			if(finger_pt[0].x != NULL && finger_pt[1].x != NULL 
				&&finger_pt[0].y != NULL && finger_pt[1].y != NULL)

			{
				//��ǥ ũ�⸦ ���� ������ ���Ѵ�.
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
				//�ٽý�� �ϹǷ� �ʱ�ȭ..
				finger_pt[0].x = NULL;
				finger_pt[0].y = NULL;
				finger_pt[1].x = NULL;
				finger_pt[1].y = NULL;

				//ȭ����� ��ġ ǥ��
				cvCircle(m_image_dest,Right, 3, CV_RGB(0,255,0), -1, 8, 0); // ������ - �׸�
				cvCircle(m_image_dest,Left, 3, CV_RGB(255,0,0), -1, 8, 0); // ���� - ����

				//������ �� �� ������ ������ ����
				outData[1].x = Right.x;
				outData[1].y = Right.y;

				outData[2].x = Left.x;
				outData[2].y = Left.y;

			}

			///������ ����
			// ���� �ΰ� ���涧�� �������� ��Ÿ���Ƿ� �ճ� ã�Ⱑ ��Ȱ�� �ȵ� �ÿ��� ���ڰŸ��� ���� �߻�.
			// �ָ��� �� ��쿡�� ���� ũ�Ⱑ �۾����� ���� �ճ��̶�� �����Ǵ� ������ ����� ��� �߻� 
			// -������ ���� �˰����� �ʿ��� ��..
			if(cvGrabFrame( m_video ))
			{	

				//ĸó�� �������� �̹����� ��ȯ
				IplImage* m_image_video = cvRetrieveFrame( m_video);

				CvPoint2D32f ptSource[4], ptPrespective[4];

				//���� ���� �⺻ ��ǥ
				ptSource[0] = cvPoint2D32f(0, 0);
				ptSource[1] = cvPoint2D32f(m_image_video->width, 0);
				ptSource[2] = cvPoint2D32f(m_image_video->width, m_image_video->height);
				ptSource[3] = cvPoint2D32f(0, m_image_video->height);

				// ��ȯ�� �� ���� ��ǥ�� ���� (LEFT-TOP - RIGHT-TOP - RIGHT-BOTTOM - LEFT_BOTTOM ��)	
				//�հ��� ���̿� ���� ���� ũ�� Ȯ�� ��Ҹ� ���� ����/���α��� ���
				//���� 3:2
				double temp_h = 0.0, temp_w = 0.0;

				temp_w = abs(Right.x - Left.x);
				temp_h = 2 * temp_w / 3;

				//������ ��ǥ
				ptPrespective[0] = cvPoint2D32f(Right.x ,	(double)(Right.y - temp_h));
				ptPrespective[1] = cvPoint2D32f(Left.x ,	(double)(Left.y - temp_h));
				ptPrespective[2] = cvPoint2D32f(Left.x ,	Left.y );
				ptPrespective[3] = cvPoint2D32f(Right.x ,	Right.y	 );

				float newm[9];

				CvMat mat = cvMat(3, 3, CV_32F, newm); //��ȯ��� �ʱ�ȭ

				cvWarpPerspectiveQMatrix(ptSource, ptPrespective, &mat);//��� ��ȯ

				//������ ó��
				cvSubS(m_image_video,cvScalarAll(100),m_image_video);
				cvZero(m_image_dest2);
				cvWarpPerspective(m_image_video, m_image_dest2, &mat, CV_INTER_LINEAR);
				cvAdd(m_image_dest2,m_image_dest, m_image_dest);

			}
			
			//����� ���� �޸��� ����
			cvReleaseImage(&temp_mask);
			cvReleaseImage(&temp_mask2);
			cvReleaseImage(&circle_mask);
			cvReleaseImage(&in_mask);
			cvClearSeq(seq[i]);
			cvClearSeq(seq_in[i]);
		}

		// ���� ���� �ϱ� ���� ��Ʈ�� 
		outString[0] = "Center";
		outString[1] = "Right";
		outString[2] = "Left";

//// DataOut
		// ��� ���� ���� �ϸ� ��� ��
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

						//������� �ʴ� �ڸ��� NULL�� ä���� ����� �ȵȴ�.
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
		// RawImage�� �̹��� ������ ���� �Ҵ�
		RawImageData *pimage = result.getImage();
		
		// �Էµ� �̹��� ������ �� ä�μ��� �� �� ����
		pimage->resize(m_image_dest->width, m_image_dest->height, m_image_dest->nChannels);
		
		// ������ �� ũ��(pixels��) ���
		int size = m_image_dest->width * m_image_dest->height * m_image_dest->nChannels;
		
		// ���� �����ͷκ��� ���󰪸��� �Ҵ��ϱ� ���� ����
		unsigned char *ptrdata = pimage->getData();
		
		// ���� ������ ������ ������ ��ŭ memcpy
		memcpy(ptrdata, m_image_dest->imageData, size);

		// ��Ʈ�ƿ�
		opros_any mdata = result;
		ImageOut.push(result);//����

		delete pData;

		//����� �޸� ����
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
//	���̺� ����
//
//
////////////////////////////////////////////////////////



// Blob labeling�ؼ� ����� ����� rec�� �� 
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

// Blob Labeling�� ���� ���ϴ� function
// 2000�� ����ó����ȸ�� �Ǹ� �� ����
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
		if( (CurrentPoint.x != 0) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x - 1] == 255) )   // -X ����
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

		if( (CurrentPoint.x != nWidth - 1) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1] == 255) )   // -X ����
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

		if( (CurrentPoint.y != 0) && (DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x] == 255) )   // -X ����
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
	
		if( (CurrentPoint.y != nHeight - 1) && (DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x] == 255) )   // -X ����
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

// ������ ���� blob�� Į�� ���� ������ ũ�⸦ ȹ��
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

// nWidth�� nHeight���� ���� rec�� ��� ����


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

// nWidth�� nHeight���� ū rec�� ��� ����


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

