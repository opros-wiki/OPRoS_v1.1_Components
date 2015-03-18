//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_SPEECH_ISPEECH_TTS_CALLBACK_HPP
#define INCLUDE_EVOLUTION_SPEECH_ISPEECH_TTS_CALLBACK_HPP


/**
 * @file    evolution/core/resource/ISpeechTTSCallback.hpp
 * @brief   Interface for a text-to-speech callback.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ISpeechTTSCallback
 */


#include <evolution/core/resource/TTSTypes.hpp>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface ISpeechTTSCallback
     *
     * @brief This class defines a callback interface to receive notification
     * of events from the text-to-speech engine.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    class EVOLUTION_EXPORT_RESOURCE ISpeechTTSCallback
    {
    public:                  // Structors
        /**
         @brief Constructor
         */
        ISpeechTTSCallback (  )
        {
        }

        /**
         @brief Deconstructor
         */
        virtual ~ ISpeechTTSCallback (  )
        {
        }

    public:                  // Callbacks
        /**
         * @brief TTS activation event
         */
        virtual void handle_initialize (  )
        {
        }

        /**
         * @brief TTS deactivation event
         */
        virtual void handle_shutdown (  )
        {
        }

        /**
         * @brief TTS synchronization event
         */
        virtual void handle_synchronization (  )
        {
        }

        /**
         * @brief TTS finish playing a single utterance event
         */
        virtual void handle_finished_playing_utterance (  )
        {
        }
    };  // end class ISpeechTTSCallback


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_SPEECH_ISPEECH_TTS_CALLBACK_HPP
