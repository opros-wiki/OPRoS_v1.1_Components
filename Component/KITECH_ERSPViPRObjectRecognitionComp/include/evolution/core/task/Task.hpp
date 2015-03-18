//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TASK_HPP
#define INCLUDE_EVOLUTION_TASK_TASK_HPP


/**
 * @file   evolution/core/task/Task.hpp
 * @brief  Representation of high-level task.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Task
 **/


#include <vector>
#include <algorithm>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IObject.hpp>
#include <evolution/core/base/IThread.hpp>
#include <evolution/core/task/Types.hpp>
#include <evolution/core/task/TaskFunctor.hpp>
#include <evolution/core/task/TaskArg.hpp>
#include <evolution/core/task/TaskValue.hpp>
#include <evolution/core/task/Event.hpp>


namespace Evolution
{

#ifndef DOXYGEN_INTERNAL
    // Forward declarations, lets not clutter up doxygen.
    class TaskManager;
    class IConditionVariable;
    class IThread;
    class IMutex;
#endif

    /**
     * @class Task
     * @brief Represents a task in the task execution layer.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK Task : public IObject
    {
    public: // Structors.

        /**
         * @brief Constructor.
         *
         * @param manager  [in]; The task manager
         * @param proc     [in]; The task funtor
         * @param num_args [in]; Number of elements in the TaskArg list
         * @param args     [in]; The functor argument list
         * @param parent   [in]; The parent of this task
         */
        Task (TaskManager *manager, TaskFunctor* proc, unsigned int num_args, const TaskArg* args,
              Task* parent);

        /**
         * @brief Constructor.
         *
         * @param manager  [in]; The task manager
         * @param proc     [in]; The task funtor
         * @param args     [in]; The functor argument list
         * @param parent   [in]; The parent of this task
         */
        Task (TaskManager *manager, TaskFunctor* proc, const TaskArgVector* args,
              Task* parent);

        /**
         * @brief Destructor.
         */
        virtual ~Task ();

    public: // IObject implementation.

        /**
         * @brief Gets the ID of the Task.
         *
         * @return the ID string.
         */
        virtual const char* get_id ();

        /**
         * @brief Adds a reference to the Task.  Part of the IObject
         * reference counting protocol.
         *
         * @return The current reference count.
         */
        virtual unsigned add_ref ();

        /**
         * @brief Removes a reference to the Task.  Part of the IObject
         * reference counting protocol.
         *
         * @return The current reference count.
         */
        virtual unsigned remove_ref ();

        /**
         * @brief Returns the current reference count.  Part of the IObject
         * reference counting protocol.
         *
         * @return The current reference count.
         */
        virtual unsigned ref_count () ;
        
        /**
         * @brief Obtains an interface (Part of the IObject protocol).
         * Task does not support any interfaces; this method will
         * always fail.
         *
         * @param ticket        [in];  Security ticket
         * @param interface_id  [in];  The name of the interface to obtain
         * @param interface_ptr [out]; The interface
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        virtual Result obtain_interface (TicketId ticket,
                                         const char* interface_id,
                                         void** interface_ptr);

    public: // Task arguments, results, status, success/failure, termination.

        /**
         * @brief Returns a pointer to a TaskArgVector containing the task's
         * arguments.
         *
         * @return The TaskArgVector pointer.
         */
        const TaskArgVector* get_arguments () const;

        /**
         * @brief Once the task has completed, returns the result returned by
         * the task.  (A value of TASK_NO_RESULT means no meaningful
         * result was returned.)
         *
         * @return The task completion result.
         */
        TaskValue get_result () const;

        /**
         * @brief Returns the task's current status.
         *
         * @return The task status.
         */
        TaskStatus get_status () const;

        /**
         * @brief Checks whether another task requested that this task be
         * terminated.
         *
         * @see terminate
         *
         * @return true/false.
         */
        bool termination_requested ();

        /**
         * @brief Requests that this task be terminated.
         *
         * Currently does no more than request; the task itself must
         * use termination_requested to determine whether another task
         * has requested that it be terminated and abort its execution.
         *
         * @see termination_requested
         */
        void terminate ();


        /**
         * @brief Sets a flag indicating that this task failed (causes the
         * task's result, once the task is complete, to be
         * TASK_FAILURE).
         */
        void set_failed ();


        /**
         * @brief Returns a pointer to the task's TaskManager.
         *
         * @return The task manager.
         */
        TaskManager* get_task_manager () const;

        /**
         * @brief Returns a pointer to this task's parent (may be NULL).
         *
         * @return The task's parent.
         */
        Task* get_parent () const;

        /**
         * @brief Returns a pointer to the task functor associated with this task.
         *
         * @return The underlying task functor.
         */
        TaskFunctor* get_functor () const;

    public: // Events & Synchronization.

        /**
         * @brief Blocks until an event matching the specified pattern is
         * raised, or until the specified wait time has elapsed.  This
         * method is usually called by the task itself.
         *
         * If a wait time is specified and a timeout occurs, the Event
         * returned will be an event with a NULL type.
         *
         * @param  pattern    [in];      The event pattern to wait for
         * @param  wait_time  [in, opt]; The time (in ms) to wait for the event.
         *                               The default is ERSP_INFINITE
         *
         * @return The event matching the specified pattern.
         */

        Event wait_for_event(const char* pattern,
                             unsigned long wait_time = ERSP_INFINITE);

        /**
         * @brief Enables events with the specified pattern.
         *
         * Normally, if an event is raised before a task begins
         * waiting for the event, that event is lost.  But a task can
         * explicitly enable an event pattern which will cause events
         * matching that pattern to be put into the task's event queue
         * as they are raised.  Later calls to wait_for_event can then
         * consume the events.
         *
         * enable_event can be called multiple times in order to
         * enable different event patterns simultaneously.
         *
         * This method can be useful when a task contains a loop where
         * it is waiting for event and then handling them, and it
         * wants to ensure that no events are missed.
         *
         * @param pattern [in]; An event pattern to look for
         */
        void enable_event(const char* pattern);

