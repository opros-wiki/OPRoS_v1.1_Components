//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IMOTOR_COMMAND_HPP
#define INCLUDE_EVOLUTION_CORE_IMOTOR_COMMAND_HPP


/**
 * @file    evolution/core/resource/IMotorCommand.hpp
 * @brief   This file defines a class to provide a resource interface for commands to a motor device.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IMotorCommand
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/IResourceCallback.hpp>


namespace Evolution
{


    /**
     * @interface IMotorCommand
     *
     * @brief This class provides a resource interface for commands to a motor
     * device.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IMotorCommand
    {
    public:                  // Types

        /**
         * @brief This is a list of different manners in which a motor may stop.
         */
        enum StopType
        {
            STOP_OFF,
            STOP_ABRUPT,
            STOP_SMOOTH
        };                      // end enum StopType

    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IMotorCommand"

    public:                  // Structors

        /**
         * @brief Constuctor
         */
        IMotorCommand (  )
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ IMotorCommand (  )
        {
        }

    public:                  // Commands

        /**
         * @brief This function moves the motor at the specified velocity and acceleration.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  velocity [in]; The requested velocity
         * @param  acceleration [in]; The requested acceleration
         */
        virtual Result move ( TicketId ticket, double velocity,
                              double acceleration ) = 0;

        /**
         * @brief This function moves the object to the specified position with the specified velocity
         * and acceleration.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  position [in]; Desired position
         * @param  velocity [in]; The requested velocity
         * @param  acceleration [in]; The requested acceleration
         */
        virtual Result move_to ( TicketId ticket,
                                 IResourceCallback * callback,
                                 CallbackId callback_id, double position,
                                 double velocity, double acceleration ) = 0;

        /**
         * @brief This function moves the specified displacement with the specified velocity and 
         * acceleration.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  displacement [in]; Desired displacement
         * @param  velocity [in]; The requested velocity
         * @param  acceleration [in]; The requested acceleration
         */
        virtual Result move_delta ( TicketId ticket,
                                    IResourceCallback * callback,
                                    CallbackId callback_id,
                                    double displacement, double velocity,
                                    double acceleration ) = 0;

        /**
         * @brief This function stops movement.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  stop_type [in]; Stopping method
         */
        virtual Result stop ( TicketId ticket, IResourceCallback * callback,
                              CallbackId callback_id, StopType stop_type ) =
        0;

        /**
         * @brief This function sets power used by the motor while moving in absolute motor units.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  move_power [in]; Power specified in absolute motor units
         */
        virtual Result set_move_power_absolute ( TicketId ticket,
                                                 IResourceCallback * callback,
                                                 CallbackId callback_id,
                                                 double move_power ) = 0;

        /**
         * @brief This function sets power used by the motor while idle in absolute motor units.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  idle_power [in]; Power specified in absolute motor units
         */
        virtual Result set_idle_power_absolute ( TicketId ticket,
                                                 IResourceCallback * callback,
                                                 CallbackId callback_id,
                                                 double idle_power ) = 0;

        /**
         * @brief This function sets power used by the motor while moving as a percentage.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  move_percent [in]; Percent of power to use while moving
         */
        virtual Result set_move_power_percentage ( TicketId ticket,
                                                   IResourceCallback *
                                                   callback,
                                                   CallbackId callback_id,
                                                   double move_percent ) = 0;

        /**
         * @brief This function sets power used by the motor while idle as a percentage.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  idle_percent [in]; Percent of power to use while idling
         */
        virtual Result set_idle_power_percentage ( TicketId ticket,
                                                   IResourceCallback *
                                                   callback,
                                                   CallbackId callback_id,
                                                   double idle_percent ) = 0;
    };                          // end class IMotorCommand

}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_IMOTOR_COMMAND_HPP
