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

#include "KitechMonotoneCubicTrajectoryGenerationComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
KitechMonotoneCubicTrajectoryGenerationComp::KitechMonotoneCubicTrajectoryGenerationComp()
{
	_errorCode = OPROS_SUCCESS;
	_monotonicity = 0.0;
	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechMonotoneCubicTrajectoryGenerationComp::KitechMonotoneCubicTrajectoryGenerationComp(const std::string &name):Component(name)
{
	_errorCode = OPROS_SUCCESS;
	_monotonicity = 0.0;
	
	portSetup();
}

//
// destructor declaration
//

KitechMonotoneCubicTrajectoryGenerationComp::~KitechMonotoneCubicTrajectoryGenerationComp() 
{
	onDestroy();
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::SetParameter(Property parameter)
{
	if(parameter.FindName("Monotonicity") == false) {
		PrintMessage("ERROR : KitechMonotoneCubicTrajectoryGenerationComp::SetParameter() -> Can't find the Monotonicity in property\n");
		return OPROS_BAD_INPUT_PARAMETER;
	}
	_monotonicity = atof(parameter.GetValue("Monotonicity").c_str());

	_parameter = parameter;

	return _errorCode = OPROS_SUCCESS;
}

Property KitechMonotoneCubicTrajectoryGenerationComp::GetParameter()
{
	_errorCode = OPROS_SUCCESS;

	return _parameter;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::GetError()
{
	return _errorCode;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::GenerateTrajectory(vector< valarray<double> > path,double deltaTime)
{
	_acceleration.clear();
	_velocity.clear();
	_position.clear();

	if (deltaTime < 0.001) deltaTime = 0.001;
	if (path.size() < 2) return _errorCode = OPROS_BAD_INPUT_PARAMETER;

	unsigned int n = path.size () - 1;

	vector<double> h(n);
	for (unsigned int i=0; i<n; i++) {
		h[i] = path[i+1][0] - path[i][0];
		if (h[i] < deltaTime) h[i] = deltaTime;
	}

	vector< valarray<double> > m (n + 1);
	unsigned int i = 0;

	m[i] = (path[i+1] - path[i])/h[i];
	for (i=1; i<n; i++) {
		m[i] = 0.5*(path[i] - path[i-1])/h[i-1] + 0.5*(path[i+1] - path[i])/h[i];
	}
	m[i] = (path[i] - path[i-1])/h[i-1];				

	for (unsigned int i=0; i<n; i++) {
		valarray<double> Dk = (path[i+1] - path[i])/h[i];

		for (int j=0; j<(int)path[i].size(); j++) {
			if (Dk[j] == 0.) {
				m[i][j] = 0.;
				m[i+1][j] = 0.;
			}
			else {
				double ak = m[i][j]/Dk[j];
				double bk = m[i+1][j]/Dk[j];
				double tk = _monotonicity/sqrt(ak*ak + bk*bk);
				if (tk < 1.) {
					m[i][j] = tk*ak*Dk[j];
					m[i+1][j] = tk*bk*Dk[j];
				}
			}
		}
	}

	_position.reserve ((int)(path[n][0] / deltaTime + 10));

	double t = path[0][0];
	for (unsigned int i=0; i<n; i++) {
		for (; t<=path[i+1][0]; t+=deltaTime) {
			double tp = (t - path[i][0])/h[i];
			double tm = (1. - tp);
			double h00 = (1. + 2.*tp)*tm*tm;
			double h10 = tp*tm*tm;
			double h01 = tp*tp*(3. - 2.*tp);
			double h11 = tp*tp*(tp - 1.);
			
			valarray<double> p = h00*path[i] + h10*m[i] + h01*path[i+1] + h11*m[i+1];
			p[0] = t;
			_position.push_back (p);
		}
    }

	_position.push_back (path[n]);

	return _errorCode = OPROS_SUCCESS;
}

vector < valarray<double> > KitechMonotoneCubicTrajectoryGenerationComp::GetAcceleration()
{
	_errorCode = OPROS_SUCCESS;

	return _acceleration;
}

vector < valarray<double> > KitechMonotoneCubicTrajectoryGenerationComp::GetVelocity()
{
	_errorCode = OPROS_SUCCESS;

	return _velocity;
}

vector < valarray<double> > KitechMonotoneCubicTrajectoryGenerationComp::GetPosition()
{
	_errorCode = OPROS_SUCCESS;

	return _position;
}

void KitechMonotoneCubicTrajectoryGenerationComp::portSetup() {
	ProvidedServicePort *pa1;
	pa1=new TrajectoryGenerationServiceProvided(this);
	addPort("TrajectoryGenerationService",pa1);
	// export variable setup
}

// Call back Declaration
ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechMonotoneCubicTrajectoryGenerationComp::onPeriodChanged()
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
	return new KitechMonotoneCubicTrajectoryGenerationComp();
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
	return new KitechMonotoneCubicTrajectoryGenerationComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

