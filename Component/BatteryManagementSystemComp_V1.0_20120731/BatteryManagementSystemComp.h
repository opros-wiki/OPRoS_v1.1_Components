#ifndef _BatteryManagementSystemComp_COMPONENT_H
#define _BatteryManagementSystemComp_COMPONENT_H
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

#include "BatteryManagementSystemServiceProvided.h"
#include "BatteryManagementSystem.h"
#include "OprosLock.h"

class BatteryManagementSystemComp : public Component, public IBatteryManagementSystemService
{
protected:
	//	Data Port
	OutputDataPort< double > batteryChargingRate;

	//	Service Port
	IBatteryManagementSystemService *ptrBatteryManagementSystemService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	BatteryManagementSystem *batteryManagementSystem;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual double GetChargingRate();
	virtual double GetVoltage();
	virtual vector<double> GetCellVoltage();

public:
	BatteryManagementSystemComp();
	BatteryManagementSystemComp(const std::string &compId);
	virtual ~BatteryManagementSystemComp();
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

