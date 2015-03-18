
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



#include "ZoomInOut.h"

//
// constructor declaration
//
ZoomInOut::ZoomInOut()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ZoomInOut::ZoomInOut(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ZoomInOut::~ZoomInOut() {
}

void ZoomInOut::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ZoomInOut::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ZoomInOut::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ZoomInOut::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ZoomInOut::onPeriodChanged()
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
	return new ZoomInOut();
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
	return new ZoomInOut();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

