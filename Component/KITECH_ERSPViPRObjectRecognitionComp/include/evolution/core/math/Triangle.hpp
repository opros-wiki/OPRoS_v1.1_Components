//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_TRIANGLE_HPP
#define INCLUDE_EVOLUTION_MATH_TRIANGLE_HPP 

/**
 * @file    evolution/core/math/Triangle.hpp
 * @brief   A class defining a triangle in 3space.  The outward normal is 
 *          defined as right hand rule on first-second-third points.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Triangle
 */


#include <stddef.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/Line.hpp>


namespace Evolution
{

    /**
     * @class Triangle
     * @brief
     * A class defining a triangle in 3space.  The outward normal is 
     * defined as right hand rule on first-second-third points.
     *
     * @ingroup core_math
     *
     * @see Vector3
     **/
    class EVOLUTION_EXPORT_MATH Triangle
    {
    public: // Structors

        /**
         * @brief  Constructs a Triangle with 000 000 000
         */
        Triangle ()
        { 
            _a[0] = _a[1] = _a[2] = 0;
            _b[0] = _b[1] = _b[2] = 0;
            _c[0] = _c[1] = _c[2] = 0;
            _good_normal = false;
        } // End Triangle().

        /**
         * @brief  Constructs a Triangle from the 3 points specified.
         *
         * @param a [in]; First point
         * @param b [in]; Second point
         * @param c [in]; Third point
         */
        Triangle (Vector3 a, Vector3 b, Vector3 c)
        {
            _a = a;
            _b = b; 
            _c = c;
            _good_normal = true;
            _normal = cross((_a-_b),(_a-_c));
        } // End Triangle().

        /**
         * @brief  Constructs a Triangle from 9 length double array. 
         * Rudimentary bounds checking, but 9 length arrays are best.
         *
         * @param data [in]; Input array of coordinates
         * @param size [in]; Length of data
         */
        Triangle (double * data, int size)
        {
            _good_normal = false;
            _a[0] = (size>=1)?data[0]:0;
            _a[1] = (size>=2)?data[1]:0;
            _a[2] = (size>=3)?data[2]:0;
            _b[0] = (size>=4)?data[3]:0;
            _b[1] = (size>=5)?data[4]:0;
            _b[2] = (size>=6)?data[5]:0;
            _c[0] = (size>=7)?data[6]:0;
            _c[1] = (size>=8)?data[7]:0;
            _c[2] = (size>=9)?data[8]:0;

        } // End Triangle().
    
        /**
         * @brief  Constructs a Triangle from 9 length double array. 
         * Rudimentary bounds checking, but 9 length arrays are best.
         *
         * @param a [in]; First coordinate of first point
         * @param b [in]; Second coordinate of first point
         * @param c [in]; Third coordinate of first point
         * @param d [in]; First coordinate of second point
         * @param e [in]; Second coordinate of second point
         * @param f [in]; Third coordinate of second point
         * @param g [in]; First coordinate of third point
         * @param h [in]; Second coordinate of third point
         * @param i [in]; Third coordinate of Third coordinate of 
         */
        Triangle (double a, double b, double c, double d, double e, 
                  double f, double g, double h, double i)
        {
            _good_normal = false;
            _a[0] = a;
            _a[1] = b;
            _a[2] = c;
            _b[0] = d;
            _b[1] = e;
            _b[2] = f;
            _c[0] = g;
            _c[1] = h;
            _c[2] = i;

        } // End Triangle().

        /**
         * @brief  Standard destructor
         */
        ~Triangle () {} // End ~Triangle()

    public: // Access

        /**
         * @brief  Returns a reference to the first point.
         * @return A reference to the point
         */
        Vector3& one ()
        {
            _good_normal = false;
            return _a;
        }

        /**
         * @brief  Returns a reference to the second point.
         * @return A reference to the point
         */
        Vector3& two ()
        {
            _good_normal = false;
            return _b;
        }

        /**
         * @brief  Returns a reference to the third point.
         * @return A reference to the point
         */
        Vector3& three ()
        {
            _good_normal = false;
            return _c;
        }

        /**
         * @brief  Sets the points of the triangle.
         * @param a [in]; First point
         * @param b [in]; Second point
         * @param c [in]; Third point
         * @return A reference to the Triangle
         */
        Triangle&  set (Vector3& a, Vector3& b, Vector3& c)
        {
            _good_normal = false;
            _a = a; _b = b; _c = c;
            return *this;
        }

        /**
         * @brief  Sets the points of the triangle.
         *
         * @param a [in]; First coordinate of first point
         * @param b [in]; Second coordinate of first point
         * @param c [in]; Third coordinate of first point
         * @param d [in]; First coordinate of second point
         * @param e [in]; Second coordinate of second point
         * @param f [in]; Third coordinate of second point
         * @param g [in]; First coordinate of third point
         * @param h [in]; Second coordinate of third point
         * @param i [in]; Third coordinate of Third coordinate of 
         * @return A reference to the Triangle
         */
        Triangle& set (double a, double b, double c, double d, double e, 
                       double f, double g, double h, double i)
        {
            _good_normal = false;
            _a[0] = a;
            _a[1] = b;
            _a[2] = c;
            _b[0] = d;
            _b[1] = e;
            _b[2] = f;
            _c[0] = g;
            _c[1] = h;
            _c[2] = i;
            return *this;
        } // End set().

    public: // Math

        /**
         * @brief  Returns the triangle normal.
         * @return A reference to the normal vector
         */
        Vector3& normal ()
        {
            // Probably not very efficient.
            if(_good_normal)
            {
                return _normal;
            }
            else
            {
                _normal = cross((_a-_b),(_a-_c));
                _good_normal = true;
                return _normal;
            }
        }

        /**
         * @brief Returns the sum of the dim coordinates of the points.
         * @return Sum of the dimensions of the points
        */
        double dim_sum(int dim)
        {
            return _a[dim]+_b[dim]+_c[dim];
        }

        /**
         * @brief Returns if a line intersects the triangle.
         *
         * @param line [in]; Line to intersect with the Triangle.
         *
         * @return Boolean value: "true" if the line intersects the Triangle.
         */
        bool line_intersect_test(Line line);

        /**
         * @brief Returns coordinates of a line/triangle intersect.
         *
         * @param line [in]; Line to intersect with the Triangle.
         * @param intersect [out]; Boolean value: "true" if the line intersects the Triangle.
         * @return Coordinates of the intersection point, invalid data otherwise
         */
        Vector3 line_intersect_point(Line line, bool *intersect);

    protected: // Implementation.

        Vector3     _a;           ///< First  point
        Vector3     _b;           ///< Second point
        Vector3     _c;           ///< Third  point
        Vector3     _normal;      ///< 3space normal vector, (ab x ac)
        bool        _good_normal; ///< Boolean controlling normal recalculation
    }; // end class Triangle.

} // end namespace Evolution.

#endif  // INCLUDE_EVOLUTION_MATH_TRIANGLE_HPP
