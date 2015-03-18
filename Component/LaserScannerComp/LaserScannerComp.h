#ifndef _LaserScannerComp_COMPONENT_H
#define _LaserScannerComp_COMPONENT_H
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

#include "LaserScannerServiceProvided.h"
#include "LaserScanner.h"
#include "OprosLock.h"

class LaserScannerComp : public Component, public ILaserScannerService
{
protected:
	//	Data Port
	OutputDataPort< vector<double> > laserScannerData;

	//	Service Port
	ILaserScannerService *ptrLaserScannerService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	LaserScanner *laserScanner;

	OprosLock lock;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual vector<double> GetSensorValue();

public:
	LaserScannerComp();
	LaserScannerComp(const std::string &compId);
	virtual ~LaserScannerComp();
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

