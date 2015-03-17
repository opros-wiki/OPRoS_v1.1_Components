/*************************************************************

 file: OprosPrintMessage.h
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

#ifndef	_OPROS_PRINT_MESSAGE_H_
#define	_OPROS_PRINT_MESSAGE_H_

#if defined(WIN32)
#pragma warning(disable : 4996)
#endif

#include <stdarg.h>
#include <stdio.h>

inline void PrintMessage(const char *format, ...)
#if defined(_OPROS_DEBUG_) || defined(_DEBUG) || defined(NDEBUG)
{
	int n = 0;
    va_list arg_list;
	char log_text[1024+1];

	va_start (arg_list,format);
	n += vsnprintf (log_text+n, 1024-n, format, arg_list);
	va_end (arg_list);
	log_text[1024] = '\0';

	printf("%s", log_text);
}
#else
{
}
#endif

#endif	//	_OPROS_PRINT_MESSAGE_H_