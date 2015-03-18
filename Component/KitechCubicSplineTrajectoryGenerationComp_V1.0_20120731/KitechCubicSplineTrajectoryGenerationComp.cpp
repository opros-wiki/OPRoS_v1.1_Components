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

#include "KitechCubicSplineTrajectoryGenerationComp.h"
#include "OprosPrintMessage.h"
#include "MatrixAlgebra.h"

//
// constructor declaration
//
KitechCubicSplineTrajectoryGenerationComp::KitechCubicSplineTrajectoryGenerationComp()
{
	_errorCode = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechCubicSplineTrajectoryGenerationComp::KitechCubicSplineTrajectoryGenerationComp(const std::string &name):Component(name)
{
	_errorCode = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

KitechCubicSplineTrajectoryGenerationComp::~KitechCubicSplineTrajectoryGenerationComp() 
{
	onDestroy();
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::SetParameter(Property parameter)
{
	_parameter = parameter;

	return _errorCode = OPROS_SUCCESS;
}

Property KitechCubicSplineTrajectoryGenerationComp::GetParameter()
{
	_errorCode = OPROS_SUCCESS;

	return _parameter;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::GetError()
{
	return _errorCode;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::GenerateTrajectory(vector< valarray<double> > path,double deltaTime)
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

	dMatrix A(n+1, n+1);
	A.zero();
	{
		unsigned int i = 0;
		A(0, 0) = 2*h[i];
		A(0, 1) = h[i];
		for (i=1; i<n; ++i) {
			A(i, i-1) = h[i-1];
			A(i, i+0) = 2*(h[i-1] + h[i]);
			A(i, i+1) = h[i];
		}
		A(i, i-1) = h[i-1];
		A(i, i)   = 2.*h[i-1];
	}

	dMatrix b(n+1, path[0].size());
	{
		unsigned int i = 0;
		for (unsigned int j=0, m=path[0].size(); j<m; ++j) {
			b(i, j) = 6.*((path[i+1][j] - path[i][j])/h[i]);
		}
		for (i=1; i<n; ++i) {
			for (unsigned int j=0, m=path[0].size(); j<m; ++j) {
				b(i, j) = 6.*((path[i+1][j] - path[i][j])/h[i] - (path[i][j] - path[i-1][j])/h[i-1]);
			}
		}
		for (unsigned int j=0, m=path[0].size(); j<m; ++j) {
			b(i, j) = 6.*(-(path[i][j] - path[i-1][j])/h[i-1]);
		}
	}

	dMatrix z = !A*b;

	_position.reserve ((int)(path[n][0] / deltaTime + 10.0));

	double t = path[0][0];
	for (unsigned int i=0; i<n; ++i) {
		for (; t<path[i+1][0]; t+=deltaTime) {
			double tm = t - path[i][0];
			double tp = path[i+1][0] - t;

			double tm3 = tm*tm*tm;
			double tp3 = tp*tp*tp;

			valarray<double> p (path[i].size());

			p[0] = t;
			for (int j=1, m=path[i].size(); j<m; ++j) {
				p[j] = (z(i+1,j)*tm3 + z(i,j)*tp3)/(6.*h[i]) + (path[i+1][j]/h[i] - h[i]*z(i+1,j)/6.)*tm + (path[i][j]/h[i] - h[i]*z(i,j)/6.)*tp;
			}
			_position.push_back (p);
		}
	}
	_position.push_back (path[n]);

	return OPROS_SUCCESS;
}

vector < valarray<double> > KitechCubicSplineTrajectoryGenerationComp::GetAcceleration()
{
	_errorCode = OPROS_SUCCESS;

	return _acceleration;
}

vector < valarray<double> > KitechCubicSplineTrajectoryGenerationComp::GetVelocity()
{
	_errorCode = OPROS_SUCCESS;

	return _velocity;
}

vector < valarray<double> > KitechCubicSplineTrajectoryGenerationComp::GetPosition()
{
	_errorCode = OPROS_SUCCESS;

	return _position;
}


void KitechCubicSplineTrajectoryGenerationComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new TrajectoryGenerationServiceProvided(this);
	addPort("TrajectoryGenerationService",pa1);

	// export variable setup


}

// Call back Declaration
ReturnType KitechCubicSplineTrajectoryGenerationComp::onInitialize()
{
	std::map<std::string, std::string> temp = getPropertyMap();
	_parameter.SetProperty(temp);

	return SetParameter(_parameter);
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechCubicSplineTrajectoryGenerationComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechCubicSplineTrajectoryGenerationComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechCubicSplineTrajectoryGenerationComp::onPeriodChanged()
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
	return new KitechCubicSplineTrajectoryGenerationComp();
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
	return new KitechCubicSplineTrajectoryGenerationComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

