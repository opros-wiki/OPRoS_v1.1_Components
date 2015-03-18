/*
 *  Generated sources by OPRoS Component Generator (OCG V2)
 *  
 */

#include "KITECH_ERSPViPRObjectRecognitionComp.h"
#include "OprosPrintMessage.h"

#include <evolution/Base.hpp>
#include <evolution/Resource.hpp>
#include <evolution/ObjRec.hpp>

#ifndef M_PI
#define M_PI		3.14159265358979323846 
#endif


static int ImageConversion(Evolution::Matrix<int> *e_image, const BYTE *video_img, int width, int height, int pixel_bytes)
{
	e_image->zeros (abs(height), width);				//초기화//영행렬을 만듦

	int x, y;
	const BYTE *p = video_img;

	if (height < 0 && pixel_bytes == 4) {
		height = -height;

		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				(*e_image)[y][x] = (int)(p[0]*0.30 + p[1]*0.59 + p[2]*0.11);
				p += 4;
			}
		}
	}
	else if (height < 0 && pixel_bytes == 3) {
		height = -height;

		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				(*e_image)[y][x] = (int)(p[0]*0.30 + p[1]*0.59 + p[2]*0.11);
				p += 3;
			}
		}
	}
	else if (height < 0 && pixel_bytes == 1) {
		height = -height;

		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				(*e_image)[y][x] = (int)(p[0]);
				++p;
			}
		}
	}
	else if (0 < height && pixel_bytes == 4) {
		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				//이미지가 거꾸로 저장이 되어야 바른 그림이 나온다
				(*e_image)[height-1-y][x] = (int)(p[0]*0.30 + p[1]*0.59 + p[2]*0.11);
				p += 4;
			}
		}
	}
	else if (0 < height && pixel_bytes == 3) {
		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				//이미지가 거꾸로 저장이 되어야 바른 그림이 나온다
				(*e_image)[height-1-y][x] = (int)(p[0]*0.30 + p[1]*0.59 + p[2]*0.11);
				p += 3;
			}
		}
	}
	else if (0 < height && pixel_bytes == 1) {
		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				//이미지가 거꾸로 저장이 되어야 바른 그림이 나온다
				(*e_image)[height-1-y][x] = (int)(p[0]);
				++p;
			}
		}
	}
	else {
		return -1;
	}
	return 0;
}

class CEvolutionObjectRecVipr
{
public:
	Evolution::ObjRecDatabase* objrecDB;
};

//
// constructor declaration
//
KITECH_ERSPViPRObjectRecognitionComp::KITECH_ERSPViPRObjectRecognitionComp()
{
	portSetup();
}

//
// constructor declaration (with name)
//
KITECH_ERSPViPRObjectRecognitionComp::KITECH_ERSPViPRObjectRecognitionComp(const std::string &name)
{
	portSetup();
}

//
// destructor declaration
//

KITECH_ERSPViPRObjectRecognitionComp::~KITECH_ERSPViPRObjectRecognitionComp() 
{
	onDestroy();
}

void KITECH_ERSPViPRObjectRecognitionComp::portSetup() {

	// provided service port setup
	
	ProvidedServicePort *pa1;
	pa1=new VisionRecognitionServiceProvided(this);
	addPort("VisionRecognitionService",pa1);
}

// Call back Declaration
ReturnType KITECH_ERSPViPRObjectRecognitionComp::onInitialize()
{
	PrintMessage ("KITECH_ERSPViPRObjectRecognitionComp::onInitialize()\n");

	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("DBName") == false) {
		PrintMessage("ERROR : KITECH_HSVColorRecognitionComp::onInitialize() -> Can't find the DBName in property\n");
		return OPROS_FIND_PROPERTY_ERROR;
	}

	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onStart()
{
	//	dll 파일이름을 확인하여 없으면 에러 리턴
	if( !parameter.FindName("DBName") ) {
		PrintMessage("ERROR : KITECH_ERSPViPRObjectRecognitionComp::onStart() -> Can't find DataBase.\n");
		return OPROS_FIND_PROPERTY_ERROR;
	}

	std::string objName = parameter.GetValue("DBName").c_str();
	_vipr = new CEvolutionObjectRecVipr ();

	// Obtain the database, creating one if it does not exist.
	_vipr->objrecDB = Evolution::ObjRecDatabase::get_by_filename (objName.c_str(), true);
	if (_vipr->objrecDB == NULL) {
		// PrintMessageF ("ERROR - Failed to obtain ObjRecDatabase '%s'", db_file_name);
		PrintMessage("ERROR:ERSP_ObjectRecognition::onStart() -> FAILED loading of object database %s.", objName.c_str());
	}
    // Load the database from file
	if (_vipr->objrecDB->load_from (objName.c_str()) != Evolution::RESULT_SUCCESS) {
		//_vipr->objrecDB->remove_ref ();
		PrintMessage("ERROR:ERSP_ObjectRecognition::onStart() -> FAILED loading of object database %s.", objName.c_str());
    }


	PrintMessage ("SUCCESS : KITECH_ERSPViPRObjectRecognitionComp::onStart()\n");
	return OPROS_SUCCESS;
}
	
