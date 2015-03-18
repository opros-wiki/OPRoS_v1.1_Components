//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_MANAGER_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_MANAGER_HPP


/**
 * @file   evolution/core/resource/ResourceManager.hpp
 * @brief  Implements a manager and container for resources.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::ResourceManager
 * @see    Evolution::ResourceContainer
 */


#include <list>
#include <map>
#include <evolution/core/base/StringUtils.hpp>
#include <evolution/core/util/AcquirableMap.hpp>
#include <evolution/core/util/SharedLibraryTable.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/resource/IResourceContainer.hpp>


namespace Evolution
{

    // Forward declarations
    class XmlConfigSource;
    class ResourceConfigParser;
    class ResourceConfig;
    class IResourceDriver;
    class IResource;

    /**
     * @class ResourceManager
     *
     * @brief The ResourceManager class is responsible for loading resource
     * drivers and making them available to Skills. It stands as a gatekeeper
     * to resources, checking for security privileges and managing the
     * acquisition of shared resources at multiple access levels.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE  ResourceManager
    {
    public: // Structors

        /**
         * @brief Default constructor.
         */
        ResourceManager ();

        /**
         * @brief This is a constructor loading resource configuration from the
         * specified config search path. If the config_file is not
         * specified, it defaults to 'resource-config.xml'.
         *
         * @param config_path [in]; A search path for the
         * configuration file. The default system configuration path
         * is always appended to this path. If config_path=NULL, then
         * the configuration file will be searched only in the system
         * configuration path.
         * @param result      [in]; The result of construction
         * @param config_file [in, opt]; Optional. The name of the
         *                               configuration file, default=NULL
         */
        ResourceManager (const char* config_path,
                         Result* result,
                         const char* config_file = NULL);

        /**
         * @brief Destructor
         */
        ~ResourceManager ();

    public: // Initialization

        /**
         * @brief This function activates the resource manager.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result activate ();

        /**
         * @brief This function deactivates the resource manager.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result deactivate ();

        /**
         * @brief This function test whether the manager is active.
         *
         * @return true if the resource manager is active.
         */
        bool is_active () const { return (_is_active); }

    public: // Access

        /**
         * @brief This function obtains the IResourceContainer interface.
         *
         * @param ticket    [in]; The (TicketId) for security purposes
         * @param container [out]; Pointer to the IResourceContainer.
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @pre container != NULL
         *
         * @see IResourceContainer
         */
        Result get_resource_container (TicketId ticket,
                                       IResourceContainer** container);

    protected: // Inner types

        /// List of strings showing the dependencies for a given resource.
        typedef std::list<String> DependencyList;

        /**
         * @brief This encapsulates an entry in the driver table.  Each entry
         * represents a driver specified in the resource-config.xml file.
         *
         * @ingroup core_resource
         */
        struct DriverTableEntry
        {
        public: // Structors

            /**
             * @brief Constructor.
             *
             * @param a_driver [in, opt]; A resource driver, default=NULL
             */
            DriverTableEntry (IResourceDriver* a_driver = NULL)
                : driver (a_driver) {}

        public: // Data

            IResourceDriver* driver;      ///< Internal pointer to the driver.
            DependencyList  dependencies; ///< List of dependencies for the driver.
        }; // end class DriverTableEntry

        /**
         * @brief This is a table of resource drivers.
         */
        typedef AcquirableMap <String, DriverTableEntry> DriverTable;

        /**
         * @brief This is a list of driver IDs in activation order.
         */
        typedef std::list<String> DriverActivationList;

        /**
         * @brief A table of interface pointers and their containing drivers.
         */
        typedef AcquirableMap <void*, String> InterfaceTable;

    protected: // IResourceContainer interface

        /**
         * @class ResourceContainer 
         *
         * @brief This is an inner class providing the IResourceContainer
         * interface.
         */
        class EVOLUTION_EXPORT_RESOURCE  ResourceContainer : public IResourceContainer
        {
        public: // Structors

            /**
             * @brief Constructor.
             *
             * @param manager [in]; The resource manager
             */
            ResourceContainer (ResourceManager& manager) : _manager (manager) {}

            /**
             * @brief Destructor
             */
            ~ResourceContainer () {}

        public: // IResourceContainer interface

