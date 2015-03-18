//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TASKVALUE_HPP
#define INCLUDE_EVOLUTION_TASK_TASKVALUE_HPP


#include "TaskArg.hpp"

#include <memory>

/**
 * @file evolution/core/task/TaskValue.hpp
 * @brief Task return values.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/ 

namespace Evolution
{

    /**
     * @brief TaskValue is really just a TaskArg.
     *
     * @ingroup core_task
     */
    typedef TaskArg TaskValue;

    /**
     * @brief A TaskValueVector is a vector that holds TaskValues.
     *
     * @ingroup core_task
     */
    typedef TaskArgVector TaskValueVector;

    /**
     * @brief A templated std::auto_ptr to assist with your TaskValue
     * memory housekeeping.
     *
     * @ingroup core_task
     */
    typedef std::auto_ptr<TaskValue> TaskValuePtr;

    /**
     * @brief The TaskValue that indicates a null value.
     *
     * @ingroup core_task
     */
    extern EVOLUTION_EXPORT_TASK TaskValue TASK_NO_VALUE;
} // End namespace Evolution.


#endif // INCLUDE_EVOLUTION_TASK_TASKVALUE_HPP

