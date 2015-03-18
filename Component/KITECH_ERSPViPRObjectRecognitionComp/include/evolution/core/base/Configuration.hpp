//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file   evolution/core/base/Configuration.hpp
 * @brief  Manage access to configuration data.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Configuration 
 */

#ifndef INCLUDE_EVOLUTION_CONFIGURATION_HPP
#define INCLUDE_EVOLUTION_CONFIGURATION_HPP

#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/base/IMutex.hpp>

namespace Evolution
{
    /**
     * @class Configuration
     * @ingroup core_base
     * @brief Manage access to configuration data.
     *
     * @see IConfigSource String
     *
     * Overview:
     *
     * The Configuration class is the interface to the Evolution configuration
     * framework, which allows the storage of name-value pairs in an 
     * arbitrary tree hierarchy, much like the Windows registry.  Also, like the 
     * Windows registry, each node in the tree is called a key and is 
     * identified by a name.  This name is not unique.  In fact, unlike the
     * Windows registry, there can be multiple keys of the same name with  
     * the same parent.  Each name also includes an index value
     * specifying its order among similarly named siblings.  The index 
     * is specified after the key name and a colon.  For example, "Device:0" 
     * refers to the first "Device" key and "Device:4" refers to the fifth 
     * device key.  If no index is specified, an index of 0 is assumed. 
     *
     * Each key is uniquely identified by a key name containing the 
     * name of all of its parents and grandparents going up to the root of 
     * the tree, along with a 0-based index specifying its order among 
     * similarly-named sibling keys.  An example of a key name that
     * uniquely identifies a key is:
     * 
     * "Resources/Groups/Group:2/Member:1"
     *
     * This refers to the second "Member" key under the third "Group" key 
     * under the first "Groups" key under the "Resources" group.
     *
     * Each key can contain a default value, in addition to one or more
     * named values.  All values are considered to be strings, though the
     * configuration class does provide assessors that automatically 
     * convert the retrieved values to doubles and longs.
     *
     * The Evolution configuration framework is designed to work with a variety
     * of data back-ends.  This feature is facilitated by the IConfigSource 
     * interface, which defines the functionalities that the framework
     * requires from a data source.  The framework can be made to work 
     * with an arbitrary back-end simply by implementing this interface for
     * that back-end. 
     *
     * The IConfigSource.hpp header file also defines several types used by 
     * the framework. The C++ types for keys and values are ConfigKey and 
     * ConfigValue, respectively.   There is also the ConfigRoot enumeration, 
     * which identifies the set of configuration trees supported by this 
     * framework.       
     *
     */
    class EVOLUTION_EXPORT_BASE Configuration 
    {
    public: // Constants 
        enum
        {
            MAX_CONFIG_PATH_LEN = 1000
        };

    public: // Structors

        /**
         * @brief Constructor
         *
         * @param source [in]; The configuration data source
         *
         * @see IConfigSource.
         */
        Configuration(IConfigSource& source);
        
        /// Destructor
        virtual ~Configuration();

    public: // Path management

        /** 
         * @brief Initialize configuration data source of a particular type.
         * 
         * @param config_root [in]; One of the ConfigRoot values
         * @param init_data [in]; Source-specific initialization data
         *
         * @see IConfigSource::init_source
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result init_source(IConfigSource::ConfigRoot config_root, const void* init_data);
    
        /** 
         * @brief Initialize configuration data source.
         *
         * This method initializes the configuration data source without
         * the use of the ConfigRoot parameter. The data source is fully
         * specified by the init_data parameter.
         * 
         * @param init_data [in]; Source-specific initialization data
         *                 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result init_source(const void* init_data);

        /**
         * @brief Returns the root key of the configuration source.
         *
         * @return the root key of the configuration source.
         */
        IConfigSource::ConfigRoot get_config_root();
        
    public: // Getting

        /**
         * @brief Returns the IConfigSource interface.
         *
         * @return the IConfigSource interface.
         */
        IConfigSource& get_config_source () { return (_source); }
        
        /**
         * @brief Returns the const version of the IConfigSource interface.
         *
         * @return the const version of the IConfigSource interface.
         */
        const IConfigSource& get_config_source () const { return (_source); }

