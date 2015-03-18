//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK__TASK_FUNCTOR_HPP
#define INCLUDE_EVOLUTION_TASK__TASK_FUNCTOR_HPP


/**
 * @file   evolution/core/task/TaskFunctor.hpp
 * @brief  Representation of functions and closures
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::TaskFunctor
 * @see Evolution::TaskFunctorWithArgs
 **/


#include <evolution/core/task/Types.hpp>
#include <evolution/core/task/TaskArg.hpp>
#include <evolution/core/task/TaskContext.hpp>


namespace Evolution
{

    /**
     * @class TaskFunctor
     * @brief TaskFunctors represent the code associated with Tasks.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK TaskFunctor
    {
    public: // Types

        /// The task function pointer.
        typedef TaskValue* (*TaskProc)(TaskContext* context);

    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param proc [in]; The TaskProc object.
         */
        TaskFunctor (TaskProc proc);

        /// Destructor
        virtual ~TaskFunctor () {}

    public: // Execution

        /**
         * @brief Used as an invocation mechanism for the functor.
         *
         * @param context [in]; The TaskContext object
         *
         * @return The result of the invocation.
         */
        virtual TaskValue* run (TaskContext* context);

        /**
         * @brief Creates a new "curried" TaskFunctor.
         *
         * @param num_args [in]; The number of elements in TaskArg
         * @param args     [in]; The TaskArg initialization list
         *
         * @return A new TaskFunctor.
         */
        virtual TaskFunctor* curry(int num_args, TaskArg args[]);

        /**
         * @brief Creates a new "curried" TaskFunctor.
         *
         * @param args [in]; The TaskArgVector to initialize with
         *
         * @return A new TaskFunctor.
         */
        virtual TaskFunctor* curry(TaskArgVector* args);

    public: // Debugging

        /**
         * @brief Returns useful information (functor's memory address and id)
         * for debugging a TaskFunctor.
         *
         * @return A sting containing some characteristics of the functor.
         */
        virtual String debug_str () const;


    protected: // Implementation

        TaskProc  _proc;  ///< The task procedure to run.
    }; // end class TaskFunctor

    /**
     * @class TaskFunctorWithArgs
     * @brief TaskFunctorWithArgs represents a "curried" TaskFunctor.
     *
     * Currying a TaskFunctor F that takes two arguments produces a
     * TaskFunctor G that takes one argument such that F(x, y) equals
     * G(y).  That is, when TaskFunctor G is run with argument y, it
     * actually calls TaskFunctor F with arguments x and y.
     * Similarly, currying a TaskFunctor F of n arguments with m
     * curried arguments produces a TaskFunctor G of (n - m)
     * arguments.  (See http://www.wikipedia.org/wiki/Currying for
     * more information on currying.)
     *
     * TaskFunctorWithArgs is used by the ERSP Python module.
     */
    class EVOLUTION_EXPORT_TASK TaskFunctorWithArgs : public TaskFunctor
    {
    public: // Structors

        /**
         * @brief Constructs a new "curried" TaskFunctor.
         *
         * @param proc     [in]; The invocation task proc
         * @param num_args [in]; The number of elements in the TaskArg list
         * @param args     [in]; The TaskArg initialization list
         */
        TaskFunctorWithArgs (TaskProc proc, int num_args, TaskArg args[]);

        /**
         * @brief Constructs a new "curried" TaskFunctor.
         *
         * @param proc [in]; The invocation task proc
         * @param args [in]; The TaskArgVector initialization list
         */
        TaskFunctorWithArgs (TaskProc proc, TaskArgVector* args);

        virtual ~TaskFunctorWithArgs () { }

    public:

        /**
         * @brief Used as an invocation mechanism for the functor.
         *
         * @param context [in]; The TaskContext object
         *
         * @return The result of the invocation.
         */
        virtual TaskValue* run (TaskContext* context);

        /**
         * @brief Creates a new "curried" TaskFunctor.
         *
         * @param num_args [in]; The number of elements in TaskArg
         * @param args     [in]; The TaskArg initialization list
         *
         * @return A new TaskFunctor.
         */
        virtual TaskFunctor* curry(int num_args, TaskArg args[]);

        /**
         * @brief Creates a new "curried" TaskFunctor.
         *
         * @param args [in]; The TaskArgVector to initialize with
         *
         * @return A new TaskFunctor.
         */
        virtual TaskFunctor* curry(TaskArgVector* args);

    public: // Debugging

        /**
         * @brief Returns useful information (functor's memory address and id)
         * for debugging a TaskFunctor.
         *
         * @return A sting containing some characteristics of the functor.
         */
        virtual String debug_str () const;

    protected: // Implementation

        TaskArgVector  _args;
    }; // end class TaskFunctorWithArgs


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_TASK__TASK_FUNCTOR_HPP
