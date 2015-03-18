/*************************************************************

 file: KitechInverseKinematicsBasedOnPseudoInverseComp.cpp
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

#include "KitechInverseKinematicsBasedOnPseudoInverseComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
KitechInverseKinematicsBasedOnPseudoInverseComp::KitechInverseKinematicsBasedOnPseudoInverseComp()
{
	_inverseKinematics = NULL;
	_errorCode = OPROS_SUCCESS;

	portSetup();
}

//
// constructor declaration (with name)
//
KitechInverseKinematicsBasedOnPseudoInverseComp::KitechInverseKinematicsBasedOnPseudoInverseComp(const std::string &name):Component(name)
{
	_inverseKinematics = NULL;
	_errorCode = OPROS_SUCCESS;

	portSetup();
}

//
// destructor declaration
//

KitechInverseKinematicsBasedOnPseudoInverseComp::~KitechInverseKinematicsBasedOnPseudoInverseComp() 
{
	onDestroy();
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::SetParameter(Property parameter)
{
	unsigned int maximumIteration = 100;
	if(parameter.FindName("MaximumIteration") == true)			maximumIteration = (unsigned int)atoi(parameter.GetValue("MaximumIteration").c_str());
	double epsilon = 0.000001;
	if(parameter.FindName("Epsilon") == true)					epsilon = atof(parameter.GetValue("Epsilon").c_str());

	if(parameter.FindName("NumberOfJoints") == false)			return _errorCode = OPROS_INTERNAL_FAULT;
	int numberOfJoints = atoi(parameter.GetValue("NumberOfJoints").c_str());

	if(numberOfJoints < 1) return _errorCode = OPROS_INTERNAL_FAULT;

	if(_inverseKinematics != NULL) {
		delete _inverseKinematics;
	}
	_inverseKinematics = new InverseKinematicsBasedOnPseudoInverse(maximumIteration, epsilon);

	char propertyName[128];
	int jointType, jointAxis;
	double x, y, z;
	for(int i = 0; i < numberOfJoints; i++) {
		_snprintf(propertyName, sizeof(propertyName) - 1, "JointType%d", i);
		if(parameter.FindName(propertyName) == false)	goto ERROR_SetParameter;
		else											jointType = atoi(parameter.GetValue(propertyName).c_str());

		_snprintf(propertyName, sizeof(propertyName) - 1, "JointAxis%d", i);
		if(parameter.FindName(propertyName) == false)	goto ERROR_SetParameter;
		else											jointAxis = atoi(parameter.GetValue(propertyName).c_str());

		_snprintf(propertyName, sizeof(propertyName) - 1, "X%d", i);
		if(parameter.FindName(propertyName) == false)	goto ERROR_SetParameter;
		else											x = atoi(parameter.GetValue(propertyName).c_str());

		_snprintf(propertyName, sizeof(propertyName) - 1, "Y%d", i);
		if(parameter.FindName(propertyName) == false)	goto ERROR_SetParameter;
		else											y = atoi(parameter.GetValue(propertyName).c_str());

		_snprintf(propertyName, sizeof(propertyName) - 1, "Z%d", i);
		if(parameter.FindName(propertyName) == false)	goto ERROR_SetParameter;
		else											z = atoi(parameter.GetValue(propertyName).c_str());

		if(_inverseKinematics->AddJoint(jointType, jointAxis, x, y, z) < 0)	goto ERROR_SetParameter;
	}

	_parameter = parameter;

	return OPROS_SUCCESS;

ERROR_SetParameter :
	delete _inverseKinematics;
	_inverseKinematics = NULL;
	return _errorCode = OPROS_INTERNAL_FAULT;
}

Property KitechInverseKinematicsBasedOnPseudoInverseComp::GetParameter()
{
	_errorCode = OPROS_SUCCESS;

	return _parameter;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::GetError()
{
	return _errorCode;
}

ObjectPosition KitechInverseKinematicsBasedOnPseudoInverseComp::SolveForwardKinematics(vector<double> actualJointPosition)
{
	ObjectPosition position;

	if(_inverseKinematics == NULL) {
		_errorCode = OPROS_INTERNAL_FAULT;
		return position;
	}

	for(size_t i = 0; i < actualJointPosition.size(); i++) {
		actualJointPosition[i] += KDL::deg2rad;
	}

	if(_inverseKinematics->SolveForwardKinematics(actualJointPosition, position) < 0) {
		_errorCode = OPROS_INTERNAL_FAULT;
		return position;
	}

	_errorCode = OPROS_SUCCESS;

	return position;
}

vector<double> KitechInverseKinematicsBasedOnPseudoInverseComp::SolveInverseKinematics(vector<double> actualJointPosition,ObjectPosition desiredPosition)
{
	vector<double> jointPosition;

	if(_inverseKinematics == NULL) {
		_errorCode = OPROS_INTERNAL_FAULT;
		return jointPosition;
	}
	
	for(size_t i = 0; i < actualJointPosition.size(); i++) {
		actualJointPosition[i] *= KDL::deg2rad;
	}

	if(_inverseKinematics->SolveInverseKinematics(actualJointPosition, desiredPosition, jointPosition) < 0) {
		_errorCode = OPROS_INTERNAL_FAULT;
		return jointPosition;
	}

	for(size_t i = 0; i < jointPosition.size(); i++) {
		jointPosition[i] *= KDL::rad2deg;
	}

	_errorCode = OPROS_SUCCESS;

	return jointPosition;
}

void KitechInverseKinematicsBasedOnPseudoInverseComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new InverseKinematicsServiceProvided(this);
	addPort("InverseKinematicsService",pa1);
}

// Call back Declaration
ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onInitialize()
{
	std::map<std::string, std::string> temp = getPropertyMap();
	Property parameter;

	parameter.SetProperty(temp);

	return SetParameter(parameter);
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onDestroy()
{
	if(_inverseKinematics != NULL) {
		delete _inverseKinematics;
		_inverseKinematics = NULL;
	}

	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KitechInverseKinematicsBasedOnPseudoInverseComp::onPeriodChanged()
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
	return new KitechInverseKinematicsBasedOnPseudoInverseComp();
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
	return new KitechInverseKinematicsBasedOnPseudoInverseComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

