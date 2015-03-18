//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_PARALLEL_HPP
#define INCLUDE_EVOLUTION_TASK_PARALLEL_HPP


/**
 * @file   evolution/core/task/Parallel.hpp
 * @brief  Parallel execution context
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Parallel
 **/


#include <evolution/core/base/Types.hpp>
#include <evolution/core/task/Types.hpp>
#include <evolution/core/task/TaskFunctor.hpp>
#include <evolution/core/task/TaskArg.hpp>


namespace Evolution
{

    // Forward declarations
    class Task;
    class TaskManager;
    class TaskContext;

    
    /**
     * @class Parallel
     * @brief Parallel is used to run asynchronous tasks.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK Parallel
    {
    public: // Types
    
    public: // Structors

        /**
         * @brief The Parallel constructor requires a pointer to a
         * TaskContext.  Use TaskContext::task_args if you don't have
         * one.
         *
         * @param context [in]; The task context
         */
        Parallel (TaskContext* context);

        /**
         * @brief The Parallel destructor makes sure that all tasks
         * that were added to the parallel context have completed, if
         * one of the wait_for methods wasn't already used.
         */
        virtual ~Parallel ();


    public:

        /**
         * @brief Adds a task function to the parallel context.
         *
         * The task is only added, not started.
         *
         * @param functor  [in];      The task routine
         * @param num_args [in, opt]; Number of input arguments
         * @param args     [in, opt]; The input argument list
         *
         * @return a pointer to the new task object.
         */
        Task* add_task (TaskFunctor* functor,
                        unsigned int num_args = 0,
                        TaskArg* args = NULL);

        /**
         * @brief Adds a task function to the parallel context.
         *
         * The task is only added, not started.
         *
         * @param functor  [in]; The task routine
         * @param args     [in]; The input argument list
         *
         * @return a pointer to the new task object.
         */
        Task* add_task (TaskFunctor* functor, TaskArgVector* args);

        /** 
         * @brief Starts all tasks in the parallel context and waits
         * until ONE of them completes.
         *
         * The rest of the tasks are terminated.
         *
         * @param completed [out]; A pointer to the Task that completed.
         */
        Result wait_for_first_complete_task (Task** completed);

        /** 
         * @brief Starts all tasks in the parallel context and waits
         * until ALL of them complete.
         */
        Result wait_for_all_complete_tasks ();


    protected: // Methods

        // Starts up all the tasks that have been added to the
        // parallel context.
        void spawn_tasks ();


    protected: // Members

        // The task manager.
        TaskManager* _manager;

        // The current task; becomes the parent of the parallel
        // subtasks.
        Task* _task;

        // List of the parallel subtasks.
        TaskList _tasks;

        // Indicates whether the parallel context is complete (had one
        // of the wait_for methods called).  Otherwise, the destructor
        // blocks until it is complete.
        bool _complete;


    }; // end class Parallel


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_TASK_PARALLEL_HPP
