
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



#include "OpeningClosing.h"

//
// constructor declaration
//
OpeningClosing::OpeningClosing()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
OpeningClosing::OpeningClosing(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

OpeningClosing::~OpeningClosing() {
}

void OpeningClosing::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType OpeningClosing::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType OpeningClosing::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType OpeningClosing::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType OpeningClosing::onPeriodChanged()
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
	return new OpeningClosing();
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
	return new OpeningClosing();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

