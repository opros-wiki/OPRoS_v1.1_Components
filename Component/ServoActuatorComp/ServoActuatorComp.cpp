/*************************************************************

 file: ServoActuatorComp.cpp
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

#include "ServoActuatorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
ServoActuatorComp::ServoActuatorComp()
{
	hOprosAPI = NULL;
	servoActuator = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
ServoActuatorComp::ServoActuatorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	servoActuator = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

ServoActuatorComp::~ServoActuatorComp() 
{
	onDestroy();
}

ReturnType ServoActuatorComp::SetParameter(Property parameter)
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property ServoActuatorComp::GetParameter()
{
	Property parameter;

	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType ServoActuatorComp::GetError()
{
	return lastError;
}

ReturnType ServoActuatorComp::Enable()
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::Disable()
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::SetPower(double power,unsigned long time)
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::SetPower() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->SetPower(power, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

double ServoActuatorComp::GetPower()
{
	double power = 0.0;

	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::GetPower() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->GetPower(power) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return power;
}

ReturnType ServoActuatorComp::SetVelocity(double velocity,unsigned long time)
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::SetVelocity() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->SetVelocity(velocity, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}


double ServoActuatorComp::GetVelocity()
{
	double velocity = 0.0;

	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::GetVelocity() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->GetVelocity(velocity) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return velocity;
}


ReturnType ServoActuatorComp::SetPosition(double position,unsigned long time)
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::SetPosition() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->SetPosition(position, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}


double ServoActuatorComp::GetPosition()
{
	double position = 0.0;

	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::GetPosition() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->GetPosition(position) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return position;
}

void ServoActuatorComp::portSetup()
{
	ProvidedServicePort *pa1;
	pa1=new ServoActuatorServiceProvided(this);
	addPort("ServoActuatorService",pa1);
}

// Call back Declaration
ReturnType ServoActuatorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : ServoActuatorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}

	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif
	
	servoActuator = dynamic_cast<ServoActuator *>(getOprosAPI());
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onInitialize() -> Can't get a handle of ServoActuator API\n");
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	if(servoActuator->Initialize(parameter) != API_SUCCESS) {
		delete servoActuator;
		servoActuator = NULL;

#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return lastError = OPROS_INITIALIZE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onStart()
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType ServoActuatorComp::onStop()
{
	if(servoActuator == NULL) {
		PrintMessage("ERROR : ServoActuatorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(servoActuator->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onReset()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onError()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onRecover()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onDestroy()
{
	if(servoActuator != NULL) {
		servoActuator->Finalize();
		delete servoActuator;
		servoActuator = NULL;
	}

	if(hOprosAPI != NULL) {
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onEvent(Event *evt)
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onExecute()
{
	return lastError = OPROS_SUCCESS;
}
	
ReturnType ServoActuatorComp::onUpdated()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ServoActuatorComp::onPeriodChanged()
{
	return lastError = OPROS_SUCCESS;
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
	return new ServoActuatorComp();
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
	return new ServoActuatorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

