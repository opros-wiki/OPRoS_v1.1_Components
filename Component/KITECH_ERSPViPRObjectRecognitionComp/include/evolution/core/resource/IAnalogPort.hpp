//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IANALOG_PORT_HPP
#define INCLUDE_EVOLUTION_CORE_IANALOG_PORT_HPP


/**
 * @file    evolution/core/resource/IAnalogPort.hpp
 * @brief   An interface encapsulating the operations of an analog port. 
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAnalogPort 
 */


#include <evolution/core/base/Types.hpp>

namespace Evolution
{


    /**
     * @interface IAnalogPort 
     *
     * @brief The IAnalogPort class provides a resource interface to an
     * analog port.  
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAnalogPort 
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IAnalogPort"

    public: // Structors

        /**
         * @brief Constructor
         */
        IAnalogPort () {}

        /**
         * @brief Destructor
         */
        virtual ~IAnalogPort () {}

    public: // Queries

        /**
         * @brief Returns the digitized integer reading from the analog port. 
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp after reading the device
         * @param  reading [out]; The digitized integer reading from the
         *      analog port.
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result get_digital_reading (TicketId ticket, Timestamp* timestamp, long* reading) = 0;

        /**
         * @brief Returns the actual analog voltage value. 
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp after reading the device
         * @param  reading [out]; The raw analog reading from the
         *      analog port.  This value typically needs to be scaled from
         *      the digitized integer value.
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result get_analog_reading (TicketId ticket, Timestamp* timestamp, double* reading) = 0;

    public: // Writes
        /**
         * @brief Writes a digitized integer value to the analog port. 
         *
         * @param  ticket [in]; The security ticket
         * @param  value [in]; The digitized integer to write to the port. 
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result set_digital_value (TicketId ticket, long value) = 0;
        
    public: // Writes
        /**
         * @brief Writes a raw analog value to the analog port. 
         *
         * @param  ticket [in]; The security ticket
         * @param  value [in]; The raw analog value to write. 
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result set_analog_value (TicketId ticket, double value) = 0;
    }; // end class IAnalogPort


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IANALOG_PORT_HPP

