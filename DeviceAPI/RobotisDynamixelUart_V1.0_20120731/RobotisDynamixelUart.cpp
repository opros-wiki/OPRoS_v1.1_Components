/*************************************************************

 file: RobotisDynamixelUart.cpp
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

#include "RobotisDynamixelUart.h"
#include "OprosPrintMessage.h"

RobotisDynamixelUart::RobotisDynamixelUart(void)
{
	uart = NULL;
	hOprosAPI = NULL;
}

RobotisDynamixelUart::~RobotisDynamixelUart(void)
{
	Finalize();
}

int RobotisDynamixelUart::Initialize(Property parameter)
{
	if(status != DEVICE_CREATED) {
		return Finalize();
	}

	if(InitializeUART(parameter) != API_SUCCESS) {
		PrintMessage("ERROR : RobotisDynamixelUart::Initialize() -> Occur a error in InitializeUART().\n");
		Finalize();		status = DEVICE_ERROR;
		return API_ERROR;
	}

	if(parameter.FindName("Size") == false) {
		Finalize();		status = DEVICE_ERROR;
		return API_ERROR;
	}

	int size = size = atoi(parameter.GetValue("Size").c_str());
	if(size < 1) {
		Finalize();		status = DEVICE_ERROR;
		return API_ERROR;
	}

	char strTmp[16];
	for(int i = 0; i < size; i++) {
		sprintf(strTmp, "Id%d", i);
		if(parameter.FindName(strTmp) == false) {
			Finalize();		status = DEVICE_ERROR;
			return API_ERROR;
		}

		joints.push_back(DynamixelInfo(uart, (unsigned char)atoi(parameter.GetValue(strTmp).c_str())));
	}

	if(SetParameter(parameter) != API_SUCCESS) {
		Finalize();		status = DEVICE_ERROR;
		return API_ERROR;
	}
	
	status = DEVICE_READY;
	
	return API_SUCCESS;
}

int RobotisDynamixelUart::Finalize(void)
{
	Disable();

	joints.clear();

	if(uart != NULL) {
		delete uart;
		uart = NULL;
	}

	if(hOprosAPI != NULL) {
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
	}

	status = DEVICE_CREATED;

	return API_SUCCESS;
}

int RobotisDynamixelUart::Enable(void)
{
	if(status == DEVICE_CREATED || status == DEVICE_ERROR) {
		return API_ERROR;
	}
	else if(status == DEVICE_ACTIVE) {
		return API_SUCCESS;
	}
	
	for(size_t i = 0; i < joints.size(); i++) {
		if(joints[i].dynamixel->SetTorqueEnable(1) == false) {
			PrintMessage("ERROR : RobotisDynamixelUart::Enable() -> Occur a error in SetTorqueEnable().\n");
			return API_ERROR;
		}
	}

	status = DEVICE_ACTIVE;

	return API_SUCCESS;
}

int RobotisDynamixelUart::Disable(void)
{
	if(status == DEVICE_CREATED || status == DEVICE_ERROR) {
		return API_ERROR;
	}
	else if(status == DEVICE_READY) {
		return API_SUCCESS;
	}

	for(size_t i = 0; i < joints.size(); i++) {
		if(joints[i].dynamixel->SetTorqueEnable(0) == false) {
			PrintMessage("ERROR : RobotisDynamixelUart::Disable() -> Occur a error in SetTorqueEnable().\n");
			return API_ERROR;
		}
	}

	status = DEVICE_READY;

	return API_SUCCESS;
}

int RobotisDynamixelUart::SetParameter(Property parameter)
{
	int size;

	if(parameter.FindName("Size") == false)	goto ERROR_Robotis_DynamixelUART_SetParameter;
	size = atoi(parameter.GetValue("Size").c_str());

	if(size != (int)joints.size()) {
		return API_ERROR;
	}
	
	char strTmp[32];
	for(int i = 0; i < size; i++) {
		sprintf(strTmp, "MaximumPower%d", i);			if(parameter.FindName(strTmp) == false)		goto ERROR_Robotis_DynamixelUART_SetParameter;
		if(joints[i].dynamixel->SetMaximumTorque((unsigned short)atoi(parameter.GetValue(strTmp).c_str())) == false) return API_ERROR;

		sprintf(strTmp, "MaximumVelocity%d", i);		if(parameter.FindName(strTmp) == false)		goto ERROR_Robotis_DynamixelUART_SetParameter;
		if(joints[i].dynamixel->SetMovingSpeed((unsigned short)atoi(parameter.GetValue(strTmp).c_str())) == false) return API_ERROR;
		
		sprintf(strTmp, "MinimumPositionLimit%d", i);	if(parameter.FindName(strTmp) == false)		goto ERROR_Robotis_DynamixelUART_SetParameter;
		if(joints[i].dynamixel->SetCCWAngleLimit((unsigned short)((atof(parameter.GetValue(strTmp).c_str()) + 150.0) * 1023 / 300.0)) == false) return API_ERROR;

		sprintf(strTmp, "MaximumPositionLimit%d", i);	if(parameter.FindName(strTmp) == false)		goto ERROR_Robotis_DynamixelUART_SetParameter;
		if(joints[i].dynamixel->SetCWAngleLimit((unsigned short)((atof(parameter.GetValue(strTmp).c_str()) + 150.0) * 1023 / 300.0)) == false) return API_ERROR;
	}
	
	this->parameter = parameter;

	return API_SUCCESS;

ERROR_Robotis_DynamixelUART_SetParameter :
	PrintMessage("ERROR : RobotisDynamixelUart::SetParameter() -> Can't find parameters\n");

	return API_ERROR;
}

int RobotisDynamixelUart::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int RobotisDynamixelUart::SetPosition(vector<double> position, vector<unsigned long> time)
{
	if(status != DEVICE_ACTIVE) {
		return API_ERROR;
	}

	if(joints.size() != position.size()) {
		return API_ERROR;
	}

	for(size_t i = 0; i < joints.size(); i++) {
		//	Angle -> Pulse
		unsigned short tmp = (unsigned short)((position[i] + 150.0) * 1023.0 / 300.0);

		if(joints[i].dynamixel->SetGoalPosition(tmp) == false) {
			PrintMessage("ERROR : RobotisDynamixelUart::SetPosition() -> Occur a error in SetGoalPosition()\n");
			return API_ERROR;
		}
	}

	return API_SUCCESS;
}

int RobotisDynamixelUart::GetPosition(vector<double> &position)
{
	if(status != DEVICE_ACTIVE) {
		return API_ERROR;
	}

	if(joints.size() != position.size()) {
		position.resize(joints.size());
	}
	unsigned short tmp;

	for(size_t i = 0; i < joints.size(); i++) {
		if(joints[i].dynamixel->GetPresentPosition(tmp) == false) {
			PrintMessage("ERROR : RobotisDynamixelUart::GetPosition() -> Occur a error in GetPresentPosition()\n");
			position[i] = 0.0;
			return API_ERROR;
		}

		//	Pulse -> Angle
		position[i] = (double)(tmp) / 1023.0 * 300.0 + 150.0;
	}

	return API_SUCCESS;
}

int RobotisDynamixelUart::InitializeUART(Property parameter)
{
	Property uartParameter;
	char *error = NULL;

	if(parameter.FindName("UartApiName") == false)	goto ERROR_Robotis_DynamixelUART_InitializeUART;
	if(parameter.FindName("PortName") == false)		goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("PortName", parameter.GetValue("PortName"));
	if(parameter.FindName("TimeOut") == false)		goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("TimeOut", parameter.GetValue("TimeOut"));
	if(parameter.FindName("BaudRate") == false)		goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("BaudRate", parameter.GetValue("BaudRate"));
	if(parameter.FindName("DataBits") == false)		goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("DataBits", parameter.GetValue("DataBits"));
	if(parameter.FindName("StopBits") == false)		goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("StopBits", parameter.GetValue("StopBits"));
	if(parameter.FindName("Parity") == false)		goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("Parity", parameter.GetValue("Parity"));
	if(parameter.FindName("FlowControl") == false)	goto ERROR_Robotis_DynamixelUART_InitializeUART;
	else	uartParameter.SetValue("FlowControl", parameter.GetValue("FlowControl"));

	if(uart != NULL) {
		delete uart;
		uart = NULL;
	}
#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("UartApiName").c_str());
	if(hOprosAPI == NULL) {
		return API_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return API_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("UartApiName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		return API_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	error = dlerror();
	if(error != NULL) {
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return API_ERROR;
	}
#endif

	uart = (Uart *)getOprosAPI();
	if(uart == NULL) {
		Finalize();
		return API_ERROR;
	}

	//	API 초기화
	if(uart->Initialize(uartParameter) < 0) {
		Finalize();
		return API_ERROR;
	}

	if(uart->Enable() < 0) {
		Finalize();
		return API_ERROR;
	}

	return API_SUCCESS;

ERROR_Robotis_DynamixelUART_InitializeUART :
	PrintMessage("ERROR : RobotisDynamixelUart::InitializeUART() -> Can't find a property in parameter.\n");
	return API_ERROR;
}

#if defined(WIN32)
extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new RobotisDynamixelUart();
}

#endif
