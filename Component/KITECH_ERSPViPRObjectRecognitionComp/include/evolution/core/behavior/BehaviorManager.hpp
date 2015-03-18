//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_MANAGER_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_MANAGER_HPP


/**
 * @file    evolution/core/behavior/BehaviorManager.hpp
 * @brief   A behavior network loader and execution environment.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::BehaviorManager
 * @see Evolution::BehaviorNetworkFile
 * @see Evolution::BehaviorNetworkLoader
 */


#include <map>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/util/SharedLibraryTable.hpp>
#include <evolution/core/behavior/BehaviorConfigTypes.hpp>


namespace Evolution
{


    // Forward declarations.
    class XmlConfigSource;
    class BehaviorConfigParser;
    class BehaviorNetworkConfig;
    class IResourceContainer;
    class IBehaviorNetwork;
    class IBehaviorContainer;


    /**
     * @brief
     * A behavior network loader and execution environment.
     *
     * Multiple files with multiple behavior networks may be loaded
     * and unloaded (when not running). Individual networks may be
     * unloaded when not needed, and when all of the networks in a file are
     * unloaded, the network file is unloaded, too. Network IDs must
     * be unique across files, however.
     *
     * @todo
     * Add semantic checking for connections.
     *
     * @ingroup core_behavior
     **/
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorManager
    {
    public: // Structors

        /**
         * @brief Constructor for BehaviorManager objects.
         * @param resource_container [in]; The resource container to associate with the BehaviorManager.
         * @param result [out]; The result of one of several steps for the constructor. A warning will be generated if this is not RESULT_SUCCESS.
         */
        BehaviorManager (IResourceContainer& resource_container, Result& result);

        /// Destructor.
        ~BehaviorManager ();

    public: // Loading

        /**
         * @brief
         * Loads the behavior network(s) from the specified file.
         * @param ticket [in]; Security ticket
         * @param file_path [in]; filename and path to load as a behavior network xml file.
         * @param should_reload [in]; if true, will force a recreation of the network if it already exists.
         **/
        Result load_network_file (TicketId ticket, const char* file_path, bool should_reload);


        /**
         * @brief
         * Unloads the specified network file and its networks,
         * freeing all related memory.
         * @param ticket [in]; Security ticket
         * @param file_path [in]; filename of network to unload from manager
         **/
        Result unload_network_file (TicketId ticket, const char* file_path);

        /**
         * @brief
         * Unloads the specified behavior network.
         * @param ticket [in]; Security ticket
         * @param network_id [in]; unique label specifying the behavior network to unload
         **/
        Result unload_network (TicketId ticket, const char* network_id);

    public: // Execution

        /**
         * @brief
         * Runs all loaded behavior networks, returning immediately.
         * @param ticket [in]; Security ticket
         **/
        Result run (TicketId ticket);

        /**
         * @brief
         * Runs the specified behavior network, returning immediately.
         * @param ticket [in]; Security ticket
         * @param network_id [in]; unique label specifying the behavior network to run
         **/
        Result run_network (TicketId ticket, const char* network_id);


        /**
         * @brief
         * Runs all loaded behavior networks a given number of
         * invocations, blocking until complete.
         * @param ticket [in]; Security ticket
         * @param invocation_count [in]; optional.  specifies how many
         * times to invoke the behavior network before ruturning.
         * @param invocation_interval [in]; optional.  value in
         * seconds to use as a target for time taken per invocation of
         * the network.  if the network takes less time than
         * invocation_interval to run on any given iteration, it will
         * sleep the time difference.
         * Note that the invocation_interval here overrides the
         * networks' individual settings.
         **/
        Result run_count (TicketId ticket, 
                          unsigned long invocation_count = 1,
                          DiffTime invocation_interval = 0.1);

        /**
         * @brief
         * Runs the specified behavior network a given number of
         * invocations, blocking until complete.
         * @param ticket [in]; Security ticket
         * @param network_id [in]; unique label specifying the
         * behavior network to run invocation_count times
         * @param invocation_count [in]; optional.  specifies how many
         * times to invoke the behavior network before ruturning.
         * @param invocation_interval [in]; optional.  value in
         * seconds to use as a target for time taken per invocation of
         * the network.  if the network takes less time than
         * invocation_interval to run on any given iteration, it will
         * sleep the time difference.  Note that the
         * invocation_interval here overrides the network's own
         * setting.
         **/
        Result run_network_count (TicketId ticket, const char* network_id,
                                  unsigned long invocation_count = 1,
                                  DiffTime invocation_interval = 0.1);

        /**
         * @brief
         * Stops all loaded behavior networks.
         * @param ticket [in]; Security ticket
         **/
        Result stop (TicketId ticket);

        /**
         * @brief
         * Stops the specified behavior network.
         * @param ticket [in]; Security ticket
         * @param network_id [in]; unique label specifying the behavior network to stop
         **/
        Result stop_network (TicketId ticket, const char* network_id);

    public: // Accessors

        /**
         * @brief
         * Sets the invocation interval for all networks.
         * @param ticket [in]; Security ticket
         * @param interval [in]; interval (in seconds) to use as the invocation interval for all contained behavior networks.
         **/
        Result set_invocation_interval (TicketId ticket, DiffTime interval);

        /**
         * @brief
         * Sets the invocation interval by network_id.
         * @param ticket [in]; Security ticket
         * @param network_id [in]; unique label specifying the behavior network to set the invocation interval for.
         * @param interval [in]; value to set the invocation_interval for the specified network to.
         **/
        Result set_network_invocation_interval (TicketId ticket, const char* network_id,
                                                DiffTime interval);
        /**
         * @brief
         * Gets the invocation interval by network_id.
         * @param ticket [in]; Security ticket
         * @param network_id [in]; unique label specifying the behavior network to get the invocation interval for.
         * @param interval [out]; current value of specified network's invocation interval
         **/
        Result get_network_invocation_interval (TicketId ticket, const char* network_id,
                                                DiffTime* interval);

    public: // Inner types

        /**
         * @brief
         * Manages the loading and unloading of behavior network file
         * configuration information.
         *
         * This class is reference counted, so that it will be freed
         * when all of its networks have been unloaded. Do not delete!
         **/
        class EVOLUTION_EXPORT_BEHAVIOR BehaviorNetworkFile
        {
        public: // Structors

         /** 
           * Constructor.
           * @param file_path [in]; filename of BehaviorNetworkFile
           * @param search_path [in]; path to search for this BehaviorNetworkFile in
           * @param result [out]; result of constructor
         **/
            BehaviorNetworkFile (const char* file_path,
                                 const char* search_path,
                                 Result& result);

            ~BehaviorNetworkFile ();

        public: // Reference counting

            /**
             * @brief
             * Adds a reference to the file, used to release the file
             * when the last network is unloaded.
             * @return The current number of references after one is added.
             **/
            unsigned add_ref ();

            /**
             * @brief
             * Removes a reference to the file, freeing it if no
             * references remain.
             * @return The number of references after one is removed.
             **/
            unsigned remove_ref ();

        public: // Loading

            /// Loads the network file.
            Result load ();

            /// Unloads the network file.
            Result unload ();

        public: // Access

            /// Obtains the configuration information.
            const BehaviorConfigParser*  get_configuration_parser () const;

            /** 
             * @brief Obtains the file path.
             * @return A const reference to the file path.
             */
            const String& get_file_path () const { return (_file_path); }

        protected: // Implementation
            // Reference count.
            unsigned  _ref_count;                     ///< The reference count.

            // Configuration.
            String                 _file_path;        ///< The path to the file.
            XmlConfigSource*       _config_source;    ///< The configuration source.
            BehaviorConfigParser*  _config_parser;    ///< The parsed configuration information.
        }; // end inner class BehaviorNetworkFile


        /**
         * @brief
         * Manages the instantiation of the specified behavior
         * network, and obtains that network's available interfaces.
         *
         * This class is not reference-counted, but each instance
         * conceptually holds a reference to its containing
         * BehaviorNetworkFile. The actual reference-counting, as with
         * all other allocation and deallocation, is handled by the
         * outer BehaviorManager object.
         **/
        class EVOLUTION_EXPORT_BEHAVIOR BehaviorNetworkLoader
        {
        public: // Structors

            /**
             * @brief Constructor for BehaviorNetworkLoader, used by the BehaviorManager to load the network XML file, etc.
             * @param network_file [in]; The file from which to load the network.
             * @param network_config [in]; The configuration for the network to load.
             * @param resource_container [in]; The resource container for the network.
             **/
            BehaviorNetworkLoader (BehaviorNetworkFile* network_file,
                                   BehaviorNetworkConfig& network_config,
                                   IResourceContainer& resource_container);

            /// Destructor.
            ~BehaviorNetworkLoader ();

        public: // Loading

            /// Loads the behavior network.
            Result load ();

            /// Unloads the behavior network.
            Result unload ();

        public: // Status

            /**
             * @brief Indicates if the network is loaded.
             * @return Returns true if loaded, false otherwise.
             **/
            bool is_loaded () const { return (_behavior_network != NULL); }

        public: // Access

            /// Obtains the network file containing the network.
            BehaviorNetworkFile* get_network_file () { return (_network_file); }

            /// Obtains a behavior network pointer.
            IBehaviorNetwork* get_behavior_network () { return (_behavior_network); }

            /// Obtains the container interface.
            IBehaviorContainer* get_behavior_container () { return (_behavior_container); }

            /// Obtains the behavior network configuration.
            BehaviorNetworkConfig* get_behavior_network_config () { return (&_network_config); }

        protected: // Implementation
            // References.
            BehaviorNetworkFile*    _network_file;          ///< The file containing the network, held for dereferencing.

            // Config.
            BehaviorNetworkConfig&  _network_config;  ///< The network configuration.

            // Interfaces.
            IResourceContainer&     _resource_container;    ///< The resource container.
            IBehaviorNetwork*       _behavior_network;      ///< The behavior network interface.
            IBehaviorContainer*     _behavior_container;    ///< The behavior container interface.
        }; // end inner class BehaviorNetworkLoader


        /// Table of loaded network files.
        typedef std::map<String, BehaviorNetworkFile*> NetworkFileTable;

        /// Table of loaded networks.
        typedef std::map<String, BehaviorNetworkLoader*> NetworkTable;

    public:
        /// Returns a table of all loaded behavior networks.
        NetworkTable* get_behavior_networks ();

        /**
         * @brief Returns a pointer to a behavior network.
         * @param network_id [in]; ID of the network to obtain.
         **/
        IBehaviorNetwork* get_behavior_network (const String &network_id);

        /**
         * @brief Locates the networks in the specified file.
         * @param file_path [in]; The filename containing networks.
         * @param network_id_list [out]; The list of network ID's found in the file.
         **/
        Result get_networks_in_file (const char* file_path, StringList* network_id_list) const;

    protected: // Implementation
        // Tables.
        NetworkFileTable     _file_table;          ///< Table of loaded network files.
        NetworkTable         _network_table;       ///< Table of loaded behavior networks.

        // Config.
        String               _search_path;         ///< Configuration search path.

        // Environment.
        IResourceContainer&  _resource_container;  ///< The hardware resources available to behaviors.

        // Execution.
        bool                 _should_run;          ///< Should the networks continue running?
    }; // end BehaviorManager


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_MANAGER_HPP
