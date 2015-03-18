//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/SocketUtil.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


#ifndef EVOLUTION_LIBRARIES_BASE_SOCKET_UTIL_HPP
#define EVOLUTION_LIBRARIES_BASE_SOCKET_UTIL_HPP

#include <evolution/core/base/Types.hpp>
#include <evolution/core/util/ISocketProtocol.hpp>

#ifndef DOXYGEN_INTERNAL

namespace Evolution 
{

    /**
     * @class SocketUtil
     * @brief This class contains several utility methods for working with sockets.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL SocketUtil
    {
    public:
        /**
         * @brief This function prints out the last socket error.
         *
         * @param file [in]; The string containing the file where the 
         * socket error occurred.  The __FILE__ macro is usually passed
         * in here.
         *
         * @param line [in]; The line of code where the socket error 
         * occurred.  The __LINE__ macro is typically passed here.
         *
         * @ingroup core_util
         */
        static void last_socket_error(const char* file, int line);

        /**
         * @brief This function connects to an IP network address and port using a socket handle.  
         * The socket handle must describe a valid stream socket.
         *
         * @param s [in]; The socket handle to use for connecting to the 
         * remote address.
         *
         * @param address [in]; The IP address to connect to as a string in
         * either the Berkeley dot notation (x.x.x.x), or a address name 
         * string resolvable through DNS.
         *
         * @param port [in];  The port number to connect to.
         *
         * @param timeout_seconds [in];  How long (in seconds) to wait for a connection before timing out.
         *                                  If timeout_seconds is >= 0, we put the socket into non-blocking mode
         *                                  and wait up to timeout_seconds seconds for a connection to be made.
         *                                  If timeout_seconds is < 0, we retain the existing blocking state and
         *                                  rely on the Operating Sytem's internal timeout for failed connections.
         *
         * @return true if successful
         *
         * @ingroup core_util
         */ 
        static bool connect(SOCKET s, const char* address, unsigned short port, int timeout_seconds = -1 );
    };
}
    
/**
 * @brief This is a macro that calls last_socket_error with the appropriate file and line
 * number added.
 */
#define LAST_SOCKET_ERROR SocketUtil::last_socket_error(__FILE__, __LINE__)

#endif // ends DOXYGEN_INTERNAL

#endif
