//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IGRIPPER_HPP
#define INCLUDE_EVOLUTION_CORE_IGRIPPER_HPP


/**
 * @file    evolution/core/resource/IGripper.hpp
 * @brief   An interface for working with a gripper.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IGripper 
 */


#include <evolution/core/base/Types.hpp>

namespace Evolution
{


    /**
     * @interface IGripper 
     *
     * @brief This class defines a resource interface to a gripper.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IGripper 
    {
    public: // Constants
        /// Enumeration detailing possible gripper status
        typedef enum {OPENED, OPENING, CLOSED, CLOSING, UNKNOWN} GripperStatus;

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID;

    public: // Structors

        /**
         * @brief Constructor
         */
        IGripper () {}

        /**
         * @brief Destructor
         */
        virtual ~IGripper () {}

    public: // Queries

        /**
         * @brief The open function opens the gripper.
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp
         * @return  RESULT_SUCCESS on success,
         *          RESULT_FAILURE if an error occurs.
         */
        virtual Result open (TicketId ticket, Timestamp *timestamp) = 0;

        /**
         * @brief The close function closes the gripper.
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp
         * @return  RESULT_SUCCESS on success,
         *          RESULT_FAILURE if an error occurs.
         */
        virtual Result close (TicketId ticket, Timestamp *timestamp) = 0;

        /**
         * @brief The stop function stops the gripper.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  timestamp [out]; the timestamp
         * @return  RESULT_SUCCESS on success,
         *          RESULT_FAILURE if an error occurs.
         */
        virtual Result stop (TicketId ticket, Timestamp *timestamp) = 0;

        /**
         * @brief The make_autonomous function allows the gripper to operate under hardware control.
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp
         * @return  RESULT_SUCCESS on success,
         *          RESULT_FAILURE if an error occurs.
         */
        virtual Result make_autonomous (TicketId ticket, Timestamp *timestamp) = 0;

        /**
         * @brief The object_sensed function tests if an object is sensed by the gripper.
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp
         * @param  sensed_object [out]; true is an object is sensed, false otherwise
         * @return  RESULT_SUCCESS on success,
         *          RESULT_FAILURE if an error occurs.
         */
        virtual Result object_sensed (TicketId ticket, Timestamp *timestamp, bool *sensed_object) = 0;

        /**
         * @brief The get_status function returns the current status of the gripper.
         *
         * @param  ticket [in]; The security ticket
         * @param  timestamp [out]; The timestamp
         * @param  status [out]; The status of the gripper
         * @param  autonomous [out]; returns whether the gripper is autonomous
         * @param  object_sensed [out]; returns whether the gripper sense 
         *         an object.
         * @return  RESULT_SUCCESS on success,
         *          RESULT_FAILURE if an error occurs.
         */
        virtual Result get_status (TicketId ticket, Timestamp *timestamp, GripperStatus *status, bool *autonomous, bool *object_sensed) = 0;


        /**
         * @brief The status_string function converts a gripper status value to a human readable string.
         *
         * @param status [in]; The gripper status
         * @return A string describing the status.
         */
        static const char* status_string (GripperStatus status);

    }; // end class IGripper


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IGRIPPER_HPP
