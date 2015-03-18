//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/LengthPrefixedProtocol.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::LengthPrefixedProtocol
 */

#ifndef EVOLUTION_LIBRARIES_BASE_LENGTH_PREFIXED_PROTOCOL__HPP
#define EVOLUTION_LIBRARIES_BASE_LENGTH_PREFIXED_PROTOCOL__HPP

#include <evolution/core/base/Types.hpp>
#include <evolution/core/util/ISocketProtocol.hpp>

namespace Evolution
{
    /**
     * @class LengthPrefixedProtocol
     * @brief This implements a protocol where every data packet contains a
     * 4-byte length prefix, stored in network byte order. 
     *
     * This protocol is useful for binary data and is used by
     * the AudioVideoServer/Client behaviors.
     * 
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL LengthPrefixedProtocol : public ISocketProtocol
    {
    public: // Structors
        /**
         * @brief Default constructor
         */
        LengthPrefixedProtocol();
        
    public: //ISocketProtocol Interface
        /**
         * @brief Implement this to encode a buffer of specified size.
         *
         * This method encodes a buffer.  The encoded data is stored
         * internally and made available via the get_encode_buffer()
         * call. If the encoding process failed, this method must
         * ensure that the get_encode_buffer() call returns 0. 
         *
         * @param   buffer [in]; the buffer to encode.
         * @param   buffer_size [in]; the size of the buffer to encode.
         *
         * @return  A ProtocolResult value describing the result of 
         *          encoding process.
         */ 
        virtual ProtocolResult  encode(const char* buffer, size_t buffer_size);
        
        /**
         * @brief Implement this to decode a buffer of specified size.
         *
         * This method decodes a buffer.  The decoded data is stored
         * internally and made available via the get_decode_buffer()
         * call.  If the decoding process failed, this method must
         * ensure that the get_decode_buffer() call returns 0.
         *
         * @param   buffer [in]; the buffer to decode.
         * @param   buffer_size [in]; the size of the buffer to decode.
         *
         * @return  A ProtocolResult value describing the result of 
         *          decoding process.
         */ 
        virtual ProtocolResult  decode(const char* buffer, size_t buffer_size);


    };
}

#endif
