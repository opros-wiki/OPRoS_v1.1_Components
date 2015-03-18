#ifndef _ServoActuatorComp_COMPONENT_H
#define _ServoActuatorComp_COMPONENT_H
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

#include "ServoActuatorServiceProvided.h"
#include "ServoActuator.h"

class ServoActuatorComp : public Component, public IServoActuatorService
{
protected:
	//	Service Port
	IServoActuatorService *ptrServoActuatorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	ServoActuator *servoActuator;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual ReturnType SetPower(double power,unsigned long time);
	virtual double GetPower();
	virtual ReturnType SetVelocity(double velocity,unsigned long time);
	virtual double GetVelocity();
	virtual ReturnType SetPosition(double position,unsigned long time);
	virtual double GetPosition();

public:
	ServoActuatorComp();
	ServoActuatorComp(const std::string &compId);
	virtual ~ServoActuatorComp();
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

