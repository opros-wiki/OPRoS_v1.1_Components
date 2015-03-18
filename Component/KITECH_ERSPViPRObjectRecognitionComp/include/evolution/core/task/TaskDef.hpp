//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TASK_DEF_HPP
#define INCLUDE_EVOLUTION_TASK_TASK_DEF_HPP


/**
 * @file evolution/core/task/TaskDef.hpp
 * @brief Macros for defining and declaring tasks.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/ 


#include <memory>

/**
 * @brief Declares the linkage of a task
 *
 * Example usage:
 * ERSP_DECLARE_TASK_LINKAGE(MyTask, "Example.MyTask", EVOLUTION_EXPORT);
 *
 * @param tsk [in]; Task name.
 * @param tsk [in]; Task ID.
 * @param tsk [in]; Symbol export definition.
 * @ingroup core_task
 */
#define ERSP_DECLARE_TASK_LINKAGE(tsk, id, LNK) \
inline const char* __get_type_id__ ## tsk () { return id; } \
extern LNK Evolution::TaskFunctor __instance__ ## tsk; \
extern LNK Evolution::TaskFunctor* tsk

/**
 * @brief Declares a new task function.
 *
 * Example usage:
 * ERSP_DECLARE_TASK(MyTask, "Example.MyTask");
 *
 * @param tsk [in]; Task name.
 * @param tsk [in]; Task ID.
 *
 * @ingroup core_task
 */
#define ERSP_DECLARE_TASK(tsk, id) \
inline const char* __get_type_id__ ## tsk () { return id; } \
extern Evolution::TaskFunctor __instance__ ## tsk; \
extern Evolution::TaskFunctor* tsk



/** Obtains the registered ID of the task type. */
#define ERSP_TASK_ID(tsk) (__get_type_id__ ## tsk ())


/**
 * @brief Begins the implementation of a task function.
 *
 * The C++ implementation code, beginning with the opening brace {,
 * should follow immediately after ERSP_IMPLEMENT_TASK.
 *
 * Example usage:
 * ERSP_IMPLEMENT_TASK(MyTask)
 * {
 * // ...
 * }
 *
 * @ingroup core_task
 **/

#define ERSP_IMPLEMENT_TASK(tsk) \
/* The function to run. */ \
Evolution::TaskValue* __function__ ## tsk (Evolution::TaskContext* context); \
\
/* The functor instance. */ \
Evolution::TaskFunctor __instance__ ## tsk (__function__ ## tsk); \
\
/* Pointer to functor instance. */ \
Evolution::TaskFunctor* tsk = &(__instance__ ## tsk); \
\
/* Register the task functor. */ \
static Evolution::Result __result__ ## tsk = Evolution::TaskRegistry::register_task (ERSP_TASK_ID(tsk), tsk); \
\
/* The function definition follows. */ \
Evolution::TaskValue* __function__ ## tsk (Evolution::TaskContext* context)



/**
 * @brief Declares a primitive task.
 *
 * Example usage:
 * ERSP_DECLARE_PRIMITIVE(MyPrimitive, "Example.MyPrimitive");
 *
 * @ingroup core_task
 */
#define ERSP_DECLARE_PRIMITIVE(tsk, id) ERSP_DECLARE_TASK(tsk, id)


/**
 * @brief Defines a task-level wrapper around a Primitive.
 *
 * Defines the glue code that allows a Primitive to be used from the
 * task layer just like any other task.  All primitives must have
 * a corresponding ERSP_IMPLEMENT_PRIMITIVE.
 *
 * Example usage:
 * ERSP_IMPLEMENT_PRIMITIVE(MyTask, MyPrim);
 *
 * @ingroup core_task
 */
#define ERSP_IMPLEMENT_PRIMITIVE(tsk, prim) \
ERSP_IMPLEMENT_TASK(tsk) \
{ \
    Evolution::Result result; \
    Evolution::TaskStatus status; \
\
    prim* my_prim = new prim (context); \
    std::auto_ptr<prim> prim_cleanup (my_prim); \
    Evolution::Task* task = context->get_task(); \
    Evolution::TaskManager* manager = task->get_task_manager(); \
    result = manager->add_primitive (my_prim); \
    if (result == Evolution::RESULT_SUCCESS) \
    { \
        manager->wait_for_primitive(task, my_prim); \
        if (context->termination_requested()) \
        { \
          manager->remove_primitive(my_prim); \
          return NULL; \
        } \
        status = my_prim->get_status(); \
        if (status != Evolution::TASK_SUCCESS) \
        { \
            context->set_failed(); \
        } \
        Evolution::TaskValue val = my_prim->get_result(); \
        if (val.is_empty()) \
          return NULL; \
        else \
          return new Evolution::TaskValue(val); \
    } \
    else \
    { \
        return NULL; \
    } \
}


#include <evolution/core/task/Task.hpp>
#include <evolution/core/task/TaskFunctor.hpp>
#include <evolution/core/task/TaskValue.hpp>
#include <evolution/core/task/TaskContext.hpp>
#include <evolution/core/task/TaskRegistry.hpp>


#endif  // INCLUDE_EVOLUTION_TASK_TASK_DEF_HPP

