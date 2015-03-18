//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CHANNEL_FACTORY_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CHANNEL_FACTORY_HPP

/**
 * @file    evolution/core/resource/IAudioChannelFactory.hpp
 * @brief   Interface for audio channel server
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioChannelFactory
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/IAudioChannel.hpp>

namespace Evolution
{

    /**
     * @interface IAudioChannelFactory
     *
     * @brief
     * Interface to an audio 'patchbay' consisting of several record or
     * playback channels connected to hardware input or output devices.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAudioChannelFactory
    {
    public:                  // Constants

        /// ID of this interface.
        static const char* const INTERFACE_ID; // "Evolution.IAudioChannelFactory"

    public:                  // Structors

        /// Empty constructor.
        IAudioChannelFactory (  )
        {
        }

        /// Empty destructor.
        virtual ~ IAudioChannelFactory (  )
        {
        }

    public:
        /**
         * @brief Function that obtains a playback channel from the audio
         *        server.
         *
         * @param  playback_channel [in]: A pointer to a playback channel that will
         *                                be created by the audio channel factory object.
         *
         * @param  client_id [in]: A string that identifies a specific audio channel.
         *                         If an audio channel that has already been created is
         *                         requested, the channel will be treated as a FIFO object,
         *                         and audio will be processed sequentially instead of in parallel.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result obtain_playback_channel ( IAudioChannel** playback_channel,
                                                 String client_id ) = 0;

        /**
         * @brief Function that obtains a record channel from the audio
         *        server.
         *
         * @param record_channel [in]: A pointer to a recording
         * channel that will be created by the audio channel factory
         * object.
         *
         * @param client_id [in]: A string that identifies a specific
         * audio channel.  requested, this function will return an
         * error.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result obtain_record_channel ( IAudioChannel** record_channel,
                                               String client_id ) = 0;
        /**
         * @brief Function that obtains a device that monitors the
         * level of the audio input.
         *
         * @param audio_level_channel [in]: A pointer to an audio
         * level object that will be created by the audio channel
         * factory object.
         *
         * @param client_id [in]: A string that identifies a specific
         * audio channel.  requested, this function will return an
         * error.
         *
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result obtain_audio_level_channel ( IAudioChannel** audio_level_channel,
                                                    String client_id ) = 0;

    };                          // end class IAudioChannelFactory
}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CHANNEL_FACTORY_HPP
