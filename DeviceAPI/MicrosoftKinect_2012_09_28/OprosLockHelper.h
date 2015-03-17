#ifndef __OPROS_LOCK_HELPER_H__
#define __OPROS_LOCK_HELPER_H__

#include <OprosLock.h>

class OprosLockHelper
{
public:
	OprosLockHelper(OprosLock& lock)
		: mLock(lock)
	{
		mLock.Lock();
	}

	~OprosLockHelper()
	{
		mLock.Unlock();
	}

private:
	OprosLockHelper(const OprosLockHelper&);
	const OprosLockHelper& operator=(const OprosLockHelper&);

private:
	OprosLock& mLock;
};

#endif
