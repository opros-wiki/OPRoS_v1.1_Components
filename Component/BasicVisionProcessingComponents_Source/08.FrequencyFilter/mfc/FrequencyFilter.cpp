
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



#include "FrequencyFilter.h"

//
// constructor declaration
//
FrequencyFilter::FrequencyFilter()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FrequencyFilter::FrequencyFilter(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

FrequencyFilter::~FrequencyFilter() {
}

void FrequencyFilter::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType FrequencyFilter::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FrequencyFilter::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FrequencyFilter::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FrequencyFilter::onPeriodChanged()
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
	return new FrequencyFilter();
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
	return new FrequencyFilter();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

