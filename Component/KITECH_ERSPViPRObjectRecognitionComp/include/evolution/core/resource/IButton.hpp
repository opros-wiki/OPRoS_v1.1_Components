//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IBUTTON_HPP
#define INCLUDE_EVOLUTION_CORE_IBUTTON_HPP


/**
 * @file    evolution/core/resource/IButton.hpp
 * @brief   This file defines a class to provide an interface to a button.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IButton 
 */


#include <evolution/core/resource/IButtonCallback.hpp>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface IButton 
     *
     * @brief The IButton class provides a resource interface to a button.
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IButton 
    {
    public: // Constants

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IButton"

    public: // Structors

        /**
         * @brief Empty constructor
         */
        IButton () {}

        /**
         * @brief Empty destructor
         */
        virtual ~IButton () {}

    public: // Queries

        /**
         * @brief Indicates if the sensor detects contact with an object.
         *
         * @param  ticket    [in];  The security ticket
         * @param  timestamp [out]; The timestamp of the reading
         * @param  pressed   [out]; button pressed or not
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result is_pressed (TicketId ticket,
                                   Timestamp *timestamp,
                                   bool *pressed) = 0;

        /**
         * @brief The set_callback function sets the button event callback.
         *
         * @see  IButtonCallback
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  callback [in]; The callback for a button event
         * @param  event_type [in]; The event type handled by the callback
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_callback (TicketId ticket,
                                     IButtonCallback *callback,
                                     IButtonCallback::ButtonEventType event_type) = 0;

        /**
         * @brief The remove_callback function removes the callback previously set by set_callback().
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result remove_callback (TicketId ticket) = 0;
    }; // end class IButton


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IBUTTON_HPP
