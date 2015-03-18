
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



#include "AreaDetection.h"

//
// constructor declaration
//
AreaDetection::AreaDetection()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
AreaDetection::AreaDetection(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

AreaDetection::~AreaDetection() {
}

void AreaDetection::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("PositionDataOut", &PositionDataOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType AreaDetection::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType AreaDetection::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType AreaDetection::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AreaDetection::onPeriodChanged()
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
	return new AreaDetection();
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
	return new AreaDetection();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

