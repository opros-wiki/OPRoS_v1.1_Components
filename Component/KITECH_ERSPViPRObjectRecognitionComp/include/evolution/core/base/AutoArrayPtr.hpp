//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_AUTO_ARRAY_PTR_HPP
#define INCLUDE_EVOLUTION_BASE_AUTO_ARRAY_PTR_HPP

/**
 * @file   evolution/core/base/AutoArrayPtr.hpp
 * @brief  This is an auto_ptr for arrays. It verifies that they are deleted
 *         with the delete [] syntax.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::AutoArrayPtr 
 */

namespace Evolution
{

    /**
     * @class AutoArrayPtr
     *
     * @brief
     * This is an auto_ptr for arrays. It verifies that they are deleted
     * with the delete [] syntax.
     *
     * This object works in exactly the same fashion as auto_ptr, with all
     * the same functions.  The only difference is that the cleanup mechanism
     * used is delete[] instead of delete.  This is useful for giving allocated arrays
     * to an object for automatic cleanup.
     *
     * { 
     *   double * array = new double[50]; 
     *   AutoArrayPtr aap; aap.reset(array);
     *   //Scope ends and the array is cleaned up when destructor of AutoArrayPtr is called.
     * }
     *
     * @ingroup core_base
     */
    template<class X>
    class AutoArrayPtr 
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param p [in]; The initializtion parameter
         */
        explicit AutoArrayPtr(X* p = 0) : _p(p) {}

        /**
         * @brief Constructor.
         *
         * @param ap [in]; The initialization parameter
         */
        explicit AutoArrayPtr(AutoArrayPtr<X>& ap) : _p(ap.release()) {}

        /// Destructor.
        ~AutoArrayPtr() {delete [] _p;}

        /**
         * @brief Assignment operator.
         *
         * @param rhs [in]; AutoArrayPtr to assign
         *
         * @return A reference to the assigned object.
         */
        AutoArrayPtr& operator=(AutoArrayPtr<X>& rhs)
        {
            if (this != &rhs)
            {
                reset(rhs.release());
            }

            return *this;
        }

        /**
         * @brief Dereference operator.
         *
         * @return A reference to the underlying pointer.
         */
        X& operator*() {return *_p;}

        /**
         * @brief Array subscript operator.
         *
         * @param i [in]; The array index
         *
         * @return A reference to an element contained within the array.
         */
        X& operator[](int i) {return _p[i];}

        /**
         * @brief Array subscript operator.
         *
         * @param i [in]; The array index
         *
         * @return A copy of the element contained within the array.
         */
        X operator[](int i) const {return _p[i];}

        /**
         * @brief Returns the raw pointer.
         *
         * @return The underlying pointer.
         */
        X* get() const {return _p;}

        /**
         * @brief Relinquishes ownership of the underlying raw pointer
         * and returns it to the client.
         *
         * @return The underlying pointer.
         */
        X* release() {X* tp = _p; _p = 0; return tp;}

        /**
         * @brief Deallocates the underlying pointer and sets it to p.
         *
         * @param p [in]; The pointer to reset the underlying pointer with
         */
        void reset(X* p) {X* tp = _p; _p = p; delete [] tp;}

        /**
         * @brief A static method which deallocates an AutoArrayPtr.
         *
         * @param x [in]; The pointer to deallocate
         */
        static void remove(X*& x) {X* tp=x; x=0; delete [] tp;}

    private:

        X* _p;
    };

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE_AUTO_ARRAY_PTR_HPP
