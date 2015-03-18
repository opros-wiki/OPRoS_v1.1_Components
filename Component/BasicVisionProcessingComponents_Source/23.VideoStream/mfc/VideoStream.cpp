
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



#include "VideoStream.h"

//
// constructor declaration
//
VideoStream::VideoStream()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
VideoStream::VideoStream(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

VideoStream::~VideoStream() {
}

void VideoStream::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);



	// export variable setup


}

// Call back Declaration
ReturnType VideoStream::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType VideoStream::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType VideoStream::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType VideoStream::onPeriodChanged()
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
	return new VideoStream();
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
	return new VideoStream();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

