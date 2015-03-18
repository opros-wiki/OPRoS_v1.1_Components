#ifndef _UltrasonicSensorComp_COMPONENT_H
#define _UltrasonicSensorComp_COMPONENT_H
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

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "UltrasonicSensorServiceProvided.h"
#include "UltrasonicSensor.h"
#include "OprosLock.h"

class UltrasonicSensorComp : public Component, public IUltrasonicSensorService
{
protected:
	//	Data Port
	OutputDataPort< vector<double> > ultrasonicSensorData;

	//	Service Port
	IUltrasonicSensorService *ptrUltrasonicSensorService;

	//	Shared(Dynamic Linked) Library's Handle
#if defined(WIN32)
	OprosApiHandle hOprosAPI;
#else
	void *hOprosAPI;
#endif

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	UltrasonicSensor *ultrasonicSensor;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<double> GetSensorValue();

public:
	UltrasonicSensorComp();
	UltrasonicSensorComp(const std::string &compId);
	virtual ~UltrasonicSensorComp();
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

