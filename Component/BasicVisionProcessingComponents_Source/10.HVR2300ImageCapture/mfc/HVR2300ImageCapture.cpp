
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



#include "HVR2300ImageCapture.h"

//
// constructor declaration
//
HVR2300ImageCapture::HVR2300ImageCapture()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
HVR2300ImageCapture::HVR2300ImageCapture(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

HVR2300ImageCapture::~HVR2300ImageCapture() {
}

void HVR2300ImageCapture::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType HVR2300ImageCapture::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HVR2300ImageCapture::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HVR2300ImageCapture::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HVR2300ImageCapture::onPeriodChanged()
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
	return new HVR2300ImageCapture();
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
	return new HVR2300ImageCapture();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

