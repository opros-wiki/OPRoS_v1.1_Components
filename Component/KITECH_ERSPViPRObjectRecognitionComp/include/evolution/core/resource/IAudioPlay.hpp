//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_PLAY_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_PLAY_HPP


/**
 * @file    evolution/core/resource/IAudioPlay.hpp
 * @brief   This class provides an interface for audio resource play back.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAudioPlay
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/resource/AudioTypes.hpp>
#include <evolution/core/resource/IAudioBase.hpp>
#include <evolution/core/resource/IAudioCallback.hpp>

namespace Evolution
{

    /**
     * @interface IAudioPlay
     *
     * @brief
     *
     * Provides an interface for audio playback. This class can read audio from
     * files. It can also play audio data that has been encapsulated within a
     * series of audio buffers.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAudioPlay:public IAudioBase
    {
    public: // Constants

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IAudioPlay"

    public: // Structors

        /**
         * @brief Empty default constructor
         */
        IAudioPlay (  )
        {
        }

        /**
         * @brief Empty default destructor
         */
        virtual ~ IAudioPlay (  )
        {
        }

    public:
        /**
         * @brief This function sets the file to be played, and is only for file playing.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  audio_file [in]; the audio file to be played.
         *
         * return RESULT_SUCCESS if successful.
         */
        virtual Result set_audio_file ( TicketId ticket,
                                        const char *audio_file ) = 0;

    public: // Callbacks (only used for file playing)

        /**
         * @brief This function sets the callback function needed to handle the given event.
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @param  callback [in]; the callback associated with the given audio
         *                        event.
         *
         * @param  event_type [in]; the type of the event.
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result set_callback ( TicketId ticket,
                                      IAudioCallback * callback,
                                      AudioEventType event_type ) = 0;

    public: // Audio play back
        /**
         * @brief This function makes sure that the audio device 
         * has cleaned up its internal audio play device 
         * before starting to play audio.
         * @param  ticket [in]: The security ticket
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result start_audio_play ( TicketId ticket ) = 0;

        /**
         * @brief This function copies the given audio buffer into 
         * the audio device. If there is more audio data 
         * than the accepted by the internal buffer of the audio device,
         * it would be copied in subsequent calls to the function.
         *
         * @param  ticket [in]: The security ticket
         * @param  buffer [in]: Buffer where audio would be copied
         * @param  bytes_requested [in]: Size of the buffer
         * @param  bytes_provided [out]: Actual number of bytes copied into the audio buffer. 
         *
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result put_audio_buffer ( TicketId ticket,
                                          char *buffer,
                                          const int bytes_requested,
                                          int *bytes_provided ) = 0;
        /**
         * @brief Function that clears any existing buffered audio.  The audio
         *        file set by the set_audio_file() call is copied into the
         *        audio output buffer.
         * @param  ticket [in]: The (TicketId) for security purposes.
         *
         * @return RESULT_SUCCESS if successful.
         * @todo implement this in driver/audiodriver/
         */
        virtual Result clear_audio_buffer (TicketId ticket) = 0;

        /**
         * @brief When this method is called, the audio located in the path set
         *        by the set_audio_file() call is copied into the audio output buffer.
         *
         * @param  ticket [in]: The (TicketId) for security purposes.
         *
         * @return RESULT_SUCCESS if successful.
         * @todo implement this in driver/audiodriver/
         */
        virtual Result load_audio_file (TicketId ticket) = 0;

        /**
         * @brief This function stops audio play.
         * @param  ticket [in]: The security ticket
         * @return RESULT_SUCCESS if successful.
         */
        virtual Result stop_audio_play ( TicketId ticket ) = 0;

    }; // end class IAudioPlay


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IAUDIO_PLAY_HPP
