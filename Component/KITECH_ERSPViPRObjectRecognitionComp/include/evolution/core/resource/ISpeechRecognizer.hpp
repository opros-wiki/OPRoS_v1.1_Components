//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_SPEECH_ISPEECH_RECOGNIZER_HPP
#define INCLUDE_EVOLUTION_SPEECH_ISPEECH_RECOGNIZER_HPP


/**
 * @file    evolution/core/resource/ISpeechRecognizer.hpp
 * @brief   This class provides an interface for a speech recognizer.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ISpeechRecognizer
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/ISpeechRecognizerCallback.hpp>


namespace Evolution
{


    /**
     * @interface ISpeechRecognizer
     *
     * @brief This class provides an abstract interface to a speech recognizer, controlling
     * the (de)activation of recognition, the (un)loading of grammars,
     * and the propagation of events to registered callbacks.
     *
     * This class virtually implements a client for the recognizer engine.
     * The speech-enabled application needs to instantiate an 
     * ISpeechRecognizer object in order to communicate with the recognition
     * engine. The application receives the information asynchronously via 
     * the callback function that is passed in the constructor of the class.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    class EVOLUTION_EXPORT_RESOURCE ISpeechRecognizer
    {
    public:                  // Types

        /**
         * @brief This list contains the speech audio source type.
         */
        enum SourceType
        {
            SOURCE_MICROPHONE,
            SOURCE_FILE
        };

    public:                  // Constants

        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.ISpeechRecognizer"

    public:                  // Structors

        /**
         * @brief Constructor
         */
        ISpeechRecognizer (  )
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ ISpeechRecognizer (  )
        {
        }

    public:                  // Grammars

        /**
         * @brief This function obtains a list of local grammars currently available to the
         * recognizer.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  grammar_list [out]; the list of currently available grammars.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result get_available_grammars ( TicketId ticket,
                                                GrammarList *
                                                grammar_list ) = 0;

        /**
         * @brief This function loads the specified local grammar.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  grammar [in]; The grammar to be loaded.
         * @param  grammar_name [in]; The name for the grammar to be loaded.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result load_grammar ( TicketId ticket, const char *grammar,
                                      const char *grammar_name ) = 0;

        /**
         * @brief This function unloads the specified local grammar.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  grammar_name [in]; The name of the grammar to unload.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result unload_grammar ( TicketId ticket,
                                        const char *grammar_name ) = 0;

        /**
         * @brief This function enables the specified local grammar
         *   that has already been loaded using the load_grammar()
         *   method.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param grammar_name [in]; The name of the grammar to be enabled.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result enable_grammar ( TicketId ticket,
                                        const char *grammar_name ) = 0;

        /**
         * @brief This function disables a specified local grammar
         *    that has been previous loaded by the load_grammar() 
         *    method and enabled by the enable_grammar() method.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param grammar_name [in]; The name of the grammar to be disabled.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result disable_grammar ( TicketId ticket,
                                         const char *grammar_name ) = 0;

    public:                  // Recognition

        /**
         * @brief This function enables recognition (e.g. mic is turned on).
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result enable_recognition ( TicketId ticket ) = 0;

        /**
         * @brief This function disables recognition (e.g. mic is turned off).
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result disable_recognition ( TicketId ticket ) = 0;

        /**
         * @brief This starts processing the recognition results.
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result run ( TicketId ticket ) = 0;

        /**
         * @brief This function checks whether the engine is running.
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual bool is_running ( TicketId ticket ) = 0;

    public:                  // Callbacks

        /**
         * @brief Sets the callback associated with the given speech event.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; The callback to be associated with the event.
         * @param  event_type [in]; The type of event with which the callback
         *                          is associated.
         *
         * @result RESULT_SUCCESS if successful.
         */
        virtual Result set_callback ( TicketId ticket,
                                      ISpeechRecognizerCallback * callback,
                                      RecognizerEventType event_type ) = 0;

    };                          // end class ISpeechRecognizer


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_SPEECH_ISPEECH_RECOGNIZER_HPP
