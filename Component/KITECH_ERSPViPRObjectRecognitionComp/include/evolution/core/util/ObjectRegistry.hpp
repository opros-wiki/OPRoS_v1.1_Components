//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__OBJECT_REGISTRY_HPP
#define INCLUDE_EVOLUTION_BASE__OBJECT_REGISTRY_HPP


/**
 * @file    evolution/core/util/ObjectRegistry.hpp
 * @brief   Registry of behavior factory functions
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ObjectRegistry
 **/


#include <evolution/core/base/Types.hpp>
#include <evolution/core/util/AcquirableMap.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/AcquirableLock.hpp>
#include <algorithm>

namespace Evolution
{

    /**
     * @class ObjectRegistry
     *
     * @brief This class provides a registry that can be used to dynamically
     * create objects by passing in a string identification tag.  New classes
     * must register their factory creation functions and identification tags
     * with this registry.
     *
     * @param Data   type of registry data
     *
     * @ingroup core_util
     * @ingroup core_util_aibo
     **/
    template<typename Data>
    class ObjectRegistry : public AcquirableMap<String, Data>
    {
    public: // Types

        typedef AcquirableMap<String, Data>  MapType;
        typedef typename MapType::Iterator   Iterator;
        typedef typename MapType::ValueType  ValueType;

    public: // Structors

        /**
         * @brief Constructor with explicit conversion
         */
        explicit ObjectRegistry ()
            : AcquirableMap<String, Data> () {};

    public: // Lookup

        /**
         * @brief This function attempts to lookup the entry refered to by
         * ConfigString.
         *
         * @param type_id [in]; The type id of the object to look up.
         * @param data [out]; The registry entry, if found.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        Result registry_lookup (const char* type_id, Data* data)
        {
            // Get the driver
            if (type_id == NULL)
            {
                return (RESULT_INVALID_CONFIGURATION);
            }

            // Use this to acquire a lock on the registry
            AcquirableLock lock (*this);
            if (!lock.is_locked ())
            {
                return (RESULT_ACQUISITION_ERROR);
            }

            // Find the requested driver entry in the registry (possibly table.end())
            Iterator object = MapType::find(type_id);

            // Check if we couldn't find an object
            if (object == MapType::end ())
            {
//                std::cerr << "Can't find object '" << type_id << "' in " << this << "\n";
                return (RESULT_NOT_FOUND);
            }

            *data = object->second;
            return (RESULT_SUCCESS);
        } // end registry_lookup()

    public: // Registration

        /**
         * @brief This function registers the specified data under the
         * specified type.
         *
         * @param type_id [in]; The type id of the object to register
         * @param data [in]; The object to register
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        Result register_object (const char* type_id, Data data)
        {
            // Acquire a lock on the registry
            AcquirableLock lock (*this);
            if (!lock.is_locked ())
            {
                return (RESULT_ACQUISITION_ERROR);
            }

            // Make sure the object isn't already in the registry
            Iterator object = MapType::find(type_id);
            if (object != MapType::end ())
            {
                return (RESULT_ALREADY_EXISTS);
            }

            // Add the object to the registry
            // std::cerr << "Registering object type '" << type_id << "'\n";
            insert (ValueType (type_id, data));
            return (RESULT_SUCCESS);
        } // end register_object

        /**
         * @brief This function unregisters the specified key.
         *
         * @param type_id [in]; The type id of the object to unregister
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        Result unregister_object (const char* type_id)
        {
            // Acquire a lock on the registry
            AcquirableLock lock (*this);
            if (!lock.is_locked ())
            {
                return (RESULT_ACQUISITION_ERROR);
            }

            // Make sure the object is in the registry
            Iterator object = MapType::find(type_id);
            if (object == MapType::end ())
            {
                return (RESULT_NOT_FOUND);
            }
            erase (object);
            return (RESULT_SUCCESS);
        } // end unregister_object()

        /**
         * @brief return the list of keys in the registry as a string list.
         *
         * @param list [out]; The list of registry keys
         */
        void get_key_list(StringList* list)
        {
            list->clear();
            for (Iterator it = MapType::begin(); it != MapType::end(); ++it)
            {
                list->push_back((*it).first);
            }
        }

    };


} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE__OBJECT_REGISTRY_HPP
