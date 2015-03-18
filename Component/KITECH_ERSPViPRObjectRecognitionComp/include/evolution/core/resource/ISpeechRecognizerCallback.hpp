//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_SPEECH_ISPEECH_RECOGNIZER_CALLBACK_HPP
#define INCLUDE_EVOLUTION_SPEECH_ISPEECH_RECOGNIZER_CALLBACK_HPP


/**
 * @file   evolution/core/resource/ISpeechRecognizerCallback.hpp
 * @brief  Interface for a speech recognizer callback.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::ISpeechRecognizerCallback
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/RecognizerTypes.hpp>


namespace Evolution
{

    /**
     * @interface ISpeechRecognizerCallback
     *
     * @brief This class provides a callback interface to receive notification
     * of events from the recognizer.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    class EVOLUTION_EXPORT_RESOURCE ISpeechRecognizerCallback
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        ISpeechRecognizerCallback ()
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ ISpeechRecognizerCallback ()
        {
        }

    public: // Callbacks

        /**
         * @brief This function returns a Recognizer initialization event.
         */
        virtual void handle_initialize ()
        {
        }

        /**
         * @brief This function returns a Recognizer shutdown event.
         */
        virtual void handle_shutdown ()
        {
        }

        /**
         * @brief This function returns a Recognizer enabling recognition event.
         */
        virtual void handle_enable_recognition ()
        {
        }

        /**
         * @brief This function returns a Recognizer disabling recognition event.
         */
        virtual void handle_disable_recognition ()
        {
        }

        /**
         * @brief This function returns a start of speech event.
         */
        virtual void handle_start_of_speech ()
        {
        }

        /**
         * @brief This function returns an end of speech event.
         */
        virtual void handle_end_of_speech ()
        {
        }

        /** 
         * @brief This is a callback function to handle a speech
         * detected event, with no recognition result provided
         * by the engine. This means that there was only junk captured
         * by the microphone, but this junk had speech characteristics,
         * therefore the user may have tried to say something.
         *
         * @param result [in]; A reference to some statistics concerning the
         *                     speech event
         */
        virtual void handle_speech_detected (const RecognizerResult &result)
        {
        }

        /**
         * @brief This handles partial or complete recognition event.
         *
         * @param is_complete [in]; Boolean flag indicating whether
         *                          recognition was partial or complete
         * @param result [in]; The result from the recognizer engine
         */
        virtual void handle_result (bool is_complete,
                                    const RecognizerResult &result)
        {
        }

        /**
         * @brief This function handles a partial or complete
         * recognition event, including a set of alternative
         * interpretations.
         *
         * @param is_complete [in]; Boolean flag indicating whether
         *                          recognition was partial or complete
         * @param result_set  [in]; The result from the recognizer engine
         */
        virtual void handle_result_set (bool is_complete,
                                        const RecognizerResultSet &result_set)
        {
        }

        /**
         * @brief This function handles an event when a grammar is
         * loaded or unloaded.
         *
         * @param grammar   [in]; The name of the grammar that this event
         *                        is associated with
         * @param is_active [in]; If the grammar is active, this is
         *                        set to true
         */
        virtual void handle_grammar_change (const char *grammar, bool is_active)
        {
        }

    }; // end class IRecognizerCallback

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_SPEECH_ISPEECH_RECOGNIZER_CALLBACK_HPP
