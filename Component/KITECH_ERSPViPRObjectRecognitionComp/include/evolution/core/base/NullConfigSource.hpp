//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CONFIG_NULL_CONFIG_SOURCE_HPP
#define INCLUDE_EVOLUTION_CONFIG_NULL_CONFIG_SOURCE_HPP


/**
 * @file    evolution/core/base/NullConfigSource.hpp
 * @brief   This is an interface to a configuration source and generic
 *          iterators over that source.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see     Evolution::NullConfigSource
 */


#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/base/ResultCodes.hpp>


namespace Evolution
{


    /**
     * @interface NullConfigSource
     * @ingroup core_base
     *
     * @brief A configuration source that does nothing.
     * 
     * @see Configuration String
     */     
    class EVOLUTION_EXPORT_BASE NullConfigSource : public IConfigSource
    {
   
    public: // Init.

        /** 
         * @brief
         * Initialize configuration data source of a particular type.
         * 
         * This method initializes the configuration data source to
         * one of the recognized types specified in the ConfigRoot
         * enumeration.  The init_data parameter is specific to the
         * implementation of the data source.  For example, if the
         * data source is an XML file, the init_data parameter might
         * be the XML file name; if the data source is a database,
         * the init_data member might be a database handle, etc.
         * 
         * @param config_root [in]; One of the ConfigRoot values.
         * @param init_data [in]; Source-specific initialization data.
         *                 
         * @return RESULT_FAILURE.
         */
        virtual Result init_source(ConfigRoot config_root, const void* init_data) { return RESULT_FAILURE; }
        
        /** 
         * @brief
         * Initialize configuration data source.
         *
         * This method initializes the configuration data source without
         * the use of the ConfigRoot parameter. The data source is fully
         * specified by the init_data parameter.
         * 
         * @param init_data [in]; Source-specific initialization data.
         *                 
         * @return RESULT_FAILURE.
        */
        virtual Result init_source(const void* init_data) { return RESULT_FAILURE; }
        /** 
         * @brief
         * Open a named data repository.
         * 
         * Opens a data repository given a string name.  A given data store 
         * implementation may provide a number of different repositories.
         * This method allow the user to arbitrarily choose a data
         * repository given a simple string name.
         *
         * @param name [in]; The repository name.
         *
         * @return 0.
         */
        virtual ConfigKey open_repository(const char* name) { return 0; }
        
        /** 
         * @brief
         * Close a named data repository.
         * 
         * @param name [in]; The name of the repository to close.
         */
        virtual void close_repository(const char* name) {}

    public: // Getting
        /**
         * @brief
         * Returns the root key of the configuration source.
         *
         * @return INVALID_CONFIG_ROOT.
         */
        virtual ConfigRoot get_config_root() { return INVALID_CONFIG_ROOT; }
        
        /**
         * @brief 
         * Find the first configuration key with a particular key name 
         * contained in a specified parent key.
         * 
         * @param parent_key [in]; The parent key to search.
         * @param key_name [in]; The key name to search for.
         *
         * @return 0.
         */
        virtual ConfigKey find_first(ConfigKey parent_key, String& key_name) const { return 0; }
        
        /** 
         * @brief
         * Find the next configuration key, given the previous key.
         * 
         * @param current_key [in]; The current key from which to find the 
         * next key.
         * @param key_name [in]; The key name of the next desired key.
         *
         * @return 0.
         */        
        virtual ConfigKey find_next(ConfigKey current_key, String& key_name) const { return 0; }

        /** 
         * @brief
         * Find the first value of a configuration key.
         * 
         * @param current_key [in]; The current key from which to find the 
         * first key.
         * @param name [in]; The desired name of the first key.
         * @param value [in]; The value that the first key should contain.
         *
         * @return 0.
         */
        virtual ConfigValue find_first_value(ConfigKey current_key, String& name, String& value) const { return 0; }

        /** 
         * @brief
         * Find the next value, given the previous value.
         * 
         * @param current_value [in]; The current key value.
         * @param name [in]; The desired name of the next key.
         * @param value [in]; The value that the next key should contain.
         * 
         * @return 0.
         */
        virtual ConfigValue find_next_value(ConfigValue current_value, String& name, String& value) const { return 0; }

        /** 
         * @brief
         * Find a sub-key of a given key name and index.
         *
         * @param parent [in]; The parent key containing the sub-keys 
         *      to be searched.
         * @param sub_key_name [in]; The name of the desired sub-key.
         * @param index [in]; The index of the desired sub-key.
         *
         * @return 0.
         */
        virtual ConfigKey find_indexed_sub_key(ConfigKey parent, const char* sub_key_name, const int index) const  { return 0; }       

        /** 
         * @brief
         * Get the name of a configuration key.
         * 
         * @param key [in]; The key whose name should be retrieved.
         * @param name [out]; The returned name.
         * 
         * @return false.
         */
        virtual bool get_key_name(ConfigKey key, String& name) const { return false; }
        /** 
         * @brief
         * Get the data of a configuration value.
         * 
         * @param current_value [in]; The configuration value object.
         * @param name [in]; The value name.
         * @param value [out]; The value in string form.
         *
         * @return false.
         */
        virtual bool get_value_data(ConfigValue current_value, String& name, String& value) const { return false; }
        
    public: // Setting
        
        /** 
         * @brief
         * Create a new configuration key.
         * 
         * @param parent_key [in]; The parent key that will contain the 
         *      key that will be created. 
         * @param key_name [in]; The name of the key that will be created.
         * 
         * @return 0.
         */
        virtual ConfigKey create_key(ConfigKey parent_key, const char* key_name) { return 0; }

        /** 
         * @brief
         * Set a value of a given configuration key.
         * 
         * @param key [in];  The key whose value will be set.
         * @param name [in];  The name of the value that will be set.
         * @param value [in]; The value to set.
         *
         * @return RESULT_FAILURE.
         */
        virtual Result set_value(ConfigKey key, const char* name, const char* value) { return RESULT_FAILURE; }

    public: // Deleting
        /** 
         * @brief
         * Delete a key.
         *
         * @param key [in]; The key to delete.
         *
         * @return RESULT_FAILURE.
         */
        virtual Result delete_key(ConfigKey key) { return RESULT_FAILURE; }

        /** 
         * @brief 
         * Delete a value.
         * 
         * @param value [in]; The value to delete.
         *
         * @return RESULT_FAILURE.
         */
        virtual Result delete_value(ConfigValue value) { return RESULT_FAILURE; }
        
    }; // end class NullConfigSource



} // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_CONFIG_NULL_CONFIG_SOURCE_HPP

 
