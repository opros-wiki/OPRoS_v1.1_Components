//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IJOYSTICK_HPP
#define INCLUDE_EVOLUTION_CORE_IJOYSTICK_HPP


/**
 * @file    evolution/core/resource/IJoystick.hpp
 * @brief   This file defines a class to provide an interface to a joystick.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IJoystick
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @class IJoystick
     *
     * @brief This class defines a resource interface to a joystick.
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IJoystick
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IJoystick"

        /// Enumeration detailing indices for entries in a joystick coordinate array
        enum {
            X_AXIS,
            Y_AXIS,
            Z_AXIS,
            X_ROTATION,
            Y_ROTATION,
            Z_ROTATION,
            MAX_AXIS_COUNT,
            MAX_BUTTON_COUNT = 22
        };

        /// List of axis names
        static const char* AXIS_NAMES[];

    public: // Structors

        /**
         * @brief Empty constructor
         */
        IJoystick () {}

        /**
         * @brief Empty destructor
         */
        virtual ~IJoystick () {}

    public: // Queries


        /**
         * @brief This function returns the number of buttons and axes on this joystick.
         *
         * @param  ticket    [in];  The security ticket
         * @param  num_buttons [out]; The number of buttons on this joystick
         * @param  num_axes [out]; The number of axes on this joystick
         * @param  timestamp [out]; The timestamp of the reading
         **/
        virtual Result get_configuration (TicketId ticket,
                                          Timestamp* timestamp,
                                          unsigned *num_buttons,
                                          unsigned *num_axes) = 0;

        /**
         * @brief This function returns configuration information about a certain axis.
         *
         * @param  ticket    [in];  The security ticket
         * @param  axis_index [in]; Which axis to query
         * @param  timestamp [out]; The timestamp of the reading
         * @param  name [out]; The name of the axis
         * @param  min_value [out]; The minimum value attained on this axis
         * @param  max_value [out]; The maximum value attained on this axis
         * @param  n_discrete_values [out]; The number of discrete levels attained on this axis
         **/
        virtual Result get_axis_info (TicketId ticket,
                                      unsigned axis_index,
                                      Timestamp* timestamp,
                                      String *name,
                                      double *min_value,
                                      double *max_value,
                                      int *n_discrete_values) = 0;

        /**
         * @brief The get_button_info function returns configuration information about a certain button.
         *
         * @param  ticket    [in];  The security ticket
         * @param  button_index [in]; The index identifying the button
         * @param  timestamp [out]; The timestamp of the reading
         * @param  name [out]; The name of the axis
         **/
        virtual Result get_button_info (TicketId ticket,
                                        unsigned button_index,
                                        Timestamp* timestamp,
                                        String *name) = 0;


        /**
         * @brief The get_button_state function indicates if one of the buttons is depressed.
         *
         * @param  ticket    [in];  The security ticket
         * @param  button_index [in]; Which button to query
         * @param  depressed [out]; Whether the button is currently depressed
         * @param  timestamp [out]; The timestamp of the reading
         **/
        virtual Result get_button_state (TicketId ticket,
                                         unsigned button_index,
                                         Timestamp* timestamp,
                                         bool *depressed) = 0;

        /**
         * @brief The get_axis_state function returns the state of the specified axis.
         *
         * @param  ticket    [in];  The security ticket
         * @param  axis      [in];  The index identifying the axis
         * @param  timestamp [out]; The timestamp of the reading
         * @param  value [out]; The current position of the axis
         **/
        virtual Result get_axis_state (TicketId ticket,
                                       unsigned axis,
                                       Timestamp* timestamp,
                                       double *value) = 0;

    }; // end class IJoystick


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IJOYSTICK_HPP

