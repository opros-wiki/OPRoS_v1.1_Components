//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_USER_CONFIG_HPP
#define INCLUDE_EVOLUTION_USER_CONFIG_HPP

/**
 * @file   evolution/core/base/UserConfig.hpp
 * @brief  Functions to get/set user config keys/value pairs.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#include <string>

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ResultCodes.hpp>

namespace Evolution
{
    /** 
     * @brief Convenience functions for extracting information from a user's
     *        configuration file.
     *
     * @ingroup core_base
     */
    namespace UserConfig
    {

        /**
         * @brief Retrieves the value given the specified key.
         *
         * @ingroup core_base
         *
         * @param key   [in]; The user config key name
         * @param value [out]; The returned value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        extern EVOLUTION_EXPORT_BASE
        Result get_user_config (const char *key, String *value);

        /**
         * @brief Sets a key to the specified value and, optionally, saves it to a file.
         *
         * @ingroup core_base
         *
         * @param key   [in]; The config key name
         * @param value [in]; The user config value to set
         * @param save  [in]; Save the new value to permanent data store
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        extern EVOLUTION_EXPORT_BASE
        Result set_user_config (const char *key,
                                const char *value,
                                bool save = false);

    } // end namespace UserConfig

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_USER_CONFIG_HPP
