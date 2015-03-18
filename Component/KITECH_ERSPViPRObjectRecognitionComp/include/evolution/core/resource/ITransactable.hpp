//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_ITRANSACTABLE_HPP
#define INCLUDE_EVOLUTION_CORE_ITRANSACTABLE_HPP


/**
 * @file    evolution/core/resource/ITransactable.hpp
 * @brief   Interface to a resource that supports command execution in
 * transactions.  
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ITransactable
 */


#include <stddef.h>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface ITransactable
     *
     * @brief The ITransactable class provides an interface to a resource that
     * supports command execution in transactions.
     *
     * @ingroup core_resource 
     **/
    class EVOLUTION_EXPORT_RESOURCE ITransactable
    {
    public: // Types
        /// ID of the Transaction.
        typedef unsigned long TransactionId;

    public: // Constants

        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.ITransactable"

    public: // Structors

        /**
         * @brief Empty constructor
         */
        ITransactable (  )
        {
        }

        /**
         * @brief Empty destructor
         */
        virtual ~ ITransactable (  )
        {
        }

    public: // Commands

        /**
         * @brief This function begins a transaction.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  transaction_id [out]; The ID of the new transaction 
         */
        virtual Result transact ( TicketId ticket,
                                  TransactionId * transaction_id ) = 0;

        /**
         * @brief Use this function to execute commands in the current transaction and complete transaction.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  transaction_id [in]; The transaction to commit
         */
        virtual Result commit ( TicketId ticket,
                                TransactionId transaction_id ) = 0;

        /**
         * @brief Use this to roll back the commands in the current transaction.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  transaction_id [in]; The transaction to rollback/undo
         */
        virtual Result rollback ( TicketId ticket,
                                  TransactionId transaction_id ) = 0;

    };  // end class ITransactable


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_ICAMERA_HPP
