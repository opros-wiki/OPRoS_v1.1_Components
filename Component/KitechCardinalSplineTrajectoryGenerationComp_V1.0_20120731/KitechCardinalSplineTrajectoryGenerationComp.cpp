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

#include "KitechCardinalSplineTrajectoryGenerationComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
KitechCardinalSplineTrajectoryGenerationComp::KitechCardinalSplineTrajectoryGenerationComp()
{
	_errorCode = OPROS_SUCCESS;
	_tension = 0.0;

	portSetup();
}

//
// constructor declaration (with name)
//
KitechCardinalSplineTrajectoryGenerationComp::KitechCardinalSplineTrajectoryGenerationComp(const std::string &name):Component(name)
{
	_errorCode = OPROS_SUCCESS;
	_tension = 0.0;

	portSetup();
}

//
// destructor declaration
//

KitechCardinalSplineTrajectoryGenerationComp::~KitechCardinalSplineTrajectoryGenerationComp() 
{
	onDestroy();
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::SetParameter(Property parameter)
{
	if(parameter.FindName("Tension") == false) {
		PrintMessage("ERROR : KitechCardinalSplineTrajectoryGenerationComp::SetParameter() -> Can't find the Tension in property\n");
		return OPROS_BAD_INPUT_PARAMETER;
	}
	_tension = atof(parameter.GetValue("Tension").c_str());

	_parameter = parameter;

	return _errorCode = OPROS_SUCCESS;
}

Property KitechCardinalSplineTrajectoryGenerationComp::GetParameter()
{
	_errorCode = OPROS_SUCCESS;

	return _parameter;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::GetError()
{
	return _errorCode;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::GenerateTrajectory(vector< valarray<double> > path, double deltaTime)
{
	_acceleration.clear();
	_velocity.clear();
	_position.clear();

	if (deltaTime < 0.001) deltaTime = 0.001;
	if (path.size() < 2) return OPROS_BAD_INPUT_PARAMETER;

	unsigned int n = path.size () - 1;

	vector<double> h(n);
	for (unsigned int i=0; i<n; i++) {
		h[i] = path[i+1][0] - path[i][0];
		if (h[i] < deltaTime) h[i] = deltaTime;
	}

	vector< valarray<double> > m (n+1);
	unsigned int i = 0;

	m[i] = (1. - _tension)*(path[i+1] - path[i])/h[i];
	for (i=1; i<n; i++) {
		m[i] = (1. - _tension)*(path[i+1] - path[i-1])/(h[i] + h[i-1]);
	}
	m[i] = (1. - _tension)*(path[i] - path[i-1])/h[i-1];

	_position.reserve ((int)(path[n][0] / deltaTime + 10));

	double t = path[0][0];
	for (unsigned int i=0; i<n; i++) {
		for (; t<=path[i+1][0]; t+=deltaTime) {
			double t1 = (t - path[i][0])/h[i];
			double t2 = t1*t1;
			double t3 = t2*t1;

			double h00 = 2*t3-3*t2+1;
			double h10 = t3-2*t2+t1;
			double h01 = -2*t3+3*t2;
			double h11 = t3-t2;
			
			valarray<double> p = h00*path[i] + h10*m[i] + h01*path[i+1] + h11*m[i+1];
			p[0] = t;
			_position.push_back (p);
		}
    }

	_position.push_back (path[n]);

	return _errorCode = OPROS_SUCCESS;
}

vector < valarray<double> > KitechCardinalSplineTrajectoryGenerationComp::GetAcceleration()
{
	_errorCode = OPROS_SUCCESS;

	return _acceleration;
}

vector < valarray<double> > KitechCardinalSplineTrajectoryGenerationComp::GetVelocity()
{
	_errorCode = OPROS_SUCCESS;

	return _velocity;
}

vector < valarray<double> > KitechCardinalSplineTrajectoryGenerationComp::GetPosition()
{
	_errorCode = OPROS_SUCCESS;

	return _position;
}


void KitechCardinalSplineTrajectoryGenerationComp::portSetup()
{
	ProvidedServicePort *pa1;
	pa1=new TrajectoryGenerationServiceProvided(this);
	addPort("TrajectoryGenerationService",pa1);
}

// Call back Declaration
ReturnType KitechCardinalSplineTrajectoryGenerationComp::onInitialize()
{
	std::map<std::string, std::string> temp = getPropertyMap();
	_parameter.SetProperty(temp);

	return SetParameter(_parameter);
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechCardinalSplineTrajectoryGenerationComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechCardinalSplineTrajectoryGenerationComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCardinalSplineTrajectoryGenerationComp::onPeriodChanged()
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
	return new KitechCardinalSplineTrajectoryGenerationComp();
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
	return new KitechCardinalSplineTrajectoryGenerationComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

