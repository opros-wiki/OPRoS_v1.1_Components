//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR__BEHAVIOR_REGISTRY_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR__BEHAVIOR_REGISTRY_HPP


/**
 * @file    evolution/core/behavior/BehaviorRegistry.hpp
 * @brief   Registry of behavior factory functions.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IBehavior
 * @see Evolution::BehaviorConfig
 * @see Evolution::IBehaviorContainer
 * @see Evolution::IBehaviorNetwork
 * @see Evolution::BehaviorNetworkConfig
 * @see Evolution::IResourceContainer
 * @see Evolution::BehaviorRegistry
 **/


#include <string>
#include <map>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    // Forward declarations.
    class IBehavior;
    class BehaviorConfig;
    class IBehaviorContainer;


    /**
     * @brief
     * All shared libraries providing behavior network implementations
     * should export a function named "create_behavior," with an external
     * C linkage, and this signature.
     *
     * Because this function provides an IBehaviorContainer interface
     * to the new behavior, it should increment the container's
     * reference count.
     *
     * @note
     * While the function name can be overridden in the configuration
     * file, this should be done only with good reason.
     *
     * @param   ticket             [in]; Security ticket (placeholder)
     * @param   behavior_config    [in]; Configuration specifying how to instantiate
     * @param   behavior_container [in]; The behavior's container
     * @param   behavior_ptr       [out]; Pointer to the IBehaviorNetwork implementation
     * @result  RESULT_SUCCESS,
     *          RESULT_NOT_FOUND,
     *          RESULT_INSUFFICIENT_ACCESS
     * @pre     driver_ptr != NULL
     * @ingroup core_behavior
     **/
    typedef Result (*CreateBehaviorFunction)(TicketId ticket,
                                             BehaviorConfig& behavior_config,
                                             IBehaviorContainer& behavior_container,
                                             IBehavior** behavior_ptr);


    // Forward declarations.
    class IBehaviorNetwork;
    class BehaviorNetworkConfig;
    class IResourceContainer;


    /**
     * @brief
     * All shared libraries providing behavior network implementations
     * should export a function named "create_behavior_network," with
     * an external C linkage, and this signature.
     *
     * @note
     * While the function name can be overridden in the configuration
     * file, this should be done only with good reason.
     *
     * @param   ticket             [in]; Security ticket (placeholder)
     * @param   config     [in]; Configuration specifying how to instantiate
     * @param   resource_container [in]; Container of available hardware resources
     * @param   ptr         [out]; Pointer to the IBehaviorNetwork implementation
     * @result  RESULT_SUCCESS,
     *          RESULT_NOT_FOUND,
     *          RESULT_INSUFFICIENT_ACCESS
     * @pre     driver_ptr != NULL
     * @ingroup core_behavior
     **/
    typedef Result (*CreateBehaviorNetworkFunction)(TicketId ticket,
                                                    BehaviorNetworkConfig& config,
                                                    IResourceContainer& resource_container,
                                                    IBehaviorNetwork** network_ptr);


    /**
     * @brief Registry of behavior types and their creation (factory)
     * functions.
     * @ingroup core_behavior
     **/
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorRegistry
    {
    public: // Structors

        /// Default constructor.
        BehaviorRegistry () {}

    public: // Creation

        /**
         * @brief
         * Attempts to create an instance of the behavior network
         * specified by the given configuration.
         * @param ticket [in]; security ticket
         * @param config [in]; BehaviorNetworkConfig object to use to create the behavior network.
         * @param resource_container [in]; the container used to create the specified behavior network.
         * @param network_ptr [out]; on exit, the new IBehaviorNetwork object is at **network_ptr
         **/
        static Result create_behavior_network (TicketId ticket,
                                               BehaviorNetworkConfig& config,
                                               IResourceContainer& resource_container,
                                               IBehaviorNetwork** network_ptr);

        /**
         * @brief
         * Attempts to create an instance of the behavior specified by
         * the given configuration.
         *  @param ticket [in]; security ticket
         *  @param config [in]; BehaviorNetwork object to use to create the behavior.
         *  @param container [in]; the container to use to create the specified behavior.
         *  @param behavior_ptr [out]; on exit, the new IBehavior object is at **behavior_ptr
         **/
        static Result create_behavior (TicketId ticket, BehaviorConfig& config,
                                       IBehaviorContainer& container, IBehavior** behavior_ptr);

    public: // Registration

        /**
         * @brief
         * Registers the given network factory function under the
         * specified type.
         * @param type_id [in]; label to associate with the CreateBehaviorNetworkFunction.
         * @param create_func [in]; function used to create a BehaviorNetwork
         **/
        static Result register_behavior_network (const char* type_id, CreateBehaviorNetworkFunction create_func);

        /** Unregisters the specified network factory function.
         *  @param ticket [in]; security ticket
         *  @param type_id [in]; removes the id from the registry.
         */
        static Result unregister_behavior_network (TicketId ticket, const char* type_id);

        /**
         * @brief
         * Registers the given behavior factory function under the
         * specified type.
         *  @param type_id [in]; label for the CreateBehaviorFunction in the registry.
         *  @param create_func [in]; function used to create the given behavior. (constructor, usually)
         **/
        static Result register_behavior (const char* type_id, CreateBehaviorFunction create_func);

        /** Unregisters the specified behavior factory function.
         * @param ticket [in]; security ticket
         * @param type_id [in]; label to remove from the registry.
         */
        static Result unregister_behavior (TicketId ticket, const char* type_id);

    protected: // Helpers

        /// Initializes the behavior registry singleton.
        static Result initialize_behavior_registry ();

        /// Initializes the network registry singleton.
        static Result initialize_network_registry ();

    }; // end class BehaviorRegistry



} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BEHAVIOR__BEHAVIOR_REGISTRY_HPP

