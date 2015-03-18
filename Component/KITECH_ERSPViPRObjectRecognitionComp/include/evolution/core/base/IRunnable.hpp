//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_RUNNABLE_RESOURCE_HPP
#define INCLUDE_EVOLUTION_BASE_RUNNABLE_RESOURCE_HPP


/**
 * @file   evolution/core/base/IRunnable.hpp
 * @brief  This is a simple callback interface that is often used with threads.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IRunnable
 */

#include <evolution/core/base/Types.hpp>

namespace Evolution
{

    /**
     * @interface  IRunnable
     * @brief      This is a simple callback interface that is often used with threads.
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE IRunnable
    {
    public:

        /** @brief Runs the object.
         *  @return Result based on success.
         */ 
        virtual Result run() = 0;
        /// Destructor.
        virtual ~IRunnable();

    }; // end class IRunnable

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE_RUNNABLE_RESOURCE_HPP
