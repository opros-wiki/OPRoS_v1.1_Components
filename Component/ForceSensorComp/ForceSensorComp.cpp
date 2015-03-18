/*************************************************************

 file: ForceSensorComp.cpp
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

#include "ForceSensorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
ForceSensorComp::ForceSensorComp()
{
	hOprosAPI = NULL;
	forceSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
ForceSensorComp::ForceSensorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	forceSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

ForceSensorComp::~ForceSensorComp() 
{
	onDestroy();
}

ReturnType ForceSensorComp::SetParameter(Property parameter)
{
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceSensor->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property ForceSensorComp::GetParameter()
{
	Property parameter;

	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceSensor->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType ForceSensorComp::GetError()
{
	return lastError;
}

ReturnType ForceSensorComp::Enable()
{
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceSensor->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ForceSensorComp::Disable()
{
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<double> ForceSensorComp::GetSensorValue()
{
	vector<double> sensorValue;
	int ret;

	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::GetSensorValue() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = forceSensor->GetSensorValue(sensorValue);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return sensorValue;
}

void ForceSensorComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new ForceSensorServiceProvided(this);
	addPort("ForceSensorService",pa1);

	addPort("forceSensorData", &forceSensorData);
}

// Call back Declaration
ReturnType ForceSensorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}
	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	//	API Casting
	forceSensor = dynamic_cast<ForceSensor *>(getOprosAPI());
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	//	Shared Library 로드
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_NOW);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	//	API 로드
	GET_OPROS_API getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	
	//	API Casting
	forceSensor = static_cast<ForceSensor *>(getOprosAPI());
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif

	if(forceSensor->Initialize(parameter) != API_SUCCESS) {
		delete forceSensor;
		forceSensor = NULL;

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

ReturnType ForceSensorComp::onStart()
{
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceSensor->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType ForceSensorComp::onStop()
{
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ForceSensorComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceSensorComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceSensorComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceSensorComp::onDestroy()
{
	if(forceSensor != NULL) {
		delete forceSensor;
		forceSensor = NULL;
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

ReturnType ForceSensorComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceSensorComp::onExecute()
{
	vector<double> sensorValue;
		
	if(forceSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::onExecute() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	if(forceSensor->GetSensorValue(sensorValue) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}
	else {
		forceSensorData.push(sensorValue);
		lastError = OPROS_SUCCESS;
	}
	lock.Unlock();

	return lastError;
}
	
ReturnType ForceSensorComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceSensorComp::onPeriodChanged()
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
	return new ForceSensorComp();
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
	return new ForceSensorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