            /**
             * @brief This obtains the configuration information for the resources in
             * the container.
             *
             * @param ticket [in]; The (TicketId) for security purposes
             * @param config [in/out]; Returns pointer to internal resource
             *                         configuration info for this container.
             *                         Caller must NOT free the results and must
             *                         pass in a valid pointer.
             *
             * @return RESULT_SUCCESS on success or an error result.
             *
             * @pre config != NULL
             *
             * @see IResourceContainer::get_configuration()
             */
            Result get_configuration (TicketId ticket,
                                      const ResourceConfigParser** config) const;

            /**
             * @brief This obtains the specified interface on the specified resource.
             *
             * @param ticket         [in];  The (TicketId) for security purposes
             * @param resource_id    [in];  The interface ID of the requested interface
             * @param interface_name [in];  The interface name that the caller tries to obtain. There might be different resources (different resource ID) that implement the same type of interface (that will use the same interface name).
             * @param resource_interface [in/out]; Returns pointer to internal
             *                                     interface class for this container.
             *                                     Caller must NOT free the results
             *                                     and must pass in a valid pointer.
             *                                     Implementation should keep
             *                                     reference counts.
             *
             * @return RESULT_SUCCESS on success or an error result.
             *
             * @pre resource_interface != NULL
             *
             * @see IResourceContainer::obtain_interface()
             */
            Result obtain_interface (TicketId ticket,
                                     const char* resource_id,
                                     const char* interface_name,
                                     void** resource_interface);

            /**
             * @brief This releases the specified interface.
             *
             * @param  ticket             [in]; The (TicketId) for security purposes
             * @param  resource_interface [in]; The resource interface to release
             * @param  deactivate         [in, opt]; Optional boolean to trigger
             *                                       deactivation attempt,
             *                                       default=false
             *
             * @return RESULT_SUCCESS on success or an error result.
             *
             * @pre resource_interface != NULL
             *
             * @see IResourceContainer::release_interface()
             */
            Result release_interface (TicketId ticket,
                                      void* resource_interface,
                                      bool deactivate = false);

            /**
             * @brief This determines which resources, if any, that support the
             * requested interface.
             *
             * @param ticket         [in];  Reserved, use 0
             * @param interface_name [in];  Name of the interface to find
             * @param resource_list  [out]; Optional list of resources,
             *                              default=false
             *
             * @result RESULT_SUCCESS or RESULT_NOT_FOUND
             *
             * @pre interface_name != NULL
             *
             * @see IResourceContainer::find_interface()
             */
            Result find_interface (TicketId ticket,
                                   const char* interface_name,
                                   StringList* resource_list);

        protected: // Helpers

            /**
             * @brief This recursively loads the resources that a given resource has in its
             * list of dependencies.
             *
             * @note A result of failure demonstrates that one of the dependencies did
             * not load; the caller must deactivate the resources in the dependency
             * list.
             *
             * @param find_iter              [in]; The resources to activate
             * @param activated_dependencies [in]; A list if activated resources
             *
             * @return RESULT_SUCCESS on success or an error result.
             */

            Result activate_if_needed (DriverTable::iterator find_iter,
                                       DependencyList* activated_dependencies);

        protected: // Implementation

            ResourceManager& _manager;    ///< The containing ResourceManager.
        }; // end class ResourceContainer

        /// Friend class 
        friend class ResourceContainer;

    protected: // Helpers

    public:

        /**
         * @brief This loads all resources to be managed.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result load_all_resources ();

    protected:

        /**
         * @brief This loads the specified resource from a shared library.
         *
         * @param config    [in]; The resource configuration object
         * @param container [in]; The resource container
         * @param ticket    [in]; The (TicketId) for security purposes
         * @param new_entry [out]; A list of the newly loaded resources 
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result load_current_resource (const ResourceConfig& config,
                                      IResourceContainer& container,
                                      TicketId ticket,
                                      DriverTable::iterator* new_entry);

    protected: // Implementation

        // Configuration.
        String                 _config_path;           ///< The path to configuration information.
        XmlConfigSource*       _config_source;         ///< XML config source pointer
        ResourceConfigParser*  _config;                ///< XML config parser pointer
        bool                   _is_active;             ///< Activation status.

        // Driver tables.
        DriverTable            _driver_table;          ///< The table of loaded drivers.
        DriverActivationList   _driver_activate_list;  ///< List of driver IDs, by load order.
        InterfaceTable         _interface_table;       ///< Table of obtained interfaces.

        ResourceContainer      _container_interface;   ///< The IResourceContainer interface.
    }; // end class ResourceManager


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE_MANAGER_HPP
