/*************************************************************

 file: AccelerationSensorComp.cpp
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

#include "AccelerationSensorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
AccelerationSensorComp::AccelerationSensorComp()
{
	hOprosAPI = NULL;
	accelerationSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
AccelerationSensorComp::AccelerationSensorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	accelerationSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

AccelerationSensorComp::~AccelerationSensorComp() 
{
	onDestroy();
}

ReturnType AccelerationSensorComp::SetParameter(Property parameter)
{
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(accelerationSensor->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property AccelerationSensorComp::GetParameter()
{
	Property parameter;

	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(accelerationSensor->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType AccelerationSensorComp::GetError()
{
	return lastError;
}

ReturnType AccelerationSensorComp::Enable()
{
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(accelerationSensor->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::Disable()
{
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(accelerationSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<double> AccelerationSensorComp::GetSensorValue()
{
	vector<double> sensorValue;
	int ret;

	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::GetSensorValue() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = accelerationSensor->GetSensorValue(sensorValue);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return sensorValue;
}

void AccelerationSensorComp::portSetup() 
{
	addPort("accelerationSensorData", &accelerationSensorData);

	ProvidedServicePort *pa1;
	pa1=new AccelerationSensorServiceProvided(this);
	addPort("AccelerationSensorService",pa1);
}

// Call back Declaration
ReturnType AccelerationSensorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}
	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	//	API Casting
	accelerationSensor = dynamic_cast<AccelerationSensor *>(getOprosAPI());
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	//	Shared Library 로드
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_NOW);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	//	API 로드
	GET_OPROS_API getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	
	//	API Casting
	accelerationSensor = static_cast<AccelerationSensor *>(getOprosAPI());
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif

	if(accelerationSensor->Initialize(parameter) != API_SUCCESS) {
		delete accelerationSensor;
		accelerationSensor = NULL;

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

ReturnType AccelerationSensorComp::onStart()
{
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(accelerationSensor->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType AccelerationSensorComp::onStop()
{
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(accelerationSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::onDestroy()
{
	if(accelerationSensor != NULL) {
		delete accelerationSensor;
		accelerationSensor = NULL;
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

ReturnType AccelerationSensorComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::onExecute()
{
	vector<double> sensorValue;
		
	if(accelerationSensor == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::onExecute() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	if(accelerationSensor->GetSensorValue(sensorValue) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}
	else {
		accelerationSensorData.push(sensorValue);
		lastError = OPROS_SUCCESS;
	}
	lock.Unlock();

	return lastError;
}
	
ReturnType AccelerationSensorComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType AccelerationSensorComp::onPeriodChanged()
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
	return new AccelerationSensorComp();
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
	return new AccelerationSensorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

