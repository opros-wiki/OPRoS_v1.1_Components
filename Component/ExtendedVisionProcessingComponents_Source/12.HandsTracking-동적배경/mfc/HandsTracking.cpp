
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



#include "HandsTracking.h"

//
// constructor declaration
//
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

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType HandsTracking::onInitialize()
{
	// user code here
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
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HandsTracking::onExecute()
{
	// user code here
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

