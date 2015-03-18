//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_EULER_ANGLES_HPP
#define INCLUDE_EVOLUTION_MATH_EULER_ANGLES_HPP


/**
 * @file    evolution/core/math/EulerAngles.hpp
 * @brief   Implementation of an array of ZYX Euler angles.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::EulerAngles
 */

#include <math.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/math/MathUtils.hpp>

namespace Evolution
{

    // Announces the existence of class to avoid recursive include problem.
    class Vector3;
    class Matrix3;

    /**
     * @class EulerAngles
     * @brief Implementation of an array of ZYX Euler angles.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    class EVOLUTION_EXPORT_MATH EulerAngles
    {
    public:     // Types
        /**
         * @brief Enumeration used to index into EulerAngles.
         */
        typedef enum {
            YAW     = 0,   // Positive yaw turns from x towards y.
            PITCH   = 1,   // Positive pitch turns from z axis towards x.
            ROLL    = 2    // Positive roll turns from y towards z.
        } AngleType;

    public:     // Structors
        /**
         * @brief Constructs an angle from 3 points.
         * @param yaw [in]; yaw angle
         * @param pitch [in]; pitch angle
         * @param roll [in]; roll angle
         */
        EulerAngles (const double yaw = 0, const double pitch = 0, 
                     const double roll = 0)
        {
            set (yaw, pitch, roll);
        }

        /**
         * @brief Constructs an angle from an array of doubles.
         * @param arr [in]; 3 length double array, [yaw, pitch, roll]
         */
        EulerAngles (const double arr[])
        {
            set (arr[0], arr[1], arr[2]);
        }

        /**
         * @brief Constructs an angle from another angle. 
         * @param a [in]; EulerAngles to copy.
         */
        EulerAngles (const EulerAngles& a)
        {
            _data[0] = a._data[0];
            _data[1] = a._data[1];
            _data[2] = a._data[2];
        }

        /**
         * @brief Computes the ZYX EulerAngles from a 3x3 homogeneous
         * rotation matrix.
         * @param rotation [in]; rotation to generate Euler angles from.
         */
        EulerAngles (const Matrix3& rotation)
        {
            set(rotation);
        }

    public:     // Information access
        /**
         * @brief Tests if the angle exists and it is valid.
         * @return true if angle exists and is valid. 
         */
        operator bool () const 
        {
            // Even the 0/0/0 angle is a "valid" angle, so always return true.
            return true;
        }

        /**
         * @brief Accesses an indexed vector point.
         * @param index [in]; index of angle to get.
         * @return value of indexed point.
         */
        double& operator[] (int index)
        {             
            ASSERT (index >= 0 && index < 3);
            return _data[index]; 
        }

        /**
         * @brief Constant access to an indexed vector point.
         * @param index [in]; index of angle to get.
         * @return value of indexed point.
         */
        const double& operator[] (int index) const 
        {             
            ASSERT (index >= 0 && index < 3);
            return _data[index]; 
        }

        /**
         * @brief Accesses an indexed vector point.
         * @param index [in]; index of angle to get.
         * @return value of indexed point.
         */
        double& operator[] (unsigned int index) 
        {             
            ASSERT (index < 3);
            return _data[index]; 
        }

        /**
         * @brief Constant access to an indexed vector point.
         * @param index [in]; index of angle to get.
         * @return value of indexed point.
         */
        const double& operator[] (unsigned int index) const 
        { 
            ASSERT (index < 3);
            return _data[index]; 
        }
        
    public:     // Modifications

        /**
         * @brief Manually set an angle array's contents.
         * @param yaw [in]; yaw angle
         * @param pitch [in]; pitch angle
         * @param roll [in]; roll angle
         */
        void set (const double yaw, const double pitch, const double roll)
        {
            _data[0] = yaw;
            _data[1] = pitch;
            _data[2] = roll;
        }

