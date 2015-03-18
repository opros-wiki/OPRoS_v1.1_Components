/*************************************************************

 file: ManipulatorComp.cpp
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

#include "ManipulatorComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
ManipulatorComp::ManipulatorComp()
{
	hOprosAPI = NULL;
	manipulator = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
ManipulatorComp::ManipulatorComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	manipulator = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

ManipulatorComp::~ManipulatorComp() 
{
	onDestroy();
}

ReturnType ManipulatorComp::SetParameter(Property parameter)
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property ManipulatorComp::GetParameter()
{
	Property parameter;

	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType ManipulatorComp::GetError()
{
	return lastError;
}

ReturnType ManipulatorComp::Enable()
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}
	
	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::Disable()
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::SetPower(vector<double> power,vector<unsigned long> time)
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::SetPower() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->SetPower(power, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<double> ManipulatorComp::GetPower()
{
	vector<double> power;

	if(manipulator == NULL) {
		PrintMessage("ERROR : ActuatorComp::ManipulatorComp() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->GetPower(power) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return power;
}

ReturnType ManipulatorComp::SetVelocity(vector<double> velocity,vector<unsigned long> time)
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::SetVelocity() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->SetVelocity(velocity, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<double> ManipulatorComp::GetVelocity()
{
	vector<double> velocity;

	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::GetVelocity() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->GetVelocity(velocity) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return velocity;
}

ReturnType ManipulatorComp::SetPosition(vector<double> position,vector<unsigned long> time)
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : SetPosition::SetPosition() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->SetPosition(position, time) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

vector<double> ManipulatorComp::GetPosition()
{
	vector<double> position;

	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::GetPosition() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->GetPosition(position) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return position;
}

void ManipulatorComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new ManipulatorServiceProvided(this);
	addPort("ManipulatorService",pa1);

	// export variable setup
}

// Call back Declaration
ReturnType ManipulatorComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : ManipulatorComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}

	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ManipulatorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : ManipulatorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : ManipulatorComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : ManipulatorComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif
	
	manipulator = dynamic_cast<Manipulator *>(getOprosAPI());
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::onInitialize() -> Can't get a handle of Actuator API\n");
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	if(manipulator->Initialize(parameter) != API_SUCCESS) {
		delete manipulator;
		manipulator = NULL;

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

ReturnType ManipulatorComp::onStart()
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	if(manipulator->RunHoming() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType ManipulatorComp::onStop()
{
	if(manipulator == NULL) {
		PrintMessage("ERROR : ManipulatorComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(manipulator->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::onReset()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::onError()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::onRecover()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::onDestroy()
{
	if(manipulator != NULL) {
		manipulator->Finalize();
		delete manipulator;
		manipulator = NULL;
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

ReturnType ManipulatorComp::onEvent(Event *evt)
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::onExecute()
{
	return lastError = OPROS_SUCCESS;
}
	
ReturnType ManipulatorComp::onUpdated()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType ManipulatorComp::onPeriodChanged()
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
	return new ManipulatorComp();
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
	return new ManipulatorComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

