//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_ISPACIAL_SENSOR_HPP
#define INCLUDE_EVOLUTION_CORE_ISPACIAL_SENSOR_HPP


/**
 * @file    evolution/core/resource/ISpatialSensor.hpp
 * @brief   This file defines a class to provide an interface to a 
 * spatial sensor, that is a sensor that senses objects in three-dimensional
 * space.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::ISpatialSensor
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/PointSet.hpp>


namespace Evolution
{


    /**
     * @interface ISpatialSensor
     *
     * @brief This class provides an interface to a spatial sensor, that is a
     * sensor that senses objects in three-dimensional space.
     *
     * @note Interface ID = "Evolution.ISpatialSensor"
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ISpatialSensor
    {
    public:                  // Types

        class SpatialSensorParms;

    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.ISpatialSensor"

    public:                  // Structors

        /**
         * @brief Empty constructor
         */
        ISpatialSensor (  )
        {
        }

        /**
         * @brief Empty destructor
         */
        virtual ~ ISpatialSensor (  )
        {
        }

    public:                  // Commands

        /**
         * @brief The get_point_set function obtains a set of points detected by the sensor.
         *
         * @param  ticket [in]; The security ticket
         * @param  point_set [out]; The set of points
         */
        virtual Result get_point_set ( TicketId ticket,
                                       PointSet ** point_set ) = 0;

    };                          // end class ISpatialSensor


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_ISPACIAL_SENSOR_HPP
