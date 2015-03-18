
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



#include "Translate.h"

//
// constructor declaration
//
Translate::Translate()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Translate::Translate(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Translate::~Translate() {
}

void Translate::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Translate::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Translate::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Translate::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Translate::onPeriodChanged()
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
	return new Translate();
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
	return new Translate();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

