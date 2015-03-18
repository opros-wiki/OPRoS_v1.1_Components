//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_OBJECT_IMPL_HPP
#define INCLUDE_EVOLUTION_BASE_OBJECT_IMPL_HPP


/**
 * @file    evolution/core/base/ObjectImpl.hpp
 * @brief   Implementation of IObject.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IMutex
 * @see Evolution::ObjectImpl
 */


#include <evolution/core/base/IObject.hpp>

namespace Evolution
{

    class EVOLUTION_EXPORT_BASE IMutex;

    /**
     * @class ObjectImpl
     * @brief
     * This is an base interface class, with reference counting and the ability to obtain
     * other interfaces.
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE ObjectImpl : public IObject
    {
    public: // Types

        enum ThreadSafety
        {
            THREAD_SAFETY_DEFAULT,   ///< Use the default setting.
            THREAD_SAFETY_NONE,      ///< No thread safety.
            THREAD_SAFETY_SMALL,     ///< Smaller memory footprint, potentially slower.
            THREAD_SAFETY_FAST       ///< Faster execution, larger memory.
        };

    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param thread_safety [in]; The ThreadSafety setting
         */
        ObjectImpl (ThreadSafety thread_safety = THREAD_SAFETY_DEFAULT);

    protected:

        /// Destructor.
        virtual ~ObjectImpl ();

    public: // IObject interface

        /**
         * @brief Adds a reference to the object.
         *
         * @see  IObject::add_ref()
         *
         * @return The resultant reference count. 
         */
        virtual unsigned add_ref ();

        /**
         * @brief Removes a reference from the object, freeing its memory if
         * the count is zero.
         *
         * @see  IObject::remove_ref()
         *
         * @return The resultant reference count. 
         */
        virtual unsigned remove_ref ();

        /**
         * @brief Returns the current reference count.
         *
         * @return The current reference count.
         */
        virtual unsigned ref_count () ;

        /**
         * @brief Obtains the specified interface from the object if the
         * object supports that interface and caller's security ticket
         * has access to it.
         *
         * @param ticket [in]; security ticket
         * @param interface_id [in]; string identifying the desired interface to obtain
         * @param interface_ptr [out]; pointer to requested interface assigned here by completion
         *
         * @see  IObject::obtain_interface()
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result obtain_interface (TicketId ticket, 
                                         const char* interface_id,
                                         void** interface_ptr);

    public: // Thread safety

        /**
         * @brief Indicates the thread safety of the object.
         *
         * @return The ThreadSafety value.
         */
        ThreadSafety get_thread_safety () const;

        /**
         * @brief Sets the default thread safety for newly created objects
         * whose classes derive from ObjectImpl.
         *
         * @param thread_safety [in]; ThreadSafety value to set the default
         * ThreadSafety for newly created objects to
         */
        static void set_default_thread_safety (ThreadSafety thread_safety);

        /**
         * @brief Indicates if ObjectImpl is thread-safe by default.
         *
         * @return The ThreadSafety value.
         */
        static ThreadSafety get_default_thread_safety ();

    protected: // Helpers

        /**
         * @brief Lock the mutex if needed.
         *
         * @return True on success, or false otherwise.
         */
        bool maybe_lock ();

        /**
         * @brief Unlock the mutex if needed.
         *
         * @return True on success, or false otherwise.
         */
        bool maybe_unlock ();

    protected: // Implementation

        /// Reference counter used for garbage collection.
        volatile unsigned    _ref_count;

#ifndef EVOLUTION_NO_THREAD_SUPPORT
        /// Mutex to protect the object's reference count.
        IMutex*              _object_impl_mutex;
#endif

        /// Indicates if the object is thread-safe.
        ThreadSafety         _object_impl_thread_safety;

        /// Indicates if new objects will be thread-safe.
        static ThreadSafety  _default_thread_safety;
    }; // end class ObjectImpl


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_IOBJECT_HPP

