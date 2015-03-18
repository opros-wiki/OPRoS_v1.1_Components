//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE_RESOURCEUTILS_HPP
#define INCLUDE_EVOLUTION_RESOURCE_RESOURCEUTILS_HPP

/**
 * @file   evolution/core/resource/ResourceUtils.hpp
 * @brief  Resource-related utility methods.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#include <evolution/core/base/Platform.hpp>
#include <evolution/core/resource/ResourceManager.hpp>

namespace Evolution
{
    /**
     * @brief Use these convenience functions for retrieving resource
     * information from the resource configuration file.
     *
     * @ingroup core_resource
     */
    namespace ResourceUtils
    {

        /**
         * @brief Searches for a resource supporting a given interface_id
         *
         * @param container            [in]; The resource container.
         * @param resource_description [in]; The description of the resource
         *                                   (for human-readable debugging)
         * @param interface_id         [in]; e.g. ISpeechTTS::INTERFACE_ID
         * @param resource_id          [out]; The resulting resource_id that
         *                                    autodetect finds
         * @param preferred_id         [in, opt]; Optional. First choice will be
         *                                        given to the resource with
         *                                        this ID, if there is one,
         *                                        default=0
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @ingroup core_resource
         */
        EVOLUTION_EXPORT_RESOURCE
        Result search_for_resource(IResourceContainer& container,
                                   const String&       resource_description,
                                   const char*         interface_id,
                                   String*             resource_id,
                                   const char*         preferred_id = 0);

    } // end namespace ResourceUtils

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_RESOURCE_RESOURCEUTILS_HPP
