//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_AUDIO_TYPES_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_AUDIO_TYPES_HPP


/**
 * @file    evolution/core/resource/AudioTypes.hpp
 * @brief   This defines basic types for audio capture and play back.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

namespace Evolution
{

    /**
     * @brief This variable defines basic types for audio capture and play back.
     *
     * @ingroup core_resource
     */
    enum SampleFormat
    {
        FORMAT_UNKNOWN=-1, ///< Useful for detecting uninitialized variable
        S16_LE=0,          ///< 16-bits signed little-endian
        U8,                ///< 8-bits unsigned 
        GSM610,            ///< GSM compressed audio
        FLOAT32_LE         ///< 32-bits float little-endian, normalized [-1,1]
    };

    enum ChannelsMode
    {
        CHANNELS_MONO=0,   ///< Mono audio capture
        CHANNELS_STEREO    ///< Stereo audio capture
    };

    enum SamplingRate
    {
        SR_22K=0,          ///< 22050 Hz sampling rate
        SR_44K,            ///< 44100 Hz sampling rate
        SR_11K,            ///< 11025 Hz sampling rate
        SR_16K,            ///< 16000 Hz sampling rate
        SR_8K              ///<  8000 Hz sampling rate
    };

    /**
     * @brief This variable represents audio events in which a callback may
     * register interest.
     *
     * @ingroup core_resource
     */
    enum AudioEventFlags
    {
        AUDIO_EVENT_NONE             = 0x0,
        AUDIO_EVENT_FINISHED_PLAYING = 0x01,
        AUDIO_EVENT_HANDLE_SHUTDOWN  = 0x02,
        AUDIO_EVENT_HANDLE_LEVEL     = 0x04
    };

    /**
     * @brief This variable is a set of AudioEventFlags, ORed together.
     *
     * @ingroup core_resource
     */
    typedef unsigned long AudioEventType;

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE_AUDIO_TYPES_HPP
