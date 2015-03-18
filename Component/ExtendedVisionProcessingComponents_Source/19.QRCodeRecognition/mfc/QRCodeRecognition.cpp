
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



#include "QRCodeRecognition.h"

//
// constructor declaration
//
QRCodeRecognition::QRCodeRecognition()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
QRCodeRecognition::QRCodeRecognition(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

QRCodeRecognition::~QRCodeRecognition() {
}

void QRCodeRecognition::portSetup() {

	// data port setup

	addPort("ImageIn", &ImageIn);

	addPort("ImageOut", &ImageOut);

	addPort("PositionOut", &PositionOut);



	// export variable setup


}

// Call back Declaration
ReturnType QRCodeRecognition::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType QRCodeRecognition::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType QRCodeRecognition::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType QRCodeRecognition::onPeriodChanged()
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
	return new QRCodeRecognition();
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
	return new QRCodeRecognition();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

