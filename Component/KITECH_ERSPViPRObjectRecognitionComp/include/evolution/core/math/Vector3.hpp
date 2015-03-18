//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_VECTOR3_HPP
#define INCLUDE_EVOLUTION_MATH_VECTOR3_HPP


/**
 * @file    evolution/core/math/Vector3.hpp
 * @brief   Simple 3D vector class.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Vector3
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/math/MathUtils.hpp>

namespace Evolution
{

    class EulerAngles;
    class Matrix3;

    /**
     * @class Vector3
     * @brief
     * A 3-dimensional vector.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    class EVOLUTION_EXPORT_MATH Vector3
    {
    public:     // Structors

        /**
         * @brief Construct a vector from 3 points.
         * @param x [in]; 0th element of Vector3
         * @param y [in]; 1th element of Vector3
         * @param z [in]; 2th element of Vector3
         */
        Vector3 (const double x = 0, const double y = 0, const double z = 0)
        {
            set (x, y, z);
        }

        /**
         * @brief Construct a vector from an array of 3 points.
         * @param arr [in]; array to initialize with, length >= 3
         * @pre arr must be 3 or longer length array
         */
        Vector3 (const double arr[])   // Length of input array >= 3.
        {
            set (arr[0], arr[1], arr[2]);
        }

        /**
         * @brief Construct a vector from another vector.
         * @param v [in]; Vector3 to use in copy constructor
         */
        Vector3 (const Vector3& v)
        {
            _data[0] = v._data[0];
            _data[1] = v._data[1];
            _data[2] = v._data[2];
        }

        /**
         * @brief
         * Constructs as a rotation vector from the matrix, using
         * Rodrigues' formula.
         * @param m [in]; Matrix3 to convert to a rotation vector.
         **/
        explicit Vector3 (const Matrix3& m)
        {
            set_rotation_matrix (m);
        }

    public: // Information access

        /**
         * @brief Test if position vector is not the origin (0, 0, 0)
         *        returns true if it is not the origin and false otherwise.
         */
        operator bool () const 
        {
            return ((_data[0] != 0.0) || (_data[1] != 0.0) 
                    || (_data[2] != 0.0));
        }
        
        /**
         * @brief Access an indexed vector point.
         * @param index [in]; index to access
         * @return value at index
         */
        double& operator[] (int index)
        {             
            ASSERT (index >= 0 && index < 3);
            return _data[index]; 
        }

        /**
         * @brief Constant access to an indexed vector point.
         * @param index [in]; index to access
         * @return value at index
         */
        const double& operator[] (int index) const 
        {             
            ASSERT (index >= 0 && index < 3);
            return _data[index]; 
        }

        /**
         * @brief Access an indexed vector point.
         * @param index [in]; index to access
         * @return value at index
         */
        double& operator[] (unsigned int index) 
        {             
            ASSERT (index < 3);
            return _data[index]; 
        }

        /**
         * @brief Constant access to an indexed vector point.
         * @param index [in]; index to access
         * @return value at index
         */
        const double& operator[] (unsigned int index) const 
        { 
            ASSERT (index < 3);
            return _data[index]; 
        }
        
        /**
         * @brief Cast a vector into a length 3 array of doubles.
         */
        operator const double * () const
        {
            return _data;
        }

        /**
         * @brief The length of the vector squared.
         * @return length of the vector squared, as a double.
         */
        double lengthsquared() const
        {
            return ((_data[0]*_data[0]) + (_data[1]*_data[1]) + 
                    (_data[2]*_data[2]));
        }

        /**
         * @brief The length of the vector.
         * @return Length of the vector 
         */
        double length() const
        {
            return (sqrt((_data[0]*_data[0]) + (_data[1]*_data[1]) + 
                         (_data[2]*_data[2])));
        }

        /**
         * @brief The Euclidean norms of the vector.
         * @return The Euclidean norm of the vector
         */
        double norm() const
        {
            return (length());
        }

        /**
         * @brief Return vector as a const pointer.
         * @return double pointer to internal data array
         */
        const double * get_data () const { return _data; } 

    public:     // Modifications

        /**
         * @brief Manually set a vector's contents.
         * @param x [in]; x value of the Vector3
         * @param y [in]; y value of the Vector3
         * @param z [in]; z value of the Vector3
         */
        void set (const double x, const double y, const double z)
        {
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
        }

        /**
         * @brief Manually set a vector's contents.
         * @param data [in]; double array of length >= 3
         */
        void set (const double *data)
        {
            if(data == NULL) return;
            memcpy(_data, data, 3 * sizeof(double));
        }

        /**
         * @brief Sets as a rotation vector from the matrix, using Rodrigues' formula.
         * @param m [in]; Matrix3 to convert to a rotation vector.
         **/
        void set_rotation_matrix (const Matrix3& m);       
                                                    
        /**
         * @brief Clear all the elements.
         */
        void clear ()
        {
            set(0, 0, 0);
        }

        /**
         * @brief Assign one vector to another vector.
         * @param v [in]; vector to copy
         * @return reference to this
         */
        Vector3& operator= (const Vector3 &v)
        {
            _data[0] = v._data[0];
            _data[1] = v._data[1];
            _data[2] = v._data[2];
            
            return *this;
        }

        /**
         * @brief Assign an array of doubles to a vector.
         * @param arr [in]; array to copy
         * @return reference to this
         */
        Vector3& operator= (const double arr[])
        {
            // @todo Check for arrays with length < 3?
            _data[0] = arr[0];
            _data[1] = arr[1];
            _data[2] = arr[2];
            
            return *this;
        }

        /**
         * @brief Add one vector to another vector.
         * @param v [in]; vector to add to *this
         * @return reference to *this
         */
        Vector3& operator+= (const Vector3& v)
        {
            _data[0] += v._data[0];
            _data[1] += v._data[1];
            _data[2] += v._data[2];
            
            return *this;
        }

        /**
         * @brief Add one vector to another vector.
         * @param v [in]; vector to subtract from *this
         * @return reference to *this
         */
        Vector3& operator-= (const Vector3& v) 
        {
            _data[0] -= v._data[0];
            _data[1] -= v._data[1];
            _data[2] -= v._data[2];

            return *this;
        }

        /**
         * @brief Scale a vector.
         * @param scale [in]; factor to scale vector by
         * @return reference to *this
         */
        Vector3& operator*= (const double scale)
        {
            _data[0] *= scale;
            _data[1] *= scale;
            _data[2] *= scale;
            
            return *this;
        }
        
        /**
         * @brief Normalize position vector
         *        returns old length.
         * @return Old vector length
         */
        double normalize ()
        {
            bool non_origin = *this;
            if (!non_origin)
            {
                return 0.0;
            }
            double len = length();
            double invlen = 1 / len;
            *this *= invlen;
            return len;
        }

    public:     // Binary operators

        /**
         * @brief Test if two vectors are equal.
         * @param v [in]; vector to compare against
         * @return true if equal, false otherwise
         */
        bool operator== (const Vector3& v) const
        {
            return (double_equal(_data[0], v._data[0]) &&
                    double_equal(_data[1], v._data[1]) &&
                    double_equal(_data[2], v._data[2]));
        }

        /**
         * @brief Test if two vectors are inequal.
         * @param v [in]; vector to compare against
         * @return true if not equal, false otherwise
         */
        bool operator!= (const Vector3& v) const
        {
            return !(*this == v);
        }

        /**
         * @brief Return bool indicating which vector lexicographically xyz smaller.
         * @param other [in]; vector to compare against
         * @return true if other is greater than *this
         */
        bool operator< (const Vector3& other) const
        {
            
            if(_data[0]<other[0])
            {
                return true;
            }
            else if((double_equal(_data[0], other[0])) && _data[1]<other[1])
            {
                return true;
            }
            else if((double_equal(_data[0], other[0])) && 
                    (double_equal(_data[1], other[1])) && _data[2]<other[2])
            {
                return true;
            }
            return false;
        }

    public:     // Arithmetic operators

        /**
         * @brief Return the sum of two vectors.
         * @param v [in]; vector to sum with *this
         * @return Vector3 that is the sum of *this and v
         */
        Vector3 operator+ (const Vector3& v) const
        {
            Vector3 r = *this;
            return r += v;
        }

        /**
         * @brief Return the difference of two vectors.
         * @param v [in]; vector to subtract from *this
         * @return Vector3 that is the difference of *this and v
         */
        Vector3 operator- (const Vector3& v) const
        {
            Vector3 r = *this;
            return r -= v;
        }

        /**
         * @brief Negative Vector.
         * @return Vector3 that is *this * -1
         */
        Vector3 operator- () const
        {
            Vector3 r = *this;
            return r *= (-1);
        }

        /**
         * @brief Return the scaling of a vector.
         * @param scale [in]; factor to scale a vector by
         * @return Vector3 that is *this * scale
         */
        Vector3 operator* (const double scale) const
        {
            Vector3 r = *this;
            return r *= scale;
        }

    public: // Vector operators

        /**
         * @brief Dot product.
         * @param other [in]; right hand side of dot product
         * @return value of dot product.
         **/
        double dot (const Vector3& other) const
        {
            const Vector3& self = *this;
            return (self._data[0]*other._data[0] + 
                    self._data[1]*other._data[1] + 
                    self._data[2]*other._data[2]);
        }

        /**
         * @brief Cross product.
         * @param other [in]; right hand side of cross product
         * @return value of cross product.
         **/
        Vector3 cross (const Vector3& other) const
        {
            const Vector3& self = *this;
            Vector3 r (self._data[1]*other._data[2] 
                       - self._data[2]*other._data[1],
                       self._data[2]*other._data[0] 
                       - self._data[0]*other._data[2],
                       self._data[0]*other._data[1] 
                       - self._data[1]*other._data[0]);
            
            return r;
        }

    public: // Conversion to EulerAngles

        /**
         * @brief Convert position vector to yaw/pitch that define that
         * direction.  Roll angle will be 0.
         */
        operator EulerAngles const ();

    public: // Output

        /**
         * @brief Print the contents of a Vector to the specified stream.
         * @param out [in]; stream to write to
         * @param vec [in]; vector to write
         * @return reference to passed in ostream
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::Vector3 & vec)
        {
            return (out << "( " << vec[0] << ", " << vec[1] << ", " << vec[2]
                    << " )");
        }

    protected: // Implementation

        /// Friend class declaration
        friend class Vector3Field;

        /**
         * @brief The actual x/y/z coordinates of the vector.
         **/
        double _data[3]; // x/y/z coordinates.
    }; // end class Vector3

    // External operators.
    /**
     * @brief Return the scaling of a vector.
     * @param scale [in]; scaling factor
     * @param v [in]; Vector3 to multiply with
     * @return true if equal, false otherwise
     */
    inline Vector3 EVOLUTION_EXPORT_MATH operator* (const double scale, 
                                                    const Vector3& v)
    {
        return (v * scale);
    }

    /**
     * @brief Return the dot product of two vectors.
     * @param a [in]; left hand vector
     * @param b [in]; right hand vector
     * @return double dot product of a and b
     */
    inline double
    EVOLUTION_EXPORT_MATH dot (const Vector3& a, const Vector3& b)
    {
        return a.dot (b);
    }

    /**
     * @brief Return the cross product of two vectors.
     * @param a [in]; left hand vector
     * @param b [in]; right hand vector
     * @return Vector3 cross product of a and b
     */
    inline Vector3
    EVOLUTION_EXPORT_MATH cross (const Vector3& a, const Vector3& b)
    {
        return a.cross (b);
    }

} // end namespace Evolution.

#endif  // INCLUDE_EVOLUTION_MATH_VECTOR3_HPP
