//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-lock.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

/* Define a private trivial efficient locking mechanism,
 * amply adequate for our humble needs.
 */

#ifndef __ERSP_OBJREC_LOCK_H_
#define __ERSP_OBJREC_LOCK_H_
#include "ersp-objrec-common.h"

/* Choices of threading implementations. */
#define THREAD_SUPPORT_NONE             0
#define THREAD_SUPPORT_CHECK            1
#define THREAD_SUPPORT_PTHREADS         2
#define THREAD_SUPPORT_WIN32            3

#ifndef THREAD_SUPPORT
#ifdef _WIN32
#define THREAD_SUPPORT  THREAD_SUPPORT_WIN32
#else
#define THREAD_SUPPORT  THREAD_SUPPORT_PTHREADS
#endif
#endif


/* Define:
 *   ERSP_ObjrecLock:              a platform-specific lock structure.
 *   ERSP_OBJREC_LOCK_INITIALIZER: a list of values that can be used to
 *                                 statically initialize a mutex.
 */
#if (THREAD_SUPPORT == THREAD_SUPPORT_PTHREADS)
#  include <pthread.h>
#endif

ERSP_OBJREC_BEGIN_DECLS

#if (THREAD_SUPPORT == THREAD_SUPPORT_PTHREADS)
#  define ERSP_OBJREC_LOCK_INITIALIZER  PTHREAD_MUTEX_INITIALIZER
typedef pthread_mutex_t ERSP_ObjrecLock;
#elif (THREAD_SUPPORT == THREAD_SUPPORT_NONE) || (THREAD_SUPPORT == THREAD_SUPPORT_CHECK)

#  define ERSP_OBJREC_LOCK_INITIALIZER  0
typedef char ERSP_ObjrecLock;

#elif (THREAD_SUPPORT == THREAD_SUPPORT_WIN32)
typedef struct
{
    void *mutex;               /* a HANDLE */
    int is_locked;
} ERSP_ObjrecLock;
#define ERSP_OBJREC_LOCK_INITIALIZER { NULL, 0 }


#else    /* THREAD_SUPPORT set to unknown value */
#  error FIXME: thread support not available on your platform.
#endif  

ERSP_OBJREC_EXPORT
void ERSP_objrec_lock_init (ERSP_ObjrecLock *lock);

ERSP_OBJREC_EXPORT
void ERSP_objrec_lock      (ERSP_ObjrecLock *lock);

ERSP_OBJREC_EXPORT
void ERSP_objrec_unlock    (ERSP_ObjrecLock *lock);
ERSP_OBJREC_END_DECLS
#endif
