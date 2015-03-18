
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



#include "ObjectTrackingByOpticalFlow.h"

//
// constructor declaration
//
ObjectTrackingByOpticalFlow::ObjectTrackingByOpticalFlow()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ObjectTrackingByOpticalFlow::ObjectTrackingByOpticalFlow(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ObjectTrackingByOpticalFlow::~ObjectTrackingByOpticalFlow() {
}

void ObjectTrackingByOpticalFlow::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ObjectTrackingByOpticalFlow::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ObjectTrackingByOpticalFlow::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ObjectTrackingByOpticalFlow::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTrackingByOpticalFlow::onPeriodChanged()
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
	return new ObjectTrackingByOpticalFlow();
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
	return new ObjectTrackingByOpticalFlow();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