        /**
         * @brief Get a string value given an absolute key name.
         * 
         * @param key_name [in];  The absolute name of the key containing
         *      the desired value
         * @param name [in]; The name of the desired value
         * @param value [out]; The returned value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_value(const char* key_name, const char* name, String* value) const;

        /** 
         * @brief Get a double value given an absolute key name.
         * 
         * @param key_name [in];  The absolute name of the key containing
         *      the desired value
         * @param name [in]; The name of the desired value
         * @param value [out]; The returned double value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_value(const char* key_name, const char* name, double* value) const;

        /** 
         * @brief Get a long value given an absolute key name.
         * 
         * @param key_name [in];  The absolute name of the key containing
         *      the desired value
         * @param name [in]; The name of the desired value
         * @param value [out]; The returned long value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result get_value(const char* key_name, const char* name, long* value) const;
        
        /** 
        * @brief Get an unsigned long value given an absolute key name.
        * 
        * @param key_name [in];  The absolute name of the key containing
        *      the desired value
        * @param name [in]; The name of the desired value
        * @param value [out]; The returned long value
        *
        * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result get_value(const char* key_name, const char* name, unsigned long* value) const;

        /** 
         * @brief Get a series of long values given a key name. 
         * 
         * @param key_name [in];  The name of the key 
         * containing the desired values.  This name could be 0
         * @param value_name [in]; The name of the desired value
         * @param buffer [out]; The buffer containing the series values
         * @param size [out]; The number of values returned in buffer
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_series(const char* key_name, const char* value_name, long* buffer, unsigned int* size) const;
        
        /**
         * @brief Get a string value given a key and an optional relative key name.
         * 
         * @param key [in];  The key from which to obtain the value
         * @param sub_key_name [in]; An optional sub-key name.  If this
         *      parameter is 0, then the value will be obtained from 
         *      key.  If not null, the sub-key name identifies the sub-key
         *      of key from which the value will be retrieved
         * @param name [in]; The name of the desired value
         * @param value [out]; The returned value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        
        Result get_value(ConfigKey key, const char* sub_key_name, const char* name, String* value) const;
        
        /**
         * @brief Get a double value given a key and an optional relative key name.
         * 
         * @param key [in];  The key from which to obtain the value
         * @param sub_key_name [in]; An optional sub-key name.  If this
         *      parameter is 0, then the value will be obtained from 
         *      key.  If not null, the sub-key name identifies the sub-key
         *      of key from which the value will be retrieved
         * @param name [in]; The name of the desired value
         * @param value [out]; The returned value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        
        Result get_value(ConfigKey key, const char* sub_key_name, const char* name, double* value) const;
        
        /**
         * @brief Get a long value given a key and an optional relative key name.
         * 
         * @param key [in];  The key from which to obtain the value
         * @param sub_key_name [in]; An optional sub-key name.  If this
         *      parameter is 0, then the value will be obtained from 
         *      key.  If not null, the sub-key name identifies the sub-key
         *      of key from which the value will be retrieved
         * @param name [in]; The name of the desired value
         * @param value [out]; The returned value
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_value(ConfigKey key, const char* sub_key_name, const char* name, long* value) const;

        /**
        * @brief Get an unsigned long value given a key and an optional relative key name.
        * 
        * @param key [in];  The key from which to obtain the value
        * @param sub_key_name [in]; An optional sub-key name.  If this
        *      parameter is 0, then the value will be obtained from 
        *      key.  If not null, the sub-key name identifies the sub-key
        *      of key from which the value will be retrieved
        * @param name [in]; The name of the desired value
        * @param value [out]; The returned value
        *
        * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result get_value(ConfigKey key, const char* sub_key_name, const char* name, unsigned long* value) const;

        /**
         * @brief Get a series of long values given a key and an optional 
         * relative key name.
         * 
         * @param key [in];  The key from which to obtain the value
         * @param sub_key_name [in]; An optional sub-key name.  If this
         *      parameter is 0, then the value will be obtained from 
         *      key.  If not null, the sub-key name identifies the sub-key
         *      of key from which the value will be retrieved
         * @param value_name [in]; The name of the desired value
         * @param buffer [out]; The buffer containing the series values
         * @param size [out]; The number of values returned in buffer
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_series(ConfigKey key, const char* sub_key_name, const char* value_name, long* buffer, unsigned int* size) const;
        
        /** 
         * @brief Find a key given an absolute key name.
         * 
         * @param key_name [in]; The absolute key name of the key to look for
         *
         * @return The key if found, or 0 if not.
         */
        ConfigKey find_key(const char* key_name) const;
        
        /** 
         * @brief Find a key given a parent key and a relative key name.
         * 
         * @param parent [in]; The parent key containing the key to be found
         * @param sub_key [in];  The name identifying the key relative to
         *      the parent key
         *
         * @return The key if found, or 0 if not.
         */
        ConfigKey find_key(ConfigKey parent, const char* sub_key) const;
        
