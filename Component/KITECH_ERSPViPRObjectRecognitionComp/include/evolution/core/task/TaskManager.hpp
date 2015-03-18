//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_TASK_TASKMANAGER_HPP
#define INCLUDE_EVOLUTION_TASK_TASKMANAGER_HPP


#include <utility>
#include <list>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IObject.hpp>
#include <evolution/core/task/Types.hpp>
#include <evolution/core/task/Parallel.hpp>
#include <evolution/core/task/Event.hpp>
#include <evolution/core/task/EventFilter.hpp>
#include <evolution/core/task/CallbackBehavior.hpp>


/**
 * @file evolution/core/task/TaskManager.hpp
 * @brief The task manager.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::TaskManager
 **/


namespace Evolution
{

    class ResourceManager;
    class IResourceContainer;
    class BehaviorManager;
    class BehaviorDataWriter;
    class IThread;
    class IMutex;
    class IConditionVariable;

    class Task;
    class Primitive;
    class TaskInputBehavior;


    /**
     * @brief A templated std::list for holding Task pointers.
     */
    typedef std::list<Task*> TaskList;


    /**
     * @class TaskManager
     * @brief The task manager.
     *
     * Responsible for keeping track of tasks, resources, and
     * primitives (behavior networks).
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK TaskManager
    {

    protected:

        char _id[64];
        unsigned _refcount;
        bool _initialized;
        TaskList _tasks;
        IMutex* _task_lock;
        IConditionVariable* _task_status_condition;
        IConditionVariable* _primitive_status_condition;

    public: // Structors

        /**
         * @brief Constructor.  Not to be used, use get_task_manager
         * instead.
         */
        TaskManager ();

        /**
         * @brief Destructor.  Make sure to always call shutdown to
         * cleanly shutdown the task manager.
         */
        virtual ~TaskManager ();

    public:

        /**
         * @brief Returns the singleton global task manager.
         *
         * @return The global TaskManager.
         */
        static TaskManager* get_task_manager();

        /**
         * @brief Initializes the task manager.  Explicit
         * initialization is usually not required.
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result initialize ();

        /**
         * @brief Cleanly shuts down the task manager.
         *
         * Attempts to terminate all tasks and deactivate all resources.
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result shutdown ();

    public: // Access

        /**
         * @brief Gets the resource container being used by the task manager.
         *
         * @param container [out]; The resource container
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result get_resource_container (IResourceContainer** container);

        /**
         * @brief Returns the current rate at which primitive networks
         * are being run, in Hertz.
         *
         * @returns The execution frequency of the network.
         */
        double get_network_rate ();

        /**
         * @brief Sets the rate at which primitive networks will be
         * run, in Hertz.
         *
         * @param rate_hz [in]; The network execution rate
         */
        void set_network_rate(double rate_hz);


    public: // ** Tasks

        // -- Methods
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief For internal use only.
         */
        Result add_task (Task* task);

        /**
         * @brief For internal use only.
         */
        Result wait_for_task (Task* waiter, Task* task);

        /**
         * @brief For internal use only.
         */
        Result wait_for_first_complete_task (Task* waiter, TaskList* tasks,
                                             Task** completed,
                                             bool terminate_others);

        /**
         * @brief For internal use only.
         */
        Result wait_for_all_complete_tasks (Task* waiter, TaskList* tasks);
#endif

        /**
         * @brief Starts up a new, top-level task.
         *
         * The fuction install_task starts the specified task running in 
         * its own new thread and returns immediately.  Note that the returned
         * Task is a new reference, so the calling code must call
         * Task::remove_ref on it when it's no longer needed (it is
         * safe to call remove_ref immediately after installing the
         * task if you don't need to keep track of the task at all).
         *
         * @param functor  [in]; The function to execute
         * @param num_args [in]; The number of elements in the TaskArg list
         * @param args     [in]; The TaskArg list which contains the function
         *                       arguments
         *
         * @return A newly created reference to the Task.
         */
        Task* install_task (TaskFunctor* functor, int num_args = 0, TaskArg* args = NULL);

        /**
         * @brief Starts up a new, top-level task.
         *
         * Starts the specified task running in its own thread and
         * returns immediately.
         *
         * Potentially could be a source of memory leaks as there is
         * nobody to clean up the return TaskValue of the task when it
         * completes.
         *
         * @param functor  [in]; The function to execute
         * @param args     [in]; The TaskArgVector which contains the function
         *                       arguments
         *
         * @return A newly created reference to the Task.
         */
        Task* install_task (TaskFunctor* functor, TaskArgVector* args);


    public:
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief For internal use only.
         */
        void update_task_status (Task* task, TaskStatus status);

        /*
         * @brief For internal use only.
         */
        void signal_task_termination (Task* task);
#endif


