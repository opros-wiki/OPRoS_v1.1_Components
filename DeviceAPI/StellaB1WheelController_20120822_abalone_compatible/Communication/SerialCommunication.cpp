/*************************************************************

 file: SerialCommunication.cpp
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

#include "SerialCommunication.h"

#include "SerialWindows.h"
#include "SerialLinux.h"

SerialCommunication::SerialCommunication(void)
{
	handle = NULL;
}

SerialCommunication::~SerialCommunication(void)
{
	Finalize();
}

int SerialCommunication::Initialize(Property parameter)
{
	if(parameter.FindName("PortName") == false)	return API_ERROR;
	string portName = parameter.GetValue("PortName");
	if(parameter.FindName("TimeOut") == false)		return API_ERROR;
	unsigned long timeOut = (unsigned long)atol(parameter.GetValue("TimeOut").c_str());
	if(parameter.FindName("BaudRate") == false)	return API_ERROR;
	unsigned long baudRate = (unsigned long)atol(parameter.GetValue("BaudRate").c_str());
	if(parameter.FindName("DataBits") == false)	return API_ERROR;
	char dataBits = (char)atoi(parameter.GetValue("DataBits").c_str());
	if(parameter.FindName("StopBits") == false)	return API_ERROR;
	char stopBits = (char)atoi(parameter.GetValue("StopBits").c_str());
	if(parameter.FindName("Parity") == false)		return API_ERROR;
	char parity = (char)atoi(parameter.GetValue("Parity").c_str());
	if(parameter.FindName("FlowControl") == false)	return API_ERROR;
	char flowControl = (char)atoi(parameter.GetValue("FlowControl").c_str());

	if(handle != NULL) {
		return 0;
	}

#ifdef WIN32
	handle = new SerialWindows(portName, timeOut, baudRate, dataBits, stopBits, parity, flowControl);
#else
	handle = new SerialLinux(portName, timeOut, baudRate, dataBits, stopBits, parity, flowControl);
#endif

	this->parameter = parameter;

	return API_SUCCESS;
}

int SerialCommunication::Finalize(void)
{
	if(handle != NULL) {
		handle->CloseSerial();
		delete handle;
		handle = NULL;
	}

	return API_SUCCESS;
}

int SerialCommunication::Enable(void)
{
	if(handle == NULL) {
		return API_ERROR;
	}

	if(handle->OpenSerial() < 0) {
		return API_ERROR;
	}

	return API_SUCCESS;
}

int SerialCommunication::Disable(void)
{
	if(handle == NULL) {
		return API_ERROR;
	}

	if(handle->CloseSerial() < 0) {
		return API_ERROR;
	}

	return API_SUCCESS;
}

int SerialCommunication::SetParameter(Property parameter)
{
	if(parameter.FindName("TimeOut") == false)		return API_ERROR;
	unsigned long timeOut = (unsigned long)atol(parameter.GetValue("TimeOut").c_str());
	if(parameter.FindName("BaudRate") == false)	return API_ERROR;
	unsigned long baudRate = (unsigned long)atol(parameter.GetValue("BaudRate").c_str());
	if(parameter.FindName("DataBits") == false)	return API_ERROR;
	char dataBits = (char)atoi(parameter.GetValue("DataBits").c_str());
	if(parameter.FindName("StopBits") == false)	return API_ERROR;
	char stopBits = (char)atoi(parameter.GetValue("StopBits").c_str());
	if(parameter.FindName("Parity") == false)		return API_ERROR;
	char parity = (char)atoi(parameter.GetValue("Parity").c_str());
	if(parameter.FindName("FlowControl") == false)	return API_ERROR;
	char flowControl = (char)atoi(parameter.GetValue("FlowControl").c_str());
	
	if(handle != NULL) {
		if(handle->SetParameter(timeOut, baudRate, dataBits, stopBits, parity, flowControl) < 0) {
			return API_ERROR;
		}
	}

	this->parameter.SetValue("TimeOut", parameter.GetValue("TimeOut"));
	this->parameter.SetValue("BaudRate", parameter.GetValue("BaudRate"));
	this->parameter.SetValue("DataBits", parameter.GetValue("DataBits"));
	this->parameter.SetValue("StopBits", parameter.GetValue("StopBits"));
	this->parameter.SetValue("Parity", parameter.GetValue("Parity"));
	this->parameter.SetValue("FlowControl", parameter.GetValue("FlowControl"));
	
	return API_SUCCESS;
}

int SerialCommunication::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int SerialCommunication::Lock(void)
{
	lock.Lock();

	return API_SUCCESS;
}

int SerialCommunication::Unlock(void)
{
	lock.Unlock();

	return API_SUCCESS;
}

int SerialCommunication::Write(unsigned char *data, int size)
{
	if(handle == NULL) {
		return API_ERROR;
	}

	return handle->WriteSerial(data, size);
}

int SerialCommunication::Read(unsigned char *data, int size)
{
	if(handle == NULL) {
		return API_ERROR;
	}

	return handle->ReadSerial(data, size);
}
