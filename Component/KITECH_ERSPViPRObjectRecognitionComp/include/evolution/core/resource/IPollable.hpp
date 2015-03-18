//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IPOLLABLE_HPP
#define INCLUDE_EVOLUTION_CORE_IPOLLABLE_HPP


/**
 * @file    evolution/core/resource/IPollable.hpp
 * @brief   This file defines a class to provide an interface to pollable
 * resource.  
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IPollable
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    // Forward declarations.
    class IResourceCallback;


    /**
     * @interface IPollable 
     *
     * @brief This class provides a resource interface to a pollable resource, that is, a
     * resource that polls data from some hardware device, and needs
     * to be told periodically to do so. Any resource that supports
     * this interface and is configured to be in a polling group will
     * be polled automagically by the ResourceManager.
     *
     * @note Interface ID = "Evolution.IPollable"
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IPollable 
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        IPollable () {}

        /**
         * @brief Destructor
         */
        virtual ~IPollable () {}

    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IPollable"

    public: // Command

        /**
         * @brief The poll function tells the resource to perform its polling.
         * @param  ticket [in]; The (TicketId) for security purposes.
         */
        virtual Result poll (TicketId ticket) = 0;

        /**
         * @brief The register_callback function registers a callback to be notified when the polling occurs.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  callback [in]; The callback class used to test command completion
         * @param  callback_id [in]; ID of the specific callback for this command
         * @param  registration_id [in]; Registration ID to use for the resource callback
         */
        virtual Result register_callback (TicketId ticket,
                                          IResourceCallback* callback, CallbackId callback_id,
                                          RegistrationId* registration_id) = 0;

        /**
         * @brief This function unregisters a callback.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  registration_id [in]; Registration ID of IResourceCallback to unregister
         */
        virtual Result unregister_callback (TicketId ticket, RegistrationId registration_id) = 0;

    }; // end class IPollable


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_IPOLLABLE_HPP

