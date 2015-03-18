#ifndef _AccelerationSensorComp_COMPONENT_H
#define _AccelerationSensorComp_COMPONENT_H
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

#include "AccelerationSensorServiceProvided.h"
#include "AccelerationSensor.h"
#include "OprosLock.h"

class AccelerationSensorComp: public Component
	,public IAccelerationSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<double> > accelerationSensorData;

	//	Service Port
	IAccelerationSensorService *ptrAccelerationSensorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	AccelerationSensor *accelerationSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<double> GetSensorValue();

public:
	AccelerationSensorComp();
	AccelerationSensorComp(const std::string &compId);
	virtual ~AccelerationSensorComp();
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

