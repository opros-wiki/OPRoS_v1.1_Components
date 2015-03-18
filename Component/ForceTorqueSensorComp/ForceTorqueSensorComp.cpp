/*************************************************************

 file: ForceTorqueSensorComp.cpp
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

#include "ForceTorqueSensorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
ForceTorqueSensorComp::ForceTorqueSensorComp()
{
	hOprosAPI = NULL;
	forceTorqueSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
ForceTorqueSensorComp::ForceTorqueSensorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	forceTorqueSensor = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

ForceTorqueSensorComp::~ForceTorqueSensorComp() 
{
	onDestroy();
}

ReturnType ForceTorqueSensorComp::SetParameter(Property parameter)
{
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceTorqueSensor->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property ForceTorqueSensorComp::GetParameter()
{
	Property parameter;

	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceTorqueSensor->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType ForceTorqueSensorComp::GetError()
{
	return lastError;
}
ReturnType ForceTorqueSensorComp::Enable()
{
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceTorqueSensor->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::Disable()
{
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceTorqueSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<ForceTorqueSensorData> ForceTorqueSensorComp::GetSensorValue()
{
	vector<ForceTorqueSensorData> sensorValue;
	int ret;

	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceSensorComp::ForceTorqueSensorComp() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = forceTorqueSensor->GetSensorValue(sensorValue);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return sensorValue;
}

void ForceTorqueSensorComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new ForceTorqueSensorServiceProvided(this);
	addPort("ForceTorqueSensorService",pa1);

	addPort("forceTorqueSensorData", &forceTorqueSensorData);
}

// Call back Declaration
ReturnType ForceTorqueSensorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}
	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	//	API Casting
	forceTorqueSensor = dynamic_cast<ForceTorqueSensor *>(getOprosAPI());
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	//	Shared Library 로드
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_NOW);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	//	API 로드
	GET_OPROS_API getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	
	//	API Casting
	forceTorqueSensor = static_cast<ForceTorqueSensor *>(getOprosAPI());
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif

	if(forceTorqueSensor->Initialize(parameter) != API_SUCCESS) {
		delete forceTorqueSensor;
		forceTorqueSensor = NULL;

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

ReturnType ForceTorqueSensorComp::onStart()
{
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceTorqueSensor->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType ForceTorqueSensorComp::onStop()
{
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(forceTorqueSensor->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::onDestroy()
{
	if(forceTorqueSensor != NULL) {
		delete forceTorqueSensor;
		forceTorqueSensor = NULL;
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

ReturnType ForceTorqueSensorComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::onExecute()
{
	vector<ForceTorqueSensorData> sensorValue;
		
	if(forceTorqueSensor == NULL) {
		PrintMessage("ERROR : ForceTorqueSensorComp::onExecute() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	if(forceTorqueSensor->GetSensorValue(sensorValue) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}
	else {
		forceTorqueSensorData.push(sensorValue);
		lastError = OPROS_SUCCESS;
	}
	lock.Unlock();

	return lastError;
}
	
ReturnType ForceTorqueSensorComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType ForceTorqueSensorComp::onPeriodChanged()
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
	return new ForceTorqueSensorComp();
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
	return new ForceTorqueSensorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

