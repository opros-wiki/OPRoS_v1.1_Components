//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IRANGE_SENSOR_HPP
#define INCLUDE_EVOLUTION_CORE_IRANGE_SENSOR_HPP


/**
 * @file    evolution/core/resource/IRangeSensor.hpp
 * @brief   An interface encapsulating the operations of sensors
 *          that return a range to a target or obstacle.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IRangeSensor 
 */


#include <evolution/core/base/Types.hpp>

namespace Evolution
{


    /**
     * @interface IRangeSensor 
     *
     * @brief The IRangeSensor class provides a resource interface to a range
     * sensor, that is, a sensor that returns a distance reading to a sensed
     * physical object. IRangeSensor is not unit specific. Specific sensors
     * that derive from this class must specify their own units.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IRangeSensor 
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IRangeSensor"

    public: // Structors

        /**
         * @brief Constructor
         */
        IRangeSensor () {}

        /**
         * @brief Destructor
         */
        virtual ~IRangeSensor () {}

    public: // Queries

        /**
         * @brief The get_distance_reading function obtains a distance reading
         * from the sensor.
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp after reading the device
         * @param  distance [out]; The returned distance reading
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result get_distance_reading (TicketId ticket,
                                             Timestamp* timestamp, 
                                             double* distance) = 0;

        /**
         * @brief This function obtains the raw, unprocessed reading from the
         * sensor.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  timestamp [out]; the timestamp of the reading
         * @param  raw_reading [out]; the returned distance reading
         * @return  RESULT_SUCCESS on success, or some error value
         */
        virtual Result get_raw_reading (TicketId ticket,
                                        Timestamp* timestamp,
                                        double* raw_reading);

        /**
         * @brief Returns the polar-gaussian parameters of the sensor.
         *
         * This method only needs to be implemented for sensors that detect
         * in a conic area (e.g. sonar).  IRangeSensor provides a default
         * implementations that return results typical for sensors whose
         * detection shape approximate a ray, like IR sensors.  Drivers
         * that implement this method should read in these values as
         * resource configuration parameters for the sensor.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param theta_spread [out]; The angular spread of the sensor in
         *      radians.
         * @param rho_spread [out];  The radial error spread factor of
         *      the sensor.
         * @return  RESULT_SUCCESS on success, or some error value
         */
        virtual Result get_polar_gaussian_parameters(TicketId ticket,
                                                     double* theta_spread,
                                                     double* rho_spread);

    }; // end class IRangeSensor


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IRANGE_SENSOR_HPP

