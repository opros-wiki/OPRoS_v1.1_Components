//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_UTIL_SIGNAL_THREAD_HPP
#define INCLUDE_EVOLUTION_CORE_UTIL_SIGNAL_THREAD_HPP


/**
 * @file    evolution/core/util/SignalThread.hpp
 * @brief   A thread implementation which signals when it has
 *          reach some desired point in execution.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::SignalThread
 **/


#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IThread.hpp>
#include <evolution/core/base/IConditionVariable.hpp>
#include <evolution/core/base/IMutex.hpp>


namespace Evolution
{


    /**
     * @class SignalThread
     *
     * @brief
     * A thread implementation which signals when it has
     * reach some desired point in execution.
     * 
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL SignalThread
    {
    public: // Structors
        
        /**
         * @brief
         * Constructor.
         */
        SignalThread();

        /**
         * @brief
         * Destructor.
         */
        virtual ~SignalThread();

    public: // Execution

        /**
         * @brief
         * Starts the signal thread.
         *
         * @param start_wait_time_out [in]; The timeout when waiting
         *      for the thread's signal after starting.
         *
         * @return True if starting of the thread was successful.
         */
        bool start(unsigned long start_wait_time_out = ERSP_INFINITE);

        /** 
         * @brief
         * Halts the thread.
         *
         * @param time_out [in]; The timeout while waiting for the
         *      thread to stop.
         */
        virtual void halt(unsigned long time_out = ERSP_INFINITE);

    public: // Status

        /**
         * @brief
         * Signals that the thread has started.
         */
        void signal_start();

        /**
         * @brief
         * Waits for the thread to exit.
         *
         * @param time_out [in]; The wait timeout
         */
        void wait_for_exit(unsigned long time_out);

        /**
         * @brief
         * Returns true if the thread is running.
         *
         * @return True if the thread is running.
         */
        bool running();

        /**
         * @brief
         * Set the running state of the the thread.
         *
         * @param flag [in]; True if the thread is running.
         */
        void set_running(bool flag);

        /**
         * @brief
         * Returns true if the thread is started.
         *
         * 
         * @return True if the thread has been started.
         */
        bool started() { return _started; }
        
    protected: // Helpers

        /**
         * @brief
         * Runs when the thread is started.
         */
        virtual void run();

        /**
         * @brief
         * Thread procedure for SignalThread.
         *
         * @param param [in]; The thread parameter.
         *
         * @return The thread result.
         */
        static ThreadProcResult THREAD_PROC_CALL thread_proc(ThreadProcParam param);

    protected: // Implementation
        bool                            _brun;          ///< True if thread is running.
        Evolution::IMutex*              _brun_mutex;
        Evolution::IConditionVariable*  _start_cond;    ///< Condition variable that signals when the thread is started.
        Evolution::IThread*             _thread;        ///< Pointer to thread object.
        bool                            _started;       ///< True if thread is started.        
    };


} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_UTIL_SIGNAL_THREAD_HPP
