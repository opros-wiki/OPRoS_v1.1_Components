//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE_MOTIONCONSTANTS_HPP
#define INCLUDE_EVOLUTION_RESOURCE_MOTIONCONSTANTS_HPP


/**
 * @file    evolution/core/resource/MotionConstants.hpp
 * @brief   Default constants for resources.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


#include <evolution/core/base/Config.hpp>
#include <evolution/core/base/Export.hpp>


namespace Evolution
{

    // Linear velocities.
    /** 
     * The default linear velocity.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double DEFAULT_LINEAR_VELOCITY;     
    /** 
     * The maximum linear velocity.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double MAX_LINEAR_VELOCITY;   
    /** 
     * The step of a linear velocity interval.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double DEFAULT_LINEAR_VELOCITY_STEP;        
     

    // Angular velocities.
    /** 
     * The default angular velocity.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double DEFAULT_ANGULAR_VELOCITY;
    /** 
     * The maximum angular velocity.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double MAX_ANGULAR_VELOCITY;
    /** 
     * The step of an angular velocity interval.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double DEFAULT_ANGULAR_VELOCITY_STEP;


    // Linear accelerations.
    /** 
     * The default linear acceleration.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double DEFAULT_LINEAR_ACCELERATION;  
    /** 
     * The maximum linear acceleration.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double MAX_LINEAR_ACCELERATION;  


    // Angular accelerations.
    /** 
     * The default angular acceleration.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double DEFAULT_ANGULAR_ACCELERATION;  
    /** 
     * The maximum angular acceleration.
     *
     * @ingroup resource
     */
    EVOLUTION_EXPORT_RESOURCE extern const double MAX_ANGULAR_ACCELERATION;  

} // End namespace Evolution.


#endif  // INCLUDE_EVOLUTION_RESOURCE_MOTIONCONSTANTS_HPP
