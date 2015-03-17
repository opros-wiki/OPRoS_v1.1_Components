/*************************************************************

 file: TcpSocket.cpp
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


#include "TcpSocket.h"


TcpSocket::TcpSocket(void)
{
	tcpSocket = NULL;
}


TcpSocket::~TcpSocket(void)
{
	Finalize();
}


int TcpSocket::Initialize(Property parameter)
{
	if(SetParameter(parameter) < 0) {
		return -1;
	}

#if defined(WIN32)
	tcpSocket = new TcpSocketWindows(parameter.GetValue("IP"), (short)atoi(parameter.GetValue("Port").c_str()), (unsigned long)atol(parameter.GetValue("TimeOut").c_str()));
#else
	tcpSocket = new TcpSocketLinux(parameter.GetValue("IP"), (short)atoi(parameter.GetValue("Port").c_str()), (unsigned long)atol(parameter.GetValue("TimeOut").c_str()));
#endif
	return API_SUCCESS;
}

int TcpSocket::Finalize(void)
{
	if(tcpSocket != NULL) {
		delete tcpSocket;
		tcpSocket = NULL;
	}

	return API_SUCCESS;
}

int TcpSocket::Enable(void)
{
	if(tcpSocket == NULL) {
		return -1;
	}

	if(tcpSocket->OpenTCPSocket() < 0) {
		return -1;
	}

	return API_SUCCESS;
}

int TcpSocket::Disable(void)
{
	if(tcpSocket == NULL) {
		return -1;
	}

	if(tcpSocket->CloseTCPSocket() < 0) {
		return -1;
	}

	return API_SUCCESS;
}

int TcpSocket::SetParameter(Property parameter)
{
	if(parameter.FindName("IP") == false)		return -1;
	if(parameter.FindName("Port") == false)		return -1;
	if(parameter.FindName("TimeOut") == false)	return -1;

	if(tcpSocket != NULL) {
		if(tcpSocket->SetParameter(parameter.GetValue("IP"), (short)atoi(parameter.GetValue("Port").c_str()), (unsigned long)atol(parameter.GetValue("TimeOut").c_str())) < 0) {
			return -1;
		}
	}

	this->parameter = parameter;

	return API_SUCCESS;
}

int TcpSocket::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int TcpSocket::Lock(void)
{
	if(tcpSocket == NULL) {
		return -1;
	}

	lock.Lock();

	return API_SUCCESS;
}

int TcpSocket::Unlock(void)
{
	if(tcpSocket == NULL) {
		return -1;
	}

	lock.Unlock();

	return API_SUCCESS;
}

int TcpSocket::Write(unsigned char *data, int size)
{
	if(tcpSocket == NULL) {
		return -1;
	}

	return tcpSocket->WriteTCPSocket(data, size);
}

int TcpSocket::Read(unsigned char *data, int size)
{
	if(tcpSocket == NULL) {
		return -1;
	}

	return tcpSocket->ReadTCPSOcket(data, size);
}

#if defined(WIN32)
extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new TcpSocket();
}

#endif