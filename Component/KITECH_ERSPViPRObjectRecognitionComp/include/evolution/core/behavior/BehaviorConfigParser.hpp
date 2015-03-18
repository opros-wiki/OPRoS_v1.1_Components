//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BEHAVIOR__BEHAVIOR_CONFIG_PARSER_HPP
#define INCLUDE_EVOLUTION_CORE_BEHAVIOR__BEHAVIOR_CONFIG_PARSER_HPP


/**
 * @file    evolution/core/behavior/BehaviorConfigParser.hpp
 * @brief   Parser of behavior configuration.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::BehaviorConfigParser
 **/


#include <list>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Configuration.hpp>
#include <evolution/core/behavior/BehaviorConfigTypes.hpp>
#include <evolution/core/behavior/SemanticConfigParser.hpp>


namespace Evolution
{

    /// Convinience typedef for a list of behaviors composing the network.
    typedef std::list<BehaviorNetworkConfig*> BehaviorNetworkList;
    

    /**
     * @brief
     * Parse behavior configuration data.
     *
     * This class is responsible for parsing behavior network configuration
     * data into an in-memory representation of behavior networks, including
     * behaviors, behavior connections, parameters, input and output ports,
     * etc.  
     *
     * @ingroup core_behavior
     * @brief   Manage access to behavior configuration data.
     *
     * @sa      IConfigSource Configuration String
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorConfigParser : public Configuration {
    public: // Structors
        /**
         * @brief
         * Constructor.
         *
         * @param source [in]; reference to configuration source.
         */
        BehaviorConfigParser(IConfigSource& source);

        /**
         * @brief
         * Destructor.
         */
        virtual ~BehaviorConfigParser();
        
    public: // Load configuration.
        
        /**
         * @brief
         * Returns a reference to the network list.
         *
         * @return
         * The reference to the behavior network list.
         */
        const BehaviorNetworkList& get_network_list() const;

        /**
         * @brief
         * Load configuration data from data source.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_config();

        /**
         * @brief
         * Parse a behavior schema.
         *
         * @param behavior_config [out]; The BehaviorConfig object that
         *      will contain the parsed schema data.
         * @param behavior_key [in];  The behavior schema configuration key.
         * @param type [in]; The behavior type name identifying the schema.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_schema(BehaviorConfig* behavior_config, ConfigKey behavior_key, const char* type);

    protected: // Parsing methods.
        /**
         * @brief
         * Parse behavior networks.
         *
         * This is the starting point for parsing behavior networks from
         * a behavior network configuration data source.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_networks();

        /**
         * @brief 
         * Parse behaviors from behavior network configuration data.
         * 
         * Parse behaviors from a given behavior network configuration key.  
         * The key should points to the configuration data defining a 
         * behavior network.
         * 
         * @param network_config [out]; The NetworkConfig object that will
         *      be updated with behaviors after the parsed.
         * @param network_key [in];  The behavior network configuration key.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_behaviors(BehaviorNetworkConfig* network_config, ConfigKey network_key);

        /** 
         * @brief 
         * Parse connections from the behavior network configuration data.
         * 
         * Parse behavior network connections from a given behavior network
         * configuration key.  The key should points to the configuration
         * data defining a behavior network.
         * 
         * @param network_config [out]; The behavior network config object
         *      that will be updated with the parsed data.
         * @param network_key [in];  The behavior network configuration key.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_connections(BehaviorNetworkConfig* network_config, ConfigKey network_key);
        
        /**
         * @brief
         * Parse a port group, return the number of ports parse in the group.
         * 
         * Parse a port group configuration and updates the BehaviorConfig
         * object with the parsed information. 
         * 
         * @param behavior_config [out]; The behavior config that will be 
         *      updated with the parsed port group data.
         * @param parent_key [in];  The key of the parent configuration.
         * @param input [in]; true if port is an input port, or false if output.
         * @param repeat_index [in]; The port repeat index.
         *
         * @return the number of parsed ports.
         */
        int parse_port_group(BehaviorConfig* behavior_config, ConfigKey parent_key, bool input, int repeat_index = -1);
        
        /** 
         * @brief 
         * Parse behavior connection ports.
         * 
         * Parse the port connection information from the schema and
         * store it in the behavior's BehaviorConfig object.
         * 
         * @param behavior_config [out]; The behavior config that will be 
         *      updated after the connection ports are parsed.
         * @param schema_key [in];  The key of the behavior schema.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_ports(BehaviorConfig* behavior_config, ConfigKey schema_key);

        /** 
         * @brief 
         * Parse behavior parameters.
         * 
         * This method parses parameter values for a behavior configuration.
         * The default parameter values in the specified schema key is parsed, 
         * followed by the parameter values specified in the behavior key,
         * which overrides the default values in the schema key.  The 
         * behavior_config parameter object will be updated with the parsed
         * parameter values.
         *
         * @param behavior_config [out]; The behavior config that will 
         *      be updated with parameter values after the parse.
         * @param behavior_key [in];  The behavior configuration key.
         * @param schema_key [in];  The key of the behavior schema.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_parameters(BehaviorConfig* behavior_config, ConfigKey behavior_key, ConfigKey schema_key);

        /**
         * @brief
         * Verify behavior connections in a behavior network.
         *
         * This method checks that the behavior connections are all valid
         * for a behavior network by checking the connection information
         * stored in the network's BehaviorNetworkConfig object.  
         * 
         * @param network_config [in];  The behavior network config for
         *      the network whose connections will be verified.
         */
        void verify_connections(BehaviorNetworkConfig* network_config); 

    protected:
        BehaviorNetworkList          _network_list; ///< The parsed behavior network list.
    };        

} // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_CORE_BEHAVIOR__BEHAVIOR_CONFIG_PARSER_HPP
