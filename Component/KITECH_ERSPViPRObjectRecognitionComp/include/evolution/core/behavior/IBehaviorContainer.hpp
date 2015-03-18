//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR_IBEHAVIOR_CONTAINER_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_IBEHAVIOR_CONTAINER_HPP


/**
 * @file    evolution/core/behavior/IBehaviorContainer.hpp
 * @brief   Interface for a container of behaviors.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IResourceContainer
 * @see Evolution::IBehaviorNetwork
 * @see Evolution::IBehaviorContainer
 */


#include <evolution/core/base/IObject.hpp>
#include <evolution/core/behavior/IBehavior.hpp>


namespace Evolution
{


    // Forward declarations.
    class IResourceContainer;
    class IBehaviorNetwork;


    /**
     * @brief
     * Interface to a set of behaviors, allowing interaction between
     * these behaviors and the external environment.
     *
     * IBehaviorNetwork and IBehaviorContainer are closely related. 
     * Generally, a class that implements one will implement the
     * other. IBehaviorNetwork deals purely with the execution of a
     * set of (usually connected) behaviors. IBehaviorContainer
     * provides the means to access and manipulate that set, and, for
     * contained behaviors, to access the external environment.
     *
     * @see  IBehaviorNetwork
     *
     * @ingroup core_behavior */
    class EVOLUTION_EXPORT_BEHAVIOR IBehaviorContainer : public IObject
    {
    public: // Types
        typedef unsigned short PortId;  ///< Convenience.

    public: // Constants

        /// Interface ID.
        static const char* const INTERFACE_ID; // "Evolution.IBehaviorContainer"

    public: // Structors

        /**
         * @brief
         * Constructor.
         */
        IBehaviorContainer () {}

        /**
         * @brief
         * Destructor.
         */
        virtual ~IBehaviorContainer () {}

    public: // Access

        /**
         * @brief
         * Obtains the root configuration information for the
         * behaviors in the container.
         * 
         * @param ticket [in]; The security ticket.
         * @param parser [out]; The parser used for parsing behaviors in
         *      the IBehaviorContainer.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_configuration (TicketId ticket, const BehaviorConfigParser** parser) const = 0;

        /**
         * @brief
         * Obtains a container of resources available to this behavior 
         * environment.
         * 
         * @param ticket [in]; The security ticket.
         * @param resource_container [out]; Pointer to the resource container
         *      used by behaviors in IBehaviorContainer.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_resource_container (TicketId ticket, IResourceContainer** resource_container) const = 0;

    }; // end class IBehaviorContainer


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BEHAVIOR_IBEHAVIOR_CONTAINER_HPP
