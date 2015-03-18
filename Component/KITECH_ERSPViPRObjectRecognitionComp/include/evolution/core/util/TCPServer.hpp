//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/TCPServer.hpp
 * @brief   A generic server for TCP-based network communication.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::TCPServer 
 */

#ifndef EVOLUTION_LIBRARIES_BASE_TCP_SERVER_HPP
#define EVOLUTION_LIBRARIES_BASE_TCP_SERVER_HPP

#include <evolution/core/util/ISocketProtocol.hpp>
#include <evolution/core/util/SignalThread.hpp>
#include <evolution/core/util/TCPClient.hpp>
#include <evolution/core/util/ThreadPool.hpp>
#include <evolution/core/base/RefPtr.hpp>

namespace Evolution {

    /**
     * @class TCPServer
     * @brief A class implementing a generic server for 
     * socket-based TCP communication.
     * 
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL TCPServer 
    {
    protected:
#ifndef DOXYGEN_INTERNAL
        // Forward declaration.
        class EVOLUTION_EXPORT_UTIL ListenThread;

        /**
         * @class SendThreadData
         *
         * @brief Encapsulated data passed to the send thread.
         * 
         * @ingroup core_util
         */
        struct SendThreadData
        {
            /**
             * @brief 
             * Constructor
             * 
             * @param server [in]; Reference to the server associated with
             *      this SendThreadData instance.
             * @param client [in]; The client connection assocated with this
             *      SendThreadData instance.
             * @param message [in]; Points to the message to send.
             * @param size [in]; The size of the message to send.
             */          
            SendThreadData(TCPServer& server, TCPClient& client, const char* message, int size) :
                _server(server), _client(client), _message_size(size) 
            {
                // Make a copy of the message.
                _message = new char[size];
                memcpy(_message, message, size);
                _client.add_ref();
            }
            
            /**
             * @brief
             * Destructor.
             */
            ~SendThreadData()
            {
                delete[] _message;
                _client.remove_ref();
            }
           
            TCPServer& _server; ///< The reference to the associated server.
            TCPClient& _client; ///< The reference to the associated client.
            char* _message;     ///< The pointer to the message to send.
            int   _message_size; ///< The size of the message to send.
        };
#endif
        
    public: // Structors.
        /**
         * @brief Constructor
         *
         * @param   port    [in]; The port that the server should listen to.
         */
        TCPServer(unsigned short port);

        /**
         * @brief Destructor
         */
        virtual ~TCPServer();
    
        // Overridable Interface.

    public: // Event_handling.
        /**
         * @brief Reimplement this for special handling on acceptance of new
         * client connections.
         *
         * @param   client  [in]; reference to client just accepted.
         */
        virtual void on_accept(TCPClient *client) {}
            
        /**
         * @brief
         * Reimplement this for special handling on disconnection of a client.
         *
         * @param   client  [in]; client that just got disconnected. 
         */
        virtual void on_disconnect(TCPClient *client) {}
            
        /**
         * @brief Must reimplement this to handle processing of client messages.
         *
         * @param   client  [in]; reference to client sending message.
         * @param   buffer  [in]; pointer to buffer containing message.
         * @param   buffer_size [in]; size of message buffer.
         */
        virtual void on_read(TCPClient& client, const char* buffer, int buffer_size) = 0;
            
        /**
         * @brief Must reimplement this to create the protocol used by this
         * server type.
         *
         * Each TCPServer-derived class must reimplement this method
         * to return a protocol object for use by the class.  This protocol
         * object must implement theISocketProtocol interface and also
         * be allocated on the heap.  Whenever a new client connection is
         * made, this method is called to create a new protocol object for
         * the client connection to encode and decode all communications to
         * and from the client.  If no encoding or decoding is necessary,
         * it returns a pointer to the NullProtocol object.
         *
         * @return  Pointer to a heap-allocated protocol object.
         */
        virtual ISocketProtocol* create_protocol() = 0;
            
        /**
         * @brief This returns true or false depending on whether the server is
         * listening.
         *
         * This function should be called right after a server is constructed
         * to see if it is listening on the desired port.  A server could fail
         * to listen on a desired port if another service is already listening
         * on that port.
         *
         * @return true if server is listening on desired port.
         */
        virtual bool is_listening();
            
    public: // Message handling
        /**
         * @brief This sends a message to a specified client using a
         * separate thread.
         *
         * @param client    [in]; The reference of client to send 
         *      message to.
         * @param msg       [in]; Points to message to send.
         * @param len       [in]; The length of message to send.
         */
        virtual void send_message(TCPClient& client, const char* msg, int len);
            
        /**
         * @brief This sends a message to a specified client synchronously.
         *
         * @param client    [in]; The reference of client to send message to.
         * @param msg       [in]; Points to message to send.
         * @param len       [in]; The length of message to send.
         *
         * @return True if send was successful.
         */
        virtual bool send_message_sync(TCPClient& client, const char* msg, int len, int timeout=1);

        /**
         * @brief This broadcast a message to all connected clients.
         * 
         * @param msg       [in]; Points to message to send.
         * @param len       [in]; The length of message to send.
         * @param synchronous [in]; If true, do the broadcast in the
         *                    the same thread.
         */
        virtual void broadcast(const char* msg, int len, bool synchronous = false);
            
