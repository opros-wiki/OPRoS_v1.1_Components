//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_CALLBACK_BEHAVIOR_HPP
#define INCLUDE_EVOLUTION_TASK_CALLBACK_BEHAVIOR_HPP


/**
 * @file   evolution/core/task/CallbackBehavior.hpp
 * @brief  A behavior that allows one to register callback
 *         functions that are called with input values.
 *         (Used by the task system to integrate tasks and
 *         behaviors.)
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::CallbackBehavior
 * @see Evolution::Callback
 **/


#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IObject.hpp>
#include <evolution/core/behavior/BehaviorImpl.hpp>


namespace Evolution
{


    /**
     * @class CallbackBehavior
     * @brief This class makes it convenient for non-behaviors to
     * monitor behavior outputs.
     *
     * Once this behavior is in a network, its register_callback
     * method can be used to connect it to another behavior's output
     * port and register a callback object that will be notified when
     * output data is pushed.
     *
     * This class is used in the TEL when defining subclasses of
     * Primitive to gather outputs from behaviors.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK CallbackBehavior : public BehaviorImpl
    {
    public: // Types

        /**
         * @brief Callback defines the interface to be used by classes
         * implementing callbacks.  The object passed to
         * register_callback must be a subclass of Callback.
         */
        class EVOLUTION_EXPORT_TASK Callback
        {
        public:
            /**
             * @brief Constructor.
             */
            Callback () {};

            /**
             * @brief Destructor.
             */
            virtual ~Callback () {};

            /**
             * @brief This is the method that CallbackBehavior calls
             * when new data is pushed.

             * @param source      [in]; A pointer to the behavior pushing the data
             * @param source_port [in]; The output port the data is coming from
             * @param data        [in]; A pointer to the BehaviorData
             */
            virtual void handle_input (IBehavior *source, PortId source_port,
                                       BehaviorData *data) = 0;
        };


    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param ticket       [in]; The security ticket
         * @param container    [in]; Reference to the IBehaviorContainer
         *                           object that will contain this behavior
         * @param input_count  [in]; The number of input ports that the
         *                           behavior will have
         * @param output_count [in]; The number of output ports that 
         *                           the behavior will have
         */
        CallbackBehavior (TicketId ticket,
                          IBehaviorContainer& container,
                          PortCount input_count,
                          PortCount output_count);

        /**
         * @brief Destructor.
         */
        virtual ~CallbackBehavior ();


    public: // Methods for registering and unregistering callbacks.

        /**
         * @brief Registers a callback for a particular output from a
         * behavior.
         *
         * This method registers the callback and connects this
         * behavior to the behavior the callback is interested in.
         *
         * @param source      [in] A pointer to the behavior the
         *                         output will come from
         * @param source_port [in] The output port the data will come
         *                         from
         * @param cb          [in] A pointer to the Callback that will
         *                         handle the data
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result register_callback(IBehavior* source, PortId source_port,
                                         Callback* cb);

        /**
         *
         * @brief Unregisters a previously-registered callback.
         *
         * The tuple (source, source_port, cb) must match a previously
         * registered tuple.  This method does not disconnect the
         * source behavior.
         *
         * @param source      [in]; A pointer to the behavior the
         *                          the ouput will come from
         * @param source_port [in]; The output port the data will come
         *                          from
         * @param cb          [in]; A pointer to a registered Callback
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result unregister_callback(IBehavior* source, PortId source_port,
                                           Callback *cb);

    public: // Overridden BehaviorImpl methods.

        /**
         * @brief No output from this behavior.
         *
         * @return RESULT_SUCCESS.
         */
        virtual Result compute_output ();

        /**
         * @brief Takes inputs, looks for the matching registered callbacks,
         * and if there are any it calls them.
         *
         * @param   ticket      [in];      The security ticket
         * @param   input_port  [in];      The input port to which the data is headed
         * @param   source      [in];      The ID of the upstream behavior, source of the data
         * @param   source_port [in];      The output port from which the data is coming.
         * @param   data        [in, opt]; The data to be received
         *
         * @return  RESULT_SUCCESS or a failure code.
         */
        virtual Result push_input(TicketId ticket, PortId input_port,
                                  IBehavior* source, PortId source_port,
                                  BehaviorData* data);

        /**
         * @brief Obtains the ID of the behavior instance.
         * 
         * @return The id of the behavior.
         **/
        virtual const char* get_id() const;

    protected: // Types

        typedef String CallbackKey;

        typedef std::list<Callback *> CallbackList;

        typedef std::multimap<CallbackKey, CallbackList*> CallbackTable;


    protected: // Methods

        /**
         * @brief Creates a unique identifier for this callback.
         *
         * @param source      [in]; A pointer to the behavior
         * @param source_port [in]; The pehavior's output port
         *
         * @return
         */
        virtual CallbackKey make_callback_key(IBehavior* source, PortId source_port);


    protected: // Members

        IBehaviorNetwork* _network;

        CallbackTable _callback_table;


    }; // end class CallbackBehavior

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_TASK_CALLBACK_BEHAVIOR_HPP
