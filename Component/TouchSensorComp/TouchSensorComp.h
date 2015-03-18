#ifndef _TouchSensorComp_COMPONENT_H
#define _TouchSensorComp_COMPONENT_H
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

#include "TouchSensorServiceProvided.h"
#include "TouchSensor.h"
#include "OprosLock.h"


class TouchSensorComp: public Component, public ITouchSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<bool> > touchSensorData;

	//	Service Port
	ITouchSensorService *ptrTouchSensorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	TouchSensor *touchSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<bool> GetSensorValue();

public:
	TouchSensorComp();
	TouchSensorComp(const std::string &compId);
	virtual ~TouchSensorComp();
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

