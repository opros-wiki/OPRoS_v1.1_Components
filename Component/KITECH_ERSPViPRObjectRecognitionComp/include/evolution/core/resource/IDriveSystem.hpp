//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IDRIVE_SYSTEM_HPP
#define INCLUDE_EVOLUTION_CORE_IDRIVE_SYSTEM_HPP


/**
 * @file    evolution/core/resource/IDriveSystem.hpp
 * @brief   This file defines a class to provide an interface to a drive system.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IDriveSystem
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/IResourceCallback.hpp>
#include <evolution/core/resource/IMotorCommand.hpp>


namespace Evolution
{


    /**
     * @interface IDriveSystem
     *
     * @brief This defines a resource interface for commands to a drive system.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IDriveSystem
    {
    public:                  // Types

        /**
         * @brief Different manners in which a motor may stop
         */
        typedef IMotorCommand::StopType StopType;

    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IDriveSystem"

    public:                  // Structors

        /**
         * @brief Constructor
         */
        IDriveSystem (  )
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ IDriveSystem (  )
        {
        }

    public:                  // Commands

        /**
         * @brief Use this to move at the specified velocity and acceleration.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  velocity [in]; The desired velocity
         * @param  acceleration [in]; The desired accelration
         * @param  angular_velocity [in]; The desired angular velocity
         * @param  angular_acceleration [in]; The desired angular acceleration
         */
        virtual Result move_and_turn ( TicketId ticket, double velocity,
                                       double acceleration,
                                       double angular_velocity,
                                       double angular_acceleration ) = 0;

        /**
         * @brief Use this to move the specified displacement with the specified velocity and 
         * acceleration.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback class to use to verify command completion
         * @param  callback_id [in]; Specific callback_id  associated with this command
         * @param  displacement [in]; Desired displacement distance
         * @param  velocity [in]; Desired velocity
         * @param  acceleration [in]; Desired acceleration
         */
        virtual Result move_delta ( TicketId ticket,
                                    IResourceCallback * callback,
                                    CallbackId callback_id,
                                    double displacement, double velocity,
                                    double acceleration ) = 0;

        /**
         * @brief Use this to turn the specified displacement with the specified velocity and 
         * acceleration. Uses callbacks to track asynchronous completion.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; The callback class used to test command completion
         * @param  callback_id [in]; ID of the specific callback for this command
         * @param  angular_displacement [in]; Desired angular heading
         * @param  angular_velocity [in]; Desired angular velocity.
         * @param  angular_acceleration [in]; Desired angular acceleration.
         */
        virtual Result turn_delta ( TicketId ticket,
                                    IResourceCallback * callback,
                                    CallbackId callback_id,
                                    double angular_displacement,
                                    double angular_velocity,
                                    double angular_acceleration ) = 0;

        /**
         * @brief Use this to stop movement. Uses callbacks to track asynchronous completion.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; The callback class used to test command completion
         * @param  callback_id [in]; ID of the specific callback for this command
         * @param  stop_type [in]; Specific manner used for stopping
         **/
        virtual Result stop ( TicketId ticket, IResourceCallback * callback,
                              CallbackId callback_id, StopType stop_type ) =
        0;

        /**
         * @brief This sets power used for moving as a percentage.
         *  Uses callbacks to track asynchronous completion.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; The callback class used to test command completion
         * @param  callback_id [in]; ID of the specific callback for this command
         * @param  move_power [in]; Percentage of power used for movement
         */
        virtual Result set_move_power ( TicketId ticket,
                                        IResourceCallback * callback,
                                        CallbackId callback_id,
                                        double move_power ) = 0;

        /**
         * @brief This sets power used for idling as a percentage.
         *  Uses callbacks to track asynchronous completion.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; The callback class used to test command completion
         * @param  callback_id [in]; ID of the specific callback for this command
         * @param  idle_power [in]; Percentage of power used during idling
         */
        virtual Result set_idle_power ( TicketId ticket,
                                        IResourceCallback * callback,
                                        CallbackId callback_id,
                                        double idle_power ) = 0;

        /**
         * @brief Use this to get the velocity(v) and angular velocity(w) of the drive system.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  v [in]; The current linear velocity
         * @param  w [in]; The current angular velocity
         */
        virtual Result get_velocities ( TicketId ticket, double *v,
                                        double *w ) = 0;

    };                          // end class IDriveSystem


}                               //  end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_IDRIVE_SYSTEM_HPP
