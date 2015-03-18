
#ifndef _KitechVerilookFaceRecognitionComp_COMPONENT_H
#define _KitechVerilookFaceRecognitionComp_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>

#include "VisionRecognitionServiceProvided.h"

#include "NCore.h"
#include "VLExtractor.h"
#include "Verilook/DetectionData.h"
#include "Verilook/SProfiler.h"
#include "Verilook/FaceDatabase.h"
#include "Verilook/Utils.h"
#include "Verilook/VLDatabase.h"
#include <algorithm>		// sort사용을 위해

class KitechVerilookFaceRecognitionComp: public Component
	,public IVisionRecognitionService
{
protected:
	::Property parameter;

	HNLExtractor	_hExtractor;
// service
// method for provider
	IVisionRecognitionService *ptrVisionRecognitionService;

public:
	virtual ReturnType SetParameter(::Property parameter);
	virtual ::Property GetParameter();
	virtual ReturnType GetError();
	virtual vector<VisionRecognitionResult> Recognize(vector<unsigned char> image,int width,int height,int pixelBytes);

	void MakeGrayNImage( HNImage *nImage, const BYTE *RGBimage, int width, int height, int pixelByte );
	NResult DetectFaceOnce(HNLExtractor _hExtractor, HNImage hImage, std::vector<VisionRecognitionResult> &faceResult);
protected:
// data


//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	KitechVerilookFaceRecognitionComp();
	KitechVerilookFaceRecognitionComp(const std::string &compId);
	virtual ~KitechVerilookFaceRecognitionComp();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();


};

#endif

