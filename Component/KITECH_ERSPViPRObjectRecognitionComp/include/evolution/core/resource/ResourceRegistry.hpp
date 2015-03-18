//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR__RESOURCE_REGISTRY_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR__RESOURCE_REGISTRY_HPP


/**
 * @file   evolution/core/resource/ResourceRegistry.hpp
 * @brief  This contains a registry of resource factory functions.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::ResourceRegistry
 */


namespace Evolution
{

    // Forward declarations.
    class IResourceDriver;
    class ResourceConfig;
    class IResourceContainer;

    /**
     * @brief All shared libraries providing resource network implementations
     * should export a function named "create_resource," with external
     * C linkage, and this signature.
     *
     * Because this function provides an IResourceContainer interface
     * to the new resource, it should increment the container's
     * reference count.
     *
     * @note While the function name can be overridden in the configuration
     * file, this should be done only with good reason.
     *
     * @param ticket             [in]; Security ticket (placeholder)
     * @param resource_config    [in]; Configuration specifiying how to instantiate
     * @param resource_container [in]; The resource's container
     * @param resource_ptr       [out]; Pointer to the IResourceDriver implementation
     *
     * @result RESULT_SUCCESS,
     *         RESULT_NOT_FOUND,
     *         RESULT_INSUFFICIENT_ACCESS
     *
     * @pre driver_ptr != NULL
     *
     * @ingroup core_resource
     */
    typedef Result (*CreateResourceFunction)(TicketId ticket,
                                             const ResourceConfig& resource_config,
                                             IResourceContainer& resource_container,
                                             IResourceDriver** resource_ptr);

    /**
     * @class ResourceRegistry
     *
     * @brief This class provides a registry of resource driver types and their
     * creation (factory) functions.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ResourceRegistry
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        ResourceRegistry () {}

    public: // Creation

        /**
         * @brief The create_resource function attempts to create an instance
         * of the resource specified by the given configuration.
         *
         * @param ticket       [in]; The (TicketId) for security purposes.
         * @param config       [in]; Object in charge of handling the configuration of the desired resource.
         * @param container    [in]; The resource container object.
         * @param resource_ptr [in/out]; Returns a Pointer to the IResourceDriver
         *                               implementation. Caller must NOT free the
         *                               results and must pass in a valid
         *                               pointer.
         *
         * @pre resource_ptr != NULL
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        static Result create_resource (TicketId ticket,
                                       const ResourceConfig& config,
                                       IResourceContainer& container,
                                       IResourceDriver** resource_ptr);

    public: // Registration

        /**
         * @brief The register_resource function registers the given resource
         * factory function under the specified type ID.
         *
         * @param type_id     [in]; An ID to associate the resource with
         * @param create_func [in]; The resource factory function
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        static Result register_resource (const char* type_id,
                                         CreateResourceFunction create_func);

        /**
         * @brief This function unregisters the specified resource factory
         * function.
         *
         * @param ticket  [in]; The (TicketId) for security purposes
         * @param type_id [in]; The ID of the resource to unregister
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        static Result unregister_resource (TicketId ticket, const char* type_id);

    protected: // Helpers

        /**
         * @brief This function initializes the resource registry singleton.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        static Result initialize_resource_registry ();

    }; // end class ResourceRegistry

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BEHAVIOR__RESOURCE_REGISTRY_HPP
