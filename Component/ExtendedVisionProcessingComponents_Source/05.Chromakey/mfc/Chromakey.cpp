
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



#include "Chromakey.h"

//
// constructor declaration
//
Chromakey::Chromakey()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Chromakey::Chromakey(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

Chromakey::~Chromakey() {
}

void Chromakey::portSetup() {

	// data port setup

	addPort("ImageIn1", &ImageIn1);

	addPort("ImageIn2", &ImageIn2);

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType Chromakey::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Chromakey::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Chromakey::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Chromakey::onPeriodChanged()
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
	return new Chromakey();
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
	return new Chromakey();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

