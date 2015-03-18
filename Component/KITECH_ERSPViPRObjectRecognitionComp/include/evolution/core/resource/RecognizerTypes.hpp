//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_SPEECH_RECOGNIZER_TYPES_HPP
#define INCLUDE_EVOLUTION_SPEECH_RECOGNIZER_TYPES_HPP


/**
 * @file    evolution/core/resource/RecognizerTypes.hpp
 * @brief   This class defines basic types for speech recognition.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see     Evolution::RecognizerResult
 */


#include <vector>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @class RecognizerResult
     *
     * @brief This defines some results from speech recognition.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    struct EVOLUTION_EXPORT_RESOURCE RecognizerResult
    {
        String text;         ///< The recognized text.
        String meaning;      ///< The interpreted meaning of the text.
        String status;       ///< Status of the recognized text: Accepted or Rejected
        double confidence;   ///< The confidence in the result.
    }; // end struct RecognizerResult


    /**
     * @brief This list defines a sequence of recognition results, used for a set of result
     * alternatives.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    typedef std::vector<RecognizerResult> RecognizerResultSet;


    /**
     * @brief This is a list of Recognizer events in which a callback may register interest.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    enum RecognizerEventFlags
    {
        RECOGNIZER_EVENT_INITIALIZATION       = 0x01,
        RECOGNIZER_EVENT_SHUTDOWN             = 0x02,
        RECOGNIZER_EVENT_START_SPEECH         = 0x04,
        RECOGNIZER_EVENT_END_SPEECH           = 0x08,
        RECOGNIZER_EVENT_RESULT               = 0x10,
        RECOGNIZER_EVENT_RESULT_SET           = 0x20,
        RECOGNIZER_EVENT_GRAMMAR_CHANGE       = 0x40,
        RECOGNIZER_EVENT_ENABLED_RECOGNITION  = 0x80,
        RECOGNIZER_EVENT_DISABLED_RECOGNITION = 0x12,
        RECOGNIZER_EVENT_GENERIC              = 0x14,
        RECOGNIZER_EVENT_SPEECH_DETECTED      = 0x18
    };


    /**
     * @brief This represents a set of RecognizerEventFlags, ORed together.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    typedef unsigned long RecognizerEventType;


    /**
     * @brief This is a list of grammar names, used to enumerate the available grammars.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    typedef std::vector<String> GrammarList;

    /** 
     * @brief Label indicating unsuccessful speech decoding 
     * throughout the speech-related drivers, behaviors, and applications.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    EVOLUTION_EXPORT_RESOURCE extern const char* const ASR_TEXT_REJECTED;

    /** 
     * @brief Label indicating successful speech decoding
     * throughout the speech-related drivers, behaviors, and applications.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    EVOLUTION_EXPORT_RESOURCE extern const char* const ASR_TEXT_ACCEPTED;

    /** 
     * @brief Label indicating successful speech detection
     * throughout the speech-related drivers, behaviors, and applications.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    EVOLUTION_EXPORT_RESOURCE extern const char* const ASR_SPEECH_DETECTED;

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_SPEECH_RECOGNIZER_TYPES_HPP
