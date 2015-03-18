/*
 *  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>



#include "KitechLinearTrajectoryGenerationComp.h"

//
// constructor declaration
//
KitechLinearTrajectoryGenerationComp::KitechLinearTrajectoryGenerationComp()
{
	_errorCode = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechLinearTrajectoryGenerationComp::KitechLinearTrajectoryGenerationComp(const std::string &name):Component(name)
{
	_errorCode = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

KitechLinearTrajectoryGenerationComp::~KitechLinearTrajectoryGenerationComp() 
{
	onDestroy();
}

ReturnType KitechLinearTrajectoryGenerationComp::SetParameter(Property parameter)
{
	_parameter = parameter;

	return _errorCode = OPROS_SUCCESS;
}

Property KitechLinearTrajectoryGenerationComp::GetParameter()
{
	_errorCode = OPROS_SUCCESS;

	return _parameter;
}

ReturnType KitechLinearTrajectoryGenerationComp::GetError()
{
	return _errorCode;
}

ReturnType KitechLinearTrajectoryGenerationComp::GenerateTrajectory(vector< valarray<double> > path,double deltaTime)
{
	_acceleration.clear();
	_velocity.clear();
	_position.clear();

	if (path.size() < 2) return _errorCode = OPROS_BAD_INPUT_PARAMETER;

	unsigned int n = path.size () - 1;

	vector<double> h(n);
	for (unsigned int i=0; i<n; i++) {
		h[i] = path[i+1][0] - path[i][0];
		if (h[i] < deltaTime) h[i] = deltaTime;
	}

	_position.reserve ((int)(path[n][0] / deltaTime + 10));
	
	double t = path[0][0];
	for (unsigned int i=0; i<n; i++) {
		valarray<double> m = (path[i+1] - path[i])/h[i];
		
		for (; t<=path[i+1][0]; t+=deltaTime) {
			valarray<double> p = m*(t - path[i][0]) + path[i];
			p[0] = t;
			_position.push_back (p);
		}
    }
	_position.push_back (path[n]);

	_errorCode = OPROS_SUCCESS;
}

vector < valarray<double> > KitechLinearTrajectoryGenerationComp::GetAcceleration()
{
	_errorCode = OPROS_SUCCESS;

	return _acceleration;
}

vector < valarray<double> > KitechLinearTrajectoryGenerationComp::GetVelocity()
{
	_errorCode = OPROS_SUCCESS;

	return _velocity;
}

vector < valarray<double> > KitechLinearTrajectoryGenerationComp::GetPosition()
{
	_errorCode = OPROS_SUCCESS;

	return _position;
}

void KitechLinearTrajectoryGenerationComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new TrajectoryGenerationServiceProvided(this);
	addPort("TrajectoryGenerationService",pa1);
	// export variable setup
}

// Call back Declaration
ReturnType KitechLinearTrajectoryGenerationComp::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechLinearTrajectoryGenerationComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechLinearTrajectoryGenerationComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechLinearTrajectoryGenerationComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}







#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new KitechLinearTrajectoryGenerationComp();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new KitechLinearTrajectoryGenerationComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

