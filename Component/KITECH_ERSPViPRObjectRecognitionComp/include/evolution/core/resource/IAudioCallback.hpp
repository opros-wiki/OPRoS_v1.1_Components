//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CALLBACK_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CALLBACK_HPP

/**
 * @file    evolution/core/resource/IAudioCallback.hpp
 * @brief   Defines callback mechanism for audio operations.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioCallback
 */

#include <evolution/core/base/Types.hpp>

namespace Evolution
{

    /**
     * @interface IAudioCallback
     *
     * @brief This class defines an interface to handle changes in audio levels
     * through a callback mechanism.
     *
     * @see AudioTypes.hpp
     *
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IAudioCallback
    {
    public: // Structors
        /**
         @brief Constructor
         */
        IAudioCallback () {}

        /**
         @brief Destructor
         */
        virtual ~IAudioCallback () {}

    public: // General audio functions  -----------
        /**
         * @brief Callback associated with the shutdown of an audio object.
         */
        virtual void handle_shutdown () {}

    public: // Audio playback functions -----------

        /**
         * @brief This function handles a finished audio-playing event.
         */
        virtual void handle_finished_playing () {}

    public: // Audio level functions  -----------
        /**
         * @brief
         * The handle_audio_level function controls the audio level, on
         * a scale from 0 to 1.
         *
         * @param level [in]; the level of the audio.
         */
        virtual void handle_audio_level (double level) {}

    }; // end class IAudioCallback


} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CALLBACK_HPP
