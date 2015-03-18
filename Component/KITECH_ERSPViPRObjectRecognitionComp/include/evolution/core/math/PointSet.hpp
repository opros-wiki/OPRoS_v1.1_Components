//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_POINT_SET_HPP
#define INCLUDE_EVOLUTION_MATH_POINT_SET_HPP


/**
 * @file    evolution/core/math/PointSet.hpp
 * @brief   A set of 3-dimensional points.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::PointSet
 */


#include <stddef.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Vector3.hpp>


namespace Evolution
{

    /**
     * @class PointSet
     *
     * @brief
     * A set of 3-dimensional points. The class is reference counted,
     * because the driver will generally be in a separate shared
     * library from the caller, and, depending on the platform, may be
     * allocated from a different heap.
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH PointSet
    {
    public: // Structors

        /// Constructor.
        /**
         * @brief  Construct a point set containing 0 points
         */
        PointSet ()
        {
            _ref_count = 1;
            _size = 0;
            _points = NULL;
            _timestamp = 0;
        } // end PointSet()

        /**
         * @brief  Construct a point set with room for 'size' points
         * Timestamp is optionally set
         */
        PointSet (size_t size, Timestamp timestamp = 0)
        {
            _ref_count = 1;
            _size = size;
            _points = new Vector3[_size];
            _timestamp = timestamp;
        }

        /// Destructor.
        /**
         * @brief  Standard destructor-- deallocates point list
         */
        ~PointSet ()
        {
            delete[] _points;
        } // end ~PointSet()

    public: // Access

        /**
         * @brief  Returns the size of the point set
         */
        size_t size () const { return (_size); }

        /**
         * @brief  Returns a reference to a specific indexed point
         */
        Vector3& operator[] (unsigned index)
        {
            return (_points [index]);
        }

        /**
         * @brief  Returns a constant reference to a specific indexed point
         */
        const Vector3& operator[] (unsigned index) const
        {
            return (_points [index]);
        }

        /**
         * @brief  Returns the pointset's timestamp
         */
        Timestamp get_timestamp () const
        {
            return (_timestamp);
        }

        /**
         * @brief  Sets the pointset's timestamp
         */
        void set_timestamp (Timestamp time)
        {
            _timestamp = time;
        }

    public: // Comparison

        /**
         * @brief Operator ==
         * @param other [in]; Pointset to be compared with the current one.
         * @return Boolean indicating whether the two point sets are equal.
         */
        bool operator==(const PointSet &other) const
        {
            if(_size != other._size) return false;
            if(double_not_equal(_timestamp, other._timestamp)) return false;
            size_t ii=0;
            for( ii=0; ii<_size; ii++)
            {
                //FIXME: check to see if ii is equal to any of the other points?
                if (_points[ii] != other._points[ii])
                {
                    return false;
                }
            }
            return true;
        }

    public: // Size

        /**
         * @brief  Resize the point set, deleting all points in it if
         * the size changed.
         */
        void resize (size_t size)
        {
            if (size != _size)
            {
                delete[] _points;
                _size = size;
                _points = new Vector3 [_size];
            }
        }

    protected: // Implementation

        /**
         * @brief  A counter of the number of external objects who have
         * active pointers to this point set.
         */
        unsigned     _ref_count;

        /**
         * @brief  The number of points in this point set.
         */
        size_t  _size;

        /**
         * @brief  The points themselves
         */
        Vector3*     _points;

        /**
         * @brief  A timestamp, for people who care about stuff like that
         */
        Timestamp  _timestamp;
    }; // end class PointSet

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_POINT_SET_HPP