        /**
         * @brief Use this to multicast a message to a list of clients.
         *
         * @param clients   [in]; The list of clients to send the message to.
         * @param msg       [in]; Points to message to send.
         * @param len       [in]; The length of message to send.
         * @param synchronous [in]; If true, do the multicast in the 
         *                    same thread.
         */
        virtual void multicast(TCPClientList& clients, const char *msg, int len, bool synchronous = false);
        
        /**
         * @brief
         * Returns the number of clients connected to the server.
         *
         * @return The number of connected clients. 
         */
        unsigned get_client_count() const;
            
    protected: // Thread send procedure.
        /**
         * @brief Use this to send thread procedure.
         *
         * @param param [in]; The thread parameter.
         *
         * @return The thread result.
         */
        static ThreadProcResultDecl send_thread_proc(ThreadProcParam param);
            
    protected: // Methods.
        /**
         * @brief Use this to add a client to the server.
         *
         * @param new_client [in]; The new client to add to the server.
         */
        virtual void add_client(TCPClient* new_client);

        /**
         * @brief Use this to delete a client from the server.
         * @param it [in];  Iterator pointing to client to delete
         */
        virtual void delete_client(TCPClientList::iterator it);

        /**
         * @brief Use this to delete a client from the server.
         * @param client [in]; Pointer to client object to delete
         */
        virtual void delete_client(TCPClient* client);

        /**
         * @brief Use this to delete all clients from the server.
         */
        virtual void delete_all_clients();
        
    protected: // AcceptClient management.
        /**
         * @brief This creates a client with a given protocol, socket, and
         * address.  This function does NOT add the client to this server.
         * 
         * @param protocol [in]; Protocol object used by the client.
         * @param s [in]; Socket handle for the client connection.
         * @param addr [in]; Pointer to the address of the client connection.
         *
         * @return The TCPClient object encapsulating the new client connection.
         */
        virtual TCPClient* create_client(ISocketProtocol* protocol, SOCKET s, sockaddr* addr) { return new TCPClient(protocol, s, addr); }

    protected: // Attributes.
        /**
         * @brief  The server's listen thread
         */
        ListenThread*      _listen_thread;

        /**
         * @brief  The list of all connected  clients
         */
        TCPClientList      _all_clients;

        /**
         * @brief  The IMutex for synching server operations
         */
        IMutex*            _mutex;

        /**
         * @brief  The thread pool for sending messages with blocking sockets 
         */
        ThreadPool         _send_thread_pool;
        
        
        /**
         * @class ReadThread
         * 
         * @ingroup core_util
         * 
         * @brief This class handles the thread that reads messages from
         * clients. 
         */
        class EVOLUTION_EXPORT_UTIL ReadThread : public SignalThread  
        {
        public: // Structors
            /**
             * @brief Constructor
             *
             * @param   server  [in] reference to the server serviced by this
             * thread.
             * @param   mutex   [in] pointer to mutex used for synchronization
             * in this thread.
             * @param   port    [in] port the server is listening to.
             */
            ReadThread(TCPServer& server, IMutex* mutex, unsigned short port);

            /**
             * @brief Destructor
             */
            virtual ~ReadThread();

            /**
             * @brief Use this to halt the thread's execution.
             */
            virtual void halt();
            
            /**
             * @brief This is called after the server's select() call.
             */
            virtual void exit_select();

        protected:
            /**
             * @brief The code that the thread executes
             */
            virtual void run();

            /**
             * @brief This is called prior to the server's select() call.
             *
             * @return True if select initialization was successful.
             */
            virtual bool init_select();
            
            /**
             * @brief This initializes structures passed to the server's select
             * call.
             *
             * @return The last socket descriptor value + 1.
             */
            virtual int setup_select();

        protected:
            TCPServer&           _server;            ///< Reference to server serviced by this read thread.
            IMutex*                  _mutex;             ///< IMutex for synch.        
            unsigned short          _port;              ///< Server port
            SOCKET                  _select_socket;     ///< A socket used to trigger unblocking of selects.
            SOCKET                  _select_client;     ///< A socket for unblocking select. 
            fd_set                  _rfds;              ///< Read descriptor set.
            fd_set                  _xfds;              ///< Exception descriptor set.

            friend class ListenThread; ///< friend class declaration
        };
        friend class ReadThread;

        /**
         * @class ListenThread
         * 
         * @ingroup core_util
         *
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
            ListenThread(TCPServer& server, IMutex* mutex, unsigned short port);

            /**
             * @brief Destructor
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
             * @brief This returns the listening status of the thread
             * 
             * @return true if server is listening on desired port.
             */
            virtual bool is_listening() { return _listening; }

        public: // Attributes
            TCPServer&           _server;            ///< Reference to the server serviced by this listen thread.
            IMutex*                  _mutex;             ///< IMutex for synch.        
            ReadThread*             _read_thread;       ///< The read thread used by the server.
            SOCKET                  _listen_socket;     ///< The listen socket.
            unsigned short          _port;              ///< The port to listen to.
            bool                    _listening;         ///< True if is listening on _port.
        };
        friend class ListenThread;
    };
}

#endif 
