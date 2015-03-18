#ifndef _PositionSensorComp_COMPONENT_H
#define _PositionSensorComp_COMPONENT_H
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

#include "PositionSensorServiceProvided.h"
#include "PositionSensor.h"
#include "OprosLock.h"

class PositionSensorComp: public Component, public IPositionSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<ObjectPosition> > positionSensorData;

	//	Service Port
	IPositionSensorService *ptrPositionSensorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	PositionSensor *positionSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<ObjectPosition> GetSensorValue();

public:
	PositionSensorComp();
	PositionSensorComp(const std::string &compId);
	virtual ~PositionSensorComp();
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

