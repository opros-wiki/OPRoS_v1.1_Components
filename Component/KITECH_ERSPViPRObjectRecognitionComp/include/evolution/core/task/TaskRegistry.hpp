//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK__TASK_REGISTRY_HPP
#define INCLUDE_EVOLUTION_TASK__TASK_REGISTRY_HPP


/**
 * @file    evolution/core/task/TaskRegistry.hpp
 * @brief   Registry of task functions.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::TaskRegistry
 **/


#include <evolution/core/util/ObjectRegistry.hpp>
#include <evolution/core/task/TaskFunctor.hpp>


namespace Evolution
{


    /**
     * @class TaskRegistry
     * @brief
     * Registry of task functions, specifically the C wrappers of task
     * functions.
     *
     * The task registry keeps a global table of loaded task
     * functions, mainly so that an external scripting language may
     * query all available tasks and import them into the scripting
     * context.
     *
     * Tasks are registered through the ERSP_DECLARE_TASK macro.
     *
     * @see  ERSP_DECLARE_TASK, ERSPTaskWrapper.
     *
     * @ingroup core_task
     **/
    class EVOLUTION_EXPORT_TASK TaskRegistry
    {
    public: // Types

        typedef ObjectRegistry<TaskFunctor*>  RegistryType;
        typedef RegistryType::iterator        Iterator;

    public: // Structors

        /// Constructor.
        TaskRegistry () {}

        /// Destructor.
        ~TaskRegistry () {}

    public: // Access

        /**
         * @brief Looks up a registered task.
         *
         * @param type_id [in]; The ID of the desired task
         *
         * @return The TaskFunctor, if found.  NULL otherwise.
         */
        static TaskFunctor* find_task (const char* type_id);

        /**
         * @brief Given a TaskFunctor, finds the ID of the task if it
         * has been registered.
         *
         * Note: Uniqueness of task IDs may not be enforced.
         *
         * @param functor [in]; The task to be looked up
         *
         * @return A string containing the ID of the task, or NULL if the task
         * is not registered.
         */
        static char* find_task_id (const TaskFunctor* functor);

        /**
         * @brief Beginning position on the task table.
         *
         * @return The task table iterator.
         */
        static Iterator begin ();

        /**
         * @brief Ending position on the task table.
         *
         * @return The task table iterator.
         */
        static Iterator end ();

    public: // Library loading

        /**
         * @brief Loads the specified task library, ensuring that tasks in
         * that library will be available.
         *
         * @param libname [in]; The task library to be loaded
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        static Result load_library (const char* libname);

        /**
         * @brief Unloads the specified task library, if no other clients are
         * still using it.
         *
         * @param libname [in]; The task library to be unloaded
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        static Result unload_library (const char* libname);

    public: // Registration

        /**;
         * @brief Registers the given task with the specified type ID.
         *
         * @param type_id [in]; The task type id to associate with the task
         * @param functor [in]; The functor to register
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        static Result register_task (const char* type_id, TaskFunctor* functor);

        /**
         * @brief Unregisters the specified task.
         *
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param type_id [in]; The task type id to associated with the task
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        static Result unregister_task (TicketId ticket, const char* type_id);

    public: // Output

        /**
         * @brief Prints the names of the registered tasks to the stream.
         *
         * @param out [in]; The output stream
         */
        static void print_tasks (std::ostream& out);

    protected: // Helpers

        /**
         * @brief Initializes the task registry singleton.
         *
         * @return RESULT_SUCCESS if the operation succeeded.
         */
        static Result initialize_task_registry ();

    }; // end class TaskRegistry



} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_TASK__TASK_REGISTRY_HPP
