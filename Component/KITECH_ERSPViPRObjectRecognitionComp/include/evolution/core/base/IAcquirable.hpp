//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_ACQUIRABLE_RESOURCE_HPP
#define INCLUDE_EVOLUTION_BASE_ACQUIRABLE_RESOURCE_HPP


/**
 * @file   evolution/core/base/IAcquirable.hpp
 * @brief  Classes of resources that may be acquired for exclusive use.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IAcquirable
 */

#include <evolution/core/base/Types.hpp>

namespace Evolution
{


    /**
     * @interface  IAcquirable
     * @brief      Classes of resources that may be acquired for exclusive use.
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE IAcquirable
    {
    public: // Acquisition

        /**
         * @brief Acquires the resource.
         *
         * @return true if success, false if failure
         */
        virtual bool acquire () = 0;

        /**
         * @brief Releases the resource.
         *
         * @return true if success, false if failure
         */
        virtual bool release () = 0;

    public: // Status report

        /**
         * @brief Returns if the resource is currently acquired.
         *
         * @return true if success, false if failure
         */
        virtual bool is_acquired () const = 0;

    }; // end class IAcquirable


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_BASE_ACQUIRABLE_RESOURCE_HPP
