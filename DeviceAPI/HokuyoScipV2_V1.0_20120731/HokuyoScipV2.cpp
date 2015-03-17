/*************************************************************

 file: HokuyoScipV2.cpp
 author: E.C. Shin
 begin: January 30 2010
 copyright: (c) 2010 KITECH, OPRoS
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
#include <string.h>
#include "HokuyoScipV2.h"
#include "OprosTimer.h"
#include "OprosPrintMessage.h"

//#define DUMMY

#if defined(WIN32)
#pragma warning(disable : 4996)
#endif

#define	ASCII_LF						(10)
#define	ASCII_CR						(13)
#define	SCIPV2_0_DELAY					(500)
#define	MAXIMUM_CLUSTER_SIZE			(99)

HokuyoScipV2::HokuyoScipV2(void)
{
	hOprosAPI = NULL;
	uart = NULL;
}

HokuyoScipV2::~HokuyoScipV2(void)
{
	if(uart != NULL) {
		delete uart;
	}
}

int HokuyoScipV2::Initialize(Property parameter)
{
	if(InitializeUART(parameter) != API_SUCCESS) {
		return API_ERROR;
	}

	if(SetParameter(parameter) != API_SUCCESS) {
		return API_ERROR;
	}

	if(Hokuyo_SCIPV2_0_ChangeVer2_0() < 0) {
		return API_ERROR;
	}

	if(Hokuyo_SCIPV2_0_Reset() < 0) {
		return API_ERROR;
	}

	return API_SUCCESS;
}

int HokuyoScipV2::Finalize(void)
{
	FinalizeUART();

	return API_SUCCESS;
}

int HokuyoScipV2::Enable(void)
{
	unsigned char status;

	uart->Lock();
	if(Hokuyo_SCIPV2_0_Enable(&status) < 0) {
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();

	return API_SUCCESS;
}

int HokuyoScipV2::Disable(void)
{
	if(Hokuyo_SCIPV2_0_Disable() < 0) {
		return API_ERROR;
	}

	return API_SUCCESS;
}

int HokuyoScipV2::SetParameter(Property parameter)
{
	if(parameter.FindName("X") == false)				return API_ERROR;
	if(parameter.FindName("Y") == false)				return API_ERROR;
	if(parameter.FindName("Z") == false)				return API_ERROR;
	if(parameter.FindName("Roll") == false)				return API_ERROR;
	if(parameter.FindName("Pitch") == false)			return API_ERROR;
	if(parameter.FindName("Yaw") == false)				return API_ERROR;
	if(parameter.FindName("StartAngle") == false)		return API_ERROR;
	if(parameter.FindName("EndAngle") == false)			return API_ERROR;
	if(parameter.FindName("MaximumPoint") == false)		return API_ERROR;
	if(parameter.FindName("StartPoint") == false)		return API_ERROR;
	if(parameter.FindName("EndPoint") == false)			return API_ERROR;
	if(parameter.FindName("DeltaPoint") == false)		return API_ERROR;
	if(parameter.FindName("ErrorRate") == false)		return API_ERROR;

	maximumStep = atoi(parameter.GetValue("MaximumStep").c_str());
	startStep = atoi(parameter.GetValue("StartStep").c_str());
	endStep = atoi(parameter.GetValue("EndStep").c_str());
	deltaStep = atoi(parameter.GetValue("DeltaStep").c_str());

	if(endStep < 0 || endStep > maximumStep) {
		return API_ERROR;
	}

	if(startStep < 0 || startStep >= endStep || startStep >= endStep) {
		return API_ERROR;
	}

	if(deltaStep <= 0 || deltaStep > MAXIMUM_CLUSTER_SIZE) {
		return API_ERROR;
	}

	scanStepSize = (endStep	- startStep) / deltaStep + 1;
	scannedData.resize(scanStepSize);

	this->parameter = parameter;

	return 0;
}

int HokuyoScipV2::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int HokuyoScipV2::GetSensorValue(vector<double> &sensorValue)
{
	uart->Lock();

	if(Hokuyo_SCIPV2_0_GetLastData() < 0) {
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();

	for(int i = 0; i < scanStepSize; i++) {
		sensorValue.push_back((double)scannedData[i] * 0.001);
	}

	return scanStepSize;
}

int HokuyoScipV2::InitializeUART(Property parameter)
{
	Property uartParameter;

	if(parameter.FindName("UartApiName") == false)	return API_ERROR;
	if(parameter.FindName("PortName") == false)		return API_ERROR;	uartParameter.SetValue("PortName", parameter.GetValue("PortName"));
	if(parameter.FindName("TimeOut") == false)		return API_ERROR;	uartParameter.SetValue("TimeOut", parameter.GetValue("TimeOut"));
	if(parameter.FindName("BaudRate") == false)		return API_ERROR;	uartParameter.SetValue("BaudRate", parameter.GetValue("BaudRate"));
	if(parameter.FindName("DataBits") == false)		return API_ERROR;	uartParameter.SetValue("DataBits", parameter.GetValue("DataBits"));
	if(parameter.FindName("StopBits") == false)		return API_ERROR;	uartParameter.SetValue("StopBits", parameter.GetValue("StopBits"));
	if(parameter.FindName("Parity") == false)		return API_ERROR;	uartParameter.SetValue("Parity", parameter.GetValue("Parity"));
	if(parameter.FindName("FlowControl") == false)	return API_ERROR;	uartParameter.SetValue("FlowControl", parameter.GetValue("FlowControl"));

	if(uart != NULL) {
		delete uart;
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
		PrintMessage("ERROR : InitializeUART() -> dlopen (%s)\n",parameter.GetValue("UARTAPIName").c_str());
		return API_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage ("ERROR -> %s\n", error);
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return API_ERROR;
	}
#endif

	uart = (Uart *)getOprosAPI();
	if(uart == NULL) {
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return API_ERROR;
	}

	//	API 초기화
	if(uart->Initialize(uartParameter) < 0) {
		FinalizeUART();
		return API_ERROR;
	}

	if(uart->Enable() < 0) {
		FinalizeUART();
		return API_ERROR;
	}

	return API_SUCCESS;
}

int HokuyoScipV2::FinalizeUART(void)
{
	if(uart != NULL) {
		uart->Disable();
		uart->Finalize();
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

	return API_SUCCESS;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_SendPack(unsigned char *data, int size)
{
	if(uart == NULL) {
		return -1;
	}

	int ret;
	OprosTimer timer;
	timer.StartTimeTick();

	if(size <= 0) {
		return 0;
	}
	
	for(int i = 0; i < size; i += ret) {
		ret = uart->Write((data + i), size - i);
		if(ret < 0) {
			return -1;
		}
		else if(ret == 0) {
			if(timer.GetTimeTick() > SCIPV2_0_DELAY) {
				return -1;
			}
		}
	}

	return size;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_RecvPack(unsigned char *data, int size)
{
	if(uart == NULL) {
		return -1;
	}

	int count = 0;
	int ret;
	bool isLast = false;
	OprosTimer timer;

	timer.StartTimeTick();

	for(int i = 0; i < size; i += ret) {
		ret = uart->Read((data + i), 1);
		
		if(ret < 0) {
			return -1;
		}
		else if(ret == 0) {
			if(timer.GetTimeTick() > SCIPV2_0_DELAY) {
				return -1;
			}
			OprosSleep(0);
		}
		else {
			if(*(data + count++) == ASCII_LF) {
				if(isLast == true) {
					return count;
				}
				else
					isLast = true;
			}
			else {
				isLast = false;
			}
		}
	}

	return -1;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_CheckSum(unsigned char *data, int size)
{
	unsigned short checkSum = 0x00;
	int index;

	for(index = 0; index < size; index++) {
		checkSum += *(data + index);
	}
	checkSum = (checkSum & 0x3F) + *(data + index);
	if(checkSum == 0x6F) {
		return 0;
	}
	
	return -1;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_ParsingData(unsigned char *data, int size)
{
	int index = 0;
	unsigned char buf[4096];

	for(int i = 0; i < size; i++) {
		if(((i % 66) == 64) || ((i % 66) == 65)) {	// 64, 65 번째 데이터 버림
			continue;
		}
		if(*(data + i) != ' ') {
			buf[index++] = *(data + i);
		}
	}

	index -= 3;		// 마지막 3 byte 버림

	int count = 0;
	if(index != (int)scannedData.size()) {
		scannedData.resize(index);
	}
	for(int i = 0; i < index; count++) {
		scannedData[count] = (buf[i++] - 0x30) << 12;
		scannedData[count] += (buf[i++] - 0x30) << 6;
		scannedData[count] += (buf[i++] - 0x30);

		if(scannedData[count] == 0) {
			scannedData[count] = 8000;
		}
		else if(scannedData[count] < 20) {
			scannedData[count] = -1;
		}
		else if(scannedData[count] > 8000) {
			scannedData[count] = 8000;
		}
	}

	return index;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_ParsingTimeStamp(unsigned char *data)
{
	int timeStamp;

	timeStamp = ((*(data + 0) - 0x30) << 18);
	timeStamp += ((*(data + 1) - 0x30) << 12);
	timeStamp += ((*(data + 2) - 0x30) << 6);
	timeStamp += (*(data + 3) - 0x30);

	return timeStamp;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_ChangeVer2_0(void)
{
#ifndef DUMMY
	unsigned char sendData[] = {'S', 'C', 'I', 'P', '2', '.', '0', ASCII_LF};
	unsigned char recvData[16] = {0, };

	if(Hokuyo_SCIPV2_0_SendPack(sendData, sizeof(sendData)) < 0) {
		return -1;
	}

	if(Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData)) < 0) {
		return -1;
	}
	
	sendData[sizeof (sendData)-1] = '\0';
	recvData[sizeof (sendData)-1] = '\0';

	if (strcmp ((char *)sendData, (char *)recvData) != 0) {
		return -1;
	}
#else
	printf ("HokuyoScipV2::Hokuyo_SCIPV2_0_ChangeVer2_0()\n");
#endif
	return 0;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_Reset(void)
{
#ifndef DUMMY
	unsigned char sendData[] = {'R', 'S', ';', 'R', 'E', 'S', 'E', 'T', ASCII_LF};
	unsigned char recvData[1024] = {0, };

	if(Hokuyo_SCIPV2_0_SendPack(sendData, sizeof(sendData)) < 0) {
		return -1;
	}

	if(Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData)) < 0) {
		return -1;
	}

	sendData[sizeof (sendData)-1] = '\0';
	recvData[sizeof (sendData)-1] = '\0';

	if (strcmp ((char *)sendData, (char *)recvData) != 0) {
		return -1;
	}
#else
	printf ("HokuyoScipV2::Hokuyo_SCIPV2_0_Reset()\n");
#endif
	return 0;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_SetBaudRate(int baudRate)
{
#ifndef DUMMY
	unsigned char sendData[16] = {0, };
	unsigned char recvData[32] = {0, };

	int len;

	if(!((baudRate == 19200) || (baudRate == 57600) || (baudRate == 115200) || (baudRate == 250000) || (baudRate == 500000) || (baudRate == 750000))) {
		return -1;
	}

	len = sprintf((char *)sendData, "SS%6d;SS", baudRate);
	sendData[len++] = ASCII_LF;
	
	if(Hokuyo_SCIPV2_0_SendPack(sendData, len) < 0) {
		return -1;
	}

	if(Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData)) < 0) {
		return -1;
	}
#else
	printf ("HokuyoScipV2::Hokuyo_SCIPV2_0_SetBaudRate(%d)\n", baudRate);
#endif
	return 0;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_Enable(unsigned char *status)
{
#ifndef DUMMY
	unsigned char sendData[] = {'B', 'M', ';', 'E', 'N', ASCII_LF};
	unsigned char recvData[1024] = {0, };

	if(Hokuyo_SCIPV2_0_SendPack(sendData, sizeof(sendData)) < 0) {
		return -1;
	}

	int recv_size=0;
	if((recv_size = Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData))) < 0) {
		return -1;
	}

	sendData[sizeof (sendData)-1] = '\0';
	recvData[sizeof (sendData)-1] = '\0';

	if (strcmp ((char *)sendData, (char *)recvData) != 0) {
		uart->Unlock();
		return -1;
	}

	status[0] = recvData[sizeof(sendData)];
#else
	status[0] = 0;
	printf ("HokuyoScipV2::Hokuyo_SCIPV2_0_Enable(%d)\n", status[0]);
#endif
	return 0;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_Disable(void)
{
#ifndef DUMMY
	unsigned char sendData[] = {'Q', 'T', ';', 'D', 'I', ASCII_LF};
	unsigned char recvData[1024] = {0, };

	if(Hokuyo_SCIPV2_0_SendPack(sendData, sizeof(sendData)) < 0) {
		return -1;
	}

	if(Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData)) < 0) {
		return -1;
	}

	sendData[sizeof (sendData)-1] = '\0';
	recvData[sizeof (sendData)-1] = '\0';

	if (strcmp ((char *)sendData, (char *)recvData) != 0) {
		return -1;
	}
#else
	printf ("HokuyoScipV2::Hokuyo_SCIPV2_0_Disable()\n");
#endif
	return 0;
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_MeasureAndGetData(void)
{
#ifndef DUMMY
	unsigned char sendData[32] = {0, };
	unsigned char recvData[4096] = {0, };
	int nSend, nRecv;

	nSend = sprintf((char *)sendData, "MD%04d%04d%02d%01d%02d;MD", startStep, endStep, deltaStep, 0, 0);
	sendData[nSend++] = ASCII_LF;

	if(Hokuyo_SCIPV2_0_SendPack(sendData, nSend) < 0) {
		return -1;
	}
	OprosSleep(50);

	if((nRecv = Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData))) < 0) {
		return -1;
	}
	if((nRecv = Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData))) < 0) {
		return -1;
	}
	
	return Hokuyo_SCIPV2_0_ParsingData(&recvData[nSend + 10], nRecv - nSend - 10);
#else
	for (int i = 0; i < (int)scannedData.size (); i++) {
		scannedData[i] = 8000;
	}
	return 0;
#endif
}

int HokuyoScipV2::Hokuyo_SCIPV2_0_GetLastData(void)
{
#ifndef DUMMY
	unsigned char sendData[32] = {0, };
	unsigned char recvData[4096] = {0, };
	int nSend, nRecv;
	
	nSend = sprintf((char *)sendData, "GD%04d%04d%02d;GD", startStep, endStep, deltaStep, 0, 0);
	sendData[nSend++] = ASCII_LF;
	
	if(Hokuyo_SCIPV2_0_SendPack(sendData, nSend) < 0) {
		return -1;
	}
	
	if((nRecv = Hokuyo_SCIPV2_0_RecvPack(recvData, sizeof(recvData))) < 0) {
		return -1;
	}

	if(!((recvData[nSend] == '0') && (recvData[nSend + 1] == '0'))) {
		return -1;
	}

	return Hokuyo_SCIPV2_0_ParsingData(&recvData[nSend + 10], nRecv - nSend - 10);
#else
	for (int i = 0; i < (int)scannedData.size (); i++) {
		scannedData[i] = 8000;
	}
	return 0;
#endif
}

#if !defined(DEVICE_TEST)
#if defined(WIN32)
extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new HokuyoScipV2();
}
#else
extern "C"
{
	OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new HokuyoScipV2();
}

#endif
#endif