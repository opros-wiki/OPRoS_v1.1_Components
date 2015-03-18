/*************************************************************

 file: KitechFifthOrderPolynomialTrajectoryGenerationComp.cpp
 author: E.C. Shin
 begin: January 31 2011
 copyright: (c) 2011 KITECH, OPRoS
 email: unchol@kitech.re.kr

***************************************************************

OPRoS source code is provided under a dual license mode:
 LGPL and OPRoS individually.

LGPL: You can redistribute it and/or modify it under the terms
 of the Less GNU General Public License as published by the Free
 Software Foundation, either version 3 of the License.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 Less GNU General Public License for more details. 

 You should have received a copy of the Less GNU General Public
 License along with this program. If not, see 
 <http://www.gnu.org/licenses/>.

OPRoS individual license: An individual license is a license for
 modifying the source code of OPRoS and distiributing it in a
 closed format for commercial purposes.

 If you are interested in this option, please see 
 <http://www.opros.or.kr>.

This license policy may be changed without prior notice.

***************************************************************/

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



#include "KitechFifthOrderPolynomialTrajectoryGenerationComp.h"

//
// constructor declaration
//
KitechFifthOrderPolynomialTrajectoryGenerationComp::KitechFifthOrderPolynomialTrajectoryGenerationComp()
{
	_errorCode = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
KitechFifthOrderPolynomialTrajectoryGenerationComp::KitechFifthOrderPolynomialTrajectoryGenerationComp(const std::string &name):Component(name)
{
	_errorCode = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

KitechFifthOrderPolynomialTrajectoryGenerationComp::~KitechFifthOrderPolynomialTrajectoryGenerationComp() 
{

}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::SetParameter(Property parameter)
{
	_parameter = parameter;

	return _errorCode = OPROS_SUCCESS;
}

Property KitechFifthOrderPolynomialTrajectoryGenerationComp::GetParameter()
{
	_errorCode = OPROS_SUCCESS;

	return _parameter;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::GetError()
{
	return _errorCode;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::GenerateTrajectory(vector< valarray<double> > path,double deltaTime)
{
	vector< valarray<double> > a(6);

	_acceleration.clear();
	_velocity.clear();
	_position.clear();

	if(path.size() < 2) {
		return _errorCode = OPROS_INTERNAL_FAULT;
	}

	for(size_t i = 1; i < path.size(); i++) {
		if(path[i - 1].size() != path[i].size()) {
			return _errorCode = OPROS_INTERNAL_FAULT;
		}
	}

	for(size_t i = 0; i < path.size() - 1; i++) {
		if(path[i + 1][0] - path[i][0] <= 0) {
			return _errorCode = OPROS_INTERNAL_FAULT;
		}
	}

	for(size_t i = 0; i < a.size(); i++) {
		a[i].resize(path[0].size() - 1);
	}
	
	for(size_t i = 0; i < path.size() - 1; i++) {
		int size = (int)((path[i + 1][0] - path[i][0]) / deltaTime);

		a[0] = path[i];
		a[1] = 0.0;
		a[2] = 0.0;
		a[3] = (20.0*path[i + 1] - 20.0*path[i]) / (2.0*pow(path[i + 1][0] - path[i][0], 3));
		a[4] = (30.0*path[i] - 30.0*path[i + 1]) / (2.0*pow(path[i + 1][0] - path[i][0], 4));
		a[5] = (12.0*path[i + 1] - 12.0*path[i]) / (2.0*pow(path[i + 1][0] - path[i][0], 5));

		for(int j = 1; j <= size; j++) {
			double t = deltaTime * (double)j;

			_position.push_back((a[0]) + (a[1] * t) + (a[2] * pow(t, 2)) + (a[3] * pow(t, 3)) + (a[4] * pow(t, 4)) + (a[5] * pow(t, 5)));
			_velocity.push_back((a[1]) + 2.0*(a[2] * t) + 3.0*(a[3] * pow(t, 2)) + 4.0*(a[4] * pow(t, 3)) + 5.0*(a[5] * pow(t, 4)));
			_acceleration.push_back(2.0*(a[2]) + 6.0*(a[3] * t) + 12.0*(a[4] * pow(t, 2)) + 20.0*(a[5] * pow(t, 3)));
		}
	}

	return _errorCode = OPROS_SUCCESS;
}

vector < valarray<double> > KitechFifthOrderPolynomialTrajectoryGenerationComp::GetAcceleration()
{
	_errorCode = OPROS_SUCCESS;

	return _acceleration;
}

vector < valarray<double> > KitechFifthOrderPolynomialTrajectoryGenerationComp::GetVelocity()
{
	_errorCode = OPROS_SUCCESS;

	return _velocity;
}

vector < valarray<double> > KitechFifthOrderPolynomialTrajectoryGenerationComp::GetPosition()
{
	_errorCode = OPROS_SUCCESS;

	return _position;
}

void KitechFifthOrderPolynomialTrajectoryGenerationComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new TrajectoryGenerationServiceProvided(this);
	addPort("TrajectoryGenerationService",pa1);
}

// Call back Declaration
ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onInitialize()
{
	std::map<std::string, std::string> temp = getPropertyMap();
	_parameter.SetProperty(temp);

	return SetParameter(_parameter);
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechFifthOrderPolynomialTrajectoryGenerationComp::onPeriodChanged()
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
	return new KitechFifthOrderPolynomialTrajectoryGenerationComp();
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
	return new KitechFifthOrderPolynomialTrajectoryGenerationComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

