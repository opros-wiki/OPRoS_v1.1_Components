/*************************************************************

 file: OprosApi.h
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

#ifndef	_OPROS_API_H_
#define	_OPROS_API_H_

#include "ApiTypes.h"
#include "Property.h"

#if defined(WIN32)
#include <Windows.h>

#define	OprosApiHandle HMODULE

#else
#include <dlfcn.h>

#define OprosApiHandle	void *

#endif

#define	API_SUCCESS			0
#define	API_ERROR			-1
#define	API_NOT_SUPPORTED	-2

class OprosApi
{
public :
	OprosApi(void) { };
	virtual ~OprosApi(void) { };

public:
	virtual int Initialize(Property parameter) { return API_NOT_SUPPORTED; }
	virtual int Finalize(void) { return API_NOT_SUPPORTED; }
	virtual int Enable(void) { return API_NOT_SUPPORTED; }
	virtual int Disable(void) { return API_NOT_SUPPORTED; }
	virtual int SetParameter(Property parameter) { return API_NOT_SUPPORTED; }
	virtual int GetParameter(Property &parameter) { return API_NOT_SUPPORTED; }
	virtual int OnExecute(void) { return API_NOT_SUPPORTED; }
};

typedef	OprosApi *(*GET_OPROS_API)();

#endif	//	_OPROS_API_H_