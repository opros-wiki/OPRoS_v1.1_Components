#ifndef _ActuatorComp_COMPONENT_H
#define _ActuatorComp_COMPONENT_H
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

#include "ActuatorServiceProvided.h"
#include "Actuator.h"

class ActuatorComp: public Component, public IActuatorService
{
protected:
	//	Service Port
	IActuatorService *ptrActuatorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	Actuator *actuator;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual ReturnType SetPower(double power,unsigned long time);
	virtual double GetPower();

public:
	ActuatorComp();
	ActuatorComp(const std::string &compId);
	virtual ~ActuatorComp();
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

