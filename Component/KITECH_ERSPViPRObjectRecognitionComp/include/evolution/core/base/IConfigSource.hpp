//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CONFIG__ICONFIG_SOURCE_HPP
#define INCLUDE_EVOLUTION_CONFIG__ICONFIG_SOURCE_HPP


/**
 * @file   evolution/core/base/IConfigSource.hpp
 * @brief  Interface to a configuration source and generic iterators over that source.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IConfigSource 
 */


#include <string>
#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{
    /**
     * @brief The configuration key is a handle to a configuration path. 
     *
     * @ingroup core_base
     */
    typedef const void*  ConfigKey;

    /**
     * @brief The configuration value is a handle to a configuration value.
     *
     * @ingroup core_base
     */
    typedef const void*  ConfigValue;

    
    /**
     * @interface IConfigSource
     * @ingroup core_base
     * @brief   Abstract interface defining the functionalities of 
     *          the configuration data source.
     *
     * A configuration data source is a hierarchical data store for
     * configuration data used by ERSP.  The data store is abstractly
     * structured as a tree of configuration keys, which each key being
     * a name-value pair.  Both the name and the value are stored as
     * strings.  Each configuration key can also contain zero or more 
     * sub-keys, forming a tree.  The IConfigSource interface describe
     * some standard ways of accessing this tree of configuration keys.
     * The implementation of these methods are dependent on the underlying
     * data store.  ERSP provides an XML-based data store implementation
     * of this interface in XmlConfigSource.
     * 
     * @see     Configuration String XmlConfigSource.
     */     
    class EVOLUTION_EXPORT_BASE IConfigSource 
    {
    public: // Constants 

        /// Configuration roots.
        typedef enum {
            INVALID_CONFIG_ROOT,
            CUSTOM_CONFIG_ROOT,
            APPLICATION_CONFIG_ROOT,
            BEHAVIOR_CONFIG_ROOT,
            PERSONALITY_CONFIG_ROOT,
            RESOURCE_CONFIG_ROOT,
            SEMANTIC_CONFIG_ROOT,
            USER_CONFIG_ROOT
        } ConfigRoot;
   
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
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result init_source(ConfigRoot config_root, const void* init_data) = 0;
        
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
         * @return RESULT_SUCCESS on success or an error code on failure.
        */
        virtual Result init_source(const void* init_data) = 0;

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
         * @return The configuration key corresponding to the opened
         *      repository.
         */
        virtual ConfigKey open_repository(const char* name) = 0;
        
        /** 
         * @brief
         * Close a named data repository.
         * 
         * @param name [in]; The name of the repository to close.
         */
        virtual void close_repository(const char* name) = 0;

    public: // Getting
        /**
         * @brief
         * Returns the root key of the configuration source.
         *
         * @return the rook key of the configuration source.
         */
        virtual ConfigRoot get_config_root() = 0;
        
        /**
         * @brief 
         * Find the first configuration key with a particular key name 
         * contained in a specified parent key.
         * 
         * @param parent_key [in]; The parent key to search.
         * @param key_name [in]; The key name to search for.
         *
         * @return The first configuration key with the specified name or
         *      NULL if not found.
         */
        virtual ConfigKey find_first(ConfigKey parent_key, String* key_name) const = 0;
        
        /** 
         * @brief
         * Find the next configuration key, given the previous key.
         * 
         * @param current_key [in]; The current key from which to find the 
         * next key.
         * @param key_name [in]; The key name of the next desired key.
         *
         * @return The next configuration key or NULL if not found.
         */        
        virtual ConfigKey find_next(ConfigKey current_key, String* key_name) const = 0;

        /** 
         * @brief
         * Find the first value of a configuration key.
         * 
         * @param current_key [in]; The current key from which to find the 
         * first key.
         * @param name [in]; The desired name of the first key.
         * @param value [in]; The value that the first key should contain.
         *
         * @return The first configuration value or NULL if not found.
         */
        virtual ConfigValue find_first_value(ConfigKey current_key, String* name, String* value) const = 0;

        /** 
         * @brief
         * Find the next value, given the previous value.
         * 
         * @param current_value [in]; The current key value.
         * @param name [in]; The desired name of the next key.
         * @param value [in]; The value that the next key should contain.
         * 
         * @return The next configuration value or NULL if not found.
         */
        virtual ConfigValue find_next_value(ConfigValue current_value, String* name, String* value) const = 0;

        /** 
         * @brief
         * Find a sub-key of a given key name and index.
         *
         * @param parent [in]; The parent key containing the sub-keys 
         *      to be searched.
         * @param sub_key_name [in]; The name of the desired sub-key.
         * @param index [in]; The index of the desired sub-key.
         *
         * @return The sub-key with the given name and index or NULL
         *      if not found.
         */
        virtual ConfigKey find_indexed_sub_key(ConfigKey parent, const char* sub_key_name, const int index) const = 0;        

        /** 
         * @brief
         * Get the name of a configuration key.
         * 
         * @param key [in]; The key whose name should be retrieved.
         * @param name [out]; The returned name.
         * 
         * @returns true if the key name is retrieved.
         */
        virtual bool get_key_name(ConfigKey key, String* name) const = 0;

        /** 
         * @brief
         * Get the data of a configuration value.
         * 
         * @param current_value [in]; The configuration value object.
         * @param name [in]; The value name.
         * @param value [out]; The value in string form.
         *
         * @returns true if the value is retrieved.
         */
        virtual bool get_value_data(ConfigValue current_value, String* name, String* value) const = 0;
        
    public: // Setting
        
        /** 
         * @brief
         * Create a new configuration key.
         * 
         * @param parent_key [in]; The parent key that will contain the 
         *      key that will be created. 
         * @param key_name [in]; The name of the key that will be created.
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return The newly created configuration key, or 0 on failure.
         */
        virtual ConfigKey create_key(ConfigKey parent_key, const char* key_name, bool commit = true) = 0;

        /** 
         * @brief
         * Set a value of a given configuration key.
         * 
         * @param key [in];  The key whose value will be set.
         * @param name [in];  The name of the value that will be set.
         * @param value [in]; The value to set.
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result set_value(ConfigKey key, const char* name, const char* value, bool commit = true) = 0;

    public: // Deleting
        /** 
         * @brief
         * Delete a key.
         *
         * @param key [in]; The key to delete.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result delete_key(ConfigKey key) = 0;

        /** 
         * @brief 
         * Delete a value.
         * 
         * @param value [in]; The value to delete.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result delete_value(ConfigValue value) = 0;

    public:                  // File commiting
        /**
         * @brief
         * Commits (saves) changes made to the configuration
         *
         * @return  RESULT_SUCCESS if file created succesfully, otherwise an error code.
         */
        virtual Result commit () = 0;
        
    }; // end class IConfigSource



} // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_CONFIG__ICONFIG_SOURCE_HPP

 
