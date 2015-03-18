//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IBATTERY_HPP
#define INCLUDE_EVOLUTION_CORE_IBATTERY_HPP


/**
 * @file    evolution/core/resource/IBattery.hpp
 * @brief   This file defines a class to provide an interface to a battery.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IBattery 
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @interface IBattery 
     *
     * @brief This class provides a resource interface to a battery.
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IBattery 
    {
    public: // Constants

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IBattery"

    public: // Structors

        /**
         * @brief Empty constructor
         */
        IBattery () {}

        /**
         * @brief Empty destructor
         */
        virtual ~IBattery () {}

    public: // Queries

        /**
         * @brief The get_voltage function retrieves the battery voltage.
         *
         * @param  ticket    [in];  The security ticket
         * @param  timestamp [out]; The timestamp of the reading
         * @param  voltage   [out]; The battery voltage
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_voltage (TicketId ticket,
                                    Timestamp *timestamp,
                                    double *voltage) = 0;

        /**
         * @brief The get_percent_charge_left function retrieves the precentage of charge remaining in the battery.
         *
         * @param  ticket         [in];  The security ticket
         * @param  timestamp      [out]; The timestamp of the reading
         * @param  percent_charge [out]; Percentage of charge left in the battery
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_percent_charge_left (TicketId ticket,
                                                Timestamp *timestamp,
                                                double *percent_charge) = 0;
    }; // end class IBattery

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IBATTERY_HPP
