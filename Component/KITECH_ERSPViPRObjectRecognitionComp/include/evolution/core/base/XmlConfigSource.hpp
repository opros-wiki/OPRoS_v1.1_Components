//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file   evolution/core/base/XmlConfigSource.hpp
 * @brief  Class that implements the IConfigSource interface for
 *         handling configuration sources using XML files.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::XmlConfigSource
 */

#ifndef INCLUDE_EVOLUTION_XML_CONFIG_SOURCE_HPP
#define INCLUDE_EVOLUTION_XML_CONFIG_SOURCE_HPP


#include <string>
#include <list>
#include <map>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/base/IMutex.hpp>


namespace Evolution
{


    /**
     * @class XmlConfigSource
     *
     * @brief Class that implements the IConfigSource interface for
     *        handling configuration sources using XML files.
     *
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE XmlConfigSource:public IConfigSource
    {
    protected:               // Types

        /// XML document structure
        struct XmlDocument
        {
            /// Constructor
            XmlDocument ():_doc (0), _root (0), _spec (0)
            {
            }

            /// Destructor
            ~XmlDocument ()
            {
                if (_doc != NULL)
                {
                    xmlFreeDoc (_doc);
                    _doc = NULL;
                }
            }

            xmlDocPtr _doc;     ///< The XML document.
            xmlNodePtr _root;   ///< The root node of the XML document.
            xmlNodePtr _spec;   ///< The spec node of the XML document.
            String _xml_file;   ///< The XML file name.
        };

        /// XML document map
        typedef std::map < String, XmlDocument * >DocumentMap;

    public:                  // Types
        /// This is the location of a newly created file.
        enum CreateFilePath
        {
            ABSOLUTE_PATH = 0,
            SYSTEM_DEFINED_PATH,
            USER_DEFINED_PATH
        };

    public:                  // Structors

        /// Constructor.
        XmlConfigSource ();

        /// Destructor.
        virtual ~ XmlConfigSource ();

    public:                  // Initialization

        /**
         * @brief
         * Sets the search paths for data repositories.
         *
         * @param   search_path [in]; This is the list of search paths
         **/
        Result set_search_path (const char *search_path);

        /**
         * @brief
         * Initialize configuration data source.
         *
         * @param   config_root     [in] A configuration root enumeration defined in IConfigSource.hpp
         * @param   init_data       [in] Source-specific initialization parameter
         *
         * @return  RESULT_SUCCESS on success, or another value describing the failure.
         */
        virtual Result init_source (ConfigRoot config_root,
                                    const void *init_data);

        /**
        * @brief
        * Initialize configuration data source.
        *
        * @param   init_data       [in] Source-specific initialization parameter
        *
        * @return  RESULT_SUCCESS on success, or another value describing the failure.
         */
        virtual Result init_source (const void *init_data);

        /**
         * @brief
         * Open a named data repository.
         *
         * @param   name        [in] The repository's name
         *
         * @return  The root configuration Key of the repository, if it is
         *          successfully opened, or 0 if not.
         *
         * @pre     name is valid
         */
        virtual ConfigKey open_repository (const char *name);

        /**
         * @brief
         * Close a named data repository.
         *
         * @param   name        [in] The repository's name
         */
        virtual void close_repository (const char *name);

    public:                  // Getting
        
        /**
         * @brief Returns the configuration root.
         *
         * @return A copy of the configuration root.
         */
        virtual ConfigRoot get_config_root ()
        {
            return _config_root;
        }

        /**
        * @brief
        * Find the first configuration key with a particular key name.
        *
        * @param   parent_key      [in] The parent key contain keys to be found
        * @param   key_name        [out] Contains the name of the first key, if found
        *
        * @return  A configuration key if found or NULL if not.
         */
        virtual ConfigKey find_first (ConfigKey parent_key,
                                      String * key_name) const;

        /**
         * @brief
         * Find the next configuration key, given the previous key.
         *
         * @param   current_key     [in] The current key from which to look for the next key
         * @param   key_name        [out] Contains the name of the next key, if found
         *
         * @return  A configuration key if found or NULL if not.
         */
        virtual ConfigKey find_next (ConfigKey current_key,
                                     String * key_name) const;

        /**
         * @brief
         * Find the first value of a configuration key.
         *
         * @param   key         [in] The key in which to look for values
         * @param   name        [out] Contains the name of the first value, if found
         * @param   value       [out] Contains the value of the first value, if found
         *
         * @return  A configuration value if found, or NULL if not.
         */
        virtual ConfigValue find_first_value (ConfigKey key,
                                              String * name,
                                              String * value) const;

