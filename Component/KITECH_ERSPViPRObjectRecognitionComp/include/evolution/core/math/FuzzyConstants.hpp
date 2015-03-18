//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_FUZZY_CONSTANTS_HPP
#define INCLUDE_EVOLUTION_FUZZY_CONSTANTS_HPP


/**
 * @file    evolution/core/math/FuzzyConstants.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 *
 * @brief
 * Common fuzzy constants.
 *
 * These constants are highly dependent on the configuration of the
 * robot or system on which they are used. They therefore should be
 * used as a guideline only, and overridden in your code.
 *
 * @see
 * The fuzzy library README file for an explanation of these
 * constants.  Also refer to the Fuzzy Logic Toolkit section
 * of the User's Guide.
 * @ingroup core_math
 */


#include <math.h>


namespace Evolution
{

    ////////////////////////////////////////////////////////////
    // Distance
    ////////////////////////////////////////////////////////////

    const double FUZZY_HIGH_DISTANCE_T = 40.0; ///< The fuzzy 'high' t value
    const double FUZZY_HIGH_DISTANCE_W = 20.0; ///< The fuzzy 'high' width value
    
    const double FUZZY_MEDIUM_DISTANCE_T = 40.0; ///< The fuzzy 'medium' t value
    const double FUZZY_MEDIUM_DISTANCE_W = 10.0; ///< The fuzzy 'medium' width value

    const double FUZZY_LOW_DISTANCE_T = 40.0; ///< The fuzzy 'low' t value
    const double FUZZY_LOW_DISTANCE_W = 20.0; ///< The fuzzy 'low' width value
 
    const double FUZZY_VERY_LOW_DISTANCE_T = 20.0; ///< The fuzzy 'very_low' t value
    const double FUZZY_VERY_LOW_DISTANCE_W = 8.0;  ///< The fuzzy 'very_low' width value


    ////////////////////////////////////////////////////////////
    // Speed
    ////////////////////////////////////////////////////////////

    const double FUZZY_VERY_HIGH_SPEED_T = 20.0; ///<Fuzzy system's very high speed t
    const double FUZZY_VERY_HIGH_SPEED_W = 10.0; ///<Fuzzy system's very high speed w (width)

    const double FUZZY_HIGH_SPEED_T = 15.0;      ///<Fuzzy system's high speed t
    const double FUZZY_HIGH_SPEED_W = 10.0;      ///<Fuzzy system's high speed w (width)

    const double FUZZY_MEDIUM_SPEED_T = 15.0;    ///<Fuzzy system's medium speed t
    const double FUZZY_MEDIUM_SPEED_W = 10.0;    ///<Fuzzy system's medium speed w (width)
 
    const double FUZZY_LOW_SPEED_T =  20.0;      ///<Fuzzy system's low speed t
    const double FUZZY_LOW_SPEED_W =  20.0;      ///<Fuzzy system's low speed w (width)

    const double FUZZY_VERY_LOW_SPEED_T =  10.0; ///<Fuzzy system's very_low speed t
    const double FUZZY_VERY_LOW_SPEED_W =  10.0; ///<Fuzzy system's very_low speed w (width)

    const double FUZZY_REVERSE_SPEED_T =  -5.0;  ///<Fuzzy system's reverse speed t
    const double FUZZY_REVERSE_SPEED_W =  10.0;  ///<Fuzzy system's reverse speed w (width)



    ////////////////////////////////////////////////////////////
    // Turns
    ////////////////////////////////////////////////////////////

    const double FUZZY_SHARP_LEFT_T = M_PI/18.0; ///< Fuzzy system's sharp left turn t
    const double FUZZY_SHARP_LEFT_W = M_PI/18.0; ///< Fuzzy system's sharp left turn w
    
    const double FUZZY_LEFT_T = 0.0;      ///< Fuzzy system's left turn t
    const double FUZZY_LEFT_W = M_PI/6.0; ///< Fuzzy system's left turn w

