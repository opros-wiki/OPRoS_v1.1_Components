
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



#include "RGBValueControl.h"

//
// constructor declaration
//
RGBValueControl::RGBValueControl()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
RGBValueControl::RGBValueControl(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

RGBValueControl::~RGBValueControl() {
}

void RGBValueControl::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType RGBValueControl::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBValueControl::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RGBValueControl::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RGBValueControl::onPeriodChanged()
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
	return new RGBValueControl();
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
	return new RGBValueControl();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

