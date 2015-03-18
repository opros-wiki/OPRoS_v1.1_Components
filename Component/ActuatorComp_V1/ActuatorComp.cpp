/*************************************************************

 file: ActuatorComp.cpp
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

#include "ActuatorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
ActuatorComp::ActuatorComp()
{
	hOprosAPI = NULL;
	actuator = NULL;
	lastError = OPROS_SUCCESS;

	portSetup();
}

//
// constructor declaration (with name)
//
ActuatorComp::ActuatorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	actuator = NULL;
	lastError = OPROS_SUCCESS;

	portSetup();
}

//
// destructor declaration
//

ActuatorComp::~ActuatorComp() 
{
	onDestroy();
}

ReturnType ActuatorComp::SetParameter(Property parameter)
{
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property ActuatorComp::GetParameter()
{
	Property parameter;

	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType ActuatorComp::GetError()
{
	return lastError;
}

ReturnType ActuatorComp::Enable()
{
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::Disable()
{
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::SetPower(double power,unsigned long time)
{
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::SetPower() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->SetPower(power, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

double ActuatorComp::GetPower()
{
	double power = 0.0;

	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::GetPower() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->GetPower(power) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return power;
}

void ActuatorComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new ActuatorServiceProvided(this);
	addPort("ActuatorService",pa1);
}

// Call back Declaration
ReturnType ActuatorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : ActuatorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}

	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ActuatorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : ActuatorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ActuatorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : ActuatorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif
	
	actuator = dynamic_cast<Actuator *>(getOprosAPI());
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::onInitialize() -> Can't get a handle of Actuator API\n");
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	if(actuator->Initialize(parameter) != API_SUCCESS) {
		delete actuator;
		actuator = NULL;

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

ReturnType ActuatorComp::onStart()
{
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType ActuatorComp::onStop()
{
	if(actuator == NULL) {
		PrintMessage("ERROR : ActuatorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(actuator->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::onReset()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::onError()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::onRecover()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::onDestroy()
{
	if(actuator != NULL) {
		actuator->Finalize();
		delete actuator;
		actuator = NULL;
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

ReturnType ActuatorComp::onEvent(Event *evt)
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::onExecute()
{
	return lastError = OPROS_SUCCESS;
}
	
ReturnType ActuatorComp::onUpdated()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ActuatorComp::onPeriodChanged()
{
	return lastError = OPROS_SUCCESS;
}

#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new ActuatorComp();
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
	return new ActuatorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif


