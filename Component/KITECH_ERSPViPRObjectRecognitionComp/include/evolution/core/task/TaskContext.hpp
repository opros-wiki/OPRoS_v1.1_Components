//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TASK_CONTEXT_HPP
#define INCLUDE_EVOLUTION_TASK_TASK_CONTEXT_HPP


/**
 * @file   evolution/core/task/TaskContext.hpp
 * @brief  The execution context for a task; includes information
 *         about the task's manager, parent, arguments, etc.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::TaskContext
 **/


#include <list>
#include <memory>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/behavior/BehaviorData.hpp>
#include <evolution/core/behavior/BehaviorDataWriter.hpp>
#include <evolution/core/base/IMutex.hpp>
#include <evolution/core/task/Types.hpp>
#include <evolution/core/task/TaskArg.hpp>


namespace Evolution
{


    // Forward declarations.
    class Task;
    class TaskManager;


    /**
     * @class TaskContext
     * @brief The context in which a task is running.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK TaskContext
    {
    public: // Structors.

#ifndef DOXYGEN_INTERNAL
        /**
         * @brief For internal use only.
         *
         * Users should use TaskContext::task_args to create TaskContexts.
         */
        TaskContext (Task* task, bool is_dummy = false);
#endif

        /**
         * @brief Destructor.
         */
        virtual ~TaskContext ();

        /**
         * @brief Returns a pointer to the task associated with this
         * context.
         *
         * @return The Task pointer.
         */
        Task* get_task () const;

        /**
         * @brief Returns a pointer to the task manager associated
         * with this context.
         *
         * @return The underlying task manager.
         */
        TaskManager* get_task_manager () const;

        /**
         * @brief Returns a pointer to the vector of task arguments.
         *
         * @return The TaskArgVector pointer.
         */
        const TaskArgVector* get_arguments () const;

        /**
         * @brief Returns the indexed TaskArg from the vector of task
         * arguments.
         *
         * @pre index &lt; TaskArgVector size.
         *
         * @param n [in]; An index into the TaskArgVector
         *
         * @return The indexed TaskArg.
         */
        const TaskArg get_argument (unsigned int n) const;

        /**
         * @brief Returns true if another task has requested the
         * termination of the task associated with this context.
         *
         * @return true/false.
         */
        bool termination_requested () const;

        /**
         * @brief Sets the failure flag of the task associated with
         * this context's task.
         */
        void set_failed ();

        /**
         * @brief Returns a description of the TaskContext arguments.
         *
         * @return A string containing the description.
         */
        String args_debug_str () const;

        /**
         * @brief Creates a new context containing the specified
         * arguments.
         *
         * This method should be used inside a task that wants to call
         * another task synchronously.
         *
         * @param num_args [in]; The number of task arguments
         * @param args     [in]; The arguments list for the task
         * @param context  [in]; The parent context of the new context
         *
         * @return The newly created TaskContext object.
         */
        static TaskContext* task_args(unsigned int num_args = 0, const TaskArg* args = NULL,
                                      TaskContext* context = NULL);

        /**
         * @brief Creates a new context containing the specified
         * arguments.
         *
         * This method should be used when calling a task outside of
         * another task (in main, for example), and one wants to use a
         * task manager other than the default one.
         *
         * @param num_args [in]; The number of task arguments
         * @param args     [in]; The arguments list for the task
         * @param manager  [in]; The default task manager for the new
         *                       context
         *
         * @return The newly created TaskContext object.
         */
        static TaskContext* task_args(unsigned int num_args, const TaskArg* args,
                                      TaskManager* manager);

        /**
         * @brief Creates a new context containing the specified
         * arguments.
         *
         * This method should be used inside a task that wants to call
         * another task synchronously.
         *
         * @param args    [in]; A vector of TaskArgs
         * @param context [in]; The parent context of the new context
         *
         * @return The newly created TaskContext object.
         */
        static TaskContext* task_args(const TaskArgVector* args, TaskContext* context);

        /**
         * @brief Creates a new context containing the specified
         * arguments.
         *
         * This method should be used when calling a task outside of
         * another task (in main, for example), and one wants to use a
         * task manager other than the default one.
         *
         * @param args    [in[ A vector of TaskArgs
         * @param manager [in] The default task manager for the new
         *                     context
         *
         * @return The newly created TaskContext object.
         */
        static TaskContext* task_args(const TaskArgVector* args, TaskManager* manager);



    protected: // Members.

        Task* _task;
        bool  _is_dummy;
    }; // end class TaskContext


    /**
     * @brief A templated std::auto_ptr to help you keep track of your
     * TaskContexts.
     *
     * @ingroup core_task
     */
    typedef std::auto_ptr<TaskContext> TaskContextPtr;


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_TASK_TASK_CONTEXT_HPP

