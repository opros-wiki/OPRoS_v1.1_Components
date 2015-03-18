//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_NAVIGATION_POSE2D_HPP
#define INCLUDE_EVOLUTION_NAVIGATION_POSE2D_HPP 

/**
 * @file   evolution/core/math/Pose2D.hpp
 * @brief  Defines a class for a complete 2 dimensional pose (x, y, theta).
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

// #includes here
#include <math.h>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Vector2.hpp>
#include <evolution/core/math/Vector3.hpp>

namespace Evolution
{

    class Pose3D;

    /**
     * @class Pose2D
     *
     * @brief Storage for a complete 2 dimensional pose (x, y, theta)
     *
     * @ingroup core_math
     */
    class EVOLUTION_EXPORT_MATH Pose2D
    {
    public: // Structors

        /**
         * @brief Constructor
         *
         * @param a_x [in, opt]; Optional. Initial position in x, default = 0
         * @param a_y [in, opt]; Optional. Initial position in y, default = 0
         * @param a_theta [in, opt]; Optional. Initial direction, default = 0
         */
        Pose2D (double a_x = 0,
                double a_y = 0,
                double a_theta = 0)
            : x (a_x), y (a_y), theta (a_theta) {}

        /**
         * @brief Copy Constructor
         *
         * @param source [in]; Pose to copy.
         */
        Pose2D( const Pose2D & source ):
            x (source.x), y (source.y), theta (source.theta) {}

        /**
         * @brief Constructor from a DoubleArray of x, y, theta.
         *
         * @param array [in]; 3 or greater length DoubleArray: (x, y, theta).
         */
        Pose2D (const DoubleArray & array)
        {
            assert(array.size() >= 3);
            x = array[0];
            y = array[1];
            theta = array[2];
        }

        /**
         * @brief Converter from Pose3D to 2D.
         */
        Pose2D( const Pose3D& pose3d )
        {
            from_3d( pose3d );
        }

    public: // pose math

        /**
         * @brief The inv function treats a position as a coordinate
         * transform, and returns its inverse.
         *
         * @return Inverse coordinate transform of current position
         */
        virtual Pose2D inv() const
        {
            Pose2D inverse;
            inverse.theta = -theta;
            double s = sin(inverse.theta);
            double c = cos(inverse.theta);
            inverse.x = -(c * x - s * y);
            inverse.y = -(s * x + c * y);
            return inverse;
        }

        /**
         * @brief Treat the poses like homogeneous coordinate
         * transform matrices, and return a Pose2D of the product.
         *
         * This acts like a multiplication of homogeneous coordinate
         * transform matrices for planar position and rotation.
         *
         * Example:
         * Pose2D robot_rel_world = landmark_rel_world * robot_rel_landmark;
         *
         * Where robot_rel_landmark (for instance) stores the position
         * of the robot relative to a landmark's coordinate frame.
         *
         * @param p [in]; Pose2D to be multiplied with this Pose2D
         *
         * @return Pose2D product of the multiplication
         */
        virtual Pose2D operator* (const Pose2D& p) const
        {
            Pose2D product;
            product.theta = angle_bound( theta + p.theta );
            double s = sin(theta);
            double c = cos(theta);
            product.x = c * p.x - s * p.y + x;
            product.y = s * p.x + c * p.y + y;
            return product;
        }

        /**
         * @brief Convert a position to a Vector3, with z =
         * 0. OccupancyGrid uses Vector3s to refer to 2D points.
         */
        virtual Vector3 position_vector() const
        {
            return Vector3(x, y, 0);
        }

        /**
         * @brief Convert a position to a Vector2.
         */
        virtual Vector2 vector2() const
        {
            return Vector2(x, y);
        }

        /** @brief Return a DoubleArray of x, y, theta. */
        virtual DoubleArray double_array() const
        {
            DoubleArray result;
            result.push_back(x);
            result.push_back(y);
            result.push_back(theta);
            return result;
        }

        /** @brief Return the distance between this pose and another. */
        virtual double distance_to(const Pose2D & point) const
        {
            return ersp_hypot( point.x-x, point.y-y );
        }

        /** @brief Return the angle that points towards another pose.
         * This is *not* the difference of the thetas. */
        virtual double angle_to(const Pose2D & point) const
        {
            double dx = point.x - x;
            double dy = point.y - y;
            return atan2(dy, dx);
        }

        /** @brief Fill this Pose2D with the projection onto the plane of a 3D pose.
         *
         * Extracts 2-d pose information by projecting the position
         * and angle onto the plane.  Slope and height can be
         * returned.  They may be used to determine if the pose is
         * really 2D.
         *
         * @param pose3d [in]; The 3D pose to start with.
         * @param slope [out]; Optional slope output.
         * @param height [out]; Optional height output.
         */
        virtual void from_3d( const Pose3D& pose3d, double *slope_out = 0, double *height_out = 0 );

    public: // Data

        double x;                 ///< X coordinate
        double y;                 ///< Y coordinate
        double theta;             ///< Angle of rotation, in radians.
    }; // end class Pose2D

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_NAVIGATION_POSE2D_HPP