        /**
         * @brief
         * Disables events that were previously enabled.
         *
         * Once events are disabled, they will no longer go into the
         * task's event queue (if the task explicitly calls
         * wait_for_event it will still receive events, however).
         *
         * If a particular event pattern was enabled twice with
         * enable_event, disable_event must be called twice with that
         * pattern as an argument before the event pattern will
         * actually be disabled.
         *
         * @param pattern [in]; An event pattern to look for
         */
        void disable_event(const char* pattern);

        /**
         * @brief

         * Returns the number of events matching the specified pattern
         * currently waiting in the task's event queue.
         *
         * If the pattern is NULL, the total number of all events in
         * the task's event queue will be returned.
         *
         * @param pattern [in, opt]; An event pattern to look for
         *
         * @return The number of events matching the specified pattern.
         */
        unsigned long num_events_in_queue (const char* pattern = NULL) const;

    public:
        // These are parts of the internal Task interface and
        // should not be called by user code.
#ifndef DOXYGEN_INTERNAL

        /**
         * @brief For internal use only.
         *
         * Creates a new thread to run this task.
         */
        Result spawn ();

        /**
         * @brief For internal use only.
         *
         * Called by the TaskManager to give this task an event it has
         * registered interest in.
         */
        void receive_event (const Event& e);

        /**
         * @brief For internal use only.
         *
         * Checks whether a TaskStatus value indicates task
         * completion, either successful or unsuccessful.
         */
        static bool task_status_complete (TaskStatus status);

        /**
         * @brief For internal use only.
         *
         * Sets the status of the task and notifies the TaskManager
         * that the status may have changed.
         */
        void set_status (TaskStatus status, bool notify=true);

        /**
         * @brief For internal use only.
         *
         * Sets the dummy flag.
         */
        void set_is_dummy ();
#endif // end DOXYGEN_INTERNAL

        /**
         * @brief Returns useful information (task and functor memory address)
         * for debugging a primitive.
         *
         * @return A sting containing some characteristics of the task.
         */
        String debug_str () const;


    protected: // Methods.

        /**
         * @brief Code shared between constructors.
         */
        void construct();

        /**
         * @brief For lazy initialization of the event handling structures.
         */
        void maybe_initialize_event_queue ();

        /**
         * @brief Enables an event, but assumes the queue has already been
         * locked.
         *
         * @param pattern [in]; An event pattern to look for
         */
        void enable_event_internal(const char* pattern);

        /**
         * @brief Disables an event, but assumes the queue has already been
         * locked.
         *
         * @param pattern [in]; An event pattern to look for
         */
        void disable_event_internal(const char* pattern);

        /**
         * @brief If there's an event in the queue that matches the specified
         * pattern, it is removed and stored at the location specified
         * by event_ptr.  Returns true if a matching event was found,
         * false otherwise.
         *
         * @param pattern   [in];  An event pattern to look for
         * @param event_ptr [out]; The event matching the event pattern
         *
         * @return True if the matching event was found, false otherwise.
         */
        bool get_event (const char* pattern, Event* event_ptr);

        /**
         * @brief Removes any events from the queue that are not currently
         * enabled.
         */
        void clean_event_queue ();


        /**
         * @brief Notifies the TaskManager that the task's status may have
         * changed.
         *
         * @param new_status [in]; The new task status
         */
        void notify_of_status_change (TaskStatus new_status);

        /**
         * @brief The function run by the task thread.
         *
         * @param thread_param [in]; The function to execute
         *
         * @return NULL.
         */
        static ThreadProcResult THREAD_PROC_CALL task_thread_proc (
            ThreadProcParam thread_param);

    protected:

        /**
         * @brief Hidden copy construtor.
         *
         * @param task [in]; The task to copy
         */
        Task (const Task& task);

    protected: // Types

        typedef std::list<String> EventPatternList;

    protected: // Members

        char _id[128];
        int _ref_count;
        TaskManager* _manager;

        bool _is_dummy;

        TaskStatus _status;
        IConditionVariable* _status_condition;

        IThread* _thread;
        TaskFunctor* _functor;
        bool _thread_done;

        Task* _parent;

        TaskArgVector _args;
        TaskValue* _result;

        Event::EventList* _events;
        EventPatternList* _enabled_events;
        IConditionVariable* _event_condition;

        bool _failed;

        IMutex* _terminated_mutex;
        IMutex* _ref_count_mutex;

        bool _terminated;

        /*
         * @class EventIsDisabled
         * @brief Functor for STL filters.  Returns true if an event is
         * currently disabled by the task.
         *
         * @ingroup core_task
         */
        struct EventIsDisabled
        {
            Task* _task;

            /**
             * @brief Constructor.
             *
             * @param task [in]; The task object
             */
            EventIsDisabled (Task* task) : _task(task) { };

            /**
             * @brief Functor call operator.
             *
             * @param event [in]; The event object.
             *
             * @return True if the event is enabled, false otherwise.
             */
            bool operator () (const Event& event) const
            {
                EventPatternList::iterator i;
                i = std::find_if(_task->_enabled_events->begin(),
                                 _task->_enabled_events->end(),
                                 Event::PatternMatchesEvent(event));
                return (i == _task->_enabled_events->end());
            }
        };


        friend class TaskFunctor;

    }; // End class Task.

} // End namespace Evolution.

#endif  // INCLUDE_EVOLUTION_TASK_TASK_HPP
