
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



#include "MotionTracking.h"

//
// constructor declaration
//
MotionTracking::MotionTracking()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
MotionTracking::MotionTracking(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

MotionTracking::~MotionTracking() {
}

void MotionTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType MotionTracking::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType MotionTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType MotionTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MotionTracking::onPeriodChanged()
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
	return new MotionTracking();
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
	return new MotionTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

