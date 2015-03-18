
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
#include "OprosPrintMessage.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "BlobLabeling.h"


#include "KitechYcrcbColorRecognitionComp.h"

//
// constructor declaration
//
KitechYcrcbColorRecognitionComp::KitechYcrcbColorRecognitionComp()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechYcrcbColorRecognitionComp::KitechYcrcbColorRecognitionComp(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

KitechYcrcbColorRecognitionComp::~KitechYcrcbColorRecognitionComp() {
}
vector<VisionRecognitionResult> KitechYcrcbColorRecognitionComp::Recognize(vector<unsigned char> image,int width,int height,int pixelBytes)
{
	vector<VisionRecognitionResult> recognitionResult(0);
	VisionRecognitionResult tmpResult;

	IplImage *cvImage = cvCreateImageHeader( cvSize(width, height), 8, pixelBytes );
	cvImage->imageData = (char *)&image[0];

	IplImage *ycrcbImage = cvCreateImage( cvGetSize(cvImage), 8, pixelBytes );
	cvCvtColor(cvImage, ycrcbImage, CV_RGB2YCrCb);

	IplImage *cvMask= cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );

	for( unsigned int p = 0  ;  p < _colorRange.size()  ;  p++ ) {

		cvInRangeS( ycrcbImage,
					cvScalar(_colorRange[p].min1,_colorRange[p].min3,_colorRange[p].min2,0),
					cvScalar(_colorRange[p].max1,_colorRange[p].max3,_colorRange[p].max2,255),
					cvMask );

		CBlobLabeling blob;
		blob.SetParam( cvMask, width*height/100 );
		blob.DoLabeling();

		//recognitionResult.resize(blob.m_nBlobs);
		for( int i = 0  ;  i < blob.m_nBlobs  ;  i++ ) {
			tmpResult.name = _colorRange[p].name;

			tmpResult.point1X = blob.m_recBlobs[i].x;
			tmpResult.point1Y = blob.m_recBlobs[i].y;
			tmpResult.point2X = blob.m_recBlobs[i].x + blob.m_recBlobs[i].width;
			tmpResult.point2Y = blob.m_recBlobs[i].y;
			tmpResult.point3X = blob.m_recBlobs[i].x + blob.m_recBlobs[i].width;
			tmpResult.point3Y = blob.m_recBlobs[i].y + blob.m_recBlobs[i].height;
			tmpResult.point4X = blob.m_recBlobs[i].x;
			tmpResult.point4Y = blob.m_recBlobs[i].y + blob.m_recBlobs[i].height;
			recognitionResult.push_back(tmpResult);
			//PrintMessage("SUCCESS:KITECH_YCrCbColorRecognitionComp::recognize() -> I found %d data.(%d=%s)\n", blob.m_nBlobs, i, recognitionResult[i].name.c_str());
		}
	}

	//cvSaveImage("KITECH_YCrCbColorRecognitionComp.jpg", cvImage);

	cvReleaseImage(&cvMask);
	cvReleaseImage(&ycrcbImage);
	cvReleaseImageHeader(&cvImage);

	return recognitionResult;
}

ReturnType KitechYcrcbColorRecognitionComp::GetError()
{
	return OPROS_SUCCESS;
}

Property KitechYcrcbColorRecognitionComp::GetParameter()
{
	return this->parameter;
}

ReturnType KitechYcrcbColorRecognitionComp::SetParameter(Property parameter)
{
	this->parameter = parameter;
	return OPROS_SUCCESS;
}

void KitechYcrcbColorRecognitionComp::GetDataFromDB(const char* database)
{
	char str[256];
	CvFileStorage *fs = cvOpenFileStorage(database, 0, CV_STORAGE_READ);
	int colorCount = cvReadIntByName(fs, 0, "color_count");
	ColorRange colorRange;
	for( int i = 0  ;  i < colorCount  ;  i++ ) {
		sprintf(str, "color%d", i);
		CvSeq *s = cvGetFileNodeByName(fs, 0, str)->data.seq;
		colorRange.name = cvReadString( (CvFileNode*)cvGetSeqElem(s, 0) );
		colorRange.min1 = cvReadInt( (CvFileNode*)cvGetSeqElem(s, 1) );
		colorRange.max1 = cvReadInt( (CvFileNode*)cvGetSeqElem(s, 2) );
		colorRange.min2 = cvReadInt( (CvFileNode*)cvGetSeqElem(s, 3) );
		colorRange.max2 = cvReadInt( (CvFileNode*)cvGetSeqElem(s, 4) );
		colorRange.min3 = cvReadInt( (CvFileNode*)cvGetSeqElem(s, 5) );
		colorRange.max3 = cvReadInt( (CvFileNode*)cvGetSeqElem(s, 6) );
		_colorRange.push_back(colorRange);
	}
}

