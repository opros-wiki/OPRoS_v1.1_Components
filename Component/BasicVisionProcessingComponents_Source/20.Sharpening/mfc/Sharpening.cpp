
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



#include "Sharpening.h"

//
// constructor declaration
//
Sharpening::Sharpening()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
Sharpening::Sharpening(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

Sharpening::~Sharpening() {
}

void Sharpening::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType Sharpening::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Sharpening::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType Sharpening::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType Sharpening::onPeriodChanged()
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
	return new Sharpening();
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
	return new Sharpening();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

