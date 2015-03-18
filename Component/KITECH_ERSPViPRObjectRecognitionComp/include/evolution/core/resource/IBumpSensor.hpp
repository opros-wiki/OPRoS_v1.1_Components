//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IBUMP_SENSOR_HPP
#define INCLUDE_EVOLUTION_CORE_IBUMP_SENSOR_HPP


/**
 * @file evolution/core/resource/IBumpSensor.hpp
 * @brief   This file defines a class to provide an interface to a bump sensor.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IBumpSensor
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @interface IBumpSensor
     *
     * @brief This class provides a resource interface to a bump sensor,
     * that is, a sensor that detects impact with a physical object.
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IBumpSensor
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IBumpSensor"

    public: // Structors

        /**
         * @brief Empty constructor
         */
        IBumpSensor ()
        {
        }

        /**
         * @brief Empty destructor
         */
        virtual ~ IBumpSensor ()
        {
        }

    public: // Queries

        /**
         * @brief The is_triggered function indicates if the sensor detects
         * contact with an object.
         *
         * @param  ticket    [in];  the security ticket
         * @param  timestamp [out]; the timestamp of the reading
         * @param  triggered [out]; triggered or not
         *
         * @return RESULT_SUCCESS if successful.
         **/
        virtual Result is_triggered (TicketId ticket,
                                     Timestamp * timestamp,
                                     bool * triggered) = 0;
    }; // end class IBumpSensor

} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_IBUMP_SENSOR_HPP
