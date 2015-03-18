//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__IAUDIO_MIXER_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__IAUDIO_MIXER_HPP


/**
 * @file    core/resource/IAudioMixer.hpp
 * @brief   An interface for mixing multiple audio streams.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioMixer
 */

#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @interface IAudioMixer
     *
     * @brief
     *
     * The IAudioMixer class provides an interface to an audio mixer device.
     * This mixer device generally abstracts the analog volume controls that are
     * exposed by a sound card.
     *
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE IAudioMixer
    {
    public: // Constants

        /**
         * @brief Interface ID
         */
        static const char* const INTERFACE_ID; // Evolution.IAudioMixer

    public: // Types

        /// The audio input source
        enum InputSource
        {
            MIC_INPUT=0,  ///< Microphone input
            LINE_INPUT    ///< Line input
        };

    public: // Structors

        /**
         * @brief Empty constructor
         */
        IAudioMixer () {};

        /**
         * @brief Empty destructor
         */
        virtual ~IAudioMixer () {};

    public: // Mixers

        // Methods to set commonly used mixer settings

        /**
         * @brief This function sets the master output level, on a scale from 0 to 1.
         * @param level [in]; the level, {0,1}.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result set_master_output_level (const double level) = 0;

        /**
         * @brief This function gets the master output level, on a scale from 0 to 1.
         * @param level [out]; the level, {0,1}.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result get_master_output_level (double* level) = 0;

        /**
         * @brief This function sets the PCM output level, on a scale from 0 to 1.
         * @param level [in]; the level, {0,1}.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result set_pcm_output_level (const double level) = 0;

        /**
         * @brief This function gets the PCM output level, on a scale from 0 to 1.
         * @param level [out]; the level, {0,1}.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result get_pcm_output_level (double* level) = 0;

        /**
         * @brief This function sets the recording input level, on a scale from 0 to 1.
         * @param level [in]; the level, {0,1}.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result set_input_level (const double level) = 0;

        /**
         * @brief This function gets the recording input level, on a scale from 0 to 1.
         * @param level [out]; the level, {0,1}.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result get_input_level (double* level) = 0;

        /**
         * @brief This function switches the input source to MIC IN.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result set_input_source_mic () = 0;

        /**
         * @brief This function switches the input source to LINE IN.
         * @return RESULT_SUCCESS when successful.
         */
        virtual Result set_input_source_line () = 0;

    }; // end class IAudioMixer


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE__IAUDIO_MIXER_HPP
