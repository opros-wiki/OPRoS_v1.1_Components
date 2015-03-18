//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IAUDIO_BASE_HPP
#define INCLUDE_EVOLUTION_CORE_IAUDIO_BASE_HPP


/**
 * @file    core/resource/IAudioBase.hpp
 * @brief   Defines the basic interface to audio devices.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioBase 
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/AudioTypes.hpp>

namespace Evolution
{

    /**
     * @interface IAudioBase 
     *
     * @brief This class defines an interface to an audio device.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAudioBase 
    {
    public: // Structors

        /**
         * @brief Empty constructor
         */
        IAudioBase () {}

        /**
         * @brief Empty destructor
         */
        virtual ~IAudioBase () {}

    public: // Setting
        /** 
         * @brief The set_sample function sets the sample format.
         * @param  ticket [in]; The security ticket
         * @param  sample_format [in]; Desired sample format
         * @return RESULT_SUCCESS if successful.
         * @param  ticket [in]; The (TicketId) for security purposes.
         */
        virtual Result set_sample_format (TicketId ticket, 
                                          SampleFormat sample_format) = 0;
        /** 
         * @brief The set_sample function sets the sample format.
         * @param  ticket [in]; The security ticket
         * @param  sample_format [in]; Desired sample format
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_sample_format (TicketId ticket, 
                                          const char * sample_format) = 0;
        /** 
         * @brief The set_number_of_channels function sets the number of audio channels.
         * @param  ticket [in]; The security ticket
         * @param  number_of_channels [in]; Desired number of audio channels
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_number_of_channels (TicketId ticket, 
                                               const int number_of_channels) = 0;
        /** 
         * @brief The set_sampling_rate function sets the sampling frequency for the sound card.
         * @param  ticket [in]; The security ticket
         * @param  sampling_rate [in]; Desired sampling frequency
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_sampling_rate (TicketId ticket, 
                                          const int sampling_rate) = 0;

        /** 
         * @brief The set_block_size function sets the desired size of a block of audio. 
         * The application using the audio driver would obtain a 
         * number of bytes that is an integer multiple of the 
         * block size. If this parameter is not specified, 
         * the driver would use the fragment size as the block 
         * size (the fragment size is the size of the internal 
         * buffers in the circular ring of the sound card).
         *
         * @param  ticket [in]; The security ticket
         * @param  block_size [in]; Desired block size
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_block_size (TicketId ticket, 
                                       const int block_size) = 0;

        /** 
         * @brief The set_audio_device function sets the audio device to be opened 
         * for reading audio input. The default value is /dev/dsp 
         * in the case of live audio or the name of the file 
         * to be opened when reading from file.
         *
         * @param  ticket [in]; The security ticket
         * @param  audio_device [in]; Desired audio device
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_audio_device (TicketId ticket, 
                                         const char * audio_device) = 0;

    public: // Get methods
        /**
         * @brief The get_audio_device_handle function obtains the audio device handle
         * @param  ticket [in]; The security ticket
         * @param  audio_handle [out]; Desired audio device handle
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_audio_device_handle (TicketId ticket, 
                                                int * audio_handle) = 0;

        /**
         * @brief The get_fragment_size function obtains the fragment size used 
         * internally by the soundcard. The fragment size 
         * could be used by the application to allocate memory 
         * in order to read full fragments for better real-time 
         * performance.
         *
         * @param  ticket [in]; The security ticket
         * @param  fragment_size [out]; Size of fragment used by sound card.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_fragment_size (TicketId ticket, 
                                          int * fragment_size) = 0;

        /**
         * @brief The get_sampling_rate function obtains the current sampling rate.
         * @param  ticket [in]; The security ticket
         * @param  sampling_rate [in]; Current sampling rate
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_sampling_rate (TicketId ticket, 
                                          int * sampling_rate) = 0;

        /**
         * @brief The get_number_of_channels function obtains the current number of channels.
         * @param  ticket [in]; The security ticket
         * @param  number_of_channels [out]; Current number of channels
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_number_of_channels (TicketId ticket, 
                                               int * number_of_channels) = 0;

        /**
         * @brief The get_sample_format function obtains the current sample format.
         * @param  ticket [in]; The security ticket
         * @param  sample_format [out]; Current sample format
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result get_sample_format (TicketId ticket, 
                                          SampleFormat * sample_format) = 0;

    public: // Device open and closing
        /** 
         * @brief The open_device function opens the audio device for capture.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result open_device (TicketId ticket) = 0;

        /** 
         * @brief The close_device function closes the audio device.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result close_device (TicketId ticket) = 0;

    }; // end class IAudioBase


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IAUDIO_BASE_HPP

