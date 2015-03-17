/*************************************************************

 file: SerialLinux.cpp
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

#ifndef	WIN32
#include <fcntl.h>
#include <termio.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <poll.h>
#include <errno.h>
#include <termios.h>

#include "SerialLinux.h"

#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2

#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2

#define PCF_NONE          ((unsigned long)0x0000)
#define PCF_DTRDSR        ((unsigned long)0x0001)
#define PCF_RTSCTS        ((unsigned long)0x0002)
#define PCF_XONXOFF       ((unsigned long)0x0010)

extern int errno;

SerialLinux::SerialLinux(string portName, unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl)
: SerialImpl(portName, timeOut, baudRate, dataBits, stopBits, parity, flowControl)
{
	handle = -1;
}

SerialLinux::~SerialLinux(void)
{
	CloseSerial();
}

int SerialLinux::OpenSerial(void)
{
	CloseSerial();

	if((handle = open(portName.c_str(), O_RDWR | O_SYNC)) < 0) {
		CloseSerial();
		return -1;
	}

	if(isatty(handle) == 0) {
		CloseSerial();
		return -1;
	}

	if(ConfigureParameter() < 0) {
		CloseSerial();
		return -1;
	}

	return 0;
}

int SerialLinux::CloseSerial(void)
{
	if(handle != -1) {
		close(handle);
		handle = -1;
	}
	
	return 0;
}

int SerialLinux::WriteSerial(unsigned char *data, int size)
{
	if(handle == -1) {
		return -1;
	}

	if(size <= 0) {
		return 0;
	}

	return write(handle, data, size);
}

int SerialLinux::ReadSerial(unsigned char *data, int size)
{
	if(handle == -1) {
		return -1;
	}

	if(size <= 0) {
		return 0;
	}

	return read(handle, data, size);
}

int SerialLinux::SetParameter(unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl)
{
	this->timeOut = timeOut;
	this->baudRate = baudRate;
	this->dataBits = dataBits;
	this->stopBits = stopBits;
	this->parity = parity;
	this->flowControl = flowControl;

	return ConfigureParameter();
}

int SerialLinux::ConfigureParameter(void)
{
	struct termio term;

	if(handle == -1) {
		return -1;
	}

	if(ioctl(handle, TCGETA, &term) < 0) {
		return -1;
	}

	switch(baudRate) {
		case 110 :		term.c_cflag = B110;	break;
		case 300 :		term.c_cflag = B300;	break;
		case 600 :		term.c_cflag = B600;	break;
		case 1200 :		term.c_cflag = B1200;	break;
		case 2400 :		term.c_cflag = B2400;	break;
		case 4800 :		term.c_cflag = B4800;	break;
		case 9600 :		term.c_cflag = B9600;	break;
		case 19200 :	term.c_cflag = B19200;	break;
		case 38400 :	term.c_cflag = B38400;	break;
		case 57600 :	term.c_cflag = B57600;	break;
		case 115200 :	term.c_cflag = B115200;	break;
		default :		term.c_cflag = B9600;	break;
	}

	switch(dataBits) {
		case 5:  term.c_cflag |= CS5; break;
		case 6:  term.c_cflag |= CS6; break;
		case 7:  term.c_cflag |= CS7; break;
		case 8:  term.c_cflag |= CS8; break;
		default: term.c_cflag |= CS8; break;
	}

	switch(parity) {
		case NOPARITY: 
			break;
		case EVENPARITY: 
			term.c_cflag |= PARENB; 
			break;
		case ODDPARITY:  
			term.c_cflag |= PARENB|PARODD; 
			break;
	}

	switch (stopBits) {
		case ONESTOPBIT:   break;
		case ONE5STOPBITS: break;
		case TWOSTOPBITS:  term.c_cflag |= CSTOPB; break;
	}

	term.c_cflag |= CREAD |			// enable receiver
					HUPCL |			// lower modem lines on last close
									// 1 stop bit(since CSTOPB off)
					CLOCAL;		    // ignore modem status lines
	term.c_cflag |= (flowControl == PCF_RTSCTS)? CRTSCTS:0;		// flow control

	term.c_oflag =  0;				// turn off all output processing
	term.c_iflag =  IGNBRK |		// ignore breaks
					// ISTRIP |		// strip input to 7 bits
					IGNPAR;			// ignore input Parity errors
	term.c_iflag |=  (flowControl == PCF_XONXOFF)? IXON | IXOFF:0;	// Xon/Xoff flow control(default)

	term.c_lflag = 0;				// everything off in local flag:
									//	disables canonical mode, disables
									//	signal generation, disables echo
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME]= timeOut < 100 ? 1 : timeOut / 100;

	term.c_line  = 0;

	if(ioctl(handle, TCSETA, &term) < 0) {
		return -1;
	}

	return 0;
}

#endif
