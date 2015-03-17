/*************************************************************

 file: SerialWindows.cpp
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

#ifdef	WIN32

#include "SerialWindows.h"

SerialWindows::SerialWindows(string portName, unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl)
: SerialImpl(portName, timeOut, baudRate, dataBits, stopBits, parity, flowControl)
{
	handle = INVALID_HANDLE_VALUE;
}

SerialWindows::~SerialWindows(void)
{
	CloseSerial();
}

int SerialWindows::OpenSerial(void)
{
	if(handle != INVALID_HANDLE_VALUE) {
		return 0;
	}

	std::string prefixedPortName = (portName.substr(4) == "\\\\.\\" ? "" : "\\\\.\\") + portName;
	handle = CreateFile((char *)prefixedPortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if(handle == INVALID_HANDLE_VALUE){
		return -1;
	}

	if(SetParameter(timeOut, baudRate, dataBits, stopBits, parity, flowControl) < 0) {
		CloseSerial();
		return -1;
	}

	return 0;
}

int SerialWindows::CloseSerial(void)
{
	if(handle != INVALID_HANDLE_VALUE) {
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}

	return 0;
}

int SerialWindows::WriteSerial(unsigned char *data, int size)
{
	if(handle == INVALID_HANDLE_VALUE || size < 0) {
		return -1;
	}
	if(size == 0) {
		return 0;
	}

	DWORD dwBytesWritten = 0;
	if(WriteFile(handle, data, size, &dwBytesWritten, NULL) == TRUE) {
		return (int)dwBytesWritten;
	}

	return 0;
}

int SerialWindows::ReadSerial(unsigned char *data, int size)
{
	if(handle == INVALID_HANDLE_VALUE || size < 0) {
		return -1;
	}
	if(size == 0) {
		return 0;
	}

	DWORD dwBytesRead = 0;
	if(ReadFile(handle, data, size, &dwBytesRead, NULL) == TRUE) {
		return dwBytesRead;
	}

	return -1;
}

int SerialWindows::SetParameter(unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl)
{
	DCB dcb;
	if(GetCommState(handle, &dcb) == FALSE){
		return -1;
	}
	dcb.BaudRate = baudRate;
	dcb.ByteSize = dataBits;
	dcb.Parity = parity;
	dcb.StopBits = stopBits;
	dcb.fBinary = TRUE;
	dcb.fDsrSensitivity = false;
	dcb.fParity = 0;
	dcb.fOutX = false;
	dcb.fInX = false;
	dcb.fNull = false;
	dcb.fAbortOnError = false;
	dcb.fOutxCtsFlow = false;
	dcb.fOutxDsrFlow = false;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = false;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutxCtsFlow = false;
	dcb.fOutxCtsFlow = false;

	if(SetCommState(handle, &dcb) == FALSE){
		return -1;
	}

	COMMTIMEOUTS commTimeouts;
	if(GetCommTimeouts(handle, &commTimeouts) == FALSE) {
		return -1;
	}
	
	commTimeouts.ReadIntervalTimeout = 1;
	commTimeouts.ReadTotalTimeoutConstant = timeOut;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.WriteTotalTimeoutConstant = timeOut;
	commTimeouts.WriteTotalTimeoutMultiplier = 0;
	
	if(SetCommTimeouts(handle, &commTimeouts) == FALSE) {
		return -1;
	}

	return 0;
}


#endif
