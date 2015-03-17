/*************************************************************

 file: Device.h
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

#ifndef	_DEVICE_H_
#define	_DEVICE_H_

#include "OprosApi.h"

enum DEVICE_STATUS {
	DEVICE_CREATED		= 0x01,
	DEVICE_READY		= 0x02,
	DEVICE_ACTIVE		= 0x03,
	DEVICE_ERROR		= 0x04,
};

class Device : public OprosApi
{
public:
	Device(void) { _status = DEVICE_CREATED; };
	virtual ~Device(void) { };

protected:
	DEVICE_STATUS _status;

public:
	DEVICE_STATUS GetStatus(void)
	{
		return _status;
	}
};

#endif	//	_DEVICE_H_