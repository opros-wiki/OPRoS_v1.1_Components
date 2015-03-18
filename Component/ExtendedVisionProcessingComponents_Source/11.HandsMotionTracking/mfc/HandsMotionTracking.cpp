
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



#include "HandsMotionTracking.h"

//
// constructor declaration
//
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

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType HandsMotionTracking::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HandsMotionTracking::onStop()
{
	// user code here
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
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsMotionTracking::onExecute()
{
	// user code here
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

