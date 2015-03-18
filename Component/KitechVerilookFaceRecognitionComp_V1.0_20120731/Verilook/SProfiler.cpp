/**
 * CSProfiler class used to time application
 * execution.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#include <windows.h>
#include "SProfiler.h"

/**
 * Constructor
 * Initialize memeber variables and
 * get current performance-counter frequency, in counts per second
 */
CSProfiler::CSProfiler() :
	m_StartTime(0),
	m_StopTime(0)
{
	::QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
}

/**
 * Destructor
 */
CSProfiler::~CSProfiler()
{
}

/**
 * Get the performance-counter frequency and store it in
 * m_StartTime member variable
 */
void CSProfiler::Start()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
}

/**
 * Get the performance-counter frequency and store it in
 * m_StopTime member variable
 */
void CSProfiler::Stop()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_StopTime);
}

/**
 * Calculate time in seconds, elapsed between Start and Stop
 * method calls
 */
double CSProfiler::GetElapsed() const
{
	const double t = static_cast<double>(m_StopTime - m_StartTime) / m_Frequency;
	return t;
}
