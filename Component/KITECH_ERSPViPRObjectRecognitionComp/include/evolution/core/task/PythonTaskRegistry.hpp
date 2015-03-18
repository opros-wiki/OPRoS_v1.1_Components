//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  This material contains trade secrets and confidential information
//  of Evolution Robotics, Inc.  Any use, reproduction, disclosure or
//  dissemination is strictly prohibited without the explicit written
//  permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/tools/python/TaskModule/embed/PythonTaskRegistry.hpp
 * @brief   Registry for custom Python tasks.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::PythonTaskRegistry
 **/

#ifndef INCLUDE_EVOLUTION_PYTHON__PYTHON_TASK_REGISTRY_HPP
#define INCLUDE_EVOLUTION_PYTHON__PYTHON_TASK_REGISTRY_HPP


#include <evolution/core/base/Types.hpp>



namespace Evolution
{


    /**
     * @brief Registry for custom tasks that are written in Python.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_PYTHON_TASK_EMBED PythonTaskRegistry
    {
    public:
        /**
         * @brief
         * Registers the given task with the specified type ID.
         *
         * @param type_id  [in] The ID under which to register the task.
         * @param code     [in] The python code that will be executed
         *                      to define the task.
         * @param fn_name  [in] The name of the Python function defined in code
         *                      that will be called for the task.
         **/
        static Result register_task(const char* type_id, const char* code,
                                    const char* fn_name);
    };


}


#endif
