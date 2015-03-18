//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_NAVIGATION_POSECOV2DT_HPP
#define INCLUDE_EVOLUTION_NAVIGATION_POSECOV2DT_HPP 

/**
 * @file   evolution/core/math/PoseCov2DT.hpp
 * @brief  Storage for a 2D pose with covariance matrix and a timestamp.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#include <evolution/core/math/PoseCov2D.hpp>
#include <evolution/core/math/Pose2DT.hpp>

namespace Evolution
{

    /**
     * @class PoseCov2DT
     *
     * @brief Storage for a 2D pose with covariance matrix and a
     * timestamp.  Needed by components of VSLAM, odometry, and
     * others.
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH PoseCov2DT: public PoseCov2D
    {
    public: // Structors

        /**
         * @brief Default constructor, sets everything to 0.
         */
        PoseCov2DT(): PoseCov2D(), timestamp( 0 ) {}

        /**
         * @brief Conversion constructor.
         *
         * @param pose_in [in]; Pose2D to convert from.
         * @param time_in [in, opt]; Initial timestamp, default = 0.
         */
        PoseCov2DT( const PoseCov2D & pose_in, Timestamp time_in = 0 ):
            PoseCov2D( pose_in ),
            timestamp( time_in ) {}

        /** @brief Copy contents of source pose into new PoseCov2D and leave
         * covariance with zeros.*/
        PoseCov2DT( const Pose2DT & source ): PoseCov2D( source ), timestamp( source.timestamp ) {}

    public:
        /**
         * @brief Treat the poses like homogeneous coordinate
         * transform matrices, and return a PoseCov2D of the product.
         *
         * This acts like a multiplication of homogeneous coordinate
         * transform matrices for planar position and rotation.  The
         * corresponding composition operation is performed on the
         * covariance matrix as well.
         *
         * Example:
         * PoseCov2D robot_rel_world = landmark_rel_world * robot_rel_landmark;
         *
         * Where robot_rel_landmark (for instance) stores the position
         * of the robot relative to a landmark's coordinate frame.
         *
         * This acts exactly the same as Pose2D::operator*(), with the
         * addition of the covariance matrix composition.
         *
         * The timestamp in the result is the maximum of the
         * timestamps of the multiplicands.
         */
        PoseCov2DT operator*( const PoseCov2DT& pose ) const;

        /**
         * @brief Treat the poses like homogeneous coordinate
         * transform matrices, and return a PoseCov2DT of the product.
         *
         * This is the same as the other operator*() functions here,
         * but it preserves the timestamp of the PoseCov2DT operand.
         */
        PoseCov2DT operator*( const PoseCov2D& pose ) const;

    public: // Data

        Timestamp timestamp; ///< The time stamp

    }; // end class PoseCov2DT

    /**
     * @brief Treat the poses like homogeneous coordinate
     * transform matrices, and return a PoseCov2DT of the product.
     *
     * This is the same as the other operator*() functions here,
     * but it preserves the timestamp of the PoseCov2DT argument.
     */
    PoseCov2DT EVOLUTION_EXPORT_MATH operator*( const PoseCov2D& a, const PoseCov2DT& b );

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_NAVIGATION_POSECOV2DT_HPP
