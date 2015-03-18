//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_SPEECH_TTS_TYPES_HPP
#define INCLUDE_EVOLUTION_SPEECH_TTS_TYPES_HPP


/**
 * @file    evolution/core/resource/TTSTypes.hpp
 * @brief   Basic types for TTS
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


namespace Evolution
{

    /**
     * @brief This class provides a list of TTS events in which a callback 
     * may register interest.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    enum TTSeventFlags
    {
        TTS_EVENT_INITIALIZATION             = 0x01,
        TTS_EVENT_SHUTDOWN                   = 0x02,
        TTS_EVENT_SYNCHRONIZATION            = 0x04,
        TTS_EVENT_FINISHED_PLAYING_UTTERANCE = 0x08
    };


    /**
     * @brief A set of TTSeventFlags, ORed together
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    typedef unsigned long TTSeventType ;

    /**
     * @brief Type of sources used to provide the text to be synthesized.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    enum SourceType
    {
        SOURCE_TEXT,
        SOURCE_FILE
    };

    
    /**
     * @brief Type of source: SOURCE_TEXT or SOURCE_FILE as defined in
     * the enumeration SourceType.
     *
     * @ingroup core_resource
     * @ingroup hri_speech
     */
    typedef unsigned long TTSSourceType;

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_SPEECH_TTS_TYPES_HPP