ReturnType KITECH_ERSPViPRObjectRecognitionComp::onStop()
{
#if defined(WIN32)
	Sleep(100);
#endif

	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onReset()
{
	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onError()
{
	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onRecover()
{
	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onDestroy()
{
	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onEvent(Event *evt)
{
	//	KITECH_ERSPViPRObjectRecognitionComp에서는 사용되지 않음.

	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onExecute()
{
	//PrintMessage("SUCCESS:KITECH_ERSPViPRObjectRecognitionComp::onExecute()\n");

	return OPROS_SUCCESS;
}
	
ReturnType KITECH_ERSPViPRObjectRecognitionComp::onUpdated()
{
	//	KITECH_ERSPViPRObjectRecognitionComp에서는 사용되지 않음.

	return OPROS_SUCCESS;
}

ReturnType KITECH_ERSPViPRObjectRecognitionComp::onPeriodChanged()
{
	//	KITECH_ERSPViPRObjectRecognitionComp에서는 사용되지 않음.

	return OPROS_SUCCESS;
}


bool KITECH_ERSPViPRObjectRecognitionComp::SetParameter(Property parameter)
{
	//	API 핸들이 NULL이거나 컴포넌트의 상태가 ACTIVE 또는 INACTIVE 상태가 아니면
	//	에러를 리턴
	if( (getStatus() != OPROS_CS_ACTIVE && getStatus() != OPROS_CS_INACTIVE)) {
		return false;
	}

	//	API의 SetParameter 함수 호출
	return true;
}
Property KITECH_ERSPViPRObjectRecognitionComp::GetParameter()
{
	return this->parameter;
}

vector<VisionRecognitionResult> KITECH_ERSPViPRObjectRecognitionComp::Recognize(vector<unsigned char> image,int width,int height,int pixelBytes)
{
	//PrintMessage("SUCCESS:KITECH_ERSPViPRObjectRecognitionComp::Recognize()\n");

	vector<VisionRecognitionResult> _recognitionResult(0);

	IplImage *cvImage = cvCreateImageHeader( cvSize(width, height), 8, pixelBytes );
	cvImage->imageData = (char *)&image[0];

	Evolution::Matrix<int> e_img;
	Evolution::Image capture_img;

	if (ImageConversion (&e_img, (BYTE *)cvImage->imageData, width, height, pixelBytes) < 0) {
		return _recognitionResult;
	}

	capture_img.copy_matrix(e_img);		//이미지를 얻음.

	//특징점 추출
	Evolution::ObjRecKeypointList* capture_features = new Evolution::ObjRecKeypointList ();
	if (capture_features->extract (&capture_img) != Evolution::RESULT_SUCCESS) {
		capture_features->remove_ref ();
		return _recognitionResult;
	}

	//특징점들을 비교하여 사물 인식
	Evolution::ObjRecQuery* query = new Evolution::ObjRecQuery ();
	query->recognize (_vipr->objrecDB, capture_features);

	// The image features are not needed after recognition is performed 
	// so we should release the resource.
	capture_features->remove_ref ();

	int n = query->get_num_matches ();
	_recognitionResult.resize(n);

	for( int i = 0  ;  i < n  ;  i++ )
	{
		Evolution::ObjRecQuery::MatchInfo info[1]; 
		query->get_match_info (i, info);
	
		_recognitionResult[i].name = info->label;
		_recognitionResult[i].point1X = (long)info->rectangle[0][0];
		_recognitionResult[i].point1Y = height-(long)info->rectangle[0][1];
		_recognitionResult[i].point2X = (long)info->rectangle[1][0];
		_recognitionResult[i].point2Y = height-(long)info->rectangle[1][1];
		_recognitionResult[i].point3X = (long)info->rectangle[2][0];
		_recognitionResult[i].point3Y = height-(long)info->rectangle[2][1];
		_recognitionResult[i].point4X = (long)info->rectangle[3][0];
		_recognitionResult[i].point4Y = height-(long)info->rectangle[3][1];
	}
	query->remove_ref (); // clean-up the query object

	cvReleaseImageHeader(&cvImage);


	return _recognitionResult;
}


#ifdef MAKE_COMPONENT_DLL
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new KITECH_ERSPViPRObjectRecognitionComp();
}

void releaseComponent(Component *com)
{
	delete com;
}


#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(IComponent *com);
}
Component *getComponent()
{
	return new KITECH_ERSPViPRObjectRecognitionComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

