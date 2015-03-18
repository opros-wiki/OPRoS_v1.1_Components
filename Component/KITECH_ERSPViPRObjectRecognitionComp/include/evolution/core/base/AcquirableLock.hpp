//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_RESOURCE_LOCK_HPP
#define INCLUDE_EVOLUTION_BASE_RESOURCE_LOCK_HPP


/**
 * @file   evolution/core/base/AcquirableLock.hpp
 * @brief  Safely locks and releases objects instantiating the IAcquirable
 *         interface.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::AcquirableLock
 */


#include <evolution/core/base/IAcquirable.hpp>


namespace Evolution
{


    /**
     * @class AcquirableLock
     * @brief Safely locks and releases objects instantiating the IAcquirable
     * interface.
     *
     * The destructor unlocks the lock. This class can be used
     * to automatically lock and unlock an acquirable resource
     * (e.g. a mutex) in a scope block. 
     *
     * Example:
     * @code
     * int do_something(IMutex* mutex)
     * {
     *    // Locks the mutex for the duration of the method's
     *    // scope.
     *    AcquirableLock lock(&mutex);
     *
     *    // Do something.
     *    ...
     *
     *    if (error) {
     *      // No need to unlock mutex. The lock's destructor
     *      // will unlock the mutex automatically.
     *      return -1;
     *    }
     *    
     *    // Do more.
     *    ...
     *    
     *    // Automatically unlock the mutex on return.
     *    return 0;
     * }
     * @endcode 
     *
     * @note
     * The class is completely inlined to avoid linking, and thus
     * requires no export directive.
     *
     * @ingroup core_base
     */
    class AcquirableLock
    {
    public: // Structors

        /**
         * @brief Constructor. Acquires the given resource.
         *
         * @param resource [in]; An acquirable resource
         */
        AcquirableLock (IAcquirable & resource) : _resource (resource)
        {
            _is_locked = _resource.acquire ();
        } // end AcquirableLock()

        /// Destructor. Releases the associated resource.
        virtual ~AcquirableLock ()
        {
            if (_is_locked)
            {
                unlock();
            }
        } // end ~AcquirableLock()

        /**
         * @brief Lock the resource if it is not already locked.
         */
        virtual void lock ()
        {
            if (!_is_locked)
            {
                _resource.acquire ();
                _is_locked = true;
            }
        }

        /**
         * @brief Unlock the resource if currently locked.
         */
        virtual void unlock()
        {
            if (_is_locked)
            {
                _resource.release ();
                _is_locked = false;
            }
        }

    public: // Status report

        /**
         * @brief Check if the resource is locked.
         *
         * @return true if successful false otherwise.
         */
        bool is_locked () const
        {
            return _is_locked;
        }

    protected: // Implementation

        /**
         * @brief This is the acquirable resource itself.
         */
        IAcquirable & _resource;

        /**
         * @brief Flag indicating if the resource is locked.
         */
        bool _is_locked;

    }; // end class AcquirableLock

} // end namespace Evolution



#endif // INCLUDE_EVOLUTION_BASE_RESOURCE_LOCK_HPP
