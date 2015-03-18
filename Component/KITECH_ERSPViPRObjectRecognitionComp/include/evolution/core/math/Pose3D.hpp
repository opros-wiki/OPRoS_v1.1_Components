//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_NAVIGATION_POSE3D_HPP
#define INCLUDE_EVOLUTION_NAVIGATION_POSE3D_HPP 

/**
 * @file   evolution/core/math/Pose3D.hpp
 * @brief  Defines a class for a complete 3 dimensional pose (translation and rotation).
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#include <iostream>

#include <evolution/core/base/Export.hpp>
#include <evolution/core/math/Matrix3.hpp>
#include <evolution/core/math/Vector3.hpp>

namespace Evolution
{

    class Pose2D;

    /**
     * @class Pose3D
     *
     * @brief A class for representing a complete 3 dimensional pose
     * (translation and rotation).
     *
     * @ingroup core_math
     */
    class EVOLUTION_EXPORT_MATH Pose3D
    {
    public: // Structors

        /**
         * @brief Default constructor giving a 0 offset.
         */
        Pose3D()
        {
            _rotation.eye();
        }

        /**
         * @brief Default constructor for pure translation.
         *
         * @param translation [in]; Translation component of pose.
         */
        Pose3D( Vector3 translation ):
            _translation( translation )
        {
            _rotation.eye();
        }

        /**
         * @brief Constructor from Vector3 and Matrix3
         *
         * @param translation [in]; Translation component of pose.
         * @param rotation [in]; Rotation matrix.  Must be a valid
         *                       rotation matrix (inverse == transpose).
         */
        Pose3D( Vector3 translation, Matrix3 rotation ):
            _translation( translation ),
            _rotation( rotation )
        {}

        /**
         * @brief Constructor from Pose2D
         */
        Pose3D( const Pose2D & pose2d );

    public: // pose math

        /**
         * @brief The inv function treats a pose as a coordinate
         * transform, and returns its inverse.
         *
         * @return Inverse transform of current pose.
         */
        Pose3D inv() const
        {
            Pose3D inverse;
            inverse._rotation = _rotation;
            inverse._rotation.transpose();

            inverse._translation = inverse._rotation * -_translation;

            return inverse;
        }

        /**
         * @brief Treat the positions as coordinate transforms, and return
         * the product.
         *
         * Example:
         * Pose3D camera_rel_world = robot_rel_world * camera_rel_robot;
         *
         * Where camera_rel_robot (for instance) stores the position
         * of the camera relative to the robot's coordinate frame.
         *
         * @param p [in]; Pose3D to be multiplied with this Pose3D
         *
         * @return Pose3D product of the multiplication
         */
        Pose3D operator* (const Pose3D& p) const
        {
            Pose3D product;

            product._rotation = _rotation * p._rotation;
            product._translation = _rotation * p._translation + _translation;

            return product;
        }

        /**
         * @brief Transform a 3D point.
         *
         * @param v [in]; Vector3 to be multiplied with this Pose3D
         *
         * @return Vector3 The transformed point.
         */
        Vector3 operator* (const Vector3 & v) const
        {
            return _rotation * v + _translation;
        }

        /** @brief Equality operator.*/
        bool operator== ( const Pose3D & other ) const
        {
            return _translation == other._translation &&
                _rotation == other._rotation;
        }

        double x() const { return _translation[0]; }
        double y() const { return _translation[1]; }
        double z() const { return _translation[2]; }

        friend std::ostream& 
        operator<<( std::ostream& out, const Evolution::Pose3D& pose )
        {
            out << "Translation: " << pose._translation << "\n";
            out << "Rotation : " << pose._rotation;
            return out;
        }
    
    public: // Data

        Vector3 _translation;
        Matrix3 _rotation;
    }; // end class Pose3D

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_NAVIGATION_POSE3D_HPP
