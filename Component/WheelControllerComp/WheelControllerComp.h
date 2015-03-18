#ifndef _WheelControllerComp_COMPONENT_H
#define _WheelControllerComp_COMPONENT_H
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

#include "WheelControllerServiceProvided.h"
#include "WheelController.h"

class WheelControllerComp : public Component, public IWheelControllerService
{
protected:
	//	Service Port
	IWheelControllerService *ptrWheelControllerService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	WheelController *wheelController;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual ReturnType SetPosition(ObjectLocation position);
	virtual ObjectLocation GetPosition();
	virtual vector<long> GetOdometery();
	virtual ReturnType DriveWheel(double linearVelocity, double angularVelocity);
	virtual ReturnType MoveWheel(double distance, double linearVelocity);
	virtual ReturnType RotateWheel(double angle, double angularVelocity);
	virtual ReturnType StopWheel();
	virtual bool IsWheelRunning();

public:
	WheelControllerComp();
	WheelControllerComp(const std::string &compId);
	virtual ~WheelControllerComp();
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

