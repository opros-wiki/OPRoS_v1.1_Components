/*************************************************************

 file: UltrasonicSensorComp.cpp
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

#include "UltrasonicSensorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
UltrasonicSensorComp::UltrasonicSensorComp()
{
	hOprosAPI = NULL;
	ultrasonicSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
UltrasonicSensorComp::UltrasonicSensorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	ultrasonicSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

UltrasonicSensorComp::~UltrasonicSensorComp() 
{
	onDestroy();
}

ReturnType UltrasonicSensorComp::SetParameter(Property parameter)
{
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property UltrasonicSensorComp::GetParameter()
{
	Property parameter;

	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType UltrasonicSensorComp::GetError()
{
	return lastError;
}

ReturnType UltrasonicSensorComp::Enable()
{
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::Disable()
{
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<double> UltrasonicSensorComp::GetSensorValue()
{
	vector<double> sensorValue;

	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::GetSensorValue() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	if(ultrasonicSensor->GetSensorValue(sensorValue) < 0)		lastError = OPROS_CALL_API_ERROR;
	else														lastError = OPROS_SUCCESS;

	lock.Unlock();

	return sensorValue;
}

void UltrasonicSensorComp::portSetup()
{
	addPort("ultrasonicSensorData", &ultrasonicSensorData);

	ProvidedServicePort *pa1;
	pa1=new UltrasonicSensorServiceProvided(this);
	addPort("UltrasonicSensorService",pa1);
}

// Call back Declaration
ReturnType UltrasonicSensorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}

	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	ultrasonicSensor = dynamic_cast<UltrasonicSensor *>(getOprosAPI());
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't get a handle of UltrasonicSensor API\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_NOW);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	GET_OPROS_API getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	ultrasonicSensor = static_cast<UltrasonicSensor *>(getOprosAPI());
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onInitialize() -> Can't get a handle of UltrasonicSensor API\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif

	if(ultrasonicSensor->Initialize(parameter) != API_SUCCESS) {
		delete ultrasonicSensor;
		ultrasonicSensor = NULL;

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

ReturnType UltrasonicSensorComp::onStart()
{
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType UltrasonicSensorComp::onStop()
{
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::onReset()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::onError()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::onRecover()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::onDestroy()
{
	if(ultrasonicSensor != NULL) {
		ultrasonicSensor->Finalize();
		delete ultrasonicSensor;
		ultrasonicSensor = NULL;
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

ReturnType UltrasonicSensorComp::onEvent(Event *evt)
{
	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::onExecute()
{
	vector<double> sensorValue;

	lock.Lock();
	if(ultrasonicSensor == NULL) {
		PrintMessage("ERROR : UltrasonicSensorComp::onExecute() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(ultrasonicSensor->GetSensorValue(sensorValue) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}
	lock.Unlock();

	ultrasonicSensorData.push(sensorValue);

	return lastError = OPROS_SUCCESS;
}
	
ReturnType UltrasonicSensorComp::onUpdated()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType UltrasonicSensorComp::onPeriodChanged()
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
	return new UltrasonicSensorComp();
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
	return new UltrasonicSensorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

