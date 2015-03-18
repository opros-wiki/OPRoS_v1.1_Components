//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IAVOIDANCE_HPP
#define INCLUDE_EVOLUTION_CORE_IAVOIDANCE_HPP


/**
 * @file    evolution/core/resource/IAvoidance.hpp
 * @brief   An interface for performing various types of obstacle and hazard
 *          avoidance.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAvoidance
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface IAvoidance
     *
     * @brief The IAvoidance classs provides a resource interface to a bump
     * sensor, that is, a sensor that detects impact with a physical object.
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IAvoidance
    {
    public:                  // Constants
        /// ID of this interface.
        static const char *const INTERFACE_ID;  // "Evolution.IAvoidance"

    public:                  // Structors

        /// Empty constructor.
        IAvoidance (  )
        {
        }

        /// Empty destructor.
        virtual ~ IAvoidance (  )
        {
        }

    public:                  // Queries

        /**
         * @brief
         * Enable avoidance.
         *
         * @param  ticket    [in];  the security ticket
         **/
        virtual Result enable_avoidance ( TicketId ticket ) = 0;
        
        /**
         * @brief
         * Disable avoidance.
         *
         * @param  ticket    [in];  the security ticket
         **/
        virtual Result disable_avoidance ( TicketId ticket ) = 0;

        /**
         * @brief
         * Check if avoidance is enabled.
         *
         * @param  ticket    [in];  the security ticket
         * @param  enabled  [out];  true if avoidance is enabled.
         **/
        virtual Result avoidance_enabled ( TicketId ticket, bool *enabled ) = 0;

        /**
         * @brief
         * Gets the value of an avoidance parameter.
         *
         * @param  ticket    [in];  the security ticket
         * @param  name      [in];  the name of the parameter to set.
         * @param  value     [out]; the value of the specified parameter.
         **/
        virtual Result get_parameter ( TicketId ticket, const char* name, double *value ) = 0;

        /**
         * @brief
         * Sets the value of an avoidance parameter.
         *
         * @param  ticket    [in];  the security ticket
         * @param  name      [in];  the name of the parameter to set.
         * @param  value     [in];  the value to set.
         **/
        virtual Result set_parameter ( TicketId ticket, const char* name, double value ) = 0;
    };                          // end class IAvoidance


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_IAVOIDANCE_HPP
