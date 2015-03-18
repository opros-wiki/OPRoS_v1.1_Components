//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IMOTOR_QUERY_HPP
#define INCLUDE_EVOLUTION_CORE_IMOTOR_QUERY_HPP


/**
 * @file    evolution/core/resource/IMotorQuery.hpp
 * @brief   This file defines a class to provide an interface for queries of a
 * motor device.  @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IMotorQuery
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface IMotorQuery
     *
     * @brief The IMotorQuery class provides a resource interface for queries
     * of a motor device.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IMotorQuery
    {
    public:                  // Structors

        /**
         * @brief Constructor
         */
        IMotorQuery (  )
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ IMotorQuery (  )
        {
        }

    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IMotorQuery"

    public:                  // Queries

        /**
         * @brief The get_position function returns the current position in the configured units. While
         * this value is easily derivable from raw_position() and
         * position_factor(), it is also the most commonly useful of
         * the three.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  position [out]; Returns current position using the configured units
         */
        virtual Result get_position ( TicketId ticket, double *position ) = 0;

        /**
         * @brief The get_raw_position function returns the current raw position, generally the encode
         * count of the motor.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  raw_position [out]; Returns current position using raw motor output
         */
        virtual Result get_raw_position ( TicketId ticket,
                                          double *raw_position ) = 0;

        /**
         * @brief This function gets the configuration factor to convert a raw position to a
         * position.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  position_factor [out]; Factor used to convert raw position to configured units
         */
        virtual Result get_position_factor ( TicketId ticket,
                                             double *position_factor ) = 0;

        /**
         * @brief This function returns the current motor velocity.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  velocity [out]; Current motor velocity
         */
        virtual Result get_velocity ( TicketId ticket, double *velocity ) = 0;

        /**
         * @brief Returns if the motor is currently moving.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  moving [out]; True if moving, False otherwise
         */
        virtual Result is_moving ( TicketId ticket, bool * moving ) = 0;

    };                          // end class IMotorQuery

}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_IMOTOR_QUERY_HPP
