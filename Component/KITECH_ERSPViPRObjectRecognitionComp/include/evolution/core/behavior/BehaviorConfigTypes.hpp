//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BEHAVIOR__BEHAVIOR_CONFIG_TYPES_HPP
#define INCLUDE_EVOLUTION_CORE_BEHAVIOR__BEHAVIOR_CONFIG_TYPES_HPP


/**
 * @file    evolution/core/behavior/BehaviorConfigTypes.hpp
 * @brief   Behavior configuration types.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::BaseBehaviorConfig
 * @see Evolution::BehaviorPortConfig
 * @see Evolution::BehaviorParameterConfig
 * @see Evolution::BehaviorConfig
 * @see Evolution::BehaviorConnectionConfig
 * @see Evolution::BehaviorNetworkConfig
 **/


#include <string>
#include <map>
#include <list>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/util/AcquirableMap.hpp>


namespace Evolution
{


    class BehaviorConfigParser;


    /**
     * @brief The default behavior library if not specified, is '__internal'.
     **/
    extern EVOLUTION_EXPORT_BEHAVIOR const char* const DEFAULT_BEHAVIOR_LIBRARY;


    /**
     * @class BaseBehaviorConfig
     * @brief Base class for all behavior configurations.
     * @ingroup core_behavior
     **/
    class EVOLUTION_EXPORT_BEHAVIOR BaseBehaviorConfig
    {
    public: // Structors
        /**
         * @brief
         * Constructor
         *
         * @param parser [in]; The parser object used for parsing the 
         *      configuration data.
         * @param key [in]; An optional configuration key to the configuration
         *      data.
         */
        BaseBehaviorConfig(BehaviorConfigParser& parser, ConfigKey key = 0);

        /**
         * @brief
         * No configuration source constructor.
         *
         * This constructor is used for creating a behavior entirely in
         * code without any configuration data to parse.
         *
         * @param id [in]; The ID of the BehaviorConfig object.
         */
        BaseBehaviorConfig(const char* id = 0);

        /** 
         * @brief 
         * Destructor
         */
        virtual ~BaseBehaviorConfig();

    public: // Accessors
        /** 
         * @brief
         * Set the configuration key of this configuration.
         *
         * @param key [in]; Set the configuration key containing the data
         *      of the BehaviorConfig object.
         */
        void set_key(ConfigKey key);

        /**
         * @brief
         * Get the ID of this configuration.
         * 
         * @param id [out]; The returned ID of the BehaviorConfig object.
         *
         * @return RESULT_SUCCESS on success, or an error value on failure.
         */
        virtual Result get_id(String* id) const;

        /**
         * @brief
         * Obtains the root parser.
         *
         * @return The reference to the parser object used by the 
         *      BehaviorConfig object.
         */
        virtual const BehaviorConfigParser& get_parser () const;

    protected:
        BehaviorConfigParser&   _parser;    ///< Reference to the parser object.
        ConfigKey               _key;       ///< The configuration key encapsulated by this config.
        bool                    _source;    ///< Is source true?

        bool                    _no_config_source;  ///< Is there a config source?
        String                  _id;        ///< String id        
    }; // end class BaseBehaviorConfig


    /**
     * @class BehaviorPortConfig
     * @brief Behavior port configuration.
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorPortConfig : public BaseBehaviorConfig
    {
    public: // Structors
        /**
         * @brief
         * Constructor
         * @param parser [in]; The parser object used for parsing the 
         *      configuration data.
         * @param key [in]; An optional configuration key to the configuration
         *      data.
         * @param repeat_index [in]; The optional index of a port if it is
         *      a repeating port.
         */      
        BehaviorPortConfig(BehaviorConfigParser& parser, ConfigKey key = 0, int repeat_index = -1);

        /**
         * @brief
         * Destructor
         */
        virtual ~BehaviorPortConfig();

