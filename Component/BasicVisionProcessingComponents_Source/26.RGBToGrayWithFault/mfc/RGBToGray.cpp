
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



#include "RGBToGray.h"

//
// constructor declaration
//
RGBToGray::RGBToGray()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
RGBToGray::RGBToGray(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

RGBToGray::~RGBToGray() {
}

void RGBToGray::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType RGBToGray::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBToGray::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBToGray::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBToGray::onPeriodChanged()
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
	return new RGBToGray();
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
	return new RGBToGray();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