    public: // ** Primitives

        // -- Types
        typedef void* NetworkId;


        // -- Methods
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief For internal use only.
         */
        Result add_primitive (Primitive* primitive);

        /**
         * @brief For internal use only.
         */
        Result remove_primitive (Primitive* primitive);

        /**
         * @brief For internal use only.
         */
        Result wait_for_primitive (Task* waiter, Primitive* primitive);

        // -- Methods used by primitives.
        /**
         * @brief For internal use only.
         *
         * Instantiates a behavior network associated with a primitive
         * task and begins executing the network.
         *
         * This method is called by the
         * Primitive::initialize. Subclasses of primitive do not need
         * to use this method.
         */
        Result add_primitive_network (Primitive* primitive,
                                      const char* network_name,
                                      NetworkId* network_id);

        /**
         * @brief For internal use only.
         *
         * Removes the specified primitive behavior network from the
         * Task Manager.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         *
         * @return RESULT_SUCCESS if the attempt to register the
         * callback succeeded.
         */
        Result remove_primitive_network (NetworkId network_id);

#endif // end DOXYGEN_INTERNAL

        /**
         * @brief Installs a callback to handle output from a
         * primitive behavior.
         *
         * Primitives use this method to install a callback object to
         * handle outputs from behaviors in their associated behavior
         * networks.
         *
         * When data is output on the specified port on the specified
         * behavior, the callback object's handle_input method will be
         * called.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior whose output will be monitored
         * @param port_name   [in]; The name of the output port to monitor
         * @param callback    [in]; The callback object that will handle the outputs
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result add_prim_value_callback (NetworkId network_id,
                                        const char* behavior_id,
                                        const char* port_name,
                                        CallbackBehavior::Callback* callback);

        /**
         * @brief Installs a callback to handle output from a
         * primitive behavior.
         *
         * Primitives use this method to install a callback object to
         * handle outputs from behaviors in their associated behavior
         * networks.
         *
         * When data is output on the specified port on the specified
         * behavior, the callback object's handle_input method will be
         * called.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior whose output will be monitored
         * @param port_id     [in]; The ID of the output port to monitor
         * @param callback    [in]; The callback object that will handle the outputs
         *;
         * @return RESULT_SUCCESS if the attempt to register the
         * callback succeeded.
         */
        Result add_prim_value_callback (NetworkId network_id,
                                        const char* behavior_id,
                                        IBehavior::PortId port_id,
                                        CallbackBehavior::Callback* callback);

        /**
         * @brief Supplies a constant input to a primitive behavior.
         *
         * Primitives use this method to supply a behavior in their
         * associated behavior network with a constant input.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior to which input will be supplied
         * @param port_name   [in]; The name of the port to send input to
         * @param data        [in]; The value to be supplied as input
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result set_prim_input_value (NetworkId network_id,
                                     const char* behavior_id,
                                     const char* port_name,
                                     BehaviorDataWriter* data);

        /**
         * @brief Supplies a constant input to a primitive behavior.
         *
         * Primitives use this method to supply a behavior in their
         * associated behavior network with a constant input.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior to which input will be supplied
         * @param port_id     [in]; The ID of the port to send input to
         * @param data        [in]; The value to be supplied as input
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result set_prim_input_value (NetworkId network_id,
                                     const char* behavior_id,
                                     IBehavior::PortId port_id,
                                     BehaviorDataWriter* data);

        /**
         * @brief Stops supplying an input to a primitive behavior.
         *
         * Primitives use this method to stop supplying a previously
         * set constant input to a behavior in their associated
         * behavior network.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior
         * @param port_name   [in]; The name of the port to send input to
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result unset_prim_input_value (NetworkId network_id,
                                       const char* behavior_id,
                                       const char* port_name);

        /**
         * @brief Stops supplying an input to a primitive behavior.
         *
         * Primitives use this method to stop supplying a previously
         * set constant input to a behavior in their associated
         * behavior network.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior
         * @param port_id     [in]; The ID of the port to send input to
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result unset_prim_input_value (NetworkId network_id,
                                       const char* behavior_id,
                                       IBehavior::PortId port_id);


        /**
         * @brief Sets a parameter value for a primitive behavior.
         *
         * Primitives use this method to set parameter values for
         * behaviors in their associated behavior network.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior
         * @param param       [in]; The name of the parameter to set
         * @param value       [in]; The parameter value
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result set_prim_behavior_parameter (NetworkId network_id,
                                            const char* behavior_id,
                                            const char* param,
                                            const char* value);

        /**
         * @brief Sets a parameter value for a primitive behavior.
         *
         * Primitives use this method to set parameter values for
         * behaviors in their associated behavior network.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior
         * @param param       [in]; The name of the parameter to set
         * @param value       [in]; The parameter value
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result set_prim_behavior_parameter (NetworkId network_id,
                                            const char* behavior_id,
                                            const char* param,
                                            double value);

        /**
         * @brief Sets a parameter value for a primitive behavior.
         *
         * Primitives use this method to set parameter values for
         * behaviors in their associated behavior network.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior
         * @param param       [in]; The name of the parameter to set
         * @param value       [in]; The parameter value
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result set_prim_behavior_parameter (NetworkId network_id,
                                            const char* behavior_id,
                                            const char* param,
                                            long value);

        /**
         * @brief Sets a parameter value for a primitive behavior.
         *
         * Primitives use this method to set parameter values for
         * behaviors in their associated behavior network.
         *
         * @param network_id  [in]; The behavior network containing the behavior of interest
         * @param behavior_id [in]; The ID of the behavior
         * @param param       [in]; The name of the parameter to set
         * @param value       [in]; The parameter value
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        Result set_prim_behavior_parameter (NetworkId network_id,
                                            const char* behavior_id,
                                            const char* param,
                                            bool value);


        /**
         * @brief Looks up a behavior and port in a primitive network.
         *
         * This function can be used to look up a behavior and port by
         * name.
         *
         * @param network_id   [in];  The behavior network containing the
         *                            behavior of interest
         * @param behavior_id  [in];  The ID of the behavior
         * @param port_name    [in];  The name of the port
         * @param is_input_port[in];  Whether looking up an input port
         * @param behavior_ptr [out]; A pointer to the behavior
         * @param port_ptr     [out]; A pointer to the port ID
         *
         * @return RESULT_SUCCESS if the behavior and port exist.
         */
        Result find_behavior_port (NetworkId network_id,
                                   const char* behavior_id,
                                   const char* port_name,
                                   bool is_input_port,
                                   IBehavior** behavior_ptr,
                                   IBehavior::PortId* port_ptr);        
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief For internal use only.
         *
         * Returns the behavior manager that the task manager uses.
         */
        BehaviorManager* get_behavior_manager (NetworkId network_id) const;


