
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



#include "Bumblebee.h"

//
// constructor declaration
//
Bumblebee::Bumblebee()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Bumblebee::Bumblebee(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

Bumblebee::~Bumblebee() {
}

void Bumblebee::portSetup() {

	// data port setup

	addPort("ImageOut1", &ImageOut1);

	addPort("ImageOut2", &ImageOut2);



	// export variable setup


}

// Call back Declaration
ReturnType Bumblebee::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Bumblebee::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Bumblebee::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Bumblebee::onPeriodChanged()
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
	return new Bumblebee();
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
	return new Bumblebee();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