        /** 
         * @brief Find a key with a particular value given an absolute key name.
         * 
         * @param key_name [in]; The absolute key name to search
         * @param value_name [in]; The name of the matching value
         * @param value [in]; The matching value
         *
         * @return The configuration key if found or 0 if not.
         */
        ConfigKey find_key_with_value(const char* key_name, const char* value_name, const char* value) const;
        
        /** 
         * @brief Find a key with a particular value given a parent key and a 
         * relative key name. 
         * 
         * @param parent [in]; The parent key
         * @param sub_key_name [in]; The sub-key name relative to the
         * parent key 
         * @param value_name [in]; The name of the matching value
         * @param value [in]; The matching value
         *
         * @return The configuration key if found or 0 if not.
         */
        ConfigKey find_key_with_value(ConfigKey parent, const char* sub_key_name, const char* value_name, const char* value) const;

        /** 
         * @brief Find the root key of a named repository.
         * 
         * @param name [in]; The repository name
         * @param init_data [in]; Source-specific initialization data
         * 
         * @return The root configuration key if found or 0 if not.
         */
        ConfigKey find_repository_key(const char* name, const void* init_data = NULL) const;
        
    public: // Setting

        /**
         * @brief Create a key given an absolute key name.
         * 
         * @param key_name [in]; The absolute name of the key to be created
         *
         * @return The created configuration key or 0 on creation failure.
         */
        ConfigKey create_key(const char* key_name);
        
        /** 
         * @brief Create a subkey give a parent key and a relative sub-key name.
         * 
         * @param parent [in]; The parent key of the key to be created
         * @param sub_key_name [in]; The name of the key to be created
         *      relative to the parent key
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         *
         * @return The created configuration key or 0 on creation failure.
         */
        ConfigKey create_sub_key(ConfigKey parent, const char* sub_key_name, bool commit = true);

        /** 
         * @brief Set a string value give an absolute key name.
         * 
         * @param key_name [in]; The name of the key whose value will be set
         * @param name [in]; The name of the value to be set
         * @param value [in]; The value to set
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result set_value(const char* key_name, const char* name, const char* value, bool commit = true);
        
        /** 
         * @brief Set a double value given an absolute key name.
         *
         * @param key_name [in]; The name of the key whose value will be set
         * @param name [in]; The name of the value to be set
         * @param dvalue [in]; The value to set
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result set_value(const char* key_name, const char* name, double dvalue, bool commit = true);
        
        /**
         * @brief Set a long value given an absolute key name.
         * 
         * @param key_name [in]; The name of the key whose value will be set
         * @param name [in]; The name of the value to be set
         * @param lvalue [in]; The value to set
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result set_value(const char* key_name, const char* name, long lvalue, bool commit = true);
        
        /**
        * @brief Set an unsinged long value given an absolute key name.
        * 
        * @param key_name [in]; The name of the key whose value will be set
        * @param name [in]; The name of the value to be set
        * @param lvalue [in]; The value to set
        * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
        * 
        * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result set_value(const char* key_name, const char* name, unsigned long lvalue, bool commit = true);

        /** 
         * @brief Set a string value given a key and an optional relative key name.
         * 
         * @param key [in]; The key whose value, or the value of whose
         *      sub-key if sub_key_name is not 0, will be set
         * @param sub_key_name [in]; The name of the key whose value will
         *      be set, relative to key.  If 0, then the value to be set 
         *      will be on key
         * @param name [in]; The name of the value to be set
         * @param value [in]; The value to set
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result set_value(ConfigKey key, const char* sub_key_name, const char* name, const char* value,
        bool commit = true);
        
        /** 
         * @brief Set a double value given a key and an optional relative key name.
         * 
         * @param key [in]; The key whose value, or the value of whose
         *      sub-key if sub_key_name is not 0, will be set
         * @param sub_key_name [in]; The name of the key whose value will
         *      be set, relative to key.  If 0, then the value to be set 
         *      will be on key
         * @param name [in]; The name of the value to be set
         * @param dvalue [in]; The double value to set
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result set_value(ConfigKey key, const char* sub_key_name, const char* name, double dvalue, bool
        commit = true);
        
        /**
         * @brief Set a long value given a key and an optional relative key name.
         * 
         * @param key [in]; The key whose value, or the value of whose
         *      sub-key if sub_key_name is not 0, will be set
         * @param sub_key_name [in]; The name of the key whose value will
         *      be set, relative to key.  If 0, then the value to be set 
         *      will be on key
         * @param name [in]; The name of the value to be set
         * @param lvalue [in]; The long value to set
         * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result set_value(ConfigKey key, const char* sub_key_name, const char* name, long lvalue, bool
        commit = true);
   
        /**
        * @brief Set an unsinged long value given a key and an optional relative key name.
        * 
        * @param key [in]; The key whose value, or the value of whose
        *      sub-key if sub_key_name is not 0, will be set
        * @param sub_key_name [in]; The name of the key whose value will
        *      be set, relative to key.  If 0, then the value to be set 
        *      will be on key
        * @param name [in]; The name of the value to be set
        * @param lvalue [in]; The long value to set
        * @param commit [in]; (Optional) Whether to commit the change immediately (defaults to true)
        * 
        * @return RESULT_SUCCESS on success or an error code on failure.
        */
        Result set_value(ConfigKey key, const char* sub_key_name, const char* name, unsigned long lvalue, bool
            commit = true);

