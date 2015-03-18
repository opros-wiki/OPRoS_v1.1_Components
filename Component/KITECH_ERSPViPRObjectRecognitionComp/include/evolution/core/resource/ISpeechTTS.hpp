//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_SPEECH_ISPEECH_TTS_HPP
#define INCLUDE_EVOLUTION_SPEECH_ISPEECH_TTS_HPP


/**
 * @file    evolution/core/resource/ISpeechTTS.hpp
 * @brief   Interface for a text-to-speech.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ISpeechTTS 
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/ISpeechTTSCallback.hpp>
#include <evolution/core/resource/TTSTypes.hpp>

namespace Evolution
{


    /**
     * @interface ISpeechTTS 
     *
     * @brief This class provides an abstract interface to a SpeechTTS, controlling
     * the (de)activation of text-to-speech, the (un)loading of languages,
     * and the propagation of events to registered callbacks.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    class EVOLUTION_EXPORT_RESOURCE ISpeechTTS 
    {
    public: // Constants

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.ISpeechTTS"

    public: // Structors
        /**
         @brief Constructor
         */
        ISpeechTTS () {}

        /**
         @brief Destructor
         */
        virtual ~ISpeechTTS () {}

    public: // Setting

        /**
         * @brief This function obtains the list of languages that the tts engine
         * supports.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param language_list [out]; List of languages this device supports
         *
         * @return Result indicating success or failure.
         */
        virtual Result get_supported_languages(TicketId ticket, StringList *language_list) = 0;

        /**
         * @brief This function obtains the list of voices that the tts engine
         * supports.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param language [in]; The language which the voice belongs to
         * @param voice_list [out]; List of voices this device supports
         *
         * @return Result indicating success or failure.
         */
        virtual Result get_supported_voices(TicketId ticket, const String &language, StringList *voice_list) = 0;

        /**
         * @brief The set_language function sets the language to be used.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  language_name [in]; the name of the language to be used.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result set_language (TicketId ticket, const char * language_name) = 0;

        /**
         * @brief The set_voice function sets the voice to be used
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  voice_name [in]; the name of the voice to be used.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result set_voice (TicketId ticket, const char * voice_name) = 0;

    public: // Text-to-Speech

        /**
         * @brief The synthesize function synthesizes text, returning immediately.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  source [in]; the text to synthesize.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result synthesize (TicketId ticket, const char* source) = 0;

        /**
         * @brief The synthesize function synthesizes wide char text, returning immediately.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  source [in]; the text to synthesize.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result synthesize (TicketId ticket, const wchar_t* source) = 0;

        /**
         * @brief The synthesize_file function synthesizes a file, returning immediately.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  file_path [in]; the path to the file to be synthesized.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result synthesize_file (TicketId ticket, const char* file_path) = 0;

        /**
         * @brief The synchronize function is used to synchronize SpeechTTS.
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result synchronize (TicketId ticket) = 0;

        /**
         * @brief The stop function stops SpeechTTS.
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result stop (TicketId ticket) = 0;

    public: // Callbacks

        /**
         * @brief The set_callback functinon sets the SpeechTTS event callback.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; a callback handler associated with the given event type.
         * @param  event_type [in]; the type of event to associate the callback with.
         *
         * @result RESULT_SUCCESS, if successful.
         */
        virtual Result set_callback (TicketId ticket,
                                     ISpeechTTSCallback* callback,
                                     TTSeventType event_type) = 0;

    }; // end class ISpeechTTS


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_SPEECH_ISPEECH_TTS_HPP
