//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_REF_PTR_HPP
#define INCLUDE_EVOLUTION_BASE_REF_PTR_HPP

/**
 * @file   evolution/core/base/RefPtr.hpp
 * @brief  A reference-counted pointer class.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::RefPtr
 */


namespace Evolution
{

    /**
     * @class RefPtr
     * @brief A reference-counted pointer class.
     * This should work on any objects which have an interface
     * with the functions add_ref() and remove_ref().
     * This class is especially useful in collection classes.
     * Beware of cycles in your reference-counted pointers.
     *
     * @param The type of the reference-counted object
     * @ingroup core_base
     */
    template<class T>
    class RefPtr
    {
    public: // constructor/destructor

        /**
         * @brief Constructor.
         *
         * Often the object pointed to by (ptr) will already 
         * have the reference count, so don't 
         * increment it again.  
         *
         * Most of the objects in ERSP seem to start 
         * their reference counts at 1, so you'll have to set 
         * add_reference to false quite often.
         *
         * @param ptr [in]; Bare pointer (defaults to NULL)
         * @param add_reference [in]; Flag indicating whether 
         * the reference count of the object should be incremented
         * on construction (defaults to true)
         *
         */ 
        RefPtr(T* ptr = NULL, bool add_reference = true) : _ptr(ptr)
        {
            if (add_reference) 
            {
                add_ref();
            }
        }

        /**
         * @brief Copy constructor.
         *
         * Increments the reference count of the object
         * if the pointer is non-NULL.
         *
         * @param refptr [in]; The ptr to be copied
         */
        RefPtr(const RefPtr& refptr) : _ptr(refptr._ptr)
        {
            add_ref();
        }
        
        /**
         * @brief Destructor
         *
         * Decrements the reference count of the object
         * if the pointer is non-NULL.
         */
        ~RefPtr()
        {
            if (_ptr)
            {
                _ptr->remove_ref();
            }
        }

        /**
         * @brief Assignment operator.
         *
         * Increments the reference count of the object
         * if the pointer is non-NULL.
         * Also checks to make sure that there is no self-
         * assigning.
         *
         * @param refptr [in]; The reference to be assigned
         *
         * @return A reference to this object.
         */
        RefPtr& operator=(const RefPtr& refptr)
        {
            if (_ptr != refptr._ptr)
            {
                T* oldptr = _ptr;
                _ptr = refptr._ptr;
                add_ref();
                if (oldptr)
                {
                    oldptr->remove_ref();
                }
            }
            return *this;
        }

    public: // Dereferencing operators

        /**
         * @brief Dereference operator.
         *
         * @return The underlying pointer.
         */
        T* operator->() const
        {
            return _ptr;
        }

        /**
         * @brief Dereference operator
         *
         * @return A reference to the underlying pointer.
         */
        T& operator*() const
        {
            return *_ptr;
        }

    public: // Other operators

        /**
         * @brief Equality operator.
         *
         * @param refptr [in]; The RefPtr to compare against
         *
         * @return true if equal, false otherwise.
         */
        bool operator==(const RefPtr& refptr) const
        {
            return refptr._ptr == _ptr;
        }

        /**
         * @brief Less than operator
         *
         * @param refptr [in]; The RefPtr to compare against
         *
         * @return true if less than, false otherwise.
         */
        bool operator<(const RefPtr& refptr) const
        {
            return _ptr < refptr._ptr;
        }

        /**
         * @brief Greater than operator
         *
         * @param refptr [in]; The RefPtr to compare against
         *
         * @return true if greater than, false otherwise.
         */
        bool operator>(const RefPtr& refptr) const
        {
            return _ptr > refptr._ptr;
        }

        /**
         * @brief Returns the underlying pointer.
         *
         * @return The underlying pointed.
         */
        T* get_ptr() const
        {
            return _ptr;
        }


    protected: // Internal functions

        /**
         * @brief Adds a reference to the pointed-to object
         * if the pointer is non-NULL.
         */
        void add_ref()
        {
            if (0 == _ptr) 
            {
                return;
            }
            _ptr->add_ref();
        }

    protected:

        T* _ptr; ///< Internal copy of the referenced pointer.
    };
} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_REF_PTR_HPP

