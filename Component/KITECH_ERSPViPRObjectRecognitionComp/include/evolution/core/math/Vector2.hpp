//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_VECTOR2_HPP
#define INCLUDE_EVOLUTION_MATH_VECTOR2_HPP


/**
 * @file    evolution/core/math/Vector2.hpp
 * @brief   Simple 2D vector class.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Vector2
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/Logging.hpp>
#include <evolution/core/math/MathUtils.hpp>

namespace Evolution
{

    /**
     * @class Vector2
     * @brief
     * A 2-dimensional vector.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    class EVOLUTION_EXPORT_MATH Vector2
    {
    public: // Structors

        /**
         * @brief Construct a vector from 2 points.
         * @param x [in]; value for first element of vector
         * @param y [in]; value for second element of vector
         */
        Vector2 (const double x = 0, const double y = 0)
        {
            _data[0] = x;
            _data[1] = y;
        }

        /**
         * @brief Construct a vector from an array of 2 points.
         * @param arr [in]; double array with 2 or more values to use as vector2 initializers
         * @pre length of arr is >= 2
         */
        Vector2 (const double arr[])   // Length of input array >= 2.
        {
            // @todo Check for arrays with length < 2?
            _data[0] = arr[0];
            _data[1] = arr[1];
        }

        /**
         * @brief Construct a vector from another vector.
         * @param v [in]; Vector2 to use in copy constructor
         */
        Vector2 (const Vector2& v)
        {
            _data[0] = v._data[0];
            _data[1] = v._data[1];
        }

    public: // Information access

        /**
         * @brief Access an indexed vector point.
         * @param index [in]; index to get value from.
         * @return value at index
         */
        double& operator[] (int index) { return _data[index]; }

        /**
         * @brief Constant access to an indexed vector point.
         * @param index [in]; index to get value from.
         * @return value at index
         */
        const double& operator[] (int index) const { return _data[index]; }

        /**
         * @brief Access an indexed vector point.
         * @param index [in]; index to get value from.
         * @return value at index
         */
        double& operator[] (unsigned int index) { return _data[index]; }

        /**
         * @brief Constant access to an indexed vector point.
         * @param index [in]; index to get value from.
         * @return value at index
         */
        const double& operator[] (unsigned int index) const { return _data[index]; }
        
        /**
         * @brief Cast a vector into a length 2 array of doubles.
         */
        operator const double *() const      // Array cast (array length == 2).
        {
            return _data;
        }

    public: // Modifications

        /**
         * @brief Manually set a vector's contents.
         * @param x [in]; value to set the 0th vector position to.
         * @param y [in]; value to set the 1th vector position to.
         */
        void set (const double x, const double y)
        {
            _data[0] = x;
            _data[1] = y;
        }
                                                    
        /**
         * @brief Clear all the elements.
         */
        void clear ()
        {
            set(0, 0);
        }

        /**
         * @brief Assign one vector to another vector.
         * @param v [in]; Vector2 to use in copy constructor
         * @return reference to this object
         */
        Vector2& operator= (const Vector2 &v)      // Vector assignment.
        {
            _data[0] = v._data[0];
            _data[1] = v._data[1];

            return *this;
        }

        /**
         * @brief Assign an array of doubles to a vector.
         * @param arr [in]; double array with 2 or more values to use as vector2 initializers
         * @return reference to this object
         */
        Vector2& operator= (const double arr[])    // Array assignment.
        {
            //@todo Check for arrays with length < 2?
            _data[0] = arr[0];
            _data[1] = arr[1];

            return *this;
        }

        /**
         * @brief Add one vector to another vector.
         * @param v [in]; Vector2 to add to this vector
         * @return reference to this object
         */
        Vector2& operator+= (const Vector2& v)     // Vector addition.
        {
            _data[0] += v._data[0];
            _data[1] += v._data[1];

            return *this;
        }

        /**
         * @brief Add one vector to another vector.
         * @param v [in]; Vector2 to subtract from this vector
         * @return reference to this object
         */
        Vector2& operator-= (const Vector2& v)     // Vector subtraction.
        {
            _data[0] -= v._data[0];
            _data[1] -= v._data[1];

            return *this;
        }

        /**
         * @brief Scale a vector.
         * @param scale [in]; double to use as a scaling factor
         * @return reference to this object
         */
        Vector2& operator*= (const double scale)   // Scaling.
        {
            _data[0] *= scale;
            _data[1] *= scale;

            return *this;
        }

        /**
         * @brief  Calculate the dot product.
         * @param other [in]; Vector2 to dot with this.
         * @return dot product of this and v
         */
        double dot (const Vector2& other) const
        {
            const Vector2& self = *this;
            return (self._data[0]*other._data[0] + 
                    self._data[1]*other._data[1]);
        }

        /**
         * @brief  return the 0th or x element of this Vector2
         * @return x or 0th value of this Vector2
         */
        double &x()
        {
            return _data[0];
        }

        /**
         * @brief  return the 1th or y element of this Vector2
         * @return y or 1th value of this Vector2
         */
        double &y()
        {
            return _data[1];
        }

        /**
         * @brief  Rotate a 2D vector by an angle theta 
         *         where theta is from the positive x axis.
         * @param theta [in]; angle to rotate this object by
         */
        void rotate(const double &theta)
        {
            double s = sin(theta);
            double c = cos(theta);
            
            double tmp_x =   x() * c + y() * s;
            
            double tmp_y = - x() * s + y() * c;
            
            x() = tmp_x;
            y() = tmp_y;
            
        }
         
        /**
         * @brief Return the Euclidean norm of the vector.
         * @return Norm of the vector.
         */
        double norm()
        {
            return sqrt(this->dot(*this));
        }

    public: // Binary operators

        /**
         * @brief Test if two vectors are equal.
         * @param v [in]; Vector2 to compare against.
         * @return true if equal, false otherwise
         */
        bool operator== (const Vector2& v) const   // Equality.
        {
            /* @todo
              return (_data[0] == v.vec[0] &&
              _data[1] == v._data[1] &&
              _data[2] == v._data[2]);

              std::cerr << '\n' << _data[0] << '-' << v._data[0] << '=' << _data[0]-v._data[0];
              std::cerr << '\n' << _data[1] << '-' << v._data[1] << '=' << _data[1]-v._data[1];
              std::cerr << '\n' << _data[2] << '-' << v._data[2] << '=' << _data[2]-v._data[2];
              std::cerr << "\nepsilon:" << std::numeric_limits<double>::epsilon();
              std::cerr << '\n';
            */
            return (double_equal(_data[0], v._data[0]) &&
                    double_equal(_data[1], v._data[1]));

        }

        /**
         * @brief Test if two vectors are inequal.
         * @param v [in]; Vector2 to compare against.
         * @return true if not equal, false otherwise
         */
        bool operator!= (const Vector2& v) const   // Inequality.
        {
            return !(*this == v);
        }

        /**
         * @brief Return the sum of two vectors.
         * @param v [in]; Vector2 to sum with
         * @return new Vector2 that is the sum of *this and v.
         */
        Vector2 operator+ (const Vector2& v) const // Vector addition.
        {
            Vector2 r = *this;
            return r += v;
        }

        /**
         * @brief Return the difference of two vectors.
         * @param v [in]; Vector2 to subtract from *this.
         * @return new Vector2 that is the difference of *this and v.
         */
        Vector2 operator- (const Vector2& v) const // Vector subtraction.
        {
            Vector2 r = *this;
            return r -= v;
        }

        /**
         * @brief Negative Vector.
         * @return new Vector2 that *this * -1
         */
        Vector2 operator- () const // Vector subtraction.
        {
            Vector2 r = *this;
            return r *= -1;
        }

        /**
         * @brief Return the scaling of a vector.
         * @param scale [in]; factor to multiply by
         * @return Vector2 that is *this * scale
         */
        Vector2 operator* (const double scale) const   // Scaling.
        {
            Vector2 r = *this;
            return r *= scale;
        }

    public: // Output

        /**
         * @brief Print a the contents of a Vector to the specified stream.
         * @param out [in]; stream to write to
         * @param vec [in]; Vector2 to write.
         * @return reference to passed in ostream
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::Vector2 & vec)
        {
            out << "( " << vec[0] << ", " << vec[1] << " )";
            return out << "\n";
        }

    protected: // Implementation.

        /// Friend class declaration
        friend class Vector2Field; 

        /**
         * @brief The actual x/y coordinates of the vector.
         **/
        double _data[2]; // x/y coordinates.
    }; // end class Vector2

    // External operators.
    /**
     * @brief Return the scaling of a vector.
     * @param scale [in]; double to multiply with
     * @param v [in]; Vector2 to multiply with
     * @return new multipled Vector2
     */
    Vector2 EVOLUTION_EXPORT_MATH operator* (const double scale, const Vector2& v);   // Scaling.

} // end namespace Evolution.

#endif  // INCLUDE_EVOLUTION_MATH_VECTOR2_HPP
