
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



#include "DistoritionRevision.h"

//
// constructor declaration
//
DistoritionRevision::DistoritionRevision()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
DistoritionRevision::DistoritionRevision(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

DistoritionRevision::~DistoritionRevision() {
}

void DistoritionRevision::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType DistoritionRevision::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType DistoritionRevision::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType DistoritionRevision::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DistoritionRevision::onPeriodChanged()
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
	return new DistoritionRevision();
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
	return new DistoritionRevision();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

