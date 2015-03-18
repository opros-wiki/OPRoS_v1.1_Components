//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IAUTHENTICATOR_HPP
#define INCLUDE_EVOLUTION_CORE_IAUTHENTICATOR_HPP


/**
 * @file    IAuthenticator.hpp
 * @brief   Interface for challenge-response-type authentication.
 * @see  Evolution::IAuthenticator 
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{
    /**
     * @brief typedef for the window ID.
     */
    typedef unsigned int WINDOW_ID;

    /**
     * @interface IAuthenticator 
     *
     * @brief Abstract interface to an authenication system.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IAuthenticator 
    {
    public: // Constants
        /**
         * @brief
         * ID of this interface.
         */ 
        static const char* const INTERFACE_ID; // "Evolution.IAuthenticator"

    public: // Structors

        /**
         * @brief
         * Constructor.
         */ 
        IAuthenticator () {}

        /**
         * @brief
         * Destructor.
         */ 
        virtual ~IAuthenticator () {}

    public: // Commands

        /**
         * @brief
         * Obtain a challenge from the authenticator.
         *
         * @param  ticket [in]; The security ticket.
         * @param  challenge [in, out]; Pointer to the data buffer that will
         *         contain a challenge.
         * @param  size      [in]; The size of the buffer.
         * @param  challenge_size [out]; The size of the challenge data.
         *
         * @return RESULT_SUCCESS on success or a error code on failure.
         */ 
        virtual Result get_challenge(TicketId ticket, char* challenge, size_t size, size_t* challenge_size) = 0;

        /**
         * @brief
         * Test a response to a challenge.
         *
         * @param  ticket [in]; The security ticket.
         * @param  challenge [in]; The challenge data.
         * @param  challenge_size [in]; The size of the challenge data.
         * @param  response [in]; The response data.
         * @param  size [in]; The size of the response data.
         *
         * @return RESULT_SUCCESS on success or a error code on failure.
         */ 
        virtual Result test_response(TicketId ticket, const char* challenge, size_t challenge_size, const char* response, size_t size) = 0;

        /**
         * @brief
         * Add a new authentication account.
         *
         * @param  ticket [in]; The security ticket.
         * @param  account_data [in]; All data necessary to set up an account.
         * @param  size [in]; The size of the account data.
         *
         * @return RESULT_SUCCESS on success or a error code on failure.
         */ 
        virtual Result add_account(TicketId ticket, const char* account_data, size_t size) = 0;
        
        /**
         * @brief
         * Change an existing authentication account.
         *
         * @param  ticket [in]; The security ticket.
         * @param  old_data [in]; The old account data needed to make the 
         *         change.
         * @param  old_size [in]; The size of the old account data.
         * @param  account_data [in]; All data necessary to set up an account.
         * @param  size [in]; The size of the account data.
         *
         * @return RESULT_SUCCESS on success or a error code on failure.
         */ 
        virtual Result change_account(TicketId ticket, const char* old_data, size_t old_size,  const char* account_data, size_t size) = 0;
        
    }; // end class IAuthenticator


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IAUTHENTICATOR_HPP