        /**
         * brief
         * Get the name of the behavior port.
         *
         * @param name [out]; The returned name of the behavior port.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_name(String* name) const;

        /**
         * @brief
         * Get the data type of the behavior port.
         *
         * @param data_type [out]; The returned data type of the behavior port.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_data_type(String* data_type) const;

        /**
         * @brief
         * Get the semantic type of the behavior port.
         *
         * @param semantic_type [out]; The returned semantic type of the
         *      behavior port.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_semantic_type(String* semantic_type) const;

        /**
         * @brief
         * Get the description of the behavior port.
         *
         * @param description [out]; The returned description of the behavior
         *      port.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_description(String* description) const;

        /**
         * @brief
         * Get the port type of the behavior port.
         *
         * @param port_type [out]; The returned port type of the behavior
         *      port
         * @param index [in]; The optional index of the port
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_port_type(String* port_type, unsigned index = 0) const;

    protected:
        String _port_name;      ///< Alternate name for use with repeat ports.
    };

    typedef std::list<BehaviorPortConfig*> BehaviorPortConfigList;

    /**
     * @class BehaviorParameterConfig
     * @brief Behavior parameter configuration
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorParameterConfig : public BaseBehaviorConfig
    {
    public: // Constants

        /** Behavior parameter types
         */
        typedef enum
        {
            INVALID_PARAM,
            STRING_PARAM,
            LONG_PARAM,
            DOUBLE_PARAM,
            GENERIC_PARAM
        } ParamType;

    public: // Structors.
        /**
         * @brief
         * Constructor
         *
         * @param parser [in]; The parser object used for parsing the 
         *      configuration data.
         * @param key [in]; A configuration key to the configuration data of the
         *      behavior parameter.
         * @param schema_key [in]; The configuration key to the behavior schema
         * @param behavior_key [in]; The configuration key to the behavior
         */
        BehaviorParameterConfig(BehaviorConfigParser& parser, ConfigKey key, ConfigKey schema_key, ConfigKey behavior_key);

        /**
         * @brief
         * No config source constructor
         * 
         * @param name [in]; The name of the parameter
         */
        BehaviorParameterConfig(const char* name);

        /**
         * @brief
         * Destructor
         */
        virtual ~BehaviorParameterConfig();

    public: // Accessors
        /**
         * @brief
         * Get the behavior parameter name.
         *
         * @return The name of the behavior parameter
         */
        const char* get_name() const { return _name.c_str(); }

        /**
         * @brief
         * Get behavior parameter type.
         *
         * @return The type of the behavior parameter
         */
        const ParamType get_type() const { return _type; };

        /**
         * @brief
         * Get the size of the generic data.
         *
         * @return The size of the generic parameter data
         */
        size_t get_generic_size() const { return _generic_size; };

        /**
         * @brief
         * Get the parameter value as string.
         * 
         * @param value [out]; The returned string value
         */
        Result get_value(String* value);

        /**
         * @brief
         * Get the parameter value as long.
         * 
         * @param value [out]; The returned long value
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_value(long* value);

        /**
         * @brief
         * Get the parameter value as double.
         * 
         * @param value [out]; The returned double value
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise
         */
        Result get_value(double* value);

        /**
         * @brief
         * Get the parameter value as generic binary data.
         * @param data_buffer [out]; Contains the returned
         *      generic parameter data.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise
         */
        Result get_value(char* data_buffer);

        /**
         * @brief
         * Sets the string parameter value.
         *
         * @param value [in]; The string value to set
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise
         */
        Result set_value(const char* value);

        /**
         * @brief
         * Sets the long parameter value.
         * 
         * @param value [in]; The long value to set
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise
         */
        Result set_value(long value);

        /**
         * @brief
         * Sets the double parameter value.
         *
         * @param value [in]; The double value to set
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise
         */
        Result set_value(double value);

        /**
         * @brief
         * Sets the generic binary parameter value.
         * 
         * @param value [in]; The generic data to set
         * @param generic_size [in]; The size of the generic data
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise
         */
        Result set_value(const char* value, size_t generic_size);

    protected: // Helpers
        /**
         * @brief
         * Set the parameter name.
         * 
         * @param name [in]; The parameter name to set.
         */
        void set_name(const char* name) { _name = name; }

        /**
         * @brief
         * Set the parameter type.
         * 
         * @param type [in]; The parameter type to set
         */
        void set_type(ParamType type) { _type = type; }

    protected: // Attributes.
        ConfigKey           _schema_key;        ///< Key to schema.
        ConfigKey           _behavior_key;      ///< Key to parent behavior.
        String              _name;              ///< Parameter name.
        ParamType           _type;              ///< Parameter type.
        size_t              _generic_size;      ///< Size of generic value.

