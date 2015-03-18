#ifndef _ManipulatorComp_COMPONENT_H
#define _ManipulatorComp_COMPONENT_H
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

#include "ManipulatorServiceProvided.h"
#include "Manipulator.h"

class ManipulatorComp : public Component, public IManipulatorService
{
protected:
	//	Service Port
	IManipulatorService *ptrManipulatorService;

	//	Shared(Dynamic Linked) Library's Handle
	OprosApiHandle hOprosAPI;

	//	Last Error
	ReturnType lastError;

	//	API's Handle
	Manipulator *manipulator;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType Enable();
	virtual ReturnType Disable();
	virtual ReturnType SetPower(vector<double> power,vector<unsigned long> time);
	virtual vector<double> GetPower();
	virtual ReturnType SetVelocity(vector<double> velocity,vector<unsigned long> time);
	virtual vector<double> GetVelocity();
	virtual ReturnType SetPosition(vector<double> position,vector<unsigned long> time);
	virtual vector<double> GetPosition();

public:
	ManipulatorComp();
	ManipulatorComp(const std::string &compId);
	virtual ~ManipulatorComp();
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

