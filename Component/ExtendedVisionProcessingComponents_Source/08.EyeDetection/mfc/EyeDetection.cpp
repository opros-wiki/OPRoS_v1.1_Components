
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



#include "EyeDetection.h"

//
// constructor declaration
//
EyeDetection::EyeDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
EyeDetection::EyeDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

EyeDetection::~EyeDetection() {
}

void EyeDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionDataOut", &PositionDataOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType EyeDetection::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EyeDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EyeDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EyeDetection::onPeriodChanged()
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
	return new EyeDetection();
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
	return new EyeDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