        // No source parameter.
        String              _ns_value;          ///< Value if no configuration source is present.

        /**
         * @brief Friend class
         * @see Evolution::BehaviorConfigParser
         */
        friend class BehaviorConfigParser;
    };

    typedef AcquirableMap<String, BehaviorParameterConfig*> BehaviorParameterConfigMap;
    typedef double DiffTime;    ///< Difference between two times in seconds.

    enum BehaviorMergeType
    {
        BEHAVIOR_MERGE_NEVER,
        BEHAVIOR_MERGE_POSSIBLE,
        BEHAVIOR_MERGE_FORCE
    };

    /**
     * @class BehaviorConfig
     * @brief Behavior configuration.
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorConfig : public BaseBehaviorConfig
    {
    public: // Structors.
        /**
         * @brief
         * Constructor.
         * 
         * @param parser [in]; The parser object used for parsing the 
         *      configuration data.
         * @param key [in]; An optional configuration key to the configuration dat      * of the behavior.
         *      
         */
        BehaviorConfig(BehaviorConfigParser& parser, ConfigKey key = 0);

        /**
         * @brief
         * No configuration source constructor.
         * 
         * @param id [in]; The behavior ID.
         * @param type [in]; The type of the behavior.
         * @param library [in]; The library containing the behavior 
         *      implementation.
         * @param input_count [in]; The number of inputs on the behavior.
         * @param output_count [in]; The number of outputs on the behavior.
         * @param ignore_schema [in]; True if the behavior's schema containing
         *      default parameter values should be ignored.
         */
        BehaviorConfig(const char* id, const char* type, const char* library = 0, size_t input_count = 0, size_t output_count = 0, bool ignore_schema = false);

        /**
         * @brief
         * Destructor.
         */
        virtual ~BehaviorConfig();

        /**
         * @brief
         * Get a behavior type.
         * 
         * @param type [out]; The returned type of the behavior.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_type(String* type) const;

        /**
         * @brief
         * Get the driver, or just return the type if there is no 
         * driver= attribute.
         *
         * 
         * @param type [out]; The driver name of the behavior, according to 
         *      DECLARE_BEHAVIOR. 
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_driver(String* type) const;

        /**
         * @brief
         * Get a string parameter.
         *
         * @param name [in]; The name of the parameter.
         * @param value [out]; The string value of the parameter.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_parameter(const char* name, String* value) const;

        /**
         * @brief
         * Get a double parameter.
         *
         * @param name [in]; The name of the parameter.
         * @param value [out]; The double value of the parameter.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_parameter(const char* name, double *value) const;

        /**
         * @brief
         * Get a long parameter.
         *
         * @param name [in]; The name of the parameter.
         * @param value [out]; The long value of the parameter.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_parameter(const char* name, long *value) const;

        /**
         * @brief
         * Get the behavior's library.
         *
         * @param library [out]; The name of the library containing the
         *      behavior's implementation.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_library(String* library) const;

        /**
         * @brief
         * Get an output port by name.
         *
         * @param name [in]; The name of the output port to get.
         *
         * @return Pointer to the named output port on success, or 
         *      NULL otherwise.
         */
        const BehaviorPortConfig* get_output_port(const char* name) const;

        /**
         * @brief
         * Get an input port by name.
         *
         * @param name [in]; The name of the input port to get.
         *
         * @return Pointer to the named input port on success, or 
         *      NULL otherwise.
         */
        const BehaviorPortConfig* get_input_port(const char* name) const;

        /**
         * @brief
         * The output port list.
         *
         * @return Reference to the output port list.
         */
        const BehaviorPortConfigList& get_output_list() const;

        /**
         * @brief
         * Get the input port list.
         *
         * @return Reference to the input port list.
         */
        const BehaviorPortConfigList& get_input_list() const;

        /**
         * @brief
         * Get the parameter map.
         *
         * @return Reference to the parameter map.
         */
        const BehaviorParameterConfigMap& get_parameter_map() const;

        /**
         * @brief
         * Add a parameter.
         *
         * @param param [in]; The parameter object to add.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result  add_parameter(BehaviorParameterConfig* param);

        /**
         * @brief
         * Add an output port.
         *
         * @param port [in]; The output port to add.
         */
        void  add_output_port(BehaviorPortConfig* port);

        /**
         * @brief
         * Add an input port.
         *
         * @param port [in]; The input port to add.
         */
        void  add_input_port(BehaviorPortConfig* port);

        /**
         * @brief
         * Set the schema key.
         *
         * @param schema_key [in]; The schema key to set for the BehaviorConfig
         *      object.
         */
        void  set_schema_key(ConfigKey schema_key) { _schema_key = schema_key; }

        /**
         * @brief
         * Sets a double parameter.
         *
         * @param name [in]; The parameter name.
         * @param value [in]; The double value to set for the parameter.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result set_parameter(const char* name, double value);

        /**
         * @brief
         * Set a string parameter.
         *
         * @param name [in]; The parameter name.
         * @param value [in]; The string value to set for the parameter.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result set_parameter(const char* name, const char* value);

        /**
         * @brief
         * Returns variable input port count.
         *
         * @return The variable input port count.
         */
        size_t get_input_port_count();

        /**
         * @brief
         * Returns variable output port count.
         *
         * @return The variable output port count.
         */
        size_t get_output_port_count();

        /**
         * @brief
         * Returns variable input port repetitions.
         *
         * @return The variable input port repetitions.
         */
        size_t get_input_reps() { return _input_reps; }

        /**
         * @brief
         * Returns variable output port count.
         *
         * @return The variable output port repetitions.
         */
        size_t get_output_reps() { return _output_reps; }

        /**
         * @brief
         * Returns the merge type, indicating how the behavior may or
         * may not be merged.
         *
         * @return The merge type.
         **/
        BehaviorMergeType get_merge_type () const { return (_merge_type); }

    protected:
        BehaviorPortConfigList      _output_list;       ///< Output port list.
        BehaviorPortConfigList      _input_list;        ///< Input port list.
        BehaviorParameterConfigMap  _param_map;         ///< Parameter map.
        ConfigKey                   _schema_key;        ///< Key to the behavior's schema.
        size_t                      _input_reps;        ///< Input connection reps.
        size_t                      _output_reps;       ///< Output connection reps.
        bool                        _variable_input;    ///< True if input sizing=variable.
        bool                        _variable_output;   ///< True if output sizing=variable.
        BehaviorMergeType           _merge_type;        ///< Type of merging possible

        // No source parameter.
        String                  _type;          ///< Type
        String                  _library;       ///< Library
        size_t                  _input_count;   ///< Input count
        size_t                  _output_count;  ///< Output count

        /**
         * @brief Friend class
         * @see Evolution::BehaviorConfigParser
         */
        friend class BehaviorConfigParser;
    }; // end class BehaviorConfig


    /// Behavior config mappings
    typedef AcquirableMap<String, BehaviorConfig*> BehaviorConfigMap;

    enum ConnectionType
    {
        CONNECTION_NORMAL = 0,
        CONNECTION_BACK_PROPAGATION
    };

    /**
     * @class BehaviorConnectionConfig
     * @brief Behavior connection configuration.
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorConnectionConfig : public BaseBehaviorConfig
    {
    public: // Constants
        static const double DEFAULT_PRIORITY;   ///< Default connection priority = 1.0
        static const double DEFAULT_WEIGHT;     ///< Default connection weight = 1.0
        static const double DEFAULT_THRESHOLD;  ///< Default connection threshold = 0.0

    public: // Structors

        /**
         * @brief
         * Constructor
         *
         * @param parser [in]; The parser object used for parsing the 
         *      configuration data.
         * @param key [in]; An optional configuration key to the configuration data 
         *      of the behavior connection.
         */
        BehaviorConnectionConfig(BehaviorConfigParser& parser, ConfigKey key = 0);
        /**
         * @brief
         * Destructor
         */
        virtual ~BehaviorConnectionConfig();

    public: // Access

        /**
         * @brief
         * Gets the connection source.
         * 
         * @param source [out]; The returned name of the behavior which is the
         *      source of the connection.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_source(String* source) const;

        /**
         * @brief
         * Gets the connection to the source port.
         * 
         * @param source_port [out]; The returned name of the port
         *      that is the source of the connection.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_source_port(String* source_port) const;

        /**
         * @brief
         * Gets the connection target.
         * 
         * @param target [out]; The returned name of the behavior that is
         *      the target of the connection.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_target(String* target) const;

        /**
         * @brief
         * Gets the connection target port.
         * 
         * @param target_port [out]; The returned name of the port that
         *      is the target of the connection.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_target_port(String* target_port) const;

        /**
         * @brief
         * Gets the connection's type.
         * 
         * @param type [out]; The returned connection type.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        Result get_type(ConnectionType* type) const;

    }; // end class BehaviorConnectionConfig

    /**
     * Behavior connection list
     * @ingroup core_behavior
     */
    typedef std::list<BehaviorConnectionConfig*>   BehaviorConnectionList;

    /**
     * @class BehaviorNetworkConfig
     * @brief Behavior network config.
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorNetworkConfig : public BaseBehaviorConfig
    {
    public: // Structors.
        /**
         * @brief
         * Constructor.
         * @param parser [in]; The parser object used for parsing the 
         *      configuration data.
         * @param key [in]; An optional configuration key to the configuration 
         *      data of the behavior network.
         */
        BehaviorNetworkConfig(BehaviorConfigParser& parser, ConfigKey key = 0);

        /**
         * @brief
         * No config source constructor.
         *
         * @param type [in]; The behavior network type.
         * @param library [in]; The optional behavior network library.
         */
        BehaviorNetworkConfig(const char* type, const char* library = 0);

        /**
         * @brief
         * Destructor.
         */
        virtual ~BehaviorNetworkConfig();

        /**
         * @brief
         * Adds a behavior to the network.
         *
         * @param id [in]; The ID of the behavior to add.
         * @param behavior_config [in]; The configuration of the behavior 
         *      to add.
         */
        void add_behavior(const char* id, BehaviorConfig* behavior_config);

        /**
         * @brief
         * Adds a connection to the network.
         *
         * @param connection_config [in]; The configuration of the connection
         *      to add.
         */
        void add_connection(BehaviorConnectionConfig* connection_config);

        /**
         * @brief
         * Gets the type.
         *
         * @param type [out]; The returned type of the behavior network.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        const Result get_type(String* type) const;

        /**
         * @brief
         * Gets the library.
         *
         * @param library [out]; The returned library of the behavior network.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        const Result get_library(String* library) const;

        /**
         * @brief
         * Gets the invocation_interval.
         *
         * @param invocation_interval [out]; The returned invocation interval
         *      of the behavior network.
         *
         * @return RESULT_SUCCESS on success, or an error code otherwise.
         */
        const Result get_invocation_interval(DiffTime* invocation_interval) const;

        /**
         * @brief
         * Gets a connection from the network by source and target.
         *
         * @param source [in]; The source of the connection to obtain.
         * @param target [in]; The target of the connection to obtain.
         *
         * @return the BehaviorConnectionConfig object of the specified
         *      connection or 0 if the connection was not found.
         */
        const BehaviorConnectionConfig* get_connection(const char* source, const char* target) const;

        /**
         * @brief
         * Gets a behavior by ID.
         *
         * @param id [in]; The ID of the behavior to get.
         *
         * @return the BehaviorConfig object of the specified behavior.
         */
        BehaviorConfig* get_behavior(const char* id) const;

        /**
         * @brief
         * Gets the network's behavior map.
         *
         * @return The behavior configuration map of the behavior network.
         */
        const BehaviorConfigMap&        get_behavior_map() const;


        /**
         * @brief
         * Gets the network's connection list.
         *
         * @return The behavior network's connection list.
         */
        const BehaviorConnectionList&   get_connection_list() const;

    protected:
        BehaviorConfigMap       _config_map;            ///< The behavior map.
        BehaviorConnectionList  _connection_list;       ///< The connection list.

        // No config source members.
        String                  _type;        ///< Type
        String                  _library;     ///< Library

        /**
         * @brief Friend class
         * @see Evolution::BehaviorConfigParser
         */
        friend class BehaviorConfigParser;
    };

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_BEHAVIOR__BEHAVIOR_CONFIG_TYPES_HPP