void KitechYcrcbColorRecognitionComp::SetDataToDB(const char* database)
{
	char str[256];
	CvFileStorage *fs = cvOpenFileStorage(database, 0, CV_STORAGE_WRITE);
	cvWriteInt(fs, "color_count", (int)_colorRange.size());
	for( int i = 0  ;  i < (int)_colorRange.size()  ;  i++ ) {
		sprintf(str, "color%d", i);
		cvStartWriteStruct(fs, str, CV_NODE_SEQ);
		cvWriteString( fs, 0, _colorRange[i].name.c_str() );
		cvWriteInt( fs, 0, _colorRange[i].min1 );
		cvWriteInt( fs, 0, _colorRange[i].max1 );
		cvWriteInt( fs, 0, _colorRange[i].min2 );
		cvWriteInt( fs, 0, _colorRange[i].max2 );
		cvWriteInt( fs, 0, _colorRange[i].min3 );
		cvWriteInt( fs, 0, _colorRange[i].max3 );
		cvEndWriteStruct(fs);
	}
	cvReleaseFileStorage(&fs);
}

void KitechYcrcbColorRecognitionComp::portSetup() {
	ProvidedServicePort *pa1;
	pa1=new VisionRecognitionServiceProvided(this);
	addPort("VisionRecognitionService",pa1);




	// export variable setup


}

// Call back Declaration
ReturnType KitechYcrcbColorRecognitionComp::onInitialize()
{
	PrintMessage ("KitechYcrcbColorRecognitionComp::onInitialize()\n");

	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("DBName") == false) {
		PrintMessage("ERROR : KitechYcrcbColorRecognitionComp::onInitialize() -> Can't find the DBName in property\n");
		return OPROS_FIND_PROPERTY_ERROR;
	}
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onStart()
{
	if(parameter.FindName("DBName")) {
		GetDataFromDB(parameter.GetValue("DBName").c_str());
		PrintMessage ("INFO : KitechYcrcbColorRecognitionComp::onStart() -> Get %d data\n", _colorRange.size());
	}
	else {
		PrintMessage ("ERROR : KitechYcrcbColorRecognitionComp::onStart() -> Can't find DBfile name with %s\n", parameter.GetValue("DBName").c_str());
	}
	//_colorRange.resize(2);
	//_colorRange[0].name = "red";
	//_colorRange[0].min1 = 0;
	//_colorRange[0].max1 = 255;
	//_colorRange[0].min2 = 150;
	//_colorRange[0].max2 = 255;
	//_colorRange[0].min3 = 0;
	//_colorRange[0].max3 = 255;
	//_colorRange[1].name = "blue";
	//_colorRange[1].min1 = 0;
	//_colorRange[1].max1 = 255;
	//_colorRange[1].min2 = 0;
	//_colorRange[1].max2 = 255;
	//_colorRange[1].min3 = 150;
	//_colorRange[1].max3 = 255;
	return OPROS_SUCCESS;
}
	
ReturnType KitechYcrcbColorRecognitionComp::onStop()
{
#if defined(WIN32)
	Sleep(100);
#endif
	if(parameter.FindName("DBName")) {
		SetDataToDB(parameter.GetValue("DBName").c_str());
	}
	else {
		PrintMessage ("KitechYcrcbColorRecognitionComp::onStart() -> Can't find the %s\n", parameter.GetValue("DBName").c_str());
	}
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechYcrcbColorRecognitionComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechYcrcbColorRecognitionComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
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
	return new KitechYcrcbColorRecognitionComp();
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
	return new KitechYcrcbColorRecognitionComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

