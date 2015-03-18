//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IRESOURCE_CONTAINER_HPP
#define INCLUDE_EVOLUTION_CORE_IRESOURCE_CONTAINER_HPP


/**
 * @file   evolution/core/resource/IResourceContainer.hpp
 * @brief  This file defines a class to provide an interface through which a
 *         resource interacts with the rest of the system.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::IResourceContainer
 */


#include <list>
#include <evolution/core/base/StringUtils.hpp>


namespace Evolution
{

    // Forward declarations.
    class ResourceConfigParser;

    /**
     * @interface IResourceContainer
     *
     * @brief The IResourceContainer class provides an interface through which
     * a resource interacts with the rest of the system.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IResourceContainer
    {
    public: // Structors

        /**
         * @brief Empty constructor
         */
        IResourceContainer () {}

        /**
         * @brief Empty destructor
         */
        virtual ~IResourceContainer () {}

    public: // Access

        /**
         * @brief This function obtains the configuration information for the
         * resources in the container.
         *
         * @param  ticket [in];     The (TicketId) for security purposes
         * @param  config [in/out]; Returns pointer to internal resource
         *                          configuration info for this container.
         *                          Caller must NOT free the results and must
         *                          pass in a valid pointer
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result get_configuration (TicketId ticket,
                                const ResourceConfigParser** config) const = 0;

        /**
         * @brief This function obtains the specified interface on the specified
         * resource. If the driver is not yet active, we will attempt to
         * activate it.
         *
         * @param ticket             [in]; The (TicketId) for security purposes
         * @param resource_id        [in]; Specifies the resource
         * @param interface_name     [in]; Specifies the interface
         * @param resource_interface [in/out]; Returns a pointer to the internal
         *                                     interface class for this container.
         *                                     Caller must NOT free the results
         *                                     and must pass in a valid pointer.
         *                                     Implementation should keep
         *                                     reference counts
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result obtain_interface (TicketId ticket,
                                         const char* resource_id,
                                         const char* interface_name,
                                         void** resource_interface) = 0;

        /**
         * @brief This function determines the resources, if any, that support
         * the requested interface.
         *
         * @param  ticket         [in];  The (TicketId) for security purposes
         * @param  interface_name [in];  Name of the interface to find
         * @param  resource_list  [out]; Optional list of resources
         *
         * @result RESULT_SUCCESS or RESULT_NOT_FOUND
         *
         * @pre interface_name != NULL
         */       
        virtual Result find_interface (TicketId ticket,
                                       const char* interface_name,
                                       StringList* resource_list) = 0;

        /**
         * @brief This function releases the specified interface.
         *
         * @param  ticket             [in]; The (TicketId) for security purposes
         * @param  resource_interface [in]; The resource interface to release
         * @param  deactivate         [in, opt]; Optional boolean to trigger
         *                                       deactivation attempt,
                                                 default=false
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result release_interface (TicketId ticket,
                                          void* resource_interface,
                                          bool deactivate = false) = 0;

    }; // end class IResourceContainer

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_IRESOURCE_CONTAINER_HPP
