
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



#include "Contrast.h"

//
// constructor declaration
//
Contrast::Contrast()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Contrast::Contrast(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Contrast::~Contrast() {
}

void Contrast::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Contrast::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Contrast::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Contrast::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Contrast::onPeriodChanged()
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
	return new Contrast();
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
	return new Contrast();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

