
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



#include "FaceDetectionASM.h"

//
// constructor declaration
//
FaceDetectionASM::FaceDetectionASM()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FaceDetectionASM::FaceDetectionASM(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FaceDetectionASM::~FaceDetectionASM() {
}

void FaceDetectionASM::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType FaceDetectionASM::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionASM::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FaceDetectionASM::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FaceDetectionASM::onPeriodChanged()
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
	return new FaceDetectionASM();
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
	return new FaceDetectionASM();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

