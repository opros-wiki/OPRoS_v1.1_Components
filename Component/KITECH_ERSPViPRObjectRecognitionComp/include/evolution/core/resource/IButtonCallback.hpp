//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_IBUTTON_CALLBACK_HPP
#define INCLUDE_EVOLUTION_IBUTTON_CALLBACK_HPP


/**
 * @file    evolution/core/resource/IButtonCallback.hpp
 * @brief   Interface for a button callback
 * @see  Evolution::IButtonCallback
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface IButtonCallback
     *
     * @brief
     * Callback This class provides an interface to receive notification of
     * events from a button.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IButtonCallback
    {
    public:
        /// Type of events that can be received from a button.
        enum ButtonEventType
        {
            BUTTON_PRESSED_EVENT  = 1 << 1,
            BUTTON_RELEASED_EVENT = 1 << 2,
            BUTTON_REMOVED_EVENT  = 1 << 3,
            BUTTON_EVENT          = BUTTON_PRESSED_EVENT | BUTTON_RELEASED_EVENT | BUTTON_REMOVED_EVENT
        };

    public: // Structors

        /**
         * @brief Constructor
         */
        IButtonCallback () {}

        /**
         * @brief Destructor
         */
        virtual ~IButtonCallback () {}

    public: // Callbacks

        /**
         * @brief This function handles a button pressed event.
         * @param button_name [in]; Name of the button
         * @param timestamp [in]; Time the event was received
         */
        virtual void handle_button_pressed (const String &button_name, Timestamp timestamp) = 0;

        /**
         * @brief This function handles a button released event.
         * @param button_name [in]; Name of the button
         * @param timestamp [in]; Time the event was received
         */
        virtual void handle_button_released (const String &button_name, Timestamp timestamp) = 0;

        /**
         * @brief This function handles a button removed event.
         * @param button_name [in]; Name of the button
         * @param timestamp [in]; Time the event was received
         */
        virtual void handle_button_removed (const String &button_name, Timestamp timestamp) = 0;

    }; // end class IButtonCallback


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_IBUTTON_CALLBACK_HPP
