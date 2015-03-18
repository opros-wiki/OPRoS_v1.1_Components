//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/ISocketProtocol.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::ISocketProtocol
 * @see Evolution::NullProtocol
 */

#ifndef EVOLUTION_LIBRARIES_BASE_SOCKET_PROTOCOL_HPP
#define EVOLUTION_LIBRARIES_BASE_SOCKET_PROTOCOL_HPP

#include <stdlib.h>
#include <list>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Platform.hpp>
#include <evolution/core/base/ResultCodes.hpp>

/**
 * @def ERSP_EINPROGRESS
 * @brief Portable EINPROGRESS (see the man pages).
 */

/**
 * @typedef UINT32
 * @brief Portable unsigned 32 bytes integer
 */

#ifdef EVOLUTION_PLATFORM_WIN32
#include <winsock.h>
typedef unsigned __int32 UINT32;
typedef char* SockOptData;
typedef unsigned long in_addr_t;
#define SIGPIPE 13
extern "C" {
    EVOLUTION_EXPORT_UTIL void evolution_initialize_windows_sockets ();
}

/// Declare to initialize sockets.
#define EVOLUTION_USE_SOCKETS   evolution_initialize_windows_sockets()
//   Disable the gazillion warnings about template expansions too big for debugger labels.
//   Like we care that microsoft included yet another arbirtrary limitation....Whaaaaa
#    pragma warning(disable : 4786)
#define ERSP_EINPROGRESS WSAEWOULDBLOCK
#else
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef u_int32_t UINT32;
typedef sockaddr SOCKADDR;
typedef sockaddr_in SOCKADDR_IN;
typedef int      SOCKET;
typedef hostent* LPHOSTENT;
typedef in_addr* LPIN_ADDR; 
typedef void* SockOptData;
#define INVALID_SOCKET -1  ///< Invalid socket notification
#define SOCKET_ERROR -1    ///< Socket error notification
#define closesocket close
/// Declare to initialize sockets (does nothing on Linux)
#define EVOLUTION_USE_SOCKETS
#define ERSP_EINPROGRESS EINPROGRESS  
#endif

namespace Evolution 
{

    /**
     * @interface ISocketProtocol
     * @brief
     * An interface defining data encoding and decoding for a 
     * network protocol. 
     *
     * A protocol object is associated with each TCP/IP network 
     * communication object in ERSP.  It defines how the data
     * will be formated for sending over the TCP/IP network.
     *
     * Objects like TCPConnection and TCPServer have a 
     * create_protocol virtual method that allow users to 
     * specify a particular implementation of ISocketProtocol
     * to use for formating the data.
     *
     * Existing examples of ISocketProtocol implementations are
     * LengthPrefixedProtocol, which appends a 4-byte length
     * prefix to all data sent over the network, and 
     * StringTerminatedProtocol, which allow the specification of
     * a string indicating the end of a data packet.  The latter
     * is useful for text-based messages and is used my the 
     * MalleableBehavior.  The former is good for binary data
     * and is used by the like of AudioVideoServerBehavior.
     * 
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL ISocketProtocol 
    {
    public: // Types
        /**
         * @brief Result values and error codes the protocol interface generates.
         */
        typedef enum {
            SUCCESS,
            SUCCESS_MORE_DATA,
            PARTIAL_DECODE,
            ENCODE_ERROR,
            DECODE_ERROR
        } ProtocolResult;
    
        /**
         * @internal
         * @brief
         * The buffer used internally to store decoded data.
         * 
         * @ingroup core_util
         */
        struct DecodeBuffer 
        {
            DecodeBuffer() : data(0), size(0) {}
            ~DecodeBuffer() { delete[] data; }

            char*   data;
            size_t  size;    
        };
        
        typedef std::list<DecodeBuffer*> DecodeList;
            
    public: // Structors
        /**
         * @brief Default constructor.
         */
        ISocketProtocol();
        
        /**
         * @brief Destructor.
         */
        virtual ~ISocketProtocol();

    public: // Interface methods
        
        /**
         * @brief Implement to encode a buffer of specified size.
         *
         * This method encodes a buffer.  The encoded data is stored
         * internally and made available via the get_encode_buffer()
         * call. This method must call clear_encode_buffer() before
         * doing anything.
         *
         * @param   buffer [in]; the buffer to encode.
         * @param   buffer_size [in]; the size of the buffer to encode.
         *
         * @return  A ProtocolResult value describing the result of 
         *          encoding process.
         */ 
        virtual ProtocolResult  encode(const char* buffer, size_t buffer_size) = 0;
        
