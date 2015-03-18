//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR__IBEHAVIOR_NETWORK_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR__IBEHAVIOR_NETWORK_HPP


/**
 * @file    evolution/core/behavior/IBehaviorNetwork.hpp
 * @brief   Interface for a network of behaviors.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IBehaviorNetwork
 **/


#include <evolution/core/base/IObject.hpp>
#include <evolution/core/behavior/IBehavior.hpp>


namespace Evolution
{


    /**
     * @brief
     * Interface to a network of behaviors.
     *
     * IBehaviorNetwork and IBehaviorContainer are closely related. 
     * Generally, a class that implements one will implement the
     * other. IBehaviorNetwork deals purely with the execution of a
     * set of (usually connected) behaviors. IBehaviorContainer
     * provides the means to access and manipulate that set, and, for
     * contained behaviors, to access the external environment.
     *
     * @see  Evolution::IBehaviorContainer
     *
     * @todo
     * We need a mechanism for one behavior network to access another.
     *
     * @ingroup core_behavior
     **/
    class EVOLUTION_EXPORT_BEHAVIOR IBehaviorNetwork : public IBehaviorContainer
    {
    public: // Types

        typedef IBehavior::PortId PortId;  ///< Port id

    public: // Constants

        /// Interface ID.
        static const char* const INTERFACE_ID; // "Evolution.IBehaviorNetwork"

    public: // Structors

        /// Constructor.
        IBehaviorNetwork () {}

        /// Destructor.
        virtual ~IBehaviorNetwork () {}

    public: // Access

        /** Obtains the ID of the network.
         * @return Returns the id string of the behavior network.
         */
        virtual const char* get_id () const = 0;
        
        /** Obtains the invocation_interval (DiffTime) of the network.
         * @return The invocation interval for the network.
         */
        virtual DiffTime get_invocation_interval() const = 0;
    
        /**
         * @brief Sets the invocation_interval (DiffTime) of the network.
         * @param interval [in]; The invocation interval.
         */
        virtual Result set_invocation_interval(DiffTime interval) = 0;

    public: // Status

        /**
         * @brief
         * Indicates if the network currently is running.
         * @return True if running, false otherwise.
         **/
        virtual bool is_running () const = 0;

    public: // Execution

        /**
         * @brief
         * Runs the network in a separate thread, returning
         * immediately.
         *
         * All behaviors are activated when execution begins; stop()
         * must be called to deactivate.
         * @param ticket [in]; must have proper ticket to run.
         * @result  RESULT_SUCCESS, RESULT_FAILURE
         **/
        virtual Result run (TicketId ticket) = 0;

        /**
         * @brief
         * Runs the network once, returning when finished.
         *
         * Activates all behaviors on the first call to run once. For
         * efficiency in the event of multiple run_once() calls,
         * behaviors remain active until a stop() call.
         * @param ticket [in]; must have proper ticket to run.
         * @result  RESULT_SUCCESS, RESULT_FAILURE
         **/
        virtual Result run_once (TicketId ticket) = 0;

        /**
         * @brief
         * Stops execution, deactivating all behaviors.
         * @param ticket [in]; must have proper ticket to run.
         * @result  RESULT_SUCCESS, RESULT_FAILURE
         **/
        virtual Result stop (TicketId ticket) = 0;

        /**
         * @brief
         * Deactivates a behavior in the network, suspending its
         * computation.
         * @param ticket [in]; must have proper ticket to run.
         * @param behavior_id [in]; id of behavior to suspend.
         **/
        virtual Result suspend_behavior (TicketId ticket, const char* behavior_id) = 0;

        /**
         * @brief
         * (Re)activates a behavior in the network, resume its
         * computation.
         * @param ticket [in]; must have proper ticket to run.
         * @param behavior_id [in]; id of behavior to resume.
         * @result  RESULT_SUCCESS, RESULT_FAILURE
         **/
        virtual Result resume_behavior (TicketId ticket, const char* behavior_id) = 0;

        /**
         * @brief Activates all behaviors.  
         * 
         * Simply activates all behaviors contained in the behavior network. 
         * @return  RESULT_SUCCESS, RESULT_FAILURE or other error.
         */
        virtual Result activate_behaviors () = 0;

    public: // Membership

        /**
         * @brief
         * Adds a behavior to the network with the specified ID, or if
         * no ID is specified, uses the ID of the behavior.
         *
         * @param   ticket      [in]; Security ticket
         * @param   behavior    [in]; The behavior to add.
         * @param   behavior_id [in]; The ID of the behavior.
         * @return  RESULT_SUCCESS, RESULT_ALREADY_EXISTS or other error.
         *
         * @pre     behavior != NULL
         * @pre     (behavior->get_id() != NULL) || (behavior_id != NULL)
         **/ 
        virtual Result add_behavior (TicketId ticket, IBehavior* behavior,
                                     const char* behavior_id = NULL) = 0;

