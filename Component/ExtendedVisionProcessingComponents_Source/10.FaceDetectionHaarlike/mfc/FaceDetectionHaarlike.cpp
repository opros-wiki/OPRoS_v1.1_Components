
#include "stdafx.h"
	
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



#include "FaceDetectionHaarlike.h"

//
// constructor declaration
//
FaceDetectionHaarlike::FaceDetectionHaarlike()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FaceDetectionHaarlike::FaceDetectionHaarlike(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FaceDetectionHaarlike::~FaceDetectionHaarlike() {
}

void FaceDetectionHaarlike::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType FaceDetectionHaarlike::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionHaarlike::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionHaarlike::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionHaarlike::onPeriodChanged()
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
	return new FaceDetectionHaarlike();
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
	return new FaceDetectionHaarlike();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

