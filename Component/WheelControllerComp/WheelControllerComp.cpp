/*************************************************************

 file: WheelControllerComp.cpp
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

#include "WheelControllerComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
WheelControllerComp::WheelControllerComp()
{
	hOprosAPI = NULL;
	wheelController = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// constructor declaration (with name)
//
WheelControllerComp::WheelControllerComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	wheelController = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

WheelControllerComp::~WheelControllerComp() 
{
	onDestroy();
}

ReturnType WheelControllerComp::SetParameter(Property parameter)
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::SetParameter() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->SetParameter(parameter) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

Property WheelControllerComp::GetParameter()
{
	Property parameter;

	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::GetParameter() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->GetParameter(parameter) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return parameter;
}

ReturnType WheelControllerComp::GetError()
{
	return lastError;
}

ReturnType WheelControllerComp::Enable()
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::Enable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->Enable() < 0) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}
	
	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::Disable()
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::Disable() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::SetPosition(ObjectLocation position)
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::SetPosition() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->SetPosition(position) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ObjectLocation WheelControllerComp::GetPosition()
{
	ObjectLocation position;

	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::GetPosition() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->GetPosition(position) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return position;
}

vector<long> WheelControllerComp::GetOdometery()
{
	vector<long> odometery;

	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::GetOdometery() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->GetOdometery(odometery) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return odometery;
}

ReturnType WheelControllerComp::DriveWheel(double linearVelocity,double angularVelocity)
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::DriveWheel() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->DriveWheel(linearVelocity, angularVelocity) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::MoveWheel(double distance,double linearVelocity)
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::MoveWheel() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->MoveWheel(distance, linearVelocity) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::RotateWheel(double angle,double angularVelocity)
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::RotateWheel() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->RotateWheel(angle, angularVelocity) < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::StopWheel()
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::StopWheel() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->StopWheel() < 0) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

bool WheelControllerComp::IsWheelRunning()
{
	bool isWheelRunning = false;

	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::IsWheelRunning() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->IsWheelRunning(isWheelRunning) < 0) {
		lastError = OPROS_CALL_API_ERROR;
	}

	lastError = OPROS_SUCCESS;

	return isWheelRunning;
}

void WheelControllerComp::portSetup() 
{
	ProvidedServicePort *pa1;
	pa1=new WheelControllerServiceProvided(this);
	addPort("WheelControllerService",pa1);

	// export variable setup
}

// Call back Declaration
ReturnType WheelControllerComp::onInitialize()
{
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	if(parameter.FindName("ApiName") == false) {
		PrintMessage("ERROR : WheelControllerComp::onInitialize() -> Can't find the APIName in property\n");
		return lastError = OPROS_FIND_PROPERTY_ERROR;
	}

	
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ApiName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : WheelControllerComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : WheelControllerComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("ApiName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : WheelControllerComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("ApiName").c_str());
		return lastError = OPROS_FIND_DLL_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : WheelControllerComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}
#endif
	
	wheelController = dynamic_cast<WheelController *>(getOprosAPI());
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::onInitialize() -> Can't get a handle of ServoActuator API\n");
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return lastError = OPROS_LOAD_DLL_ERROR;
	}

	if(wheelController->Initialize(parameter) != API_SUCCESS) {
		delete wheelController;
		wheelController = NULL;

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

ReturnType WheelControllerComp::onStart()
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::onStart() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->Enable() == API_ERROR) {
		return lastError = OPROS_ENABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType WheelControllerComp::onStop()
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::onStop() -> The handle of API is not created.\n");
		return lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->Disable() < 0) {
		return lastError = OPROS_DISABLE_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::onReset()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::onError()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::onRecover()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::onDestroy()
{
	if(wheelController != NULL) {
		wheelController->Finalize();
		delete wheelController;
		wheelController = NULL;
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

ReturnType WheelControllerComp::onEvent(Event *evt)
{
	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::onExecute()
{
	if(wheelController == NULL) {
		PrintMessage("ERROR : WheelControllerComp::IsWheelRunning() -> The handle of API is not created.\n");
		lastError = OPROS_PRECONDITION_NOT_MET;
	}

	if(wheelController->OnExecute() != API_SUCCESS) {
		return lastError = OPROS_CALL_API_ERROR;
	}

	return lastError = OPROS_SUCCESS;
}
	
ReturnType WheelControllerComp::onUpdated()
{
	return lastError = OPROS_SUCCESS;
}

ReturnType WheelControllerComp::onPeriodChanged()
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
	return new WheelControllerComp();
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
	return new WheelControllerComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

