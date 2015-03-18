
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



#include "SizeChange.h"

//
// constructor declaration
//
SizeChange::SizeChange()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
SizeChange::SizeChange(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

SizeChange::~SizeChange() {
}

void SizeChange::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType SizeChange::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType SizeChange::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType SizeChange::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SizeChange::onPeriodChanged()
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
	return new SizeChange();
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
	return new SizeChange();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

