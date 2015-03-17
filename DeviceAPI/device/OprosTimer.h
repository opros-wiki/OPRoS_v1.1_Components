/*************************************************************

 file: OprosTimer.h
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

#ifndef _OPROS_TIMER_H_
#define	_OPROS_TIMER_H_

#if defined(_OPROS_LINUX_)
#include <signal.h>
#include <sys/time.h>

#elif defined(WIN32)
#include <windows.h>

#endif



#if defined(_OPROS_LINUX_)
#define	OprosSleep(X)	usleep(X * 1000)
#elif defined(WIN32)
#define	OprosSleep(X)	Sleep(X)
#endif


class OprosTimer
{
public:
	OprosTimer(void) {}
	virtual ~OprosTimer(void) {}

private:
#if defined(_OPROS_LINUX_)
	 struct timeval m_startTime;
	 struct timeval m_presentTime;

	 struct timeval m_startMicroTime;
	 struct timeval m_presentMicroTime;
#elif defined(WIN32)
	unsigned long m_startTime;
	unsigned long m_presentTime;

#endif

public:
	void StartTimeTick(void)
	{
#if defined(_OPROS_LINUX_)
		gettimeofday(&m_startTime, NULL);
#elif defined(WIN32)
		m_startTime = GetTickCount();
		m_presentTime = 0;
#endif
	}

	unsigned long GetTimeTick(void)
	{
#if defined(_OPROS_LINUX_)
		unsigned long start_time;
		unsigned long end_time;
		gettimeofday(&m_presentTime, NULL);
	
		start_time = (m_startTime.tv_usec/1000) + (m_startTime.tv_sec*1000);
		end_time =(m_presentTime.tv_usec/1000) + (m_presentTime.tv_sec*1000);

		return (unsigned long)(end_time - start_time);
#elif defined(WIN32)
		unsigned long lTmp = GetTickCount();
		m_presentTime += (lTmp - m_startTime);
		m_startTime = lTmp;

		return m_presentTime;
#endif

		return 0;
	}

	void StartTimeTickMicro(void)
	{
#if defined(_OPROS_LINUX_)
		gettimeofday(&m_startMicroTime, NULL);
#elif defined(WIN32)
#endif
	}

	unsigned long GetTimeTickMicro(void)
	{
#if defined(_OPROS_LINUX_)
		unsigned long start_time;
		unsigned long end_time;
		gettimeofday(&m_presentMicroTime, NULL);
	
		start_time = (m_startMicroTime.tv_usec) + (m_startMicroTime.tv_sec*1000000);
		end_time =(m_presentMicroTime.tv_usec) + (m_presentMicroTime.tv_sec*1000000);

		return (unsigned long)(end_time - start_time);
#elif defined(WIN32)
#endif

	return 0;
	}
};

#endif	//	_OPROS_TIMER_H_