        /**
         * @brief Implement to decode a buffer of specified size.
         *
         * This method decodes a buffer.  The decoded data is stored
         * internally and made available via the get_decode_buffer()
         * call.  This method must be implemented for each new protocol.
         * The implementer can assume that the buffer passed in will
         * always begin with the beginning of a well-formed message.
         * This method must do the following things:
         *
         * 1) If the buffer does not contain a complete message,
         *    return ISocketProtocol::PARTIAL_DECODE.
         * 2) If the buffer contain a complete message, call the
         *    copy_decode method with the decoded message and its
         *    size.  Also, the _decode_count member must be set to
         *    the size of the buffer data used to decode the message.
         *    This value varies with the protocol, and can be but 
         *    is not necessarily the size of the decoded message.
         *    Finally, return ISocketProtocol::SUCCESS to indicate
         *    that a message has been successfully decoded.
         *
         * @param   buffer [in]; the buffer to decode.
         * @param   buffer_size [in]; the size of the buffer to decode.
         *
         * @return  A ProtocolResult value describing the result of 
         *          decoding process.
         */ 
        virtual ProtocolResult  decode(const char* buffer, size_t buffer_size) = 0;

        /**
         * @brief Decode a buffer, returning a list of heap-
         * allocated DecodeBuffer structures, each containing a disctinct
         * message parsed from the buffer.
         *
         * On success, all distinct messages found in the specified buffer
         * are returned as elements of the DecodeList parameter.  Any partial
         * message at the end of the buffer is stored for use with the next
         * decode call.  Users should always use this function to decode
         * a buffer instead of the decode() call, which always assume that
         * the passed in buffer begins with a well-formed message. Note that
         * The elements of the decode list are allocated on the heap and must 
         * be deleted after use. 
         *
         * @param   buffer [in]; the buffer to encode.
         * @param   buffer_size [in]; the size of the buffer to encode.
         * @param   decode_list [out]; the list of decoded objects.
         *
         * @return  A ProtocolResult value describing the result of 
         *          encoding process.
         */
        ProtocolResult decode_list(const char* buffer, size_t buffer_size, DecodeList* decode_list);
           
        
        /**
         * @brief Returns the encoded data.
         * 
         * Returns the pointer to a buffer containing encoded data if the
         * previous call to the encode() was successful or 0 if not. The
         * protocol object is responsible for the life cycle of the 
         * encoded data buffer.
         *
         * @param   size    [out]; The size of the encoded data.
         *
         * @return  Pointer to encoded data if previous encode() call 
         *          succeeded or 0 if not.
         */
        const char*     get_encode_buffer(size_t* size)
        {
            *size = _encode_size;
            return _encode_buffer;
        }
        
        /**
         * @brief Returns the decoded data.
         * 
         * Returns the pointer to a buffer containing decoded data if the
         * previous call to the decode() was successful or 0 if not. The
         * protocol object is responsible for the life cycle of the 
         * decoded data buffer.
         *
         * @param   size    [out]; The size of the decoded data.
         *
         * @return  Pointer to decoded data if previous decode() call 
         *          succeeded or 0 if not.
         */
        const char*     get_decode_buffer(size_t* size)
        {
            *size = _decode_size;
            return _decode_buffer;
        }
            

    protected:
        // Helpers.
        /**
         * @brief Copy the decoded message to the decode buffer.
         *
         * The decode() method must call this method with each
         * successfully decoded message.
         *
         * @param copy_msg  [in]; Pointer to the decoded message.
         * @param copy_size [in]; The size of the decoded message.
         */
        
        void copy_decode(const char* copy_msg, size_t copy_size);
        
        /**
         * @brief Clear the encode buffer.
         *
         * Must be called at the beginning of encode().
         */
        
        void clear_encode_buffer();
        
        /**
         * @brief Clear the decode buffer.
         *
         * Must be called at the beginning of decode().
         */
        void clear_decode_buffer();

        // Message accounting.
        size_t _cur_read;           ///< The length of the partial message.
        size_t _decode_size;        ///< The size of the decoded message.
        size_t _encode_size;        ///< The size of the encoded message.
        size_t _decode_count;       ///< The number of bytes decoded by the decode() method.
        char*  _cur_msg;            ///< Pointer to the cached partial message.
        char*  _encode_buffer;      ///< Pointer to the encode buffer.   
        char*  _decode_buffer;      ///< Pointer to the decode buffer.
        
        IMutex* _mutex;
    };
        

    /**
     * @class NullProtocol
     * @brief
     * Implements a null protocol that does not do any data packaging or translation.
     */
    class EVOLUTION_EXPORT_UTIL NullProtocol : public ISocketProtocol
    {
    public: // Structors
        /**
         * @brief Default constructor
         */
        NullProtocol();

    public: // ISocketProtocol.
        
        /**
         * @brief Implement to encode a buffer of specified size.
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
        virtual ProtocolResult  encode(const char* buffer, size_t buffer_size) ;
    
        /**
         * @brief Implement to decode a buffer of specified size.
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
