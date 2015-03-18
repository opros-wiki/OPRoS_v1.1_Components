
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



#include "HumanDetection.h"

//
// constructor declaration
//
HumanDetection::HumanDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
HumanDetection::HumanDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

HumanDetection::~HumanDetection() {
}

void HumanDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType HumanDetection::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HumanDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HumanDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HumanDetection::onPeriodChanged()
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
	return new HumanDetection();
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
	return new HumanDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

