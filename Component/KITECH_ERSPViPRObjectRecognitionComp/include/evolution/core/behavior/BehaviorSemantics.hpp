//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BEHAVIOR_SEMANTICS_HPP
#define INCLUDE_EVOLUTION_CORE_BEHAVIOR_SEMANTICS_HPP


/**
 * @file    evolution/core/behavior/BehaviorSemantics.hpp
 * @brief   Values for well-known semantic types that are used in behavior data.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/


namespace Evolution
{


    /**
     * The type of action command, for the Evolution.Action semantic type.
     * @ingroup core_behavior
     */
    enum BehaviorActionCommand
    {
        BEHAVIOR_ACTION_RUN,
        BEHAVIOR_ACTION_STOP,
        BEHAVIOR_ACTION_PAUSE,
        BEHAVIOR_ACTION_RESUME
    };

    /**
     * The type of stop to perform, for the Evolution.StopMovement semantic type.
     * @ingroup core_behavior
     */
    enum BehaviorStopMovement
    {
        BEHAVIOR_STOP_OFF,
        BEHAVIOR_STOP_ABRUPT,
        BEHAVIOR_STOP_SMOOTH,
        BEHAVIOR_NO_STOP
    };

    /**
     * Indices of an Evolution.FuzzyObstacleSet semantic type. 
     * @ingroup core_behavior
     */
    enum FuzzyObstacleSet
    {
        FUZZY_OBSTACLE_FRONT = 0,
        FUZZY_OBSTACLE_LEFT = 1,
        FUZZY_OBSTACLE_BACK = 2,
        FUZZY_OBSTACLE_RIGHT = 3,
        FUZZY_OBSTACLE_SIZE = 4
    };

    /**
     * Indices of an Evolution.FlowFieldData semantic type. 
     * @ingroup core_behavior
     */
    enum FlowFieldData
    {
        BLOCK_C = 0,
        BLOCK_R = 1,
        DESAMPLE_C = 2,
        DESAMPLE_R = 3
    };

    /**
     * Indices of an Evolution.BumperTriggeredSet semantic type.
     * @ingroup core_behavior
     */
    enum BumperTriggeredSet
    {
        BUMPER_TRIGGERED_FRONT = 0,
        BUMPER_TRIGGERED_LEFT = 1,
        BUMPER_TRIGGERED_BACK = 2,
        BUMPER_TRIGGERED_RIGHT = 3,
        BUMPER_TRIGGERED_SIZE = 4
    };

    /**
     * Indices of an Evolution.HazardSet semantic type. 
     * @ingroup core_behavior
     */
    enum HazardSet
    {
        HAZARD_SET_FRONT = 0,
        HAZARD_SET_LEFT = 1,
        HAZARD_SET_BACK = 2,
        HAZARD_SET_RIGHT = 3,
        HAZARD_SET_SIZE = 4
    };

    /**
     * Indices of an Evolution.DepthSet semantic type. 
     * @ingroup core_behavior
     */
    enum DepthSet
    {
        DEPTH_SET_FRONT = 0,
        DEPTH_SET_LEFT = 1,
        DEPTH_SET_BACK = 2,
        DEPTH_SET_RIGHT = 3,
        DEPTH_SET_SIZE = 4
    };

    /**
     * @brief
     * Method of avoidance to use for the Evolution.AvoidanceMethod
     * semantic type.
     *
     * The enumeration values are chosen so that the avoidance method
     * either is NO_AVOIDANCE or is a set of ORed avoidance bit flags,
     * with a value of zero meaning use default avoidance. In this
     * way, two method sets may be ANDed to find an intersection, but
     * if the intersection is empty, it still falls back to
     * DEFAULT_AVOIDANCE.
     * @ingroup core_behavior
     **/
    enum AvoidanceMethod
    {
        NO_AVOIDANCE                = 0,  ///< Run into things.
        LINEAR_AVOIDANCE_ONLY       = 1,  ///< Never turn in response to obstacles, just slow or stop.
        ANGULAR_AVOIDANCE_ONLY      = 2,  ///< Never slow down in response to obstacles, just turn to avoid them
        FUZZY_AVOIDANCE             = 3,  ///< Standard fuzzy avoidance behavior

        RCM_NO_AVOIDANCE            = 4,  ///< Run into things.  But don't since we'll rely on the RCM to do avoidance
        RCM_LINEAR_AVOIDANCE_ONLY   = 5,  ///< Never turn in response to obstacles, just slow or stop.  Use RCM.
        RCM_ANGULAR_AVOIDANCE_ONLY  = 6,  ///< Never slow down in response to obstacles, just turn to avoid them. Use RCM.
        RCM_FUZZY_AVOIDANCE         = 7   ///< Standard fuzzy avoidance behavior. Also use RCM.
    };

} // end namespace Evolution


#endif                          // INCLUDE_EVOLUTION_CORE_BEHAVIOR_SEMANTICS_HPP
