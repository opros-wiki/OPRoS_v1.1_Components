/**
 * CSProfiler class used to time application
 * execution.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#pragma once

class CSProfiler
{
private:
	__int64 m_Frequency;
	__int64 m_StartTime;
	__int64 m_StopTime;
	
public:
	CSProfiler();
	~CSProfiler();

public:
	void Start();
	void Stop();
	double GetElapsed() const;
};