    const double FUZZY_SOFT_LEFT_T = M_PI/36.0; ///< Fuzzy system's soft left turn t
    const double FUZZY_SOFT_LEFT_W = M_PI/36.0; ///< Fuzzy system's soft left turn w

    const double FUZZY_FORWARD_T = 0.0;      ///< Fuzzy system's forward turn t
    const double FUZZY_FORWARD_W = M_PI/6.0; ///< Fuzzy system's forward turn w
  
    const double FUZZY_SOFT_RIGHT_T = -FUZZY_SOFT_LEFT_T;   ///< Fuzzy system's soft right turn t
    const double FUZZY_SOFT_RIGHT_W =  FUZZY_SOFT_LEFT_W;   ///< Fuzzy system's soft right turn w

    const double FUZZY_RIGHT_T = -FUZZY_LEFT_T;             ///< Fuzzy system's right turn t
    const double FUZZY_RIGHT_W =  FUZZY_LEFT_W;             ///< Fuzzy system's right turn w
    
    const double FUZZY_SHARP_RIGHT_T = -FUZZY_SHARP_LEFT_T; ///< Fuzzy system's sharp right turn t
    const double FUZZY_SHARP_RIGHT_W =  FUZZY_SHARP_LEFT_W; ///< Fuzzy system's sharp right turn w



    ////////////////////////////////////////////////////////////
    // Target tracking
    ////////////////////////////////////////////////////////////

    const double FUZZY_TARGET_LEFT_T = 0.0;      ///< Fuzzy system's target to the left t
    const double FUZZY_TARGET_LEFT_W = M_PI/6.0; ///< Fuzzy system's target to the left w

    const double FUZZY_TARGET_FRONT_T = 0.0;      ///< Fuzzy system's target to the front t
    const double FUZZY_TARGET_FRONT_W = M_PI/6.0; ///< Fuzzy system's target to the front w

    const double FUZZY_TARGET_RIGHT_T = 0.0;      ///< Fuzzy system's target to the right t
    const double FUZZY_TARGET_RIGHT_W = M_PI/6.0; ///< Fuzzy system's target to the right w
  
  

    ////////////////////////////////////////////////////////////
    // Size
    ////////////////////////////////////////////////////////////

    const double FUZZY_VERY_LARGE_T = 2200; ///< Fuzzy system's size is very large t
    const double FUZZY_VERY_LARGE_W = 1300; ///< Fuzzy system's size is very large w
   
    const double FUZZY_LARGE_T = 1500; ///< Fuzzy system's size is large t
    const double FUZZY_LARGE_W = 500;  ///< Fuzzy system's size is large w
  
    const double FUZZY_MEDIUM_T = 500; ///< Fuzzy system's size is medium t
    const double FUZZY_MEDIUM_W = 200; ///< Fuzzy system's size is medium w
  
    const double FUZZY_SMALL_T = 250; ///< Fuzzy system's size is small t
    const double FUZZY_SMALL_W = 50;  ///< Fuzzy system's size is small w
  
    const double FUZZY_VERY_SMALL_T = 150; ///< Fuzzy system's size is very small t
    const double FUZZY_VERY_SMALL_W = 50;  ///< Fuzzy system's size is very small w



    ////////////////////////////////////////////////////////////
    // Camera heading
    ////////////////////////////////////////////////////////////

    const double FUZZY_CAMERA_LEFT_T = -M_PI/4.0;   ///< Fuzzy system's camera is pointing left t
    const double FUZZY_CAMERA_LEFT_W =  M_PI/36.0;  ///< Fuzzy system's camera is pointing left w

    const double FUZZY_CAMERA_RIGHT_T = M_PI/4.0;   ///< Fuzzy system's camera is pointing right t
    const double FUZZY_CAMERA_RIGHT_W = M_PI/36.0;  ///< Fuzzy system's camera is pointing right w

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_FUZZY_CONSTANTS_H
