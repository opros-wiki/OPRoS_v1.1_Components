//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IER_RANGE_SENSOR_HPP
#define INCLUDE_EVOLUTION_CORE_IER_RANGE_SENSOR_HPP


/**
 * @file    evolution/core/resource/IERRangeSensor.hpp
 * @brief   An interface encapsulating the operations of Evolution Robotics's
 *          USB IR range sensors.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IERRangeSensor
 */


#include <evolution/core/resource/RangeData.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/IRangeSensor.hpp>

namespace Evolution
{


    /**
     * @interface IERRangeSensor 
     *
     * @brief This class extends IRangeSensor with ER-specific features, such as 
     * blink rate and blink threshold control.  
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IERRangeSensor : public IRangeSensor
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IERRangeSensor"

    public: // Structors

        /**
         * @brief Constructor
         */
        IERRangeSensor () {}

        /**
         * @brief Destructor
         */
        virtual ~IERRangeSensor () {}

    public: // Methods

        /** 
         * @brief This function sets the ER range sensor blink rate.
         *
         * @param  ticket [in]; The security ticket
         * @param  rate   [in]; The new blink rate setting.
         *
         * @return  RESULT_SUCCESS on success,
         *          RESULT_NOT_FOUND if no object sensed,
         *          or some error value
         */

        virtual Result set_led_blink_interval(TicketId ticket, unsigned char rate) = 0;


        /**
         * @brief This function sets the sensor blink threshold.  The sensor will not blink unless
         * its readings are below the specified threshold.
         *
         * @param  ticket [in]; The security ticket
         * @param  distance [in]; The blink threshold in distance units.
         *   
         * @return  RESULT_SUCCESS on success,
         *          RESULT_NOT_FOUND if no object sensed,
         *          or some error value
         */
        virtual Result set_led_blink_distance(TicketId ticket, double distance) = 0;

        /**
         * @brief This function sets the sensor blink threshold.  The sensor will not blink unless
         * its readings are below the specified threshold.
         *
         * @param  ticket [in]; The security ticket
         * @param  tick_count [in]; The blink threshold in tick_counts
         *   
         * @return  RESULT_SUCCESS on success,
         *          RESULT_NOT_FOUND if no object sensed,
         *          or some error value
         */
        virtual Result set_led_blink_distance(TicketId ticket, unsigned char tick_count) = 0;

        
        /**
         * @brief This function turns the sensor light mode on or off.
         *
         * @param  ticket  [in]; The security ticket
         * @param  turn_on [in]; true if the sensor light should be on
         *   
         * @return  RESULT_SUCCESS on success,
         *          RESULT_NOT_FOUND if no object sensed,
         *          or some error value.
         */
        virtual Result set_sensor_light(TicketId ticket, bool turn_on) = 0;
        
        /**
         * @brief This function enables/disables software control.
         *
         * @param  ticket           [in]; The security ticket
         * @param  software_control [in]; true if the sensor should be in 
         *         software control mode
         *   
         * @return  RESULT_SUCCESS on success,
         *          RESULT_NOT_FOUND if no object sensed,
         *          or some error value
         */
        virtual Result set_sensor_mode(TicketId ticket, bool software_control) = 0;

    }; // end class IERRangeSensor


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IER_RANGE_SENSOR_HPP

