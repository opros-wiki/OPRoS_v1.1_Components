/* Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * Any reproduction is strictly prohibited without the explicit
 * written permission of Evolution Robotics, Inc.  All rights reserved.
 */

#ifndef INCLUDE_EVOLUTION_TASK_TASK_C_H
#define INCLUDE_EVOLUTION_TASK_TASK_C_H


#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/BaseC.h>


/**
 * @file evolution/core/task/TaskC.h
 * @brief C interface to the Task Execution Layer.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_task
 **/


#ifdef  __cplusplus
extern "C" {
#endif

#include <stddef.h>


    /*** C TYPES ***/

    /**
     * Q: Why are these all structs instead of typedefs to void*?
     *
     * A: To have some modicum of type safety. With structs you can't
     *    accidentally assign a pointer of one type to another.
     **/


    /** C mapping for Evolution::TaskStatus. */
    typedef enum 
    {
        ERSP_TASK_READY,
        ERSP_TASK_RUNNING,
        ERSP_TASK_TERMINATED,
        ERSP_TASK_SUCCESS,
        ERSP_TASK_FAILURE,
        ERSP_TASK_RESTART
    } ERSPTaskStatus;
    /** C mapping for Evolution::TaskManager. */
    typedef struct 
    {
        void* dummy;
    } ERSPTaskManager;

    /** C mapping for Evolution::TaskContext. */
    typedef struct 
    {
        void* dummy;
    } ERSPTaskContext;

    /** C mapping for Evolution::TaskArg. */
    typedef struct 
    {
        void* dummy;
    } ERSPTaskArg;

    /**
     * @brief
     * An array of ERSPTaskArg, to emphasize when functions operate on
     * the entire array. **/
    typedef ERSPTaskArg* ERSPTaskArgArray;

    /** C mapping for Evolution::TaskValue. */
    typedef struct 
    {
        void* dummy;
    } ERSPTaskValue;
/*     typedef ERSPTaskArg ERSPTaskValue; */

    /** C mapping for Evolution::Task. */
    typedef struct 
    {
        void* task;
    } ERSPTask;

    /** C mapping for an Evolution::TaskFunctor. */
    typedef struct 
    {
        void* dummy;
    } ERSPTaskFunctor;

    /// Convenience typedef for a pointer to an ERSP Task Functor
    typedef ERSPTaskFunctor* ERSPTaskFunctorPtr;

    /** C mapping for Evolution::Parallel. */
    typedef struct 
    {
        void* dummy;
    } ERSPParallel;

    /** C mapping for Evolution::Event. */
    typedef struct 
    {
        void* dummy;
    } ERSPEvent;

    /** C mapping for Evolution::Image. */
    typedef struct 
    {
        void* dummy;
    } ERSPImage;


    /** C mapping for ERSP_INFINITE constant. */
    extern EVOLUTION_EXPORT_TASK const unsigned long ERSP_TASK_INFINITE;


    /**
     * @brief Returns the singleton global task manager.
     *
     * @return The global TaskManager.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskManager* ersp_task_manager__get_task_manager ();

    /**
     * @brief Cleanly shuts down the task manager.
     *
     * Attempts to terminate all tasks and deactivate all resources.
     *
     * @param manager [in]; The task manager to shut down.
     *
     * @return RESULT_SUCCESS if the operation succeeded.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_manager__shutdown (ERSPTaskManager* manager);

    /**
     * @brief Initializes the task manager.  Explicit
     * initialization is usually not required.
     *
     * @param manager [in]; The task manager to initialize.
     *
     * @return RESULT_SUCCESS if the operation succeeded.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_manager__initialize (ERSPTaskManager* manager);

    /**
     * @brief Blocks until the task is complete.
     *
     * @param manager [in];
     * @param waiter  [in];
     * @param task    [in];
     *
     * @return RESULT_SUCCESS if the operation succeeded.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_manager__wait_for_task (ERSPTaskManager* manager,
                                                 ERSPTask* waiter,
                                                 ERSPTask* task);

    /**
     * @brief Blocks until a task in the list complete.
     *
     * @param manager    [in];
     * @param waiter     [in];
     * @param num_tasks  [in];
     * @param task_array [in, out];
     * @param completed_task_ptr [out];
     * @param terminate_others [in]; Boolean value indicating whether
     * to terminate the remaining tasks when the first one completes.
     *
     * @return RESULT_SUCCESS if the operation succeeded.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_manager__wait_for_first_complete_task (ERSPTaskManager* manager,
                                                                ERSPTask* waiter,
                                                                int num_tasks,
                                                                ERSPTask** task_array,
                                                                ERSPTask** completed_task_ptr,
                                                                int terminate_others);

    /**
     * @brief Starts up a new, top-level task.
     *
     * The fuction install_task starts the specified task running in its own
     * new thread and returns immediately.  Note that the returned
     * Task is a new reference, so the calling code must call
     * Task::remove_ref on it when it's no longer needed (it is
     * safe to call remove_ref immediately after installing the
     * task if you don't need to keep track of the task at all).
     *
     * @param manager  [in]; The task manager
     * @param task     [in]; The function to execute
     * @param args     [in]; List which contains the function arguments
     *
     * @return A newly created reference to the Task.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTask* ersp_task_manager__install_task (ERSPTaskManager* manager,
                                               ERSPTaskFunctorPtr task,
                                               ERSPTaskArgArray args);

    /**
     * @brief Raises an event.
     *
     * @param manager [in]; The task manager
     * @param event   [in]; The event to raise.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_manager__raise_event (ERSPTaskManager* manager,
                                         ERSPEvent* event);

    /**
     * @brief Returns the current rate at which primitive networks
     * are being run, in Hertz.
     *
     * @param manager [in]; The task manager of interest.
     *
     * @returns The execution frequency of the network.
     */
    extern EVOLUTION_EXPORT_TASK
    double ersp_task_manager__get_network_rate (ERSPTaskManager* manager);

    /**
     * @brief Sets the rate at which primitive networks will be
     * run, in Hertz.
     *
     * @param manager [in]; The task manager
     * @param rate_hz [in]; The network execution rate
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_manager__set_network_rate (ERSPTaskManager* manager, double rate_hz);

    /**
     * @brief Gets the resource container being used by the task manager.
     *
     * @param manager [in]; The task manager
     *
     * @return The resource container.
     */
    extern EVOLUTION_EXPORT_TASK
    void* ersp_task_manager__get_resource_container (ERSPTaskManager* manager);

    /**
     * @brief Creates a new context containing the specified
     * arguments.
     *
     * This method should be used inside a task that wants to call
     * another task synchronously.
     *
     * @param args           [in]; The arguments list for the task
     * @param parent_context [in]; The parent context of the new context
     *
     * @return The newly created TaskContext object.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskContext* ersp_task_context__create (ERSPTaskArgArray args,
                                                ERSPTaskContext* parent_context);

    /**
     * @brief Creates a new context containing the specified
     * arguments.
     *
     * This method should be used when calling a task outside of
     * another task (in main, for example), and one wants to use a
     * task manager other than the default one.
     *
     * @param args        [in]; The arguments list for the task
     * @param alt_manager [in]; The default task manager for the new
     *                          context
     *
     * @return The newly created TaskContext object.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskContext* ersp_task_context__create_with_manager (ERSPTaskArgArray args,
                                                             ERSPTaskManager* alt_manager);

    /**
     * @brief Creates a new context containing the specified
     * arguments.
     *
     * @param args           [in]; The arguments list for the task
     * @param source_context [in]; The context to copy from
     *
     * @return The newly created TaskContext object.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskContext* ersp_task_context__copy (ERSPTaskArgArray args,
                                              ERSPTaskContext* source_context);

    /**
     * @brief Destroys the task context.
     *
     * @param context [in]; The task context to destroy
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_context__delete (ERSPTaskContext* context);

    /**
     * @brief Returns a pointer to the vector of task arguments.
     *
     * @param context [in]; The task context
     *
     * @return The TaskArgVector pointer.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskArgArray ersp_task_context__get_args (ERSPTaskContext* context);

    /**
     * @brief Returns a pointer to the task manager associated with this
     * context.
     *
     * @param context [in]; The task context
     *
     * @return The underlying task manager.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskManager* ersp_task_context__get_task_manager (ERSPTaskContext* context);

    /**
     * @brief Returns a pointer to the task associated with this
     * context.
     *
     * @param context [in]; The task context
     *
     * @return The Task pointer.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTask* ersp_task_context__get_task (ERSPTaskContext* context);

    /**
     * @brief Sets the failure flag of the task associated with
     * this context's task.
     *
     * @param context [in]; The task context
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_context__set_failed (ERSPTaskContext* context);

    /**
     * @brief Returns true if another task has requested the
     * termination of the task associated with this context.
     *
     * @param context [in]; The task context
     *
     * @return 1 if termination requested, 0 otherwise.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_task_context__termination_requested (ERSPTaskContext* context);

    /*** TASK ARGS ***/

    /* Structors */

    /**
     * @brief Creates a ERSPTaskArgArray with the specified size.
     * Use ersp_task_arg__delete() to destroy the array.
     *
     * @param num_args [in]; The desired size of the array.
     *
     * @return The ERSPTaskArgArray of size num_args.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskArgArray ersp_task_arg__create (int num_args);

    /**
     * @brief Destroys a ERSPTaskArgArray created with
     * ersp_task_arg__create().
     *
     * @param args [in]; The array to delete.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__delete (ERSPTaskArgArray args);

    /**
     * @brief Retruns the number of elements in the ERSPTaskArgArray.
     *
     * @param args [in]; The argument array
     *
     * @return The number of elements in the array
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_task_arg__num_args (ERSPTaskArgArray args);

    /**
     * @brief Returns the argument type at the specigied index.
     *
     * @param args [in]; The argument array
     * @param index [in]; An index into the array
     *
     * @return The argument value type.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValueType ersp_task_arg__get_type (ERSPTaskArgArray args, int index);

    /**
     * @brief Returns the argument type as a human readable string.
     *
     * @param args  [in]; The argument array
     * @param index [in]; An index into the array
     *
     * @return The human readable argument type.
     */
    extern EVOLUTION_EXPORT_TASK
    const char* ersp_task_arg__get_type_name (ERSPTaskArgArray args, int index);

    /**
     * @brief Sets the value of an element in the argument list.
     *
     * @param args  [out]; The argument list
     * @param index [in];  An index into the argument list
     * @param value [in];  The value to set
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__set_bool (ERSPTaskArgArray args, int index, int value);

    /**
     * @brief Sets the value of an element in the argument list.
     *
     * @param args  [out]; The argument list
     * @param index [in];  An index into the argument list
     * @param value [in];  The value to set
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__set_double (ERSPTaskArgArray args, int index, double value);

    /**
     * @brief Sets the value of an element in the argument list.
     *
     * @param args  [out]; The argument list
     * @param index [in];  An index into the argument list
     * @param value [in];  The value to set
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__set_string (ERSPTaskArgArray args, int index, const char* value);

    /**
     * @brief Sets the value of an element in the argument list.
     *
     * @param args  [out]; The argument list
     * @param index [in];  An index into the argument list
     * @param value [in];  The value to set
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__set_pointer (ERSPTaskArgArray args, int index, void* value);

    /**
     * @brief Sets the value of an element in the argument list.
     *
     * @param args  [out]; The argument list
     * @param index [in];  An index into the argument list
     * @param array [in];  The value to set
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__set_doublearray (ERSPTaskArgArray args, int index, ERSPDoubleArray* array);

    /**
     * @brief Sets the value of an element in the argument list.
     *
     * @param args  [out]; The argument list
     * @param index [in];  An index into the argument list
     * @param image [in];  The value to set
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_arg__set_image (ERSPTaskArgArray args, int index, ERSPImage* image);

    /**
     * Returns the value of the indexed element in the argument list.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The value stored at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_task_arg__get_bool (ERSPTaskArgArray args, int index);

    /**
     * Returns the value of the indexed element in the argument list.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The value stored at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    const char* ersp_task_arg__get_string (ERSPTaskArgArray args, int index);

    /**
     * Returns the value of the indexed element in the argument list.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The value stored at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    double ersp_task_arg__get_double (ERSPTaskArgArray args, int index);

    /**
     * Returns the value of the indexed element in the argument list.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The value stored at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    void* ersp_task_arg__get_pointer (ERSPTaskArgArray args, int index);

    /**
     * Returns the value of the indexed element in the argument list.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The value stored at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPDoubleArray* ersp_task_arg__get_doublearray (ERSPTaskArgArray args, int index);

    /**
     * Returns the value of the indexed element in the argument list.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The value stored at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPImage* ersp_task_arg__get_image (ERSPTaskArgArray args, int index);


    /**
     * @brief Returns the string reperesentation (for debugging purposes) of the
     * indexed element.
     *
     * @param args  [in]; The argument list
     * @param index [in]; An index into the argument list
     *
     * @return The string representation of the value at the specified index.
     */
    extern EVOLUTION_EXPORT_TASK
    char* ersp_task_arg__as_string (ERSPTaskArgArray args, int index);

    /**
     * @brief Constructs an ERSPTaskValue.
     *
     * @param v [in]; The initialization value
     *
     * @return The ERSPTaskValue instance.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_value__create_bool (int v);

    /**
     * @brief Constructs an ERSPTaskValue.
     *
     * @param v [in]; The initialization value
     *
     * @return The ERSPTaskValue instance.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_value__create_double (double v);

    /**
     * @brief Constructs an ERSPTaskValue.
     *
     * @param v [in]; The initialization value
     *
     * @return The ERSPTaskValue instance.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_value__create_string (const char* v);

    /**
     * @brief Constructs an ERSPTaskValue.
     *
     * @param v [in]; The initialization value
     *
     * @return The ERSPTaskValue instance.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_value__create_pointer (void* v);

    /**
     * @brief Constructs an ERSPTaskValue.
     *
     * @param v [in]; The initialization value
     *
     * @return The ERSPTaskValue instance.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_value__create_doublearray (ERSPDoubleArray* v);

    /**
     * @brief Constructs an ERSPTaskValue.
     *
     * @param image [in]; The initialization value
     *
     * @return The ERSPTaskValue instance.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_value__create_image (ERSPImage* image);

    /**
     * @brief Destroys an ERSPTaskValue.
     *
     * @param value [in]; The value to destroy
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_value__delete (ERSPTaskValue* value);

    /* Access */

    /**
     * @brief Returns the value type contained within ERSPTaskValue.
     *
     * @param value [in]; The value type to retrieve
     *
     * @return The type information.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValueType ersp_task_value__get_type (ERSPTaskValue* value);

    /**
     * @brief Returns the value type as a string.
     * 
     * @param value [in]; The value type to retrieve
     *
     * @return The string representation of the value type.
     */
    extern EVOLUTION_EXPORT_TASK
    const char* ersp_task_value__get_type_name (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue.
     *
     * @param value [in]; The value to retrieve
     *
     * @return 0 if false.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_task_value__get_bool (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue.
     *
     * @param value [in]; The value to retrieve
     *
     * @return The string value.
     */
    extern EVOLUTION_EXPORT_TASK
    const char* ersp_task_value__get_string (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue.
     *
     * @param value [in]; The value to retrieve
     *
     * @return The double value.
     */
    extern EVOLUTION_EXPORT_TASK
    double ersp_task_value__get_double (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue.
     *
     * @param value [in]; The value to retrieve
     *
     * @return The pointer value.
     */
    extern EVOLUTION_EXPORT_TASK
    void* ersp_task_value__get_pointer (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue.
     *
     * @param value [in]; The value to retrieve
     *
     * @return The double array value.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPDoubleArray* ersp_task_value__get_doublearray (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue.
     *
     * @param value [in]; The value to retrieve
     *
     * @return The image value.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPImage* ersp_task_value__get_image (ERSPTaskValue* value);

    /**
     * @brief Checks if a value is contained within ERSPTaskValue.
     *
     * @param value [in]; The value to check
     *
     * @return 0 if empty.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_task_value__is_empty (ERSPTaskValue* value);

    /**
     * @brief Returns the value contained within ERSPTaskValue in string form.
     *
     * @param value [in]; The value to retrieve
     *
     * @return The value string.
     */
    extern EVOLUTION_EXPORT_TASK
    char* ersp_task_value__as_string (ERSPTaskValue* value);

    /**
     * @brief Creates a new event.
     *
     * @param type [in]; The event type.
     *
     * @return The newly created event. Use ersp_event__delete() to free the
     * event.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPEvent* ersp_event__create (const char* type);

    /**
     * @brief Creates a copy of an event.
     *
     * @param event [in]; The event to copy.
     *
     * @return The copied event. Use ersp_event__delete() to free the
     * event.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPEvent* ersp_event__copy (const ERSPEvent* event);

    /**
     * @brief Releases an event created with ersp_event__create() or
     * ersp_event__copy()
     *
     * @param event [in]; The event to delete
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_event__delete (ERSPEvent* event);

    /**
     * @brief Sets an event's property.
     *
     * @param event [in]; The event
     * @param name  [in]; The name of the property to set
     * @param value [in]; The new value
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_event__set_property (ERSPEvent* event,
                                   const char* name,
                                   ERSPTaskValue* value);

    /**
     * @brief Returns the value of the desired property contained within
     * the event. Note, the caller is responsible for freeing (using delete)
     * the result.
     *
     * @param event [in]; An event which contains the property
     * @param name  [in]; The name of the property
     *
     * @return The property value or NULL if it does not exist.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_event__get_property (const ERSPEvent* event,
                                             const char* name);

    /**
     * @brief Checks whether an event has the desired property.
     *
     * @param event [in]; An event which contains the property
     * @param name  [in]; The name of the property
     *
     * @return 1 if the property exists, 0 otherwise.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_event__has_property (const ERSPEvent* event, const char* name);

    /**
     * @brief Gets the type of an event.
     *
     * @param event [in]; The event.
     *
     * @return The type of the event.
     */
    extern EVOLUTION_EXPORT_TASK
    const char* ersp_event__get_type (const ERSPEvent* event);

    /**
     * @brief Returns a string description of an event.
     *
     * @param event [in]; The event.
     *
     * @return The event description.
     */
    extern EVOLUTION_EXPORT_TASK
    char* ersp_event__as_string (const ERSPEvent* event);

    /**
     * @brief Checks whether an event is empty.
     *
     * @param event [in]; The event.
     *
     * @return 1 if the event is empty, 0 otherwise.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_event__is_empty (const ERSPEvent* event);

    /*** TASK ***/
    /**
     * @brief Adds a reference to a task.
     *
     * @param task [in]; The task.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task__add_ref (ERSPTask* task);

    /**
     * @brief Removes a reference from a task.
     *
     * @param task [in]; The task.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task__remove_ref (ERSPTask* task);

    /**
     * @brief Returns a task's status.
     *
     * @param task [in]; The task.
     *
     * @return The task's status.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskStatus ersp_task__get_status (ERSPTask* task);

    /**
     * @brief Returns a task's result.  The caller must call free on
     * the result when it is no longer needed.
     *
     * @param task [in]; The task.
     *
     * @return The task's result.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task__get_result (ERSPTask* task);

    /**
     * @brief Returns the task manager associated with a task.
     *
     * @param task [in]; The task.
     *
     * @return The associated task manager.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskManager* ersp_task__get_task_manager (ERSPTask* task);

    /**
     * @brief Waits for an event.
     *
     * @param task [in]; The current task.
     * @param pattern [in]; The desired event specification.
     * @param wait_time [in]; The maximum time, in milliseconds, to
     *                        wait for the event (0 means forever).
     *
     * @return The event.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPEvent* ersp_task__wait_for_event (ERSPTask* task, const char* pattern,
                                          unsigned long wait_time);

    /**
     * @brief Enables an event for a task.
     *
     * All calls to ersp_task__enable_event must be matched with a
     * corresponding call to ersp_task_disable_event.
     *
     * @param task [in]; The current task.
     * @param pattern [in]; The desired event specification.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task__enable_event (ERSPTask* task, const char* pattern);

    /**
     * @brief Disables an event for a task.
     *
     * @param task [in]; The current task.
     * @param pattern [in]; The event specification to disable.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task__disable_event (ERSPTask* task, const char* pattern);

    /**
     * @brief Checks the number of pending events for a task.
     *
     * @param task [in]; The current task.
     * @param pattern [in]; The event specification to check.
     *
     * @return The number of pending events matching the pattern.
     */
    extern EVOLUTION_EXPORT_TASK
    unsigned long ersp_task__num_events_in_queue (ERSPTask* task, const char* pattern);

    /**
     * @brief Terminates a task.
     *
     * @param task [in]; The task to terminate.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task__terminate (ERSPTask* task);


    /*** TASK FUNCTOR ***/

    /**
     * @brief Runs a task functor.
     *
     * @param functor [in]; The functor to run.
     * @param context [in]; The task context containing the arguments
     *                      to pass to the functor.
     *
     * @return the ERSPTaskValue returned by the functor (must be
     * freed by caller).
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskValue* ersp_task_functor__run (ERSPTaskFunctorPtr functor, ERSPTaskContext* context);

  
    /**
     * @brief Currys a task functor.
     *
     * @param functor [in]; The functor to curry.
     * @param args    [in]; The arguments with which to curry the functor.
     *
     * @return A new, curried functor.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskFunctorPtr ersp_task_functor__curry (ERSPTaskFunctorPtr functor,
                                                 ERSPTaskArgArray args);

    /*** PARALLEL ***/

    /* Structors */
   
    /**
     * @brief Creates a new Parallel construct.
     *
     * @param context [in]; The context of the current task (or NULL
     *                      if there is no current task).
     *
     * @return A new Parallel object.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPParallel* ersp_parallel__create (ERSPTaskContext* context);

    /**
     * @brief Frees a Parallel object.
     *
     * @param parallel [in]; The Parallel construct to free.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_parallel__delete (ERSPParallel* parallel);

    /* Addition */
    /**
     * @brief Adds a task to a Parallel construct.
     *
     * @param parallel [in]; The Parallel to add the task to.
     * @param task     [in]; The task functor to be run in parallel.
     * @param args     [in]; The arguments to pass to the task functor.
     *
     * @return The task object corresponding to the newly added task
     * functor.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTask* ersp_parallel__add_task (ERSPParallel* parallel,
                                       ERSPTaskFunctorPtr task,
                                       ERSPTaskArgArray args);

    /* Waiting */
    /**
     * @brief Starts tasks in parallel and waits for the first
     * complete task.
     *
     * @param parallel [in]; The Parallel construct.
     * @param task_ptr [out]; A pointer to the first Task to complete
     *                        will be stored here.
     *
     * @return ERSP_TASK_SUCCESS if successful.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_parallel__wait_for_first_task (ERSPParallel* parallel, ERSPTask** task_ptr);

    /**
     * @brief Starts tasks in parallel and waits for all tasks to
     * complete.
     *
     * @param parallel [in]; The Parallel construct.
     *
     * @return ERSP_TASK_SUCCESS if successful.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_parallel__wait_for_all_tasks (ERSPParallel* parallel);



    /*
     *Task Registry
     */

    /**
     * @brief Looks up a task ID in the task registry.
     *
     * @param type_id [in]; The task ID.
     *
     * @return the task functor with the given ID if one exists,
     * otherwise NULL.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTaskFunctorPtr ersp_task_registry__find_task (const char* type_id);

    /**
     * @brief Returns all registered task functors.
     *
     * @param num_tasks_ptr  [out]; The number of functors will be stored
     *                       here.
     * @param task_names_ptr [out]; An array of char* will be stored
     *                       here containing the names of the task functors.
     *                       Note that this array, along with each entry,
     *                       must be freed by the caller.
     * @param task_array_ptr [out]; An array of task functors will be
     *                       stored here.  Note that this array, along
     *                       with each entry, must be freed by the caller.
     *
     * @return the task functor with the given ID if one exists,
     * otherwise NULL.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_task_registry__get_all_tasks (int *num_tasks_ptr,
                                            const char*** task_names_ptr,
                                            ERSPTaskFunctorPtr** task_array_ptr);

    /**
     * @brief Registers a task functor in the task registry.
     *
     * @param type_id [in]; The task functor ID.
     * @param functor [in]; The task functor to register.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_registry__register_task (const char* type_id,
                                                  ERSPTaskFunctorPtr functor);

    /**
     * @brief Removes a task functor from the task registry.
     *
     * @param type_id [in]; The task functor ID.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_registry__unregister_task (const char* type_id);

    /**
     * @brief Loads a shared library containing task definitions.
     *
     * @param library [in]; The name of the library.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_registry__load_library (const char* library);

    /**
     * @brief Unloads a shared library.
     *
     * @param library [in]; The name of the library.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_task_registry__unload_library (const char* library);


    /*
     * Image
     *
     * TODO: draw_arc, draw_circle, draw_solid_box, draw_ellipse,
     * write_to_matrix.
     */

    /**
     * @brief Creates a new image.
     *
     * @param width             [in]; The width of the image, in pixels.
     * @param height            [in]; The height of the image, in pixels.
     * @param color_format      [in]; The desired color format.
     * @param data              [in]; The initial image data.
     * @param data_color_format [in]; The color format of the initial image data.
     * @param time              [in]; The image timestamp.
     *
     * @return A new image.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPImage* ersp_image__create (int width, int height, ERSPColorFormat color_format, const unsigned char* data,
                                   ERSPColorFormat data_color_format, ERSPTimestamp time);

    /**
     * @brief Frees an image.
     *
     * @param image [in]; The image to free.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_image__delete (ERSPImage* image);

    /**
     * @brief Loads an image from a file.
     *
     * @param filename [in]; The path to the image file.
     *
     * @return A new image.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPImage* ersp_image__load_from_file (const char* filename);

    /**
     * @brief Makes a copy of an image.
     *
     * @param image [in]; The image to copy.
     *
     * @return A new image.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPImage* ersp_image__copy (const ERSPImage* image);

    /**
     * @brief Returns the width of an image.
     *
     * @param image [in]; The image.
     *
     * @return The image's width, in pixels.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_image__get_width (const ERSPImage* image);

    /**
     * @brief Returns the height of an image.
     *
     * @param image [in]; The image.
     *
     * @return The image's height, in pixels.
     */
    extern EVOLUTION_EXPORT_TASK
    int ersp_image__get_height (const ERSPImage* image);

    /**
     * @brief Returns an image's color format.
     *
     * @param image [in]; The image.
     *
     * @return The color format of the image.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPColorFormat ersp_image__get_color_format (const ERSPImage* image);

    /**
     * @brief Returns an image's timestamp.
     *
     * @param image [in]; The image.
     *
     * @return The timestamp of the image.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPTimestamp ersp_image__get_timestamp (const ERSPImage* image);

    /*
    // FIXME: Decide what to do here.
    extern EVOLUTION_EXPORT_TASK
    int ersp_image__get_camera (const ERSPImage* image);
    */

    /**
     * @brief Returns the value of a single color channel of a single
     * pixel in an image.
     *
     * @param image   [in]; The image.
     * @param x       [in]; The pixel x coordinate.
     * @param y       [in]; The pixel y coordinate.
     * @param channel [in]; The color channel.
     *
     * @return The value of the requested pixel's color channel.
     */
    extern EVOLUTION_EXPORT_TASK
    unsigned char ersp_image__get_pixel_color (const ERSPImage* image,
                                               int x, int y, int channel);


    /**
     * @brief Sets the color of a single pixel in an image.
     *
     * @param image        [in]; The image.
     * @param x            [in]; The pixel x coordinate.
     * @param y            [in]; The pixel y coordinate.
     * @param v1           [in]; The first color value.
     * @param v2           [in]; The second color value.
     * @param v3           [in]; The third color value.
     * @param source_space [in]; The color space in which to interpret
     *                     the color values.
     */
    extern EVOLUTION_EXPORT_TASK
    void ersp_image__set_pixel (ERSPImage* image, int x, int y,
                                unsigned char v1, unsigned char v2, unsigned char v3,
                                ERSPColorspace source_space);

    /**
     * @brief Gets the raw data associated with an image.
     *
     * @param image        [in]; The image.
     *
     * @return The image's raw data.
     */
    extern EVOLUTION_EXPORT_TASK
    const unsigned char* ersp_image__get_raw_data (ERSPImage* image);

    /**
     * @brief Writes an image to a file.
     *
     * @param image    [in]; The image.
     * @param filename [in]; The path to save the image to.
     * @param format   [in]; The name of the desired image format.
     * @param options  [in]; A string containing any format-specific
     *                 options.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_image__write_file (const ERSPImage* image,
                                       const char* filename,
                                       const char* format, const char* options);

    /**
     * @brief Writes an image to memory.
     *
     * @param image       [in]; The image.
     * @param data        [in]; A pointer to the block of memory in which
     *                    to write the image.
     * @param bufsize     [in]; The maximum size of the memory block.
     * @param format      [in]; The name of the desired image format.
     * @param options     [in]; A string containing any format-specific
     *                    options.
     * @param actual_size [out]; The number of bytes written to memory.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_image__write_mem (const ERSPImage* image,
                                      unsigned char* data, size_t bufsize,
                                      const char* format, const char* options,
                                      size_t* actual_size);

    /**
     * @brief Reads an image from a file.
     *
     * @param image       [in]; The image.
     * @param filename    [in]; The path from which to read the image.
     * @param file_format [in]; The name of the file format.
     * @param dst_format  [in]; The desired color format of the resulting image.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_image__read_file (ERSPImage* image, const char* filename,
                                      const char* file_format, ERSPColorFormat dst_format);

    /**
     * @brief Draws a straight line in an image.
     *
     * @param image [in]; The image.
     * @param x0    [in]; The x coordinate of the start of the line.
     * @param y0    [in]; The y coordinate of the start of the line.
     * @param x1    [in]; The x coordinate of the end of the line.
     * @param y1    [in]; The y coordinate of the end of the line.
     * @param r     [in]; The red value of the line color.
     * @param g     [in]; The green value of the line color.
     * @param b     [in]; The blue value of the line color.
     *
     * @return ERSP_TASK_SUCCESS on success.
     */
    extern EVOLUTION_EXPORT_TASK
    ERSPResult ersp_image__draw_line (ERSPImage* image,
                                      int x0, int y0, int x1, int y1,
                                      unsigned char r, unsigned char g,
                                      unsigned char b);

    /**
     * @brief Returns an image's raw data as a string.
     *
     * @param image [in]; The image.
     *
     * @return A string containing the image's raw data.
     */
    extern EVOLUTION_EXPORT_TASK
    const char* ersp_image__as_string (const ERSPImage* image);


#ifdef  __cplusplus
} /* end "C" */
#endif


#endif  /* INCLUDE_EVOLUTION_TASK_TASK_C_H */
