#ifndef _KitechCardinalSplineTrajectoryGenerationComp_COMPONENT_H
#define _KitechCardinalSplineTrajectoryGenerationComp_COMPONENT_H
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

#include "TrajectoryGenerationServiceProvided.h"

class KitechCardinalSplineTrajectoryGenerationComp : public Component, public ITrajectoryGenerationService
{
protected:
	ITrajectoryGenerationService *ptrTrajectoryGenerationService;

	Property _parameter;
	ReturnType _errorCode;
	double _tension;

	vector< valarray<double> > _acceleration;
	vector< valarray<double> > _velocity;
	vector< valarray<double> > _position;

public:
	virtual ReturnType SetParameter(Property parameter);
	virtual Property GetParameter();
	virtual ReturnType GetError();
	virtual ReturnType GenerateTrajectory(vector< valarray<double> > path,double deltaTime);
	virtual vector < valarray<double> > GetAcceleration();
	virtual vector < valarray<double> > GetVelocity();
	virtual vector < valarray<double> > GetPosition();

public:
	KitechCardinalSplineTrajectoryGenerationComp();
	KitechCardinalSplineTrajectoryGenerationComp(const std::string &compId);
	virtual ~KitechCardinalSplineTrajectoryGenerationComp();
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

