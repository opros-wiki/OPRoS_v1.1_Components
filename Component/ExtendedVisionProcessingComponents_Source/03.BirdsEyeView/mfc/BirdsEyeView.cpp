
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



#include "BirdsEyeView.h"

//
// constructor declaration
//
BirdsEyeView::BirdsEyeView()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
BirdsEyeView::BirdsEyeView(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

BirdsEyeView::~BirdsEyeView() {
}

void BirdsEyeView::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageOut2", &ImageOut2);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType BirdsEyeView::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType BirdsEyeView::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType BirdsEyeView::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BirdsEyeView::onPeriodChanged()
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
	return new BirdsEyeView();
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
	return new BirdsEyeView();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

