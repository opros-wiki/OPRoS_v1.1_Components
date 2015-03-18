//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_FUZZY_MEMBERSHIP_HPP
#define INCLUDE_EVOLUTION_FUZZY_MEMBERSHIP_HPP


/**
 * @file    evolution/core/math/FuzzyMembership.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @brief   Basic fuzzy membership set testing.
 * 
 * Functions for basic fuzzy membership sets.
 *
 * @see  The fuzzy library README.
 */


#include <iostream>
#include <evolution/core/math/FuzzyConstants.hpp>


namespace Evolution
{

    ////////////////////////////////////////////////////////////
    // Basic membership sets
    ////////////////////////////////////////////////////////////

    /**
     * @brief Fuzzy triangular set centered around t with the specified
     * width.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; location of triangle.
     * @param width [in]; width of triangle.
     * @return value of the t,width triangle function with x as input.
     * @ingroup core_math
     */
    inline double fuzzy_triangular (double x, double t, double width)
    {
        if (x >= (t + width))
            return 0.0;
        else if (x <= (t - width))
            return 0.0;
        else if (x > t)
            return (((t - x) / width) + 1);
        else if (x < t)
            return (((x - t) / width) + 1);
        else
            return (1.0);
    } // end fuzzy_triangular()

    /**
     * @brief Fuzzy high set starting at t with the specified slope width.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param width [in]; width of function slopes.
     * @return value of fuzzy_high (t,width) set at value x
     *
     * @ingroup core_math
     */
    inline double fuzzy_high (double x, double t, double width)
    {
        if (x >= (t + width))
            return 1.0;
        else if (x <= t)
            return 0.0;
        else
            return ((x - t) / width);
    } // end fuzzy_high()

    /**
     * @brief Fuzzy medium set centered around t with the specified width.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param width [in]; width of function slopes.
     * @return value of med. (t,width) set at value x
     *
     * @ingroup core_math
     */
    inline double fuzzy_medium (double x, double t, double width)
    {
        return fuzzy_triangular (x, t, width);
    } // end fuzzy_medium()

    /**
     * @brief Fuzzy low set starting at t with the specified slope width.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param width [in]; width of function slopes.
     * @return value of low (t,width) set at value x
     *
     * @ingroup core_math
     */
    inline double fuzzy_low (double x, double t, double width)
    {
        if (x <= (t - width))
            return 1.0;
        else if (x >= t)
            return 0.0;
        else
            return ((t - x) / width);
    } // end fuzzy_low()

    ////////////////////////////////////////////////////////////
    // Distance sets
    ////////////////////////////////////////////////////////////

