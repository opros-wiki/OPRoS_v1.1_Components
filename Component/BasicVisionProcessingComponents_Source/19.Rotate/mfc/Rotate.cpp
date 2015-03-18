
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



#include "Rotate.h"

//
// constructor declaration
//
Rotate::Rotate()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Rotate::Rotate(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Rotate::~Rotate() {
}

void Rotate::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Rotate::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Rotate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Rotate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Rotate::onPeriodChanged()
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
	return new Rotate();
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
	return new Rotate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

