//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_POLLABLE_IMPL_HPP
#define INCLUDE_EVOLUTION_CORE_POLLABLE_IMPL_HPP


/**
 * @file    evolution/core/resource/PollableImpl.hpp
 * @brief   A basic implementation of the IPollable interface.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::PollableImpl
 */


#include <evolution/core/util/RegistrationTable.hpp>
#include <evolution/core/resource/IPollable.hpp>


namespace Evolution
{


    /**
     * @class PollableImpl 
     *
     * @brief The PollableImpl class defines the default implementation of a pollable resource with
     * callbacks. Subclasses still must implement poll().
     *
     * @todo Notify callbacks in a separate thread, triggering on a
     * condition variable signal.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE PollableImpl : public IPollable
    {
    public: // Types

        /**
         * @brief This represents an entry in the pollable callback table.
         */
        struct CallbackEntry
        {
        public: // Structors

            /**
             * @brief Default constructor
             */
            CallbackEntry () : callback (0), callback_id (0), ticket (0) {}

            /**
             * @brief Data constructor
             *
             * @param a_callback    [in]; The callback to invoke
             * @param a_callback_id [in]; The callback's ID
             * @param a_ticket      [in]; The (TicketId) for security purposes
             */
            CallbackEntry (IResourceCallback* a_callback,
                           CallbackId a_callback_id,
                           TicketId a_ticket)
                : callback (a_callback),
                  callback_id (a_callback_id),
                  ticket (a_ticket) {}

        public: // Data

            IResourceCallback*  callback;     ///< Callback
            CallbackId          callback_id;  ///< Callback id
            TicketId            ticket;       ///< Security ticket
        }; // end struct CallbackEntry

        /**
         * @brief The table of callbacks
         */
        typedef RegistrationTable<CallbackEntry> CallbackTable;

    public: // Structors

        /**
         * @brief Constructor
         */
        PollableImpl () {}

        /**
         * @brief Destructor
         */
        virtual ~PollableImpl () {}

    public: // IPollable interface

        /**
         * @brief The register_callback function registers a callback to be
         * notified when the polling occurs.
         *
         * @param ticket          [in]; The (TicketId) for security purposes
         * @param callback        [in]; The callback to invoke
         * @param callback_id     [in]; The callback's ID
         * @param registration_id [out]; The callback's registration ID. To
         *                               be used when unregistering the callback
         *
         * @return RESULT_SUCCESS on success, or some error value
         */
        virtual Result register_callback (TicketId ticket,
                                          IResourceCallback* callback,
                                          CallbackId callback_id,
                                          RegistrationId* registration_id);

        /**
         * @brief The unregister_callback function unregisters a callback, checking security.
         *
         * @param ticket [in];          The (TicketId) for security purposes
         * @param registration_id [in]; The ID of the callback to be removed
         *
         * @return RESULT_SUCCESS on success, or some error value
         */
        virtual Result unregister_callback (TicketId ticket,
                                            RegistrationId registration_id);

    protected: // Helpers

        /**
         * @brief The notify_callbacks function notifies the callbacks of the
         * specified result. Generally this function is called from poll().
         *
         * @param notify_result [in]; The result to handle
         * @param data          [in]; Data specific to the result being handled
         *
         * @return RESULT_SUCCESS on success, or some error value
         */
        virtual Result notify_callbacks (Result notify_result, void* data);

    protected: // Implementation

        CallbackTable  _callback_table;  ///< The callback table
    }; // end class PollableImpl

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_POLLABLE_IMPL_HPP
