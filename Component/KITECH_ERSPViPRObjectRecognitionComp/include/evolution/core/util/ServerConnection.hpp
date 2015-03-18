//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.



/**
 * @file    evolution/core/util/ServerConnection.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ServerConnection
 */


#ifndef ServerConnection__hpp
#define ServerConnection__hpp

#include <list>
#include <evolution/core/base/Types.hpp>

namespace Evolution {

    /**
     * @brief This class encapsulates a server connection, and is used internally
     * by TCPServer.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL ServerConnection
    {
    public:
        /**
         * @brief Constructor
         *
         * @param s [in]; The socket file descriptor of the server
         * connection.
         *
         * @param addr [in]; Pointer to a sockaddr structure that contains
         * the network address of the server connection.
         */
        ServerConnection(SOCKET s, sockaddr* addr);
        /**
         * @brief Destructor
         */
        virtual ~ServerConnection();

        SOCKET      _socket;        ///< The socket handle of the server connection.
        sockaddr_in _addr;          ///< The address of the server connection.
        long        _state;         ///< The state of the connection.
        String      _saddr;         ///< The network address of the connection in string format.
        String      _robot_id;      ///< The robot id of the connection.
    };
    /**
     * @brief This is a list of server connections.
     *
     * @ingroup core_util
     */
    typedef std::list<ServerConnection*> ServerConnectionList;
}

#endif // ServerConnection__hpp
