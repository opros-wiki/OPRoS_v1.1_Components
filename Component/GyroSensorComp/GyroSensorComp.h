#ifndef _GyroSensorComp_COMPONENT_H
#define _GyroSensorComp_COMPONENT_H
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

#include "GyroSensorServiceProvided.h"
#include "GyroSensor.h"
#include "OprosLock.h"

class GyroSensorComp : public Component, public IGyroSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<OPRoS::Rotation> > gyroSensorData;

	//	Service Port
	IGyroSensorService *ptrGyroSensorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	GyroSensor *gyroSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<OPRoS::Rotation> GetSensorValue();

public:
	GyroSensorComp();
	GyroSensorComp(const std::string &compId);
	virtual ~GyroSensorComp();
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

