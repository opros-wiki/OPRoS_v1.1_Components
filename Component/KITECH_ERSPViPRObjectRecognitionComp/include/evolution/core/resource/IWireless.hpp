//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IWIRELESS_HPP
#define INCLUDE_EVOLUTION_CORE_IWIRELESS_HPP


/**
 * @file    evolution/core/resource/IWireless.hpp
 * @brief   This file defines a class to provide an interface to wireless
 * devices.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IWireless
 */


#include <evolution/core/base/Types.hpp>
#include <string>


namespace Evolution
{


    /**
     * @interface IWireless
     *
     * @brief This class provides a resource interface to wireless devices.
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IWireless
    {
    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IWireless"

    public:                  // Structors

        /**
         * @brief Empty constructor
         */
        IWireless (  )
        {
        }

        /**
         * @brief Empty destructor
         */
        virtual ~ IWireless (  )
        {
        }
        /**
         * @brief This function gets the current status of the wireless device.
         *
         * @param  ticket    [in];  The security ticket
         * @param  timestamp [out]; The timestamp of the reading
         * @param  link_quality [out]; The wireless link quality
         * @param  signal_level [out]; The wireless signal level
         * @param  noise_level [out]; The wireless noise level
         **/
        virtual Result get_status ( TicketId ticket, Timestamp * timestamp,
                                    double *link_quality,
                                    double *signal_level,
                                    double *noise_level ) = 0;

        /**
         * @brief Get the best possible wireless link quality.  Used for
         * comparing link quality.
         *
         * @param  ticket    [in];  The security ticket
         * @param  best_quality [out]; The best possible wireless link quality
         */
        virtual Result get_best_quality ( TicketId ticket,
                                          double *best_quality ) = 0;

    };                          // end class IWireless


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_IWIRELESS_HPP
