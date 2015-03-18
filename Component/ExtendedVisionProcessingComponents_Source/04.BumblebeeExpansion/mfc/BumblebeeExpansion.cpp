
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



#include "BumblebeeExpansion.h"

//
// constructor declaration
//
BumblebeeExpansion::BumblebeeExpansion()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
BumblebeeExpansion::BumblebeeExpansion(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

BumblebeeExpansion::~BumblebeeExpansion() {
}

void BumblebeeExpansion::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType BumblebeeExpansion::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType BumblebeeExpansion::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType BumblebeeExpansion::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BumblebeeExpansion::onPeriodChanged()
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
	return new BumblebeeExpansion();
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
	return new BumblebeeExpansion();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

