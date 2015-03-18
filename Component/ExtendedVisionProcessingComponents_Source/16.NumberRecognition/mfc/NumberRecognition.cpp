
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



#include "NumberRecognition.h"

//
// constructor declaration
//
NumberRecognition::NumberRecognition()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
NumberRecognition::NumberRecognition(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

NumberRecognition::~NumberRecognition() {
}

void NumberRecognition::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);



	// export variable setup


}

// Call back Declaration
ReturnType NumberRecognition::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType NumberRecognition::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType NumberRecognition::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType NumberRecognition::onPeriodChanged()
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
	return new NumberRecognition();
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
	return new NumberRecognition();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