    public: // Deleting

        /** 
         * @brief Delete a key.
         * 
         * @param key [in]; The key to be deleted
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result delete_key(ConfigKey key);

        /** 
         * @brief Delete a key given an absolute key name.
         * 
         * @param key_name [in]; The absolute key name of the key to delete
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result delete_key(const char* key_name);

        /** 
         * @brief Delete a key given a parent key and a relative key name.
         * 
         * @param parent [in]; The parent key of the key to be deleted
         * @param key_name [in]; The key name, relative to parent, of the
         *      key to be deleted
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result delete_key(ConfigKey parent, const char* key_name);

        /** 
         * @brief Delete a value given its key and name.
         * 
         * @param key [in]; The key containing the value to be deleted
         * @param value_name [in]; The name of the value to be deleted
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result delete_value(ConfigKey key, const char* value_name);

        /** 
         * @brief Delete a value given an absolute key name and the value name.
         * 
         * @param key_name [in]; The absolute key name of the key containing
         *      the value to be deleted
         * @param value_name [in]; The name of the value to be deleted
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result delete_value(const char* key_name, const char* value_name);

        /** 
         * @brief Delete a value given a parent key, a relative key name, and a
         * value name.
         * 
         * @param parent [in]; The parent key of the key containing the value 
         *      to be deleted
         * @param key_name [in]; The name of the key containing the value to
         *      be deleted, relative to parent
         * @param value_name [in]; The name of the value to be deleted
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result delete_value(ConfigKey parent, const char* key_name, const char* value_name);

    public: // Saving (manually)

        /** 
         * @brief Commit any changes you've made to the configuration to the file
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result commit();

    protected: // Helpers
#ifndef DOXYGEN_INTERNAL

        /**
         * @brief Locate a config key.
         * 
         * @param key [in]; The parent key from which to locate the value
         * @param sub_key_name [in]; The optional sub-key name of the key
         *      containing the value, relative to key
         * @param name [in]; The value name
         * @param value [in]; The value to search for
         *
         * @return The found value or 0 if not found.
         */
        ConfigValue find_value(ConfigKey key, const char* sub_key_name, const char* name, String* value) const;
 
        /** 
         * @brief Find a subkey.
         * 
         * @param parent [in]; The parent key containing the sub-key to search
         *      for
         * @param sub_key_name [in]; The name of the sub-key to look for,
         *      relative to parent
         *      
         * @return The sub-key if found or 0 if not found.
         */
        ConfigKey find_sub_key(ConfigKey parent, const char* sub_key_name) const;
 
        /** 
         * @brief Find an indexed key with a particular value.  
         *
         * That is, look for the n'th key with a particular name and a
         * particular value.
         *
         * @param first_key [in]; The first key to search
         * @param index [in]; The order of the key with a certain value
         * @param value_name [in]; The name of the value
         * @param value [in]; The matching value
         *      
         * @return The key if found or 0 if not found.
         */
        ConfigKey find_indexed_key_with_value(ConfigKey first_key, int index, const char* value_name, const char* value) const;
  
        /** 
         * @brief Parse a long series.
         * 
         * @param series_str [in]; The string containing the series
         * @param buffer [out]; The buffer containing the parsed numbers
         * @param size [out]; The number of elements in the series
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_long_series(const char* series_str, long* buffer, unsigned int* size) const;

    protected: // Attributes
        IConfigSource&              _source;      ///< Reference to source config interface
        IConfigSource::ConfigRoot   _config_root; ///< The root config
#endif // DOXYGEN_INTERNAL
    };
} // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_CONFIGURATION_HPP
 
