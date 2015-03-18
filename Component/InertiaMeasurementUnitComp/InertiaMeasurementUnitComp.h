#ifndef _InertiaMeasurementUnitComp_COMPONENT_H
#define _InertiaMeasurementUnitComp_COMPONENT_H
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

#include "InertiaMeasurementUnitServiceProvided.h"
#include "InertiaMeasurementUnit.h"
#include "OprosLock.h"

class InertiaMeasurementUnitComp : public Component, public IInertiaMeasurementUnitService
{
protected:
	//	Data Port
	OutputDataPort< vector<ObjectPosture> > inertiaMeasurementUnitData;

	//	Service Port
	IInertiaMeasurementUnitService *ptrInertiaMeasurementUnitService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	InertiaMeasurementUnit *imu;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<ObjectPosture> GetSensorValue();

public:
	InertiaMeasurementUnitComp();
	InertiaMeasurementUnitComp(const std::string &compId);
	virtual ~InertiaMeasurementUnitComp();
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

