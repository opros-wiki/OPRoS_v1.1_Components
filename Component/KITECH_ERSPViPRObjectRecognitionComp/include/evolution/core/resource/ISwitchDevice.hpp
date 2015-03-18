//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_ISWITCH_DEVICE_HPP
#define INCLUDE_EVOLUTION_CORE_ISWITCH_DEVICE_HPP


/**
 * @file    evolution/core/resource/ISwitchDevice.hpp
 * @brief   This file defines a class to provide an interface to a "switch", 
 *          a device on the NMC bus that has two states: on and off.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ISwitchDevice
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{
    /**
     * @interface ISwitchDevice
     *
     * @brief This class provides an interface to a "switch"; that is, a device
     * (on the NMC bus) that has two states: on and off.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ISwitchDevice
    {
    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.ISwitchDevice"

    public:                  // Structors

        /**
         * @brief Empty constructor
         */
        ISwitchDevice (  )
        {
        }

        /**
         * @brief Empty destructor
         */
        virtual ~ ISwitchDevice (  )
        {
        }

    public:                  // Commands


        /**
         * @brief This sets switch to on.
         * @param  ticket [in]; The (TicketId) for security purposes.
         */
        virtual Result turn_on ( TicketId ticket ) = 0;

        /**
         * @brief This sets switch to off.
         * @param  ticket [in]; The (TicketId) for security purposes.
         */
        virtual Result turn_off ( TicketId ticket ) = 0;

    };                          // end class ISwitchDevice


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_ISWITCH_DEVICE_HPP
