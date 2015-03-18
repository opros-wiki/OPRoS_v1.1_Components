//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_AGGREGATE_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_AGGREGATE_HPP


/**
 * @file    evolution/core/behavior/BehaviorAggregate.hpp
 * @brief   Base class for an aggregate behavior.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::BehaviorAggregate
 * @see Evolution::OutputPassThrough
 * @see Evolution::InputPassThrough
 */


#include <set>
#include <evolution/core/behavior/BehaviorImpl.hpp>
#include <evolution/core/behavior/BehaviorRegistry.hpp>


namespace Evolution
{

    /**
     * @class BehaviorAggregate
     * @brief Utility base class to ease implementation of aggregate
     * behaviors.
     * @ingroup core_behavior
     **/
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorAggregate : public BehaviorImpl
    {
    public: // Constants

        // IDs for the internal pass-through behaviors.

        /// Input pass-through ID.
        static const char* const INPUT_PASS_THROUGH_ID; // "Evolution.BehaviorAggregate.InputPassThrough"

        /// Output pass-through ID.
        static const char* const OUTPUT_PASS_THROUGH_ID; // "Evolution.BehaviorAggregate.OutputPassThrough"

    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param ticket       [in]; TicketId to be used as the behaviors security ticket
         * @param container    [in]; Reference to an IBehaviorContainer to place the behavior in
         * @param input_count  [in]; Number of input ports
         * @param output_count [in]; Number of output ports
         */
        BehaviorAggregate (TicketId ticket,
                           IBehaviorContainer& container,
                           PortCount input_count,
                           PortCount output_count);

        /**
         * @brief Destructor.
         */
        virtual ~BehaviorAggregate ();

    public: // IBehavior interface

        /**
         * @brief Activates the behavior.
         *
         * @param ticket [in]; Security ticket to use to try to activate the
         *                     behavior
         *
         * @return RESULT_SUCCESS or a failure code.
         */
        virtual Result activate (TicketId ticket);

        /**
         * @brief Deactivates the behavior.
         *
         * @param ticket [in]; Security ticket to use to try to deactivate the
         *                     behavior
         *
         * @return RESULT_SUCCESS or a failure code.
         */
        virtual Result deactivate (TicketId ticket);

        /**
         * @brief Handles data I/O and any necessary per-cycle computation.
         * Happens before internal network is run once.
         *
         * @return RESULT_SUCCESS or a failure code.
         */
        virtual Result prepare_network ();

        /**
         * @brief Handles data I/O and any necessary per-cycle computation.
         * Happens after internal network is run once.
         *
         * @return RESULT_SUCCESS or a failure code.
         */
        virtual Result compute_output ();

        /**
         * @brief Pushes input data to the internal behavior network.
         *
         * @param ticket      [in]; Ticket to use to try to execute push_input
         * @param input_port  [in]; PortId identifier for input port
         * @param source      [in]; Source behavior pointer
         * @param source_port [in]; Source behavior port
         * @param data        [in, opt]; BehaviorData to be pushed onto the
         *                               input port
         *
         * @return RESULT_SUCCESS or a failure code.
         *
         * @see  IBehavior::push_input()
         */
        virtual Result push_input (TicketId ticket,
                                   PortId input_port,
                                   IBehavior* source,
                                   PortId source_port,
                                   BehaviorData* data);

        /**
         * @brief Invokes the internal behavior network and propagates its
         * data downstream.
         *
         * @param ticket [in]; TicketId to use with invoke
         *
         * @return RESULT_SUCCESS or a failure code.
         *
         * @see IBehavior::invoke()
         */
        virtual Result invoke (TicketId ticket);

    protected: // Helpers

        /**
         * @brief Obtains the container for the internal network of behaviors.
         *
         * @return The container for the internal network of behaviors.
         */
        IBehaviorNetwork& get_aggregate_network ()
        {
            return (*_aggregate_network);
        }

        /**
         * @brief Obtains the container for the internal network of behaviors.
         *
         * @return The container for the internal network of behaviors.
         */
        IBehaviorContainer& get_aggregate_container ()
        {
            return (*_aggregate_container);
        }

        /**
         * @brief Obtains the input pass-through behavior.
         *
         * @return The input pass-through behavior.
         */
        IBehavior* get_input_pass_through ()
        {
            return (&_aggregate_input_pass);
        }

        /**
         * @brief Obtains the output pass-through behavior.
         *
         * @return The output pass-through behavior.
         */
        IBehavior* get_output_pass_through ()
        {
            return (&_aggregate_output_pass);
        }

    public: // Inner types

        /**
         * @brief Helper class. Holds input connections to aggregate.
         */
        class EVOLUTION_EXPORT_BEHAVIOR InputPassThrough : public BehaviorImpl
        {
        public: //Structors

            /**
             * @brief Constructor
             *
             * @param ticket     [in];  TicketId to use with internal behavior
             *                          InputPassThrough's constructor
             * @param container  [out]; IBehaviorContainer to place behavior in
             * @param port_count [in];  Number of input ports on the behavior aggregate
             * @param aggregate  [in];  Reference to parent aggregate behavior
             */
            InputPassThrough (TicketId ticket,
                              IBehaviorContainer& container,
                              PortCount port_count,
                              BehaviorAggregate& aggregate)
                : BehaviorImpl (ticket, container, port_count, port_count),
                  _aggregate (aggregate)
            {}

        public:  // IBehavior interface

            // Required, but empty implementation.
            Result compute_output () { return (RESULT_SUCCESS); }

            // Empty implementation for efficiency.
            Result invoke (TicketId ticket) { return (RESULT_SUCCESS); }

            /**
             * @brief Not sufficient to simply use compute output.
             * Passes input data through to the aggregate's inner network.
             *
             * @param ticket      [in]; TicketId to use with push_input
             * @param input_port  [in]; PortId identifier function to pass input to
             * @param source      [in]; Source behavior
             * @param source_port [in]; Source behavior output port, source of 'data'
             * @param data        [in, opt]; BehaviorData to put into input port
             *
             * @see  BehaviorImpl::push_input()
             */
            Result push_input (TicketId ticket, PortId input_port, IBehavior* source,
                               PortId source_port, BehaviorData* data);

        protected: // Implementation

            BehaviorAggregate& _aggregate;   ///< BehaviorAggregate reference
        }; // end inner class InputPassThrough

        /**
         * Helper class. Holds output connections to aggregate.
         */
        class EVOLUTION_EXPORT_BEHAVIOR OutputPassThrough : public BehaviorImpl
        {
        public: //Structors

            /**
             * @brief Constructor.
             *
             * @param ticket     [in]; TicketId to use with OutputPassThrough
             *                         constructor
             * @param container  [out]; IBehaviorContainer to place
             *                         OutputPassThrough behavior in
             * @param port_count [in]; Number of output ports on the behavior
             *                         aggregate
             * @param aggregate  [in]; Reference to parent aggregate behavior
             */
            OutputPassThrough (TicketId ticket,
                               IBehaviorContainer& container,
                               PortCount port_count,
                               BehaviorAggregate& aggregate)
                : BehaviorImpl (ticket, container, port_count, port_count),
                  _aggregate (aggregate) {}

        public:  // IBehavior interface

            // Required, but empty implementation.
            Result compute_output () { return (RESULT_SUCCESS); }

            // Empty implementation for efficiency.
            Result invoke (TicketId ticket) { return (RESULT_SUCCESS); }

            /**
             * @brief Not sufficient to simply use compute output.
             * Passes input data through to the aggregate's inner network.
             *
             * @param ticket      [in]; TicketId to use with push_input
             * @param input_port  [in];  PortId identifier function to pass input to.
             * @param source      [in]; source behavior.
             * @param source_port [in]; source behavior output port, source of 'data'
             * @param data        [in, opt]; BehaviorData to put into input port.
             *
             * @see  BehaviorImpl::push_input()
             */
            virtual Result push_input (TicketId ticket,
                                       PortId input_port, IBehavior* source,
                                       PortId source_port, BehaviorData* data);

        protected: // Implementation

            BehaviorAggregate& _aggregate;  ///< BehaviorAggregate reference
        }; // end inner class OutputPassThrough

        /**
         * @brief Friend class
         * @see Evolution::OutputPassThrough
         */
        friend class OutputPassThrough;

        /// Set of configuration structures for internal behaviors.
        typedef std::set<BehaviorConfig*> BehaviorConfigSet;

    protected: // Implementation

        void                *_internal_network;     ///< Internal network
        BehaviorConfigSet   _internal_config;      ///< Internal configuration
        IBehaviorNetwork    *_aggregate_network;    ///< IBehaviorNetwork pointer for subclasses
        IBehaviorContainer  *_aggregate_container;  ///< IBehaviorContainer pointer for subclasses
        IResourceContainer  *_resource_container;   ///< Resource container pointer


        InputPassThrough    _aggregate_input_pass;   ///< Aggregate input pass through
        OutputPassThrough   _aggregate_output_pass;  ///< Aggregate output pass through

        String              _id;                     ///< String id
        Result              _result;                 ///< Result type
    }; // end class BehaviorAggregate

/**
 * @brief
 * This handy macro creates a behavior of the specified type and ID
 * and adds it to the internal network of the aggregate.
 *
 * @param T [in]; behavior class type
 * @param I [in]; ID string of the behavior
 * @param P [in]; name of the variable to assign to the behavior pointer
 */
#define BEHAVIOR_AGGREGATE_ADD_BEHAVIOR(T,I,P) \
    Evolution::BehaviorConfig* _config_ ## P = new Evolution::BehaviorConfig (I, BEHAVIOR_TYPE(T)); \
    _internal_config.insert (_config_ ## P); \
    T* P = NULL; \
    if (Evolution::BehaviorRegistry::create_behavior (_behavior_ticket, *_config_ ## P, \
                                                      get_aggregate_container (), \
                                                      (Evolution::IBehavior**)&P) == RESULT_SUCCESS) \
    { \
        get_aggregate_network ().add_behavior (_behavior_ticket, P); \
        P->remove_ref (); \
    } \
    else \
    { \
        ERSP_LOG_WARN("Failed to create internal behavior '%s' (type %s)", I, #T); \
    }

/**
 * @brief
 * This handy macro creates a behavior of the specified type ID 
 * and adds it to the internal network of the aggregate.
 *
 * @param T [in]; behavior class type id (a string)
 * @param I [in]; ID string of the behavior
 * @param P [in]; name of the variable to assign to the behavior pointer
 */
#define BEHAVIOR_AGGREGATE_ADD_BEHAVIOR_BY_ID(T,I,P) \
    Evolution::BehaviorConfig* _config_ ## P = new Evolution::BehaviorConfig (I, T); \
    _internal_config.insert (_config_ ## P); \
    IBehavior* P = NULL; \
    if (Evolution::BehaviorRegistry::create_behavior (_behavior_ticket, *_config_ ## P, \
                                                      get_aggregate_container (), \
                                                      &P) == RESULT_SUCCESS) \
    { \
        get_aggregate_network ().add_behavior (_behavior_ticket, P); \
        P->remove_ref (); \
    } \
    else \
    { \
        ERSP_LOG_WARN("Failed to create internal behavior '%s' (type %s)", I, T); \
    }


/**
 * @brief
 * As with BEHAVIOR_AGGREGATE_ADD_BEHAVIOR, this handy macro creates a
 * behavior of the specified type and adds it to the internal network
 * of the aggregate. It takes a BehaviorConfig object, however,
 * allowing more flexibility.
 *
 * @param T [in];  behavior class type
 * @param C [in];  behavior configuration
 * @param P [in];  name of the variable to assign to the behavior pointer
 */
#define BEHAVIOR_AGGREGATE_ADD_BEHAVIOR_CONFIG(T,C,P) \
    T* P = NULL; \
    if (Evolution::BehaviorRegistry::create_behavior (_behavior_ticket, C, \
                                                      get_aggregate_container (), \
                                                      (Evolution::IBehavior**)&P) == RESULT_SUCCESS) \
    { \
        get_aggregate_network ().add_behavior (_behavior_ticket, P); \
        P->remove_ref (); \
    } \
    else \
    { \
        String behavior_id; \
        C.get_id (&behavior_id); \
        ERSP_LOG_WARN("Failed to create internal behavior '%s' (type %s)", behavior_id.c_str (), #T); \
    }

} // end namespace Evolution

#endif //INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_AGGREGATE_HPP
