//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_CONDITION_VARIABLE_HPP
#define INCLUDE_EVOLUTION_BASE_CONDITION_VARIABLE_HPP


/**
 * @file   evolution/core/base/IConditionVariable.hpp
 * @brief  This is an abstract condition variable class, to be implemented by
 *         each platform.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IConditionVariable
 */


#include <evolution/core/base/IAcquirable.hpp>


namespace Evolution
{
    /**
     * @interface IConditionVariable
     * @brief This is an abstract condition variable class, to be implemented by
     * each platform.
     *
     * Each condition variable has an associated mutex.  The associated mutex
     * is encapsulated in this class.  There is no need to create a separate
     * mutex object for use with the condition variable.  The lock and unlock
     * methods lock and unlock the associated mutex.  Use the
     * Platform::create_condition_variable() method to obtain a condition
     * variable instance.
     * @see Platform::create_condition_variable()
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE IConditionVariable:public IAcquirable
    {
    public:                  // Structors

        /**
         * @brief Default constructor.
         */
        IConditionVariable ()
        {
        }

        /**
         * @brief Destructor.
         */
        virtual ~ IConditionVariable ()
        {
        }

    public:                  // IAcquirable

        /** @brief Acquires the attached mutex.
         *
         * Acquires the attached mutex.
         * @return True if mutex locks, false otherwise.
         */
        bool acquire ()
        {
            return lock_mutex ();
        }

        /** @brief Releases the attached mutex.
         *
         * Releases the attached mutex.
         * @return True if mutex releases, false otherwise.
         */
        bool release ()
        {
            return unlock_mutex ();
        }

    public:                  // Operations

        /**
         * @brief  Wait on the condition.
         *
         * To ensure that the wait() call does not miss a signal from
         * another thread, you must lock the mutex before (and unlock
         * after).
         *
         * The wait call automically unlocks the associated mutex
         * and wait.
         *
         * Example:
         *
         * IConditionVariable* cv = Platform::create_condition_variable();<br>
         * <br>
         * Manually locks associated mutex.<br>
         * cv->lock_mutex();<br>
         * cv->wait();<br>
         * 
         * or
         *
         * Automatically lock associated mutex by specifying auto_lock<br>
         * to be true.<br>
         * cv->wait(ERSP_INFINITE, true);<br>
         *
         * @param wait_time [in]; max timeout 
         * @param auto_lock [in]; true automatically locks associated mutex
         * @return true if wait succeeded, false otherwise
         *
         **/
        virtual bool wait (unsigned long wait_time, bool auto_lock = false) = 0;

        /**
         * @brief  Signal one thread waiting on the condition.
         *
         * To ensure that the signal is not missed by wait() calls,
         * you must lock the mutex before (and unlock after).
         * 
         * Example:
         *
         * IConditionVariable* cv = Platform::create_condition_variable();<br>
         * <br>
         * Manually locks associated mutex.<br>
         * cv->lock_mutex();<br>
         * cv->signal();<br>
         * cv->unlock_mutex();<br>
         * @return true if signal succeeded, false otherwise
         **/
        virtual bool signal () = 0;

        /**
         * @brief  Signal all threads waiting on the condition.
         *
         * To ensure that the signal is not missed by wait() calls,
         * you must lock the mutex before (and unlock after).
         * @return True if broadcast succeeds.
         **/
        virtual bool broadcast () = 0;

        /** @brief Locks the attached mutex.
         *
         * Locks the attached mutex.
         * @return True if mutex locks, false otherwise.
         */
        virtual bool lock_mutex () = 0;

        /** @brief Unlocks the attached mutex.
         *
         * Unlocks the attached mutex.
         * @return True if mutex unlocks, false otherwise.
         */
        virtual bool unlock_mutex () = 0;

    };                          // end interface IConditionVariable


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_BASE_CONDITION_VARIABLE_HPP
