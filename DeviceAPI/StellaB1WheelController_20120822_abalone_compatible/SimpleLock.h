#ifndef __SIMPLE_MUTEX_H__
#define __SIMPLE_MUTEX_H__

#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

class SimpleMutex
{
public:
	SimpleMutex()
	{
#if defined(WIN32)
		InitializeCriticalSection(&criticalSection);
#else
		pthread_mutex_init(&mutex, NULL);
#endif
	}

	~SimpleMutex()
	{
#if defined(WIN32)
		DeleteCriticalSection(&criticalSection);
#else
		pthread_mutex_destroy(&mutex);
#endif
	}

protected:
	SimpleMutex(const SimpleMutex& simpleMutex);
	SimpleMutex& operator=(const SimpleMutex& simpleMutex);

public:
	void Lock()
	{
#if defined(WIN32)
		EnterCriticalSection(&criticalSection);
#else
		pthread_mutex_lock(&mutex);
#endif
	}

	void Unlock()
	{
#if defined(WIN32)
		LeaveCriticalSection(&criticalSection);
#else
		pthread_mutex_unlock(&mutex);
#endif
	}

private:
#if defined(WIN32)
	CRITICAL_SECTION criticalSection;
#else
	pthread_mutex_t mutex;
#endif
};

class SimpleReaderWriterLock
{
public:
	SimpleReaderWriterLock()
	{
#if defined(WIN32)
		readerCount = 0;
		InitializeCriticalSection(&readerCountCriticalSection);
		InitializeCriticalSection(&writerCriticalSection);
		readerCompleteEventHandler = CreateEvent(NULL, TRUE, TRUE, NULL);
#else
		pthread_rwlock_init(&rwLock, NULL);
#endif
	}

	~SimpleReaderWriterLock()
	{
#if defined(WIN32)	
		DeleteCriticalSection(&readerCountCriticalSection);
		DeleteCriticalSection(&writerCriticalSection);
		CloseHandle(readerCompleteEventHandler);
#else
		pthread_rwlock_destroy(&rwLock);
#endif
	}

protected:
	SimpleReaderWriterLock(const SimpleReaderWriterLock& simpleSemaphore);
	SimpleReaderWriterLock& operator=(const SimpleReaderWriterLock& simpleSemaphore);

public:
	void ReaderLock()
	{
#if defined(WIN32)
		EnterCriticalSection(&writerCriticalSection);
		EnterCriticalSection(&readerCountCriticalSection);
		if (++readerCount == 1)
		{
			ResetEvent(readerCompleteEventHandler);
		}
		LeaveCriticalSection(&readerCountCriticalSection);
		LeaveCriticalSection(&writerCriticalSection);
#else
		pthread_rwlock_rdlock(&rwLock);
#endif
	}

	void WriterLock()
	{
#if defined(WIN32)
		EnterCriticalSection(&writerCriticalSection);
		if (readerCount > 0)
		{
			WaitForSingleObject(readerCompleteEventHandler, INFINITE);
		}
#else
		pthread_rwlock_wrlock(&rwLock);
#endif
	}

	void ReaderUnlock()
	{
#if defined(WIN32)
		EnterCriticalSection(&readerCountCriticalSection);
		if (--readerCount == 0)
		{
			SetEvent(readerCompleteEventHandler);
		}
		LeaveCriticalSection(&readerCountCriticalSection);
#else
		pthread_rwlock_unlock(&rwLock);
#endif
	}

	void WriterUnlock()
	{
#if defined(WIN32)
		LeaveCriticalSection(&writerCriticalSection);
#else
		pthread_rwlock_unlock(&rwLock);
#endif
	}

private:
#if defined(WIN32)	
	HANDLE readerCompleteEventHandler;
	CRITICAL_SECTION readerCountCriticalSection;
	CRITICAL_SECTION writerCriticalSection;
	volatile unsigned int readerCount;
#else
	pthread_rwlock_t rwLock;
#endif
};



#endif

