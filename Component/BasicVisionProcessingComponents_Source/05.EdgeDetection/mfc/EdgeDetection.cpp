
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



#include "EdgeDetection.h"

//
// constructor declaration
//
EdgeDetection::EdgeDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
EdgeDetection::EdgeDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

EdgeDetection::~EdgeDetection() {
}

void EdgeDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType EdgeDetection::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EdgeDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType EdgeDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType EdgeDetection::onPeriodChanged()
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
	return new EdgeDetection();
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
	return new EdgeDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

