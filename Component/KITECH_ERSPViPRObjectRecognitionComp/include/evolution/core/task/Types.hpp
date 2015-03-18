//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TYPES_HPP
#define INCLUDE_EVOLUTION_TASK_TYPES_HPP


#include <list>

#include "TaskValue.hpp"

/**
 * @file    evolution/core/task/Types.hpp
 * @brief   Define enumerations and other typedefs for Tasks.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Task
 * @see Evolution::TaskStatus
 */

namespace Evolution
{
    class Task;
    class TaskContext;

    /**
     * @brief Task status values.
     *
     * @ingroup core_task
     */
    typedef enum TaskStatus {
        TASK_READY,      /*!< The task has not yet started running. */
        TASK_RUNNING,    /*!< The task is currently running. */
        TASK_TERMINATED, /*!< The task was aborted. */
        TASK_SUCCESS,    /*!< The task finished successfully. */
        TASK_FAILURE,    /*!< The task failed. */
        TASK_RESTART     /*!< The task should be restarted (e.g. after a config change) */
    };

    /**
     * @brief A list of Task pointers.
     *
     * @ingroup core_task
     */
    typedef std::list<Task*> TaskList;

    /**
     * @brief Returns a human readable description of the meaning of a
     * TaskStatus value.
     *
     * @param status [in]; The task status code
     *
     * @return The human readable description of the error code. The
     * returned string should not be deallocated.
     *
     * @ingroup core_task
     */
    EVOLUTION_EXPORT_TASK const char* status_string(TaskStatus status);

}


#endif
