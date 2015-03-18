#ifndef _InfraredSensorComp_COMPONENT_H
#define _InfraredSensorComp_COMPONENT_H
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

#include "InfraredSensorServiceProvided.h"
#include "InfraredSensor.h"
#include "OprosLock.h"

class InfraredSensorComp: public Component, public IInfraredSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<double> > infraredSensorData;

	//	Service Port
	IInfraredSensorService *ptrInfraredSensorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	InfraredSensor *infraredSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<double> GetSensorValue();

public:
	InfraredSensorComp();
	InfraredSensorComp(const std::string &compId);
	virtual ~InfraredSensorComp();
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

