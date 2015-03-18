//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_KINEMATICS_HPP
#define INCLUDE_EVOLUTION_MATH_KINEMATICS_HPP


/**
 * @file    evolution/core/math/Kinematics.hpp
 * @brief   Kinematics-related data structures and methods.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 *
 * Header file for math routines that are useful for robotic localization and kinematics.
 *
 */


#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/Matrix3.hpp>


namespace Evolution 
{

    /**
     * @struct DHParams
     * @brief Encapsulates Denavit-Hartenberg parameters.
     * 
     * Denavit-Hartenberg parameters, are a concise way of representing
     * the transformation between two joints of a robotic manipulator.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    struct EVOLUTION_EXPORT_MATH DHParams 
    {
        /**
         * @brief  Rotation around the axis to the next "a".
         */
        double theta;

        /**
         * @brief  Distance along the axis to the next "a".
         */
        double d;

        /**
         * @brief  Shortest distance between the two axes.
         */
        double a;

        /**
         * @brief  Angle between axes of the two joints.
         */
        double alpha;
    };

    /**
     * @brief
     * Translates Denavit-Hartenberg parameters into a transformation
     *
     * @param p [in]; The DH parameter to transform.
     * @param trans [out]; The resulting transformation matrix.
     * @param offset [out]; The resulting offset vector.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    extern EVOLUTION_EXPORT_MATH void dh_to_trans(const DHParams& p, Matrix3* trans, Vector3 *offset);

} // end namespace Evolution.

#endif  // INCLUDE_EVOLUTION_MATH_KINEMATICS_HPP
