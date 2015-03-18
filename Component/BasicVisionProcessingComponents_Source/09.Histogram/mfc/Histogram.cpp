
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



#include "Histogram.h"

//
// constructor declaration
//
Histogram::Histogram()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Histogram::Histogram(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Histogram::~Histogram() {
}

void Histogram::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Histogram::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Histogram::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Histogram::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Histogram::onPeriodChanged()
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
	return new Histogram();
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
	return new Histogram();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

