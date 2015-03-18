
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



#include "ColorTracking.h"

//
// constructor declaration
//
ColorTracking::ColorTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ColorTracking::ColorTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ColorTracking::~ColorTracking() {
}

void ColorTracking::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ColorTracking::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ColorTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ColorTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ColorTracking::onPeriodChanged()
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
	return new ColorTracking();
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
	return new ColorTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

