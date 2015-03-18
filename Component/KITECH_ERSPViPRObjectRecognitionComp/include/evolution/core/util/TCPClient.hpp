//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/TCPClient.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::ISocketProtocol
 * @see Evolution::TCPClient
 */

#ifndef EVOLUTION_LIBRARIES_BASE_TCP_CLIENT__HPP
#define EVOLUTION_LIBRARIES_BASE_TCP_CLIENT__HPP

#include <list>
#include <evolution/core/util/ISocketProtocol.hpp>
#include <evolution/core/base/IMutex.hpp>
#include <evolution/core/base/IThread.hpp>
#include <evolution/core/base/RefPtr.hpp>

namespace Evolution {

    class ISocketProtocol;

    /**
     * @class TCPClient
     * @ingroup core_util
     * @brief
     * Encapsulate a client connection to a SocketServer.
     */
    class EVOLUTION_EXPORT_UTIL TCPClient
    {
    public: // Structors

        /**
         * @brief Constructor
         *
         * @param protocol  [in] Pointer to a protocol object created by
         *                       the server.  This protocol object will
         *                       be destroyed by the TCPClient
         *                       instance's destructor.
         * @param s         [in] The accepted socket.
         * @param addr      [in] The accepted connection's remote address.
         */
        TCPClient(ISocketProtocol* protocol, SOCKET s, sockaddr* addr);

        void add_ref();
        void remove_ref();

        /**
         * @brief  This is a pointer to the client's communication protocol interface
         */
        ISocketProtocol* _protocol;

        /**
         * @brief  This is the connection socket
         */
        SOCKET          _socket;

        /**
         * @brief  This is the connection's remote address.
         */
        sockaddr_in     _addr;

        /**
         * @brief  This is the string form of the remote address.
         */
        String          _saddr;

        /**
         * @brief  This is the robot id.
         */
        String          _robot_id;

        /**
         * @brief  invalid
         */
        bool            _invalid;

        /**
         * @brief  Send mutex.
         */
        IMutex         *_send_mutex;
        /**
         * @brief Send result.
         */
        //ThreadProcResult _send_result;
    protected:
        /**
         * @brief
         * Destructor.
         */
        virtual ~TCPClient();

    private:
        unsigned _ref_count;

    };

    typedef RefPtr<TCPClient> TCPClientPtr;
    typedef std::list<TCPClientPtr> TCPClientList;
}

#endif
