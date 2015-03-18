
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



#include "ObjectTracking.h"

//
// constructor declaration
//
ObjectTracking::ObjectTracking()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ObjectTracking::ObjectTracking(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ObjectTracking::~ObjectTracking() {
}

void ObjectTracking::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ObjectTracking::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ObjectTracking::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ObjectTracking::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ObjectTracking::onPeriodChanged()
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
	return new ObjectTracking();
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
	return new ObjectTracking();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

