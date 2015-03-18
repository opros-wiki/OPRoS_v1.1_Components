//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IFACE_HPP
#define INCLUDE_EVOLUTION_CORE_IFACE_HPP


/**
 * @file    evolution/core/resource/IFace.hpp
 * @brief   The IFace class provides an interface for an animated facial 
 * display.
 * @see  Evolution::IFace 
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface IFace 
     *
     * @brief This class provides an abstract interface to an animated facial
     * display.
     *
     * @ingroup core_resource
     * @ingroup hri_emotion
     */
    class EVOLUTION_EXPORT_RESOURCE IFace 
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IFace"

    public: // Structors

        /**
         * @brief Constructor
         */
        IFace () {}

        /**
         * @brief Destructor
         */
        virtual ~IFace () {}

    public: // Commands

        /**
         * @brief The action function provides the animation to be played.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  action [in]; The name of the next action to play
         */
        virtual Result action (TicketId ticket, const char *action) = 0;

        /**
         * @brief  The set_emotion_value function sets the value of the current coefficient for 'emotion'.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  duration [in]; Duration this value is effective
         * @param  emotion [in]; The emotion to modify
         * @param  value [in]; The coefficient for the emotion
         */
        virtual Result set_emotion_value (TicketId ticket, float duration, 
                                          int emotion, float value) = 0;

        /**
         * @brief The set_emotion_increment function sets an increment in the current coefficient value for 'emotion'.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  duration [in]; Length of time this coeffecient should apply
         * @param  emotion [in]; The emotion to modify
         * @param  value [in]; The coefficient to apply
         */
        virtual Result set_emotion_increment (TicketId ticket, float duration, 
                                              int emotion, float value) = 0;

        /**
         * @brief The set_emotion_vector function sets the complete emotion vector.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  duration [in]; Duration for this vector
         * @param  emotions [in]; A vector of emotions to apply
         */
        virtual Result set_emotion_vector (TicketId ticket, float duration, 
                                           const float *emotions) = 0;

        /**
         * @brief The speak function defines the sets of phonemes to be
         * played.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  phonemes [in]; A list of phonemes to play
         */
        virtual Result speak (TicketId ticket, const char *phonemes) = 0;

        /**
         * @brief Wait for the emotional change to finish.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  wait_time [in]; This is an optional timeout for the wait, otherwise it blocks.
         */
        virtual Result wait_for_emotional_change (TicketId ticket, unsigned long wait_time = ERSP_INFINITE) = 0;
    }; // end class IFace


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IFACE_HPP
