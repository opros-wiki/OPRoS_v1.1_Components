
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



#include "Brightness.h"

//
// constructor declaration
//
Brightness::Brightness()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Brightness::Brightness(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Brightness::~Brightness() {
}

void Brightness::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Brightness::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Brightness::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Brightness::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Brightness::onPeriodChanged()
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
	return new Brightness();
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
	return new Brightness();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

