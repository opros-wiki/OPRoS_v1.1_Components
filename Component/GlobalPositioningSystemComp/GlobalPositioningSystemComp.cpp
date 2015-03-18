/*************************************************************

 file: GlobalPositioningSystemComp.cpp
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

#include "GlobalPositioningSystemComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
GlobalPositioningSystemComp::GlobalPositioningSystemComp()
{
	hOprosAPI = NULL;
	gps = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
GlobalPositioningSystemComp::GlobalPositioningSystemComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	gps = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

GlobalPositioningSystemComp::~GlobalPositioningSystemComp() 
{
	onDestroy();
}

ReturnType GlobalPositioningSystemComp::SetParameter(Property parameter)
{
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(gps->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property GlobalPositioningSystemComp::GetParameter()
{
	Property parameter;

	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(gps->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType GlobalPositioningSystemComp::GetError()
{
	return lastError;
}

ReturnType GlobalPositioningSystemComp::Enable()
{
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(gps->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::Disable()
{
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(gps->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<GpsData> GlobalPositioningSystemComp::GetSensorValue()
{
	vector<GpsData> sensorValue;
	int ret;

	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::GetSensorValue() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = gps->GetSensorValue(sensorValue);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return sensorValue;
}

void GlobalPositioningSystemComp::portSetup() {
	ProvidedServicePort *pa1;
	pa1=new GlobalPositioningSystemServiceProvided(this);
	addPort("GlobalPositioningSystemService",pa1);

	addPort("gpsData", &gpsData);
}

// Call back Declaration
ReturnType GlobalPositioningSystemComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}
	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	//	API Casting
	gps = dynamic_cast<GlobalPositioningSystem *>(getOprosAPI());
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	//	Shared Library 로드
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_NOW);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	//	API 로드
	GET_OPROS_API getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	
	//	API Casting
	gps = static_cast<GlobalPositioningSystem *>(getOprosAPI());
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif

	if(gps->Initialize(parameter) != API_SUCCESS) {
		delete gps;
		gps = NULL;

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

ReturnType GlobalPositioningSystemComp::onStart()
{
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(gps->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType GlobalPositioningSystemComp::onStop()
{
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(gps->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::onDestroy()
{
	if(gps != NULL) {
		delete gps;
		gps = NULL;
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

ReturnType GlobalPositioningSystemComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::onExecute()
{
	vector<GpsData> sensorValue;
		
	if(gps == NULL) {
		PrintMessage("ERROR : GlobalPositioningSystemComp::onExecute() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	if(gps->GetSensorValue(sensorValue) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}
	else {
		gpsData.push(sensorValue);
		lastError = OPROS_SUCCESS;
	}
	lock.Unlock();

	return lastError;
}
	
ReturnType GlobalPositioningSystemComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GlobalPositioningSystemComp::onPeriodChanged()
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
	return new GlobalPositioningSystemComp();
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
	return new GlobalPositioningSystemComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

