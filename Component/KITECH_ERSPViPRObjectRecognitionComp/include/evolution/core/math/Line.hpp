//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_LINE_HPP
#define INCLUDE_EVOLUTION_MATH_LINE_HPP 

/**
 * @file    evolution/core/math/Line.hpp
 * @brief   Class defining a Line in 3space.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Line
 */


#include <stddef.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Vector3.hpp>


namespace Evolution
{

    /**
     * @class Line
     *
     * @brief Class defining a Line in 3space.
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH Line
    {
    public: // Structors

        /// Constructor.
        /**
         * @brief  Construct a point set containing 0 points.
         */
        Line () :_p(0,0,0), _q(0,0,0) {}

        /**
         * @brief  Construct a point set with room for 'size' points.
         * Timestamp can be set (optional).
         */
        Line (Vector3 p, Vector3 q) :_p(p), _q(q) {}

        /**
         * @brief  Construct a point set with room for 'size' points
         * Timestamp can be set (optional).
         */
        Line (Line& other) :_p(other.p()), _q(other.q()) {}

        /**
         * @brief  Construct a Line from 6 doubles.
         * Timestamp can be set (optional).
         * @param a [in]; x coord of p1
         * @param b [in]; y coord of p1
         * @param c [in]; z coord of p1
         * @param d [in]; x coord of p2
         * @param e [in]; y coord of p2
         * @param f [in]; z coord of p2
         */
        Line (double a, double b, double c, double d, double e, double f)
            : _p(a,b,c), _q(d,e,f) {}

        /**
         * @brief  Standard destructor
         */
        ~Line ()
        {} // end ~Line()

    public: // Access

        /**
         * @brief  Returns a reference to the first point in the line.
         * @return the 1st point of the line.
         */
        Vector3& p ()
        {
            return _p;
        }

        /**
         * @brief  Returns a reference to the last point in the line.
         * @return the 2nd point of the line.
         */
        Vector3& q ()
        {
            return _q;
        }

        /**
         * @brief Returns the length of the line.
         * @return double length of the line.
         */
        double length();

        /**
         * @brief  Returns a reference to the first point in the line.
         * @return the 1st point of the line.
         */
        Vector3& start ()
        {
            return _p;
        }

        /**
         * @brief  Returns a reference to the last point in the line.
         * @return the 2nd point of the line.
         */
        Vector3& end ()
        {
            return _q;
        }

    public: // Setting

        /**
         * @brief Sets _p and _q
         * @param p [in]; new first point of line.
         * @param q [in]; new second point of line.
         */
        void set(Vector3 &p, Vector3 &q)
        {
            _p = p;
            _q = q;
        }

    protected: // Implementation

        /**
         * @brief  The points themselves
         */
        Vector3     _p; ///< first point of the line.
        Vector3     _q; ///< second point of the line.
    }; // end class Line

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_LINE_HPP
