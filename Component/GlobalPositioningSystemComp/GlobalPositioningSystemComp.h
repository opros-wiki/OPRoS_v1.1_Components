#ifndef _GlobalPositioningSystemComp_COMPONENT_H
#define _GlobalPositioningSystemComp_COMPONENT_H
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

#include "GlobalPositioningSystemServiceProvided.h"
#include "GlobalPositioningSystem.h"
#include "OprosLock.h"

class GlobalPositioningSystemComp : public Component, public IGlobalPositioningSystemService
{
protected:
	//	Data Port
	OutputDataPort< vector<GpsData> > gpsData;

	//	Service Port
	IGlobalPositioningSystemService *ptrGlobalPositioningSystemService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	GlobalPositioningSystem *gps;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<GpsData> GetSensorValue();

public:
	GlobalPositioningSystemComp();
	GlobalPositioningSystemComp(const std::string &compId);
	virtual ~GlobalPositioningSystemComp();
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

