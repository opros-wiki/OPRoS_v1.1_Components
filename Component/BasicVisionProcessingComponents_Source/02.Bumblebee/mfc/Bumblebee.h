
#include "BumblebeeApp.h"
	
#ifndef _Bumblebee_COMPONENT_H
#define _Bumblebee_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>


#include "RawImage.h"
		


class Bumblebee: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut1;
	OutputDataPort< RawImage > ImageOut2;

//event


// method for provider
	

// method for required
	


// symbol value generation
	

public:
	Bumblebee();
	Bumblebee(const std::string &compId);
	virtual ~Bumblebee();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();


};

#endif

