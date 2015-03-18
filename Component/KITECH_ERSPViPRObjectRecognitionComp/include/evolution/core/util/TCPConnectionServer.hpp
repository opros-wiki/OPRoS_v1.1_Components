//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/TCPConnectionServer.hpp
 * @brief   This class implements a TCP server for network communication.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::TCPConnectionServer
 * @see Evolution::ListenThread
 */

#ifndef EVOLUTION_LIBRARIES_BASE_TCP_CONNECTION_SERVER_HPP
#define EVOLUTION_LIBRARIES_BASE_TCP_CONNECTION_SERVER_HPP

#include <list>
#include <memory>
#include <evolution/core/util/ISocketProtocol.hpp>
#include <evolution/core/util/SignalThread.hpp>
#include <evolution/core/util/TCPConnection.hpp>
#include <evolution/core/util/ThreadPool.hpp>

namespace Evolution {

    /// Convenience typedef for a list TCP connections
    typedef std::list<TCPConnectionPtr> TCPConnectionList;

    /**
     * @class TCPConnectionServer
     * 
     * @brief A class implementing a socket-based server.
     *
     * So what is the difference between a TCPConnectionServer and 
     * a TCPServer?
     *
     * First of all, a TCPConnectionServer uses TCPConnections,
     * whereas a TCPServer uses TCPClients.  This is a big difference,
     * because TCPConnections have two threads each for sending and
     * receiving - so you shouldn't use this class for a server
     * that has many clients.
     *
     * Second of all, the TCPConnectionServer has no send_msg()
     * function, since you can send messages using the TCPConnections
     * directly.
     *
     * Otherwise, the interface is very similar.
     * 
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL TCPConnectionServer 
    {
    protected:
        // Forward declaration.
        class  ListenThread;

    public: // Structors.
            /**
             * @brief Constructor
             *
             * @param   port    [in]; The port that the server should listen to.
             */
        TCPConnectionServer(unsigned short port);

        /**
         * Destructor
         */
        virtual ~TCPConnectionServer();
    
        // Overridable Interface.

    public: // Event_handling.
            /**
             * @brief Reimplement this for special handling on acceptance of
             * new client connections.
             *
             * @param   client  [in]; The reference to client just accepted.
             */
        virtual void on_accept(TCPConnection& client) {}
            
        /**
         * @brief Reimplement this for special handling on disconnection of a
         * client.
         *
         * @param   client  [in]; The client that just got disconnected. 
         */
        virtual void on_disconnect(TCPConnection& client) {}
            
        /**
         * @brief 
         * Must reimplement this to handle processing of client messages.
         *
         * @param   client  [in]; reference to client sending message.
         * @param   buffer  [in]; pointer to buffer containing message.
         * @param   buffer_size [in]; size of message buffer.
         */
        virtual void on_read(TCPConnection& client, const char* buffer, int buffer_size) = 0;
            
        /**
         * @brief This returns whether the server is listening.
         *
         * This function should be called right after a server is constructed
         * to see if it is listening on the desired port.  A server could fail
         * to listen on a desired port if another service is already listening
         * on that port.
         *
         * @return True if server is listening on desired port.
         */
        virtual bool is_listening();
            
    public: // Message handling
            /**
             * @brief Use this to broadcast a message to all connected clients.
             * 
             * @param msg       [in]; points to message to send.
             * @param len       [in]; length of message to send.
             * @param queue_length [in]; max length of packet queue before 
             *      dropping message, 0=no max length
             */
        virtual void broadcast(const char* msg, int len, size_t queue_length = 0);

        /**
         * @brief Use this to multicast a message to a list of clients.
         *
         * @param clients   [in]; The list of clients to send the message to.
         * @param msg       [in]; Points to message to send.
         * @param len       [in]; The length of message to send.
         * @param queue_length [in]; The maximum length of packet queue before 
         *      dropping message, 0=no max length.
         */
        virtual void multicast(TCPConnectionList& clients, const char *msg, int len, size_t queue_length = 0);
            
