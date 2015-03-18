//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CHANNEL_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CHANNEL_HPP

/**
 * @file    evolution/core/resource/IAudioChannel.hpp
 * @brief   Interface for an audio channel.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioChannel
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/AudioTypes.hpp>
#include <evolution/core/resource/IAudioCallback.hpp>

namespace Evolution
{

    /**
     * @interface IAudioChannel
     *
     * @brief
     * Interface to an audio 'channel.' An audio channel is supposed to be
     * conceptually similar to a wire in a patchbay: you can connect it to a
     * source, and connect the other end to a mixer device, record device, or
     * whatever you can think of.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAudioChannel
    {
    public:                  // Constants

        /// ID of this interface.
        static const char* const INTERFACE_ID; // "Evolution.IAudioChannel"

    public:                  // Structors

        /// Empty constructor.
        IAudioChannel (  )
        {
        }

        /// Empty destructor.
        virtual ~ IAudioChannel (  )
        {
        }

    public:                  // Callbacks (only used for file playing)

        /**
         * @brief This function sets the callback function needed to handle the given event.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  callback [in]; Callback that handles the specified event.
         * @param  event_type [in]; The type of event that the callback handles.
         */
        virtual Result set_callback ( TicketId ticket,
                                      IAudioCallback * callback,
                                      AudioEventType event_type ) = 0;
    public:
        /**
         * @brief Returns the sampling rate of the audio channel.
         *
         * @return The epected sampling rate of the audio channel.
         */
        virtual long get_sample_rate () = 0;

        /**
         * @brief Returns the buffer or fragment size of the audio channel.
         *
         * @return The buffer length or fragment size of the audio channel.
         */
        virtual size_t get_buffer_size () = 0;


        /**
         * @brief Registers the channel object with the audio server.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result register_channel () = 0;

        /**
         * @brief Pauses the audio channel, halting playback. Audio samples
         *        are allowed to be queued.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result pause_channel () = 0;

        /**
         * @brief Allows the channel to send audio to the audio output device.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result run_channel () = 0;

        /**
         * @brief Sets the output normilization factor (volume) for a given audio channel.
         * @param left_volume [in];  A normalization factor, {0,1} applied to the left channel.
         * @param right_volume [in]; A normalization factor, {0,1} applied to the right channel.
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_channel_volume(float left_volume, float right_volume) = 0;

        /**
         * @brief Unregisters the channel object from the audio server.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result unregister_channel () = 0;

        /**
         * @brief Enqueues an audio buffer into the channel's queue.
         *
         * @param buffer [in]: Floating-point stereo-interleaved audio buffer.
         * @param size   [in]: The size of the buffer, in bytes.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result enqueue_buffer(void* buffer, size_t size) = 0;

        /**
         * @brief Removes an audio buffer from the channel's queue.
         *
         * @param buffer [out]: Floating-point stereo-interleaved audio buffer, allocated by
         *                    the AudioChannel object.
         * @param size   [out]: The size of the allocated buffer, in bytes.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result dequeue_buffer(void** buffer, size_t* size) = 0;

        /**
         * @brief returns true if the internal buffer is empty.
         *
         * @return true if internal audio buffer is empty.
         */
        virtual bool queue_is_empty() = 0;

        /**
         * @brief
         *
         * Returns the average power of the audio signal. This power is
         * normalized with respect to peak power. The level returned can vary
         * between zero (silence or no audio power) to one (peak power).
         *
         * @param level [out]: Relative maximum power of the audio channel.
         * @return RESULT_SUCCESS, if successful.
         */
        virtual Result get_audio_level (double* level) = 0;

        /**
         * @brief Obtains a copy of the audio server's buffer. This copy is
         *        placed into a user-supplied buffer. Note that the
         *        user-supplied buffer must be large enough to hold the audio
         *        server's buffer.  Use the get_buffer_size() function to
         *        determine the audio server's buffer length.
         *
         * @param buffer [out]: Floating-point stereo-interleaved
         * audio buffer, allocated by the AudioChannel object.
         * @param bytes_copied [out]: The size of the allocated
         * buffer, in bytes.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result copy_into_buffer(void* buffer, size_t* bytes_copied) = 0;

        /**
         * @brief Clears all audio buffers from the queue
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result clear_buffers() = 0;


    };                          // end class IAudioChannel
}                               // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_CHANNEL_HPP

