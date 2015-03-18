//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_NAVIGATION_POSE2DT_HPP
#define INCLUDE_EVOLUTION_NAVIGATION_POSE2DT_HPP 

/**
 * @file   evolution/core/math/Pose2DT.hpp
 * @brief  Storage for a 2D pose with a timestamp.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#include <evolution/core/math/Pose2D.hpp>

namespace Evolution
{

    /**
     * @class Pose2DT
     *
     * @brief Storage for a 2D pose with a timestamp.  Needed by many
     * components of VSLAM, odometry, and others.
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH Pose2DT: public Pose2D
    {
    public: // Structors

        /**
         * @brief Constructor
         *
         * @param a_x [in, opt]; Optional. Initial position in x, default = 0
         * @param a_y [in, opt]; Optional. Initial position in y, default = 0
         * @param a_theta [in, opt]; Optional. Initial direction, default = 0
         * @param a_timestamp [in, opt]; Optional. Initial timestamp, default = 0
         */
        Pose2DT( double a_x = 0,
                 double a_y = 0,
                 double a_theta = 0,
                 Timestamp a_timestamp = 0):
            Pose2D( a_x, a_y, a_theta ),
            timestamp( a_timestamp ) {}

        /**
         * @brief Conversion constructor.
         *
         * @param pose_in [in]; Pose2D to convert from.
         * @param time_in [in, opt]; Initial timestamp, default = 0.
         */
        Pose2DT( const Pose2D & pose_in, Timestamp time_in = 0 ):
            Pose2D( pose_in ),
            timestamp( time_in ) {}

        /**
         * @brief Constructor
         *
         * @param array [in]; 3 or 4 length DoubleArray: (x, y, theta [, timestamp]).
         */
        Pose2DT( const DoubleArray & array )
        {
            assert(array.size() >= 3);
            x = array[0];
            y = array[1];
            theta = array[2];
            if(array.size() >= 4)
            {
                timestamp = (Timestamp) array[3];
            }
        }

        /**
        * @brief Copy Constructor
        *
        * @param source [in]; Pose to copy.
        */
        Pose2DT( const Pose2DT & source ):
            Pose2D(source.x, source.y, source.theta),
            timestamp (source.timestamp) {}

        /** @brief Return a DoubleArray of x, y, theta, timestamp. */
        DoubleArray double_array() const
        {
            DoubleArray result;
            result.push_back(x);
            result.push_back(y);
            result.push_back(theta);
            result.push_back(timestamp);
            return result;
        }

    public: // Data

        Timestamp timestamp; ///< The time stamp

    }; // end class Pose2DT

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_NAVIGATION_POSE2DT_HPP
