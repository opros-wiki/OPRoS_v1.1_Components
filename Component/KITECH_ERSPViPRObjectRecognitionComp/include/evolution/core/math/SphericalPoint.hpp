//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_SPHERICAL_POINT_HPP
#define INCLUDE_EVOLUTION_MATH_SPHERICAL_POINT_HPP


/**
 * @file    evolution/core/math/SphericalPoint.hpp
 * @brief   A point in spherical coordinate system
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::SphericalPoint
 */


#include <stddef.h>
#include <math.h>
#include <vector>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Vector3.hpp>


namespace Evolution
{

    /**
     * @class SphericalPoint
     * @brief A point in spherical coordinates (usually representing a sensor hit).
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH SphericalPoint
    {
    public:

        /**
         * @brief   Constructor
         * @param in_r [in];     Radius
         * @param in_yaw y[in];  Yaw
         * @param in_pitch [in]; Pitch
         **/
        SphericalPoint(double in_r = 0, double in_yaw = 0, double in_pitch = 0)
        {
            set(in_r, in_yaw, in_pitch);
        }

        /**
         * @brief   Sets the radius, yaw and pitch that define the point.
         * @param in_r [in];     Radius
         * @param in_yaw [in];   Yaw
         * @param in_pitch [in]; Pitch
         * 
         **/
        void set(double in_r = 0, double in_yaw = 0, double in_pitch = 0)
        {
            r     = in_r;
            yaw   = in_yaw;
            pitch = in_pitch;

            // Precomputed sine/cosine values
            sy = sin(yaw);
            cy = cos(yaw);
            sp = sin(pitch);
            cp = cos(pitch);
        }

        /**
         * @brief   Sets the radius.
         * @param in_r [in];  Radius
         **/
        void set_r(double in_r) { r = in_r; }

        /**
         * @brief   Gets the radius.
         * @return  Radius
         **/
        double get_r() const { return r; }

        /**
         * @brief   Gets the yaw.
         * @return  Yaw
         **/
        double get_yaw() const { return yaw; }

        /**
         * @brief   Gets the pitch.
         * @return  Pitch
         **/
        double get_pitch() const { return pitch; }

        /**
         * @brief Cast from Spherical to Cartesian coordinates local to the robot.
         * @return Cartesian coordinates of the point
         *
         * @see Vector3
         **/
        operator Vector3 const ()
        {
            // For position vectors, x is forward, y is left, z is up
            // Note that this is the first column of the transformation matrix
            Vector3 dir(r * cy*cp, r * sy*cp, r * -sp);

            return dir;
        }

    protected:

        double r;           ///< Radius
        double yaw;         ///< Yaw
        double pitch;       ///< Pitch, (set to 0 for polar coordinates)
        double sy;          ///< Sine of Yaw
        double cy;          ///< Cosine of Yaw
        double sp;          ///< Sine of Pitch
        double cp;          ///< Cosine of Pitch

    }; // end class SphericalPoint

    /**
     * @brief Internal type that holds a vector of SphericalPoint's
     */
    typedef std::vector<SphericalPoint> ScanPoints;

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_SPHERICAL_POINT_HPP
