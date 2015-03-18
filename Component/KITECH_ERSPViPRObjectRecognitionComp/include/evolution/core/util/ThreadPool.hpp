//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_THREAD_POOL_HPP
#define INCLUDE_EVOLUTION_THREAD_POOL_HPP


/**
 * @file  evolution/core/util/ThreadPool.hpp
 * @brief A thread pool that faciliates the reuse of threads.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IMutex
 * @see Evolution::IConditionVariable
 * @see Evolution::ThreadPool
 */


#include <set>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/IThread.hpp>


namespace Evolution
{
    class IMutex;
    class IConditionVariable;


    /**
     * @class ThreadPool
     * @brief A thread pool that allows for recycling of threads without the
     * need to constantly create and destroy new threads.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL ThreadPool 
    {
    public: // Types

        /**
         * @brief  A collection of threads
         */
        typedef std::set<IThread*> ThreadSet;

    public: // Structors

        /**
         * @brief  Default Constructor
         */
        ThreadPool ();

        /**
         * @brief  Default Destructor
         */
        ~ThreadPool ();

        /**
         * @brief  Use this to obtain an existing shared thread pool or create
         * a new one.
         *
         * @return A thread pool.
         */
        static ThreadPool* get_shared_pool ();

    public: // Access

        /**
         * @brief  This returns the number of threads in the pool.
         *
         * @return The number of threads in the thread pool.
         */
        int get_thread_count () { return (_thread_set.size ()); }

        /**
         * @brief  This returns the number of free threads in the pool.
         *
         * @return The number of free threads in the thread pool.
         */
        int get_free_thread_count ();

    public: // Execution

        /**
         * @brief This runs the pool's procedure in a new or existing thread.
         *
         * @param proc [in]; The thread procedure to run in a thread from
         *      the thread pool.
         * @param param [in]; The thread parameter to pass to the thread.
         * @param result [out]; The thread's result after it has finished
         *      running.
         *
         * @return True if the thread is successfully started. 
         */
        bool run (ThreadProc proc, 
                  ThreadProcParam param, 
                  ThreadProcResult* result);

        /**
         * @brief  This sends a request that all threads shut down
         *
         * @return True if the request was successful.
         */
        bool request_shutdown ();

        /**
         * @brief  Wait for all threads to shutdown (or until wait_time expires)
         *
         * @param wait_time [in]; The wait time out.
         *
         * @return True if the wait was successful.
         */
        bool wait_for_all (unsigned long wait_time);

        /**
         * @brief  This forces all threads to stop.
         */
        void stop_all ();

    public: // Helpers

        /**
         * @brief  This increments the number of free threads.
         *
         * @return True if successful.
         */
        bool increment_free_thread_count ();

        /**
         * @brief  This decrement the number of free threads.
         *
         * @return True if successful.
         */
        bool decrement_free_thread_count ();

        /**
         * @brief The thread procedure used by threads from the thread pool.
         *
         * @param param [in]; The thread parameter.
         *
         * @return The thread result.
         */
        static ThreadProcResult THREAD_PROC_CALL thread_pool_thread_proc (ThreadProcParam param);

    protected: // Implementation

        /**
         * @brief The set of running threads
         */
        ThreadSet _thread_set;

        /**
         * @brief Flag specifying if the threads should run
         */
        bool _should_run;

        /**
         * @brief IMutex ensures that only one run() executes at a time, also
         * controlling access to _run_proc, _run_param, and _run_result.
         */
        IMutex* _run_mutex;

        /**
         * @brief True if run is accepted.
         */
        bool _run_accepted;


        /**
         * @brief The procedure to run next
         */
        ThreadProc _run_proc;

        /*
         * @brief The argument to the run procedure
         */
        ThreadProcParam _run_param;

        /**
         * @brief This is the pointer to the location where the result of the
         * run procedure should be stored.
         */
        ThreadProcResult* _run_result;

        /**
         * @brief This is the condition variable through which the run() method
         * signals an existing thread to run the specified procedure.
         */
        IConditionVariable* _request_condition;

        /**
         * @brief This is the condition variable through which the run() method
         * signals an existing thread to run the specified procedure.
         */
        IConditionVariable* _accept_condition;


        /**
         * @brief  This is the Mutex used for locking when reading or modifying
         * the thread count.  Also locks when shutting down all threads.
         */
        IMutex* _free_thread_mutex;

        /**
         * @brief  The number of free threads
         */
        unsigned _free_thread_count;

    }; // end class ThreadPool


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_THREAD_POOL_HPP

