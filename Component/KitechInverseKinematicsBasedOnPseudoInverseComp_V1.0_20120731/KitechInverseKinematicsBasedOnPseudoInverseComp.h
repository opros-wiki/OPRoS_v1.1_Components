#ifndef _KitechInverseKinematicsBasedOnPseudoInverseComp_COMPONENT_H
#define _KitechInverseKinematicsBasedOnPseudoInverseComp_COMPONENT_H
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

#include "InverseKinematicsServiceProvided.h"
#include "InverseKinematicsBasedOnPseudoInverse.h"

class KitechInverseKinematicsBasedOnPseudoInverseComp : public Component, public IInverseKinematicsService
{
protected:
	IInverseKinematicsService *ptrInverseKinematicsService;
	InverseKinematicsBasedOnPseudoInverse *_inverseKinematics;

	Property _parameter;
	ReturnType _errorCode;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ObjectPosition SolveForwardKinematics(vector<double> actualJointPosition);
	virtual vector<double> SolveInverseKinematics(vector<double> actualJointPosition,ObjectPosition desiredPosition);

public:
	KitechInverseKinematicsBasedOnPseudoInverseComp();
	KitechInverseKinematicsBasedOnPseudoInverseComp(const std::string &compId);
	virtual ~KitechInverseKinematicsBasedOnPseudoInverseComp();
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

