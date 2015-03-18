//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__IAUDIO_LEVEL_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__IAUDIO_LEVEL_HPP


/**
 * @file    core/resource/IAudioLevel.hpp
 * @brief   An interface for setting audio levels.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioLevel
 */


//#include <evolution/core/resource/IAudioLevelCallback.hpp>
#include <evolution/core/resource/IAudioCallback.hpp>
#include <evolution/core/resource/AudioTypes.hpp>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @interface IAudioLevel
     *
     * @brief
     *
     * This class defines an interface an object that returns the audio level.
     * The audio level is defined as the average power of an audio signal,
     * normalized with respect to the peak power of that signal.
     *
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IAudioLevel
    {
    public: // Constants

        /**
         * @brief Interface ID
         */
        static const char* const INTERFACE_ID; // Evolution.IAudioLevel

    public: // Structors

        /**
         * @brief Empty constructor
         */
        IAudioLevel () {}

        /**
         * @brief Empty destructor
         */
        virtual ~IAudioLevel () {}

    public: // Levels

        /**
         * @brief The get_audio_level function obtains the audio level, on a scale from 0 to 1.
         * @param level [out]; the level of the audio signal.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_audio_level (double* level) = 0;

        /**
         * @brief
         * Sets a callback that is called when a significant change in audio level
         * is observed. This interface is deprecated, please use an IAudioCallback.
         * @param ticket   [in]; the security ticket.
         * @param callback [in]; the callback that will handle the change in the audio level.
         * @return RESULT_SUCCESS if successful.
         */
//         virtual Result set_callback (TicketId ticket, IAudioLevelCallback* callback);

        /**
         * @brief
         * Sets a callback that is called when a significant change in audio level
         * is observed.
         * @param ticket   [in]; the security ticket.
         * @param callback [in]; the callback that will handle the change in the audio level.
         * @param event_type [in]; the type of event that the given callback will be associated with.
         * @return RESULT_SUCCESS if successful.
         */
         virtual Result set_callback (TicketId ticket, 
                                      IAudioCallback* callback,
                                      AudioEventType event_type) = 0;

        /**
         * @brief The run function starts the processing of audio level information.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result run (TicketId ticket) = 0;

        /**
         * @brief The is_running function checks whether the audio level object
         * is active and running.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @return true if audio level engine is running.
         */
        virtual bool is_running (TicketId ticket) = 0;

    }; // end class IAudioLevel


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE__IAUDIO_LEVEL_HPP