        /**
         * @brief
         * Removes the behavior with the specified ID from the network.
         *
         * @param   ticket [in]; The security ticket
         * @param   behavior_id  [in]; The ID of the behavior
         * @return  RESULT_SUCCESS, RESULT_NOT_FOUND or other error.
         *
         * @pre     behavior_id is valid
         **/
        virtual Result remove_behavior (TicketId ticket, const char* behavior_id) = 0;

        /**
         * @brief
         * Removes all behaviors.
         * @param ticket [in]; must have proper ticket to run.
         * @pre  !is_running()
         **/
        virtual Result remove_all (TicketId ticket) = 0;

        /**
         * @brief
         * Retrieves the behavior with the specified ID.
         *
         * @note    
         * The caller must call remove_ref() on the behavior when done!
         *
         * @param   ticket [in]; The security ticket.
         * @param   behavior_id  [in]; The ID of the behavior
         * @param   behavior_ptr [out]; Pointer to the behavior
         * @return  RESULT_SUCCESS, RESULT_NOT_FOUND or other error.
         *
         * @pre     behavior_id is valid
         **/
        virtual Result get_behavior (TicketId ticket, const char* behavior_id, IBehavior **behavior_ptr) = 0;

    public: // Connections

        /**
         * @brief
         * Connects two behaviors on the specified ports.
         *
         * @param  ticket [in]; The security ticket
         * @param  source_id [in]; The ID of the source behavior
         * @param  source_port [in]; The source behavior's output port for the connection
         * @param  target_id [in]; The ID of the target behavior
         * @param  target_port [in]; the target behavior's input port for the connection
         * @param  type [in]; The type of connection.  The default
         * value is CONNECTION_NORMAL, which is for standard
         * feed-forward connections.  If the connection is a feedback
         * type connection, CONNECTION_BACK_PROPAGATION should be
         * used.
         *
         * @pre  source_id specifies a behavior in the container.
         * @pre  source_port is valid output port on the source behavior.
         * @pre  target_id specifies a behavior in the container.
         * @pre  target_port is valid input port on the target behavior.
         */
        virtual Result connect_behaviors (TicketId ticket,
                                          const char* source_id, PortId source_port,
                                          const char* target_id, PortId target_port,
                                          ConnectionType type = CONNECTION_NORMAL) = 0;

        /**
         * @brief
         * Disconnects two behaviors on the specified ports.
         *
         * @param  ticket [in]; The security ticket
         * @param  source_id [in]; the ID of the source behavior
         * @param  source_port [in]; the source behavior's output port for the connection
         * @param  target_id [in]; the ID of the target behavior
         * @param  target_port [in]; the target behavior's input port for the connection
         *
         * @pre  source_id specifies a behavior in the container
         * @pre  source_port is valid output port on the source behavior
         * @pre  target_id specifies a behavior in the container
         * @pre  target_port is valid input port on the target behavior
         */
        virtual Result disconnect_behaviors (TicketId ticket,
                                             const char* source_id, PortId source_port,
                                             const char* target_id, PortId target_port) = 0;

    }; // end class IBehaviorNetwork


} // end namespace Evolution


/**
 * This macro should be put in each instantiable networks's klass.hpp file in a
 * public declaration section.  It declares a factory creation function for
 * the network and a string name to reference it, used in registry lookup.
 * @param klass  Name of the class (e.g. Foo)
 * @param driver Name of the driver (e.g. "Evolution.Foo")
 * @see Evolution::BehaviorRegistry
 */
#define DECLARE_BEHAVIOR_NETWORK(klass, driver) \
static Evolution::Result create_behavior_network(Evolution::TicketId ticket,                          \
                                      Evolution::BehaviorNetworkConfig& config,            \
                                      Evolution::IResourceContainer& resource_container,   \
                                      Evolution::IBehaviorNetwork** driver_ptr);           \
static const char* const __get_driver_id () { static const char* dr = driver; return (dr); } \
static const char* const _DRIVER_ID;


/**
 * The macro forces the behavior to be registered in the behavior registry
 * when the library is loaded.  This macro should be included in the klass.cpp
 * file.
 *
 * @param klass  Name of the class (eg Foo)
 **/
#define REGISTER_BEHAVIOR_NETWORK(klass) \
const char * const klass::_DRIVER_ID = klass::__get_driver_id(); \
static Evolution::Result __reg_result = \
  Evolution::BehaviorRegistry::register_behavior_network (klass::__get_driver_id(), klass::create_behavior_network);

#endif  // INCLUDE_EVOLUTION_BEHAVIOR__IBEHAVIOR_NETWORK_HPP
