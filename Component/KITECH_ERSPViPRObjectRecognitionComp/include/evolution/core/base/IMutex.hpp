//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_MUTEX_HPP
#define INCLUDE_EVOLUTION_BASE_MUTEX_HPP


/**
 * @file   evolution/core/base/IMutex.hpp
 * @brief  This is an abstraction of the mutex thread synchronization concept
 *         for cross-platform usage.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IMutex
 */


#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/IAcquirable.hpp>

namespace Evolution
{


    /**
     * @interface  IMutex
     * @brief
     * This is an abstraction of the mutex thread synchronization concept for 
     * cross-platform usage. Use the Platform::create_mutex() method
     * to obtain a mutex instance.
     * @see Platform::create_mutex()
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE IMutex:public IAcquirable
    {
    public:                  // Constructor / destructor

        /// Constructor.
        IMutex ()
        {
        }

        /// Destructor.
        virtual ~ IMutex ()
        {
        }

    public:                  // Safe value access

        /**
         * Obtain a shared value safely.
         *
         * @param     shared_value [in]; The shared value to obtain.
         * @param     value_copy [out]; This is a copy of the value.
         * @return  Returns true or false for success or failure.
         */
        template < class T > bool
        atomic_get_value (const T & shared_value, T & value_copy)
        {
            if (!acquire ())
                return false;
            value_copy = shared_value;
            return release ();
        }                       // end atomic_get_value()

        /**
         * Set a shared value safely.
         *
         * @param     shared_value [in,out]; The shared value to be set.
         * @param     new_value [in]; This is the new value.
         * @return  Returns true or false for success or failure.
         */
        template < class T > bool
        atomic_set_value (T & shared_value, const T & new_value)
        {
            if (!acquire ())
                return false;
            shared_value = new_value;
            return release ();
        }

        /**
         * Increment or decrement a shared value safely.
         *
         * @param     shared_value [in,out]; The value to adjust.
         * @param     adjust_amount [in]; The amount by which to adjust.
         * @param     new_value [out]; This is the new value.
         * @return  Returns true or false for success or failure.
         */
        template < class T > bool
        atomic_adjust_value (T & shared_value, const T adjust_amount,
                             T & new_value)
        {
            if (!acquire ())
                return false;
            shared_value += adjust_amount;
            new_value = shared_value;
            return release ();
        }
    };                          // end class IMutex
}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_BASE_MUTEX_HPP
