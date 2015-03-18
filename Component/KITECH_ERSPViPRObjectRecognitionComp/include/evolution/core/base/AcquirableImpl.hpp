//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_ACQUIRABLE_RESOURCE_IMPL_HPP
#define INCLUDE_EVOLUTION_BASE_ACQUIRABLE_RESOURCE_IMPL_HPP


/**
 * @file   evolution/core/base/AcquirableImpl.hpp
 * @brief  Simple implementation of the IAcquirable interface using mutexes.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::AcquirableImpl
 * @see Evolution::AcquirableResourceTempl
 */


#include <stdlib.h>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/IAcquirable.hpp>
#include <evolution/core/base/IMutex.hpp>
#include <evolution/core/base/Platform.hpp>


namespace Evolution
{

    /**
     * @class AcquirableImpl
     * @brief Simple implementation of the IAcquirable interface using mutexes.
     * 
     * @ingroup core_base
     * @ingroup core_base_aibo
     * @invariant  _acquirable_resource_impl_mutex != NULL
     */
    class EVOLUTION_EXPORT_BASE AcquirableImpl : public IAcquirable
    {
    public: // Structors

        /**
         * @brief Standard constructor
         *
         * @param recursive [in]; Determines if the AcquirableImpl is recursive or not.
         *
         * @see IAcquirable::IAcquirable()
         */
        AcquirableImpl (bool recursive = false)
        {
            _acquirable_resource_impl_mutex = Platform::create_mutex (recursive);
        }

        /**
         * @brief Standard destructor
         *
         * @see IAcquirable::~IAcquirable()
         */
        virtual ~AcquirableImpl ()
        {
            delete _acquirable_resource_impl_mutex;
            _acquirable_resource_impl_mutex = NULL;
        }

    public: // IAcquirable interface

        /**
         * @brief
         * Acquires (locks) the resource.
         *
         * @return  True or false
         */
        virtual bool acquire()
        {
            return ((_acquirable_resource_impl_mutex != NULL)
                    && (_acquirable_resource_impl_mutex->acquire ()));
        }

        /**
         * @brief
         * Release the resource.
         *
         * @return  True or false
         */
        virtual bool release()
        {
            return ((_acquirable_resource_impl_mutex != NULL)
                    && (_acquirable_resource_impl_mutex->release ()));
        }

    public: // Status report

        /**
         * @brief Checks if the mutex has been allocated.
         *
         * @return True if the contained mutex is not NULL, false otherwise.
         */
        virtual bool is_ok () const
        {
            return (_acquirable_resource_impl_mutex != NULL);
        }

        /**
         * @brief Checks if the mutex has been acquired.
         *
         * @return True if the contained mutex has been acquired, false otherwise.
         */
        virtual bool is_acquired () const 
        {
            return (_acquirable_resource_impl_mutex
                    && _acquirable_resource_impl_mutex->is_acquired ());
        }

    protected: // Implementation

        /**
         * @brief The implemented mutex
         */
        IMutex* _acquirable_resource_impl_mutex;
    }; // end class AcquirableImpl



    /**
     * @class AcquirableResourceTempl
     * @brief This template class allows virtually any other class to
     * be made an acquirable resource without declaring it.
     */
    template <class B>
    class AcquirableResourceTempl : public AcquirableImpl,
                                    public B
    {
    public:

        /**
         * @brief Simple constructor that calls both the acquirable interface
         * and the other "real" parent class.
         * @see IAcquirable::IAcquirable()
         */
        AcquirableResourceTempl () : AcquirableImpl (), B() {}

        /**
         * @brief Standard destructor
         * @see IAcquirable::IAcquirable()
         */
        virtual ~AcquirableResourceTempl () {}
    }; // end class AcquirableResourceTempl


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_ACQUIRABLE_RESOURCE_IMPL_HPP
