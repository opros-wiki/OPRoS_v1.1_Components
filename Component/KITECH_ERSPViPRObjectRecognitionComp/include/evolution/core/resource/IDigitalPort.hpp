//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IDIGITAL_PORT_HPP
#define INCLUDE_EVOLUTION_CORE_IDIGITAL_PORT_HPP


/**
 * @file    evolution/core/resource/IDigitalPort.hpp
 * @brief   An interface encapsulating the operations of a digital port.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IDigitalPort 
 */


#include <evolution/core/resource/RangeData.hpp>
#include <evolution/core/base/Types.hpp>

namespace Evolution
{


    /**
     * @interface IDigitalPort 
     *
     * @brief The IDigitalPort class provides a resource interface to a
     * digital port.  
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IDigitalPort 
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IDigitalPort"

    public: // Structors

        /**
         * @brief Constructor
         */
        IDigitalPort () {}

        /**
         * @brief Destructor
         */
        virtual ~IDigitalPort () {}

    public: // Queries

        /**
         * @brief Returns the digital reading from the digital port. 
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp after reading the device
         * @param  reading [out]; The digital reading.
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result get_reading (TicketId ticket, Timestamp* timestamp, bool* reading) = 0;

    public: // Writes
        /**
         * @brief Writes a digital value to the digital port. 
         *
         * @param  ticket [in]; The security ticket
         * @param  value [in]; The digital reading to write.
         * @return  RESULT_SUCCESS on success, or some error value.
         */
        virtual Result set_value (TicketId ticket, bool value) = 0;
    };
        
} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IDIGITAL_PORT_HPP

