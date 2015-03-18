/*************************************************************

 file: BatteryManagementSystemComp.cpp
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

#include "BatteryManagementSystemComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
BatteryManagementSystemComp::BatteryManagementSystemComp()
{
	hOprosAPI = NULL;
	batteryManagementSystem = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
BatteryManagementSystemComp::BatteryManagementSystemComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	batteryManagementSystem = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

BatteryManagementSystemComp::~BatteryManagementSystemComp() 
{
	onDestroy();
}

ReturnType BatteryManagementSystemComp::SetParameter(Property parameter)
{
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(batteryManagementSystem->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property BatteryManagementSystemComp::GetParameter()
{
	Property parameter;

	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(batteryManagementSystem->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType BatteryManagementSystemComp::GetError()
{
	return lastError;
}

ReturnType BatteryManagementSystemComp::Enable()
{
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(batteryManagementSystem->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::Disable()
{
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(batteryManagementSystem->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

double BatteryManagementSystemComp::GetChargingRate()
{
	double chargingRate;
	int ret;

	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::GetChargingRate() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = batteryManagementSystem->GetChargingRate(chargingRate);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return chargingRate;
}

double BatteryManagementSystemComp::GetVoltage()
{
	double voltage;
	int ret;

	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::GetVoltage() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = batteryManagementSystem->GetVoltage(voltage);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return voltage;
}

vector<double> BatteryManagementSystemComp::GetCellVoltage()
{
	vector<double> cellVoltage;
	int ret;

	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : AccelerationSensorComp::GetCellVoltage() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	ret = batteryManagementSystem->GetCellVoltage(cellVoltage);
	lock.Unlock();

	if(ret < 0)		lastError = OPROS_CALL_API_ERROR;
	else			lastError = OPROS_SUCCESS;

	return cellVoltage;
}

void BatteryManagementSystemComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new BatteryManagementSystemServiceProvided(this);
	addPort("BatteryManagementSystemService",pa1);

	addPort("batteryChargingRate", &batteryChargingRate);
}

// Call back Declaration
ReturnType BatteryManagementSystemComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}
	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	//	API Casting
	batteryManagementSystem = dynamic_cast<BatteryManagementSystem *>(getOprosAPI());
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	//	Shared Library 로드
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_NOW);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	//	API 로드
	GET_OPROS_API getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
	
	//	API Casting
	batteryManagementSystem = static_cast<BatteryManagementSystem *>(getOprosAPI());
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onInitialize() -> Can't get a handle of AccelerationSensor API\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif

	if(batteryManagementSystem->Initialize(parameter) != API_SUCCESS) {
		delete batteryManagementSystem;
		batteryManagementSystem = NULL;

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

ReturnType BatteryManagementSystemComp::onStart()
{
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(batteryManagementSystem->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType BatteryManagementSystemComp::onStop()
{
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(batteryManagementSystem->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::onDestroy()
{
	if(batteryManagementSystem != NULL) {
		delete batteryManagementSystem;
		batteryManagementSystem = NULL;
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

ReturnType BatteryManagementSystemComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::onExecute()
{
	double chargingRate;
		
	if(batteryManagementSystem == NULL) {
		PrintMessage("ERROR : BatteryManagementSystemComp::onExecute() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	lock.Lock();
	if(batteryManagementSystem->GetChargingRate(chargingRate) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}
	else {
		batteryChargingRate.push(chargingRate);
		lastError = OPROS_SUCCESS;
	}
	lock.Unlock();

	return lastError;
}
	
ReturnType BatteryManagementSystemComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType BatteryManagementSystemComp::onPeriodChanged()
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
	return new BatteryManagementSystemComp();
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
	return new BatteryManagementSystemComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