    public: // Utility functions.
        /**
         * @brief This function returns the number of clients currently
         * connected.
         *
         * @return The number of connected clients.
         */
        virtual size_t get_num_clients();

    protected: // Thread send procedure.
        /**
         * @brief Use this function to send thread procedure
         *
         * @param param [in]; The thread parameter.
         *
         * @return The thread result.
         */
        static ThreadProcResultDecl send_thread_proc(ThreadProcParam param);
            
    protected: // Methods.
        /**
         * @brief Use this function to add a client to the server.
         *
         * @param new_client [in]; Pointer to the new client.
         */
        virtual void add_client(TCPConnection* new_client);

        /**
         * @brief Use this function to delete a client from the server.
         * @param it  Iterator pointing to client to delete
         *
         * @param it [in]; Iterator to the client to be deleted.
         */
        virtual void delete_client(TCPConnectionList::iterator it);

        /**
         * @brief Use this function to delete a client from the server
         * 
         * @param client [in]; Pointer to client object to delete.
         */
        virtual void delete_client(TCPConnection* client);

        /**
         * @brief Use this function to delete all disconnected clients from the
         * server.
         */
        virtual void delete_disconnected_clients();

        /**
         * @brief Use this function to delete all clients from the server
         */
        virtual void delete_all_clients();
        
    protected: // AcceptClient management.
        /**
         * @brief Use this to create a client with a given protocol, socket, and
         * address.  This function does NOT add the client to this server.
         * Return null to indicate that you want to close this connection.
         * This gives a rudimentary form of access control.
         *
         * @param s [in]; Socket handle for the client connection.
         * @param addr [in]; Pointer to the address of the client connection.
         *
         * @return The TCPConnection object encapsulating the new connection.
         */
        virtual TCPConnection* create_client(SOCKET s, sockaddr* addr) 
        { 
            return new TCPConnection(this, s, addr); 
        }

    protected: // Attributes.
        /**
         * @brief  The server's listen thread
         */
        //std::auto_ptr<ListenThread> _listen_thread;
        ListenThread* _listen_thread;

        /**
         * @brief  The list of all connected  clients
         */
        TCPConnectionList _all_clients;

        /**
         * @brief  IMutex for synchronizing server operations
         */
        IMutex* _mutex;

        // Nested helper classes.
        
        /**
         * @class ListenThread
         * @ingroup core_util
         * @brief This class handles the thread that listens for new
         * connections.
         */
        class EVOLUTION_EXPORT_UTIL ListenThread : public SignalThread
        {
        public: // Structors.
            /**
             * @brief Constructor
             *
             * @param   server  [in] reference to the server serviced by this
             * thread.
             * @param   mutex   [in] pointer to mutex used for synchronization
             * in this thread.
             * @param   port    [in] port the server is listening to.
             */
            ListenThread(TCPConnectionServer& server, IMutex* mutex, unsigned short port);

            /**
             * @brief Standard destructor
             */
            virtual ~ListenThread();
        
        protected: // SignalThread overrides.
            /**
             * @brief This is the code that the thread executes.
             */
            virtual void run();

        public: // SignalThread overrides.
            /**
             * @brief Use this to halt the thread's execution.
             */
            virtual void halt();

        public: // Helpers.
            /**
             * @brief This function returns whether the listen thread is
             * listening.
             *
             * @return true if server is listening on desired port.
             */
            virtual bool is_listening() { return _listening; }

        public: // Attributes
            TCPConnectionServer&    _server;            ///< Reference to the server serviced by this listen thread.
            IMutex*                 _mutex;             ///< IMutex for synch.        
            SOCKET                  _listen_socket;     ///< The listen socket.
            unsigned short          _port;              ///< The port to listen to.
            bool                    _listening;         ///< True if is listening on _port.
        };
        friend class ListenThread;
    };
}

#endif 