    /** 
     * @brief  High distance.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_high_distance (double x, double t =
                         FUZZY_HIGH_DISTANCE_T, double w =
                         FUZZY_HIGH_DISTANCE_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_high_distance()

    /** 
     * @brief  Medium distance.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_med (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_medium_distance (double x, double t =
                           FUZZY_MEDIUM_DISTANCE_T, double w =
                           FUZZY_MEDIUM_DISTANCE_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_medium_distance()

    /** 
     * @brief  Low distance.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_low_distance (double x, double t =
                        FUZZY_LOW_DISTANCE_T, double w =
                        FUZZY_LOW_DISTANCE_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_low_distance()

    /** 
     * @brief  Low distance.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_very_low_distance (double x, double t =
                             FUZZY_VERY_LOW_DISTANCE_T, double w =
                             FUZZY_VERY_LOW_DISTANCE_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_low_distance()

    ////////////////////////////////////////////////////////////
    // Target location
    ////////////////////////////////////////////////////////////

    /** 
     * @brief  Target at right.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     */
    inline double
    fuzzy_target_right (double x, double t =
                        FUZZY_TARGET_RIGHT_T, double w =
                        FUZZY_TARGET_RIGHT_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_target_right()

    /** 
     * @brief  Target in front.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     */
    inline double
    fuzzy_target_front (double x, double t =
                        FUZZY_TARGET_FRONT_T, double w =
                        FUZZY_TARGET_FRONT_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_target_front()

    /** 
     * @brief  Target at left.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     */
    inline double
    fuzzy_target_left (double x, double t =
                       FUZZY_TARGET_LEFT_T, double w =
                       FUZZY_TARGET_LEFT_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_target_left()

    ////////////////////////////////////////////////////////////
    // Speed
    ////////////////////////////////////////////////////////////

    /** 
     * @brief  Very high speed.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_very_high_speed (double x, double t =
                           FUZZY_VERY_HIGH_SPEED_T, double w =
                           FUZZY_VERY_HIGH_SPEED_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_very_high_speed()

    /** 
     * @brief  High speed.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_high_speed (double x, double t = FUZZY_HIGH_SPEED_T, double w =
                      FUZZY_HIGH_SPEED_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_high_speed()

    /** 
     * @brief  Medium speed.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_medium_speed (double x, double t =
                        FUZZY_MEDIUM_SPEED_T, double w =
                        FUZZY_MEDIUM_SPEED_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_medium_speed()

    /** 
     * @brief  Low speed.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_low_speed (double x, double t = FUZZY_LOW_SPEED_T, double w =
                     FUZZY_LOW_SPEED_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_low_speed()

    /** 
     * @brief  Very Low speed.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_very_low_speed (double x, double t =
                          FUZZY_VERY_LOW_SPEED_T, double w =
                          FUZZY_VERY_LOW_SPEED_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_low_speed()

    /** 
     * @brief  Reverse speed.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_reverse_speed (double x, double t =
                         FUZZY_REVERSE_SPEED_T, double w =
                         FUZZY_REVERSE_SPEED_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_reverse_speed()

    ////////////////////////////////////////////////////////////
    // Steering
    ////////////////////////////////////////////////////////////

    /** 
     * @brief  Sharp left turn.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_sharp_left (double x, double t = FUZZY_SHARP_LEFT_T, double w =
                      FUZZY_SHARP_LEFT_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_sharp_left()

    /** 
     * @brief  Left turn. 
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_left (double x, double t = FUZZY_LEFT_T, double w =
                FUZZY_LEFT_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_left()

    /** 
     * @brief  Soft left turn. 
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_soft_left (double x, double t = FUZZY_SOFT_LEFT_T, double w =
                     FUZZY_SOFT_LEFT_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_soft_left()

    /** 
     * @brief  Steer ahead. 
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_forward (double x, double t = FUZZY_FORWARD_T, double w =
                   FUZZY_FORWARD_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_forward()

    /** 
     * @brief  Soft right turn. 
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_soft_right (double x, double t = FUZZY_SOFT_RIGHT_T, double w =
                      FUZZY_SOFT_RIGHT_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_soft_right()

    /** 
     * @brief  Right turn. 
     *
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     * @ingroup core_math
     */
    inline double
    fuzzy_right (double x, double t = FUZZY_RIGHT_T, double w =
                 FUZZY_RIGHT_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_right()

    /** 
     * @brief  Sharp right turn.  
     *
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     * @ingroup core_math
     */
    inline double
    fuzzy_sharp_right (double x, double t =
                       FUZZY_SHARP_RIGHT_T, double w =
                       FUZZY_SHARP_RIGHT_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_sharp_right()

    ////////////////////////////////////////////////////////////
    // Target size
    ////////////////////////////////////////////////////////////

    /**     
     * @brief  Very large target.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_very_large (double x, double t = FUZZY_VERY_LARGE_T, double w =
                      FUZZY_VERY_LARGE_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_very_large()

    /** 
     * @brief  Large target.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_large (double x, double t = FUZZY_LARGE_T, double w =
                 FUZZY_LARGE_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_large()

    /** 
     * @brief  Medium-sized target.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_medium_size (double x, double t = FUZZY_MEDIUM_T, double w =
                       FUZZY_MEDIUM_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_medium_size()

    /** 
     * @brief  Small target.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_medium (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_small (double x, double t = FUZZY_SMALL_T, double w =
                 FUZZY_SMALL_W)
    {
        return fuzzy_medium (x, t, w);
    } // end fuzzy_small()

    /** 
     * @brief  Very small target.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_very_small (double x, double t = FUZZY_VERY_SMALL_T, double w =
                      FUZZY_VERY_SMALL_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_very_small()


    ////////////////////////////////////////////////////////////
    // Camera angle
    ////////////////////////////////////////////////////////////

    /** 
     * @brief  Camera left.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_low (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_camera_left (double x, double t =
                       FUZZY_CAMERA_LEFT_T, double w =
                       FUZZY_CAMERA_LEFT_W)
    {
        return fuzzy_low (x, t, w);
    } // end fuzzy_camera_left()


    /** 
     * @brief  Camera right.
     * @param x [in]; data point to evaluate at.
     * @param t [in]; "center" point of function.
     * @param w [in]; width of function slopes.
     * @return value of fuzzy_high (t,w) set at value x
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_camera_right (double x, double t =
                        FUZZY_CAMERA_RIGHT_T, double w =
                        FUZZY_CAMERA_RIGHT_W)
    {
        return fuzzy_high (x, t, w);
    } // end fuzzy_camera_right()

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_FUZZY_MEMBERSHIP_HPP
