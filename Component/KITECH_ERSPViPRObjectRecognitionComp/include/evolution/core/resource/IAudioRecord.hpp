//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_RECORD_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_RECORD_HPP


/**
 * @file    evolution/core/resource/IAudioRecord.hpp
 * @brief   An interface for recording audio.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioRecord
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/AudioTypes.hpp>
#include <evolution/core/resource/IAudioBase.hpp>

namespace Evolution
{

    /**
     * @interface IAudioRecord
     *
     * @brief
     *
     * This class defines an interface to an object that can sample audio data
     * and place it into a series of audio buffers.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAudioRecord:public IAudioBase
    {
    public:                  // Types

        /**
         * @brief The CaptureMode var lists the modes of operation available for audio capture.
         */
        enum CaptureMode
        {
            MODE_ON_DEMAND = 0, ///< The application requests audio for processing
            MODE_STREAMING      ///< The application requests audio to be placed on a given buffer as soon as there is audio available
        };

    public:                  // Constants

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IAudioRecord"

    public:                  // Structors

        /**
         * @brief Empty constructor
         */
        IAudioRecord (  )
        {
        }

        /**
         * @brief Empty destructor
         */
        virtual ~ IAudioRecord (  )
        {
        }

    public:                  // Setting
        /**
         * @brief Sets the capture mode.
         *
         * @param  ticket [in]: The (TicketId) for security purposes.
         * @param  mode [in]: desired capture mode
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_capture_mode ( TicketId ticket,
                                          CaptureMode mode ) = 0;

        /**
         * @brief Sets the capture mode.
         *
         * @param  ticket [in]: The (TicketId) for security purposes.
         * @param  mode [in]: desired capture mode
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_capture_mode ( TicketId ticket,
                                          const char *mode ) = 0;

    public:                  // Audio capture
        /**
         * @brief The start_audio_record function makes sure that the audio device
         * has cleaned up its internal audio capture device before starting to collect audio.
         * @param  ticket [in]; The security ticket
         * @return RESULT_SUCCESS if successful
         */
        virtual Result start_audio_record ( TicketId ticket ) = 0;

        /**
         * @brief The get_audio_buffer function copies the collected audio into
         * the given buffer. If there is more audio data
         * in the internal buffer of the audio device,
         * it would be copied in subsequent calls to the function.
         *
         * @param  ticket [in]; The security ticket
         * @param  buffer [in]; Buffer where audio would be copied
         * @param  bytes_requested [in]; Size of the buffer
         * @param  bytes_provided [out]; Actual number of bytes copied into the buffer
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_audio_buffer ( TicketId ticket,
                                          char *buffer,
                                          const int bytes_requested,
                                          int *bytes_provided ) = 0;

        /**
         * @brief The stop_audio_record function stops audio capture.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @return RESULT_SUCCESS if successful
         */
        virtual Result stop_audio_record ( TicketId ticket ) = 0;

    };                          // end class IAudioRecord


}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_RECORD_HPP
