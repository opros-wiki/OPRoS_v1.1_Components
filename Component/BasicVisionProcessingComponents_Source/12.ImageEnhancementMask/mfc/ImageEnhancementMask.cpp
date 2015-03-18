
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



#include "ImageEnhancementMask.h"

//
// constructor declaration
//
ImageEnhancementMask::ImageEnhancementMask()
		:ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
ImageEnhancementMask::ImageEnhancementMask(const std::string &name):Component(name)
		,ImageIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

ImageEnhancementMask::~ImageEnhancementMask() {
}

void ImageEnhancementMask::portSetup() {

	// data port setup

	addPort("ImageOut", &ImageOut);

	addPort("ImageIn", &ImageIn);



	// export variable setup


}

// Call back Declaration
ReturnType ImageEnhancementMask::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ImageEnhancementMask::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType ImageEnhancementMask::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ImageEnhancementMask::onPeriodChanged()
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
	return new ImageEnhancementMask();
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
	return new ImageEnhancementMask();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