    public: // ** Events

        /**
         * @brief For internal use only.  Use Task::enable_event.
         */
        void enable_event (Task* task, const char* pattern);

        /**
         * @brief For internal use only.  Use Task::disable_event.
         */
        void disable_event (Task* task, const char* pattern);

        /**
         * @brief Raises an event.
         *
         * @param event [in]; The event to raise.
         */
        void raise_event (Event event);

    protected: // ** Tasks

        Result remove_task (Task* task);

        Result finish_task (Task* task);

        void raise_task_events (Task* task, TaskStatus status);

        void debug_log__task_list() const;


    protected: // ** Events

        bool event_is_enabled (const Event& event) const;

        void debug_log_enabled_events () const;


    protected: // ** Primitives

        // -- Primitives: Types
        typedef std::list<Primitive*> PrimitiveList;

        struct PrimNet
        {
            PrimNet();
            ~PrimNet();

            BehaviorManager* manager;
            CallbackBehavior* cb_behavior;
            TaskInputBehavior* input_behavior;
            Primitive* primitive;
            bool enabled;
            String network_file;
        };

        typedef std::list<PrimNet*> PrimNetList;


        // -- Primitives: Members
        ResourceManager* _resource_manager;

        IResourceContainer* _resource_container;

        PrimNetList _prim_nets;

        IThread* _prim_nets_stepper_thread;

        bool _keep_stepping_prim_nets;

        IConditionVariable* _prim_nets_condition;

        double _network_rate;

        PrimitiveList _primitives;


        // --  Primitives: Methods

        Result step_primitive_network ();

        void finish_primitive (Primitive* prim, TaskStatus status);

        void update_primitive_status (Primitive* primitive, TaskStatus status,
                                      const char* debug_str);

        Result add_callback_behavior (PrimNet* pnet);

        Result add_input_behavior (PrimNet* pnet);

        Result ensure_resource_manager ();

        Result find_named_port (NetworkId network_id,
                                const char* behavior_id,
                                bool is_input_port,
                                const char* port_name,
                                IBehavior::PortId* port_id);

        Result step_primitives ();

        Result start_behavior_network ();

        Result stop_behavior_network (bool block);

        static void* behavior_stepper_func (TaskManager*);

    protected: // ** Events

        typedef std::pair<EventFilter, Task*> EventHandler;
        typedef std::list<EventHandler> EventHandlerList;

        EventHandlerList _enabled_events;
        IConditionVariable* _event_condition;
        
#endif // end DOXYGEN_INTERNAL

    }; // end class TaskManager

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_TASK_TASKMANAGER_HPP