        /**32767
         * @brief
         * Find a key of a given key name with a particular value.
         *
         * @param   current_value   [in] The current value from which to look for the next value
         * @param   name            [out] Contains the name of the next value, if found
         * @param   value           [out] Contains the value of the next value, if found
         *
         * @return  A configuration value if found, or NULL if not.
         */
        virtual ConfigValue find_next_value (ConfigValue current_value,
                                             String * name,
                                             String * value) const;

        /**
         * @brief
         * Finds the n-th subkey of a particular name, where n is zero-based.
         *
         * @param   parent          [in] The parent key
         * @param   sub_key_name    [in] The name of the desired sub key
         * @param   index           [in] The 0-based index of the key to look for
         *
         * @return  A configuration key if found, or NULL if not.
         */
        virtual ConfigKey find_indexed_sub_key (ConfigKey parent,
                                                const char *sub_key_name,
                                                const int index) const;

        /**
         * @brief
         * Get the name of a configuration key.
         *
         * @param   key     [in] The configuration key whose name is desired
         * @param   name    [out] contains the name of the configuration if found
         *
         * @return  true on success, false on failure.
         */
        virtual bool get_key_name (ConfigKey key, String * name) const;

        /**
         * @brief
         * Get the data of a configuration value.
         *
         * @param current_value [in] The configuration value to be examined
         * @param name [out] Reports the name of this configuration value
         * @param value [out] Reports the children of this configuration value
         *
         * @return  true on success, false on failure (if current_value is NULL).
         */
        virtual bool get_value_data (ConfigValue current_value, String * name,
                                     String * value) const;

    public:                  // Setting
        /**
         * @brief
         * Create a new configuration key, or a series of configuration keys.
         *
         * @param   parent_key  [in] The parent key in which to create a new key
         * @param   key_name    [in] The name of the new key
         * @param   commit      [in] (Optional) Whether to commit the change immediately (defaults to true)
         *
         * @return  The new configuration key if creation was successful.
         */
        virtual ConfigKey create_key (ConfigKey parent_key,
                                      const char *key_name,
                                      bool commit = true);

        /**
         * @brief
         * Set a value of a given configuration key.
         *
         * @param   key     [in] The key in which to set a value
         * @param   name    [in] The name of the value to be set
         * @param   value   [in] The value to be set
         * @param   commit  [in] (Optional) Whether to commit the change immediately (defaults to true)
         *
         * @return  RESULT_SUCCESS if successful, or another value describing the failure.
         */
        virtual Result set_value (ConfigKey key, const char *name,
                                  const char *value, bool commit = true);

    public:                  // Deleting
        /**
         * @brief
         * Delete a key.
         *
         * @param  key     [in] The key to be deleted
         *
         * @return  RESULT_SUCCESS if successful, or another value describing the failure.
         */
        virtual Result delete_key (ConfigKey key);

        /**
         * @brief
         * Delete a value.
         *
         * @param   value   [in] The value to be deleted
         *
         * @return  RESULT_SUCCESS if successful, or another value describing the failure.
         */
        virtual Result delete_value (ConfigValue value);

    public:                  // File creation
        /**
         * @brief
         * Creates a new XML file and its corresponding document.
         *
         * @param   file_path [in]; place in which the file would be created
         * @param   filename  [in]; name of the XML file to be created
         * @param   root_node_name  [in]; the name of the root node to be created
         *
         * @return  RESULT_SUCCESS if file created succesfully, otherwise an error code.
         */
        virtual Result create_source (CreateFilePath file_path,
                                      const char *filename,
                                      const char *root_node_name);

    public:                  // File commiting
        /**
         * @brief
         * Commits (saves) changes made to the configuration
         *
         * @return  RESULT_SUCCESS if file created succesfully, otherwise an error code.
         */
        virtual Result commit ();

    protected:               // Helpers
        /**
         * @brief
         * Save the XML file.
         *
         * @return true on success, false on failure.
         */
        bool save ();

        /**
         * @brief
         * Finds the specified file name in the search path list.
         *
         * @param   rel_path [in]   relative path, including filename of file being searched for
         * @param   full_path [out] full path name of file
         *
         * @return  RESULT_SUCCESS if file found succesfully, otherwise an error code.
         */
        Result find_file (const char *rel_path, String * full_path);

    protected:               // Internal types

        typedef std::list < String > PathList;  ///< List of path strings.

    protected:               // Implementation
        ConfigRoot _config_root;    ///< The configuration root.
        XmlDocument _root_doc;  ///< The root document.
        DocumentMap _doc_map;   ///< Map of XML documents.
        PathList _search_path_list; ///< The configuration path.
    private:
        static int _instance_count; ///< Keeps track of XmlConfigSource instances for debugging.
    };
}                               // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_XML_CONFIG_SOURCE_HPP
