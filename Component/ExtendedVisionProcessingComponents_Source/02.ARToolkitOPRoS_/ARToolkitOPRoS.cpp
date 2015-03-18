
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



#include "ARToolkitOPRoS.h"

//
// constructor declaration
//
ARToolkitOPRoS::ARToolkitOPRoS()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ARToolkitOPRoS::ARToolkitOPRoS(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ARToolkitOPRoS::~ARToolkitOPRoS() {
}

void ARToolkitOPRoS::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ARToolkitOPRoS::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ARToolkitOPRoS::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ARToolkitOPRoS::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ARToolkitOPRoS::onPeriodChanged()
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
	return new ARToolkitOPRoS();
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
	return new ARToolkitOPRoS();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

