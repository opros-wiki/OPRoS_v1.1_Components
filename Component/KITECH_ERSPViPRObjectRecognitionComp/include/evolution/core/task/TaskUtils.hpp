//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TASKUTILS_HPP
#define INCLUDE_EVOLUTION_TASK_TASKUTILS_HPP


#include <evolution/core/base/Platform.hpp>
#include <evolution/core/task/TaskManager.hpp>

/**
 * @file evolution/core/task/TaskUtils.hpp
 * @brief Various task-related utility functions.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/ 

namespace Evolution
{
    /**
     * @brief Convenience functions for retrieving resource
     * information within the task layer.
     *
     * @ingroup core_task
     */
    namespace TaskUtils
    {
        /**
         * @brief Searches for a resource supporting a given interface_id.
         *
         * @ingroup core_task
         * @param context              [in]; The task context.
         * @param resource_description [in]; The description of the resource
         *                                   (for human-readable debugging).
         * @param interface_id         [in]; e.g. ISpeechTTS::INTERFACE_ID.
         *
         * @param resource_id         [out]; The resulting resource_id that
         *                                   autodetect finds.
         *
         **/
        EVOLUTION_EXPORT_TASK
        Result search_for_resource(const TaskContext& context,
                                   const String&      resource_description,
                                   const char*        interface_id,
                                   String*           resource_id);

    } // end namespace TaskUtils
} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_TASK_TASKUTILS_HPP