        /**
         * @brief Manually compute the EulerAngles that correspond to
         * a given 3x3 homogeneous rotation matrix.  The result is a
         * ZYX Euler Angle parameterization of the matrix (Fick
         * angles).
         * 
         * Note that it is not implemented here to avoid the recursion
         * problem with Matrix3.  See Matrix3 for the inverse
         * conversion.
         * @param rotation [in]; rotation matrix to convert to Euler angles
         * @return reference to this
         */
        EulerAngles& set (const Matrix3& rotation);

        /**
         * @brief Clears all the elements.
         */
        void clear ()
        {
            set(0, 0, 0);
        }

        /**
         * @brief Assigns one vector to another angle.
         * @param a [in]; EulerAngles to copy.
         * @return reference to this
         */
        EulerAngles& operator= (const EulerAngles &a)
        {
            _data[0] = a._data[0];
            _data[1] = a._data[1];
            _data[2] = a._data[2];
            
            return *this;
        }

        /**
         * @brief Assigns an array of doubles to a angle.
         * @param arr [in]; 3 length double array that contains Euler angles.
         * @return reference to this
         */
        EulerAngles& operator= (const double arr[])
        {
            _data[0] = arr[0];
            _data[1] = arr[1];
            _data[2] = arr[2];
            
            return *this;
        }

        /**
         * @brief Bound angle values in the range [-pi, +pi].
         * @return reference to this
         */
        EulerAngles& bound_angles()
        {
            _data[YAW  ] = angle_bound(_data[YAW  ]);
            _data[PITCH] = angle_bound(_data[PITCH]);
            _data[ROLL ] = angle_bound(_data[ROLL ]);
            
            return *this;
        }

        /**
         * @brief Return the scaling of an angle
         *        (used to convert degrees to radians).
         * @param scale [in]; value to scale Euler angle by.
         * @return reference to this
         */
        EulerAngles& operator*= (const double scale)
        {
            _data[0] *= scale;
            _data[1] *= scale;
            _data[2] *= scale;
            
            return *this;
        }
        /**
         * @brief Return the scaling of an angle
         *        (used to convert degrees to radians).
         * @param scale [in]; value to scale Euler angle by.
         * @return New EulerAngles object with result of scaling.
         */
        EulerAngles operator* (const double scale) const
        {
            EulerAngles r = *this;
            return r *= scale;
        }

    public:    // Binary operators

        /**
         * @brief Test if two vectors are equal.
         * @param a [in]; EulerAngles to compare against
         * @return true angles are equal
         */
        bool operator== (const EulerAngles& a) const
        {
            return (angle_equal(_data[0], a._data[0]) &&
                    angle_equal(_data[1], a._data[1]) &&
                    angle_equal(_data[2], a._data[2]));
        }

        /**
         * @brief Test if two vectors are unequal.
         * @param a [in]; EulerAngles to compare against
         * @return true angles are not equal
         */
        bool operator!= (const EulerAngles& a) const
        {
            return !(*this == a);
        }

    public: // Conversion to Vector3

        /**
         * @brief Convert angles to the direction vector the angles encode.
         * @return Vector3 encoding direction vector.
         */
        operator Vector3 const ();

        /**
         * @brief Convert angles to three orthogonal axes.
         * @param forward [in]; projection onto the forward (x) axis
         * @param left [in]; projection onto the left (y) axis
         * @param up [in]; projection onto the up (z) axis
         */
        void to_axis (Vector3 *forward, Vector3 *left, Vector3 *up) const;

    public: // Output

        /**
         * @brief Print a the contents of a Vector to the specified stream.
         * @param out [in]; stream to write to.
         * @param alpha [in]; Euler angles to write to stream out.
         * @return reference to passed in ostream
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::EulerAngles & alpha)
        {
            out << "( " << alpha[0] << ", " << alpha[1] << ", " << alpha[2]
                << " )";
            return out << "\n";
        }

    protected: // Implementation

        /**
         * @brief The actual angles of the angle array.
         **/
        double _data[3]; ///< Actual EulerAngles stored by object.
    };

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_EULER_ANGLES_HPP
