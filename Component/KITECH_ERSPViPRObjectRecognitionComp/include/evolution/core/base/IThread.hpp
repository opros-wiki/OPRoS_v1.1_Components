//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_THREAD_HPP
#define INCLUDE_EVOLUTION_THREAD_HPP


/**
 * @file   evolution/core/base/IThread.hpp
 * @brief  Cross-platform interface to a thread.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IThread
 */


#include <evolution/core/base/Config.hpp>
#include <evolution/core/base/Types.hpp>

/**
 * @typedef ThreadId
 * @brief Platform-independent thread ID.
 * @ingroup core_base
 */
#if defined(EVOLUTION_PLATFORM_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef DWORD ThreadId;

#elif defined(EVOLUTION_SUPPORT_POSIX_THREADS)
#include <pthread.h>

typedef pthread_t ThreadId;

#elif defined(EVOLUTION_NO_THREAD_SUPPORT)
typedef int ThreadId;
#else
#error Unspecified thread support.
#endif



namespace Evolution
{


    /**
     * @interface IThread
     * @ingroup core_base
     * @ingroup core_base_aibo
     *
     * @brief
     * Cross-platform interface to a thread.
     * Use the Platform::create_thread() to create a new thread.
     * @see Platform::create_thread()
     */
    class EVOLUTION_EXPORT_BASE IThread
    {
    public:                  // Structors

        /// Constructor
        IThread ()
        {
        }

        /// Destructor
        virtual ~ IThread ()
        {
        }

    protected:               // Assignment

        /** @brief Thread copy constructor
         * @param thread [in]; thread to copy.
         */ 
        IThread (const IThread & thread)
        {
        }

        /** @brief Thread assignment operator
         *  
         * @param thread [in]; thread to copy.
         * @return IThread reference to self.
         */
        IThread & operator= (const IThread & thread)
        {
            return (*this);
        }

#ifndef EVOLUTION_NO_THREAD_SUPPORT
    public:                  // Status

        /** @brief Obtains the ID of the thread, if it has been launched.
         *  
         * @return ThreadId of thread, if it's launched.
         */
        virtual ThreadId get_id ()const = 0;

    public:                  // Control

        /** Start the thread.
         * @return true if starts, false if fails to start
         */ 
        virtual bool start () = 0;

        /** Stop the thread.
         * @return true if stops, false if fails to stop
         */ 
        virtual bool stop () = 0;

        /** Wait for the thread to exit.
         * @param wait_ms [in]; time to wait for the thread to stop.
         * @return true if stops, false if fails to stop or times out.
         */ 
        virtual bool wait_for_exit (unsigned long wait_ms) = 0;
#endif

    }; // End interface IThread


/**
 * @typedef ThreadProcResult
 * @brief This is the thread process result data.
 * @ingroup core_base
 */

/**
 * @typedef ThreadProcParam
 * @brief This is the thread process parameter data.
 * @ingroup core_base
 */

/**
 * @typedef ThreadProc
 * @brief This is the thread process function pointer.
 * @ingroup core_base
 */

/**
 * @def THREAD_PROC_CALL
 * @brief Platform-independent qualifier for the ThreadProc function.
 * @ingroup core_base
 */

/**
 * @def ThreadProcResultDecl
 * @brief Thread process result; use in function declarations.
 * @ingroup core_base
 */

#ifndef EVOLUTION_NO_THREAD_SUPPORT
    // It is time consuming to have to type Evolution::IThread::ThreadProcResult.
#ifdef EVOLUTION_PLATFORM_LINUX
#define ThreadProcResultDecl void*
#define THREAD_PROC_CALL
    typedef void *ThreadProcResult; 
    typedef void *ThreadProcParam;  
    typedef ThreadProcResult (*ThreadProc) (ThreadProcParam);
#else
#ifdef EVOLUTION_PLATFORM_WIN32


#define ThreadProcResultDecl unsigned int WINAPI
#define THREAD_PROC_CALL WINAPI

    typedef unsigned int ThreadProcResult;  
    typedef void *ThreadProcParam; 
    // typedef LPTHREAD_START_ROUTINE ThreadProc;  ///< This is the thread process function pointer.
    typedef ThreadProcResult (__stdcall * ThreadProc) (ThreadProcParam);
#else
#error Unspecified platform.
#endif
#endif

#else

    /* dummy support to get through .hpp files */
#define ThreadProcResultDecl void*
#define THREAD_PROC_CALL
    typedef void *ThreadProcResult; 
    typedef void *ThreadProcParam;  
    typedef ThreadProcResult (*ThreadProc) (ThreadProcParam);   

#endif /* !EVOLUTION_NO_THREAD_SUPPORT */

    const ThreadProcResult THREAD_RESULT_SUCCESS = ThreadProcResult (0);    ///< Define thread process success as 0.
    const ThreadProcResult THREAD_RESULT_FAILURE = ThreadProcResult (1);    ///< Define thread process failure as 1.



}  // end namespace Evolution


#endif // INCLUDE_EVOLUTION_THREAD_HPP
