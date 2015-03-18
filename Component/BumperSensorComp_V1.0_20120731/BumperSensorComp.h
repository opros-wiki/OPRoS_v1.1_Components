#ifndef _BumperSensorComp_COMPONENT_H
#define _BumperSensorComp_COMPONENT_H
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

#include "BumperSensorServiceProvided.h"
#include "BumperSensor.h"
#include "OprosLock.h"

class BumperSensorComp : public Component, public IBumperSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<bool> > bumperSensorData;

	//	Service Port
	IBumperSensorService *ptrBumperSensorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	BumperSensor *bumperSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<bool> GetSensorValue();

public:
	BumperSensorComp();
	BumperSensorComp(const std::string &compId);
	virtual ~BumperSensorComp();
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

