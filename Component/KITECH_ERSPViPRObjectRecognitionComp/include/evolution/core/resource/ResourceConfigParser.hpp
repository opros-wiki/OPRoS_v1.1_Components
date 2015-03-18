//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE_CONFIG_PARSER_HPP
#define INCLUDE_EVOLUTION_RESOURCE_CONFIG_PARSER_HPP


/**
 * @file   evolution/core/resource/ResourceConfigParser.hpp
 * @brief  A parser of resource configuration data.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::ResourceConfigParser
 */


#include <list>
#include <evolution/core/base/Configuration.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/Kinematics.hpp>
#include <evolution/core/resource/ResourceConfigTypes.hpp>


namespace Evolution
{

    /**
     * @class ResourceConfigParser 
     *
     * @brief This class manages access to resource configuration data.
     *
     * @ingroup core_resource
     *
     * @see IConfigSource Configuration String
     */
    class EVOLUTION_EXPORT_RESOURCE ResourceConfigParser : public Configuration
    {
    public: // Types

        typedef std::list<DeviceBusConfig *>   BusList;   ///< Device bus configuration
        typedef std::list<DeviceGroupConfig *> GroupList; ///< Group list configuration
        typedef std::list<ChainConfig *>       ChainList; ///< Chain configuration
        typedef std::list<ShapeConfig *>       ShapeList; ///< Shape configuration
     
    public: // Structors

        /**
         * @brief Constructor
         *
         * @param source [in]; The configuration data source to parse
         *
         * @see IConfigSource
         */
        ResourceConfigParser(IConfigSource &source);
       
        /**
         * @brief Destructor
         */
        virtual ~ResourceConfigParser();
        
     public: // Queries

        /**
         * @brief The get_device_byname function gets a pointer to a device
         * given its string ID.
         *
         * @param name [in]; The device ID
         * @param bus  [out, opt]; Optional. The bus that device belongs to,
         *                         default=NULL
         *
         * @return A pointer to the device configuration or NULL on failure.
         */
        const DeviceConfig * get_device_byname(const char *name,
                                    const DeviceBusConfig **bus = NULL) const;

        /**
         * @brief The get_group_byname function gets a pointer to a device
         * given its string ID.
         *
         * @param name [in]; The device ID
         *
         * @return A pointer to the device group configuration,
         * or NULL on failure.
         */
        const DeviceGroupConfig * get_group_byname(const char *name) const;
        
        /**
         * @brief The get_device_bygroup function gets a list of devices
         * according to their group name.
         *
         * @param name [in]; The device ID
         *
         * @return A pointer to the device list or NULL on failure.
         */
        const DeviceList * get_device_bygroup(const char *name) const;

        /**
         * @brief The get_shape_byname function gets a pointer to a shape
         * given its name.
         *
         * @param name [in]; The shape ID
         *
         * @return A pointer to the shape configuration or NULL on failure.
         */
        const ShapeConfig * get_shape_byname(const char *name) const;

        /**
         * @brief The get_link_parameters function gets a pointer to a kinematic
         * link's parameters.
         *
         * @param name [in]; The link ID
         *
         * @return A pointer to the kinematic link params or NULL on failure.
         */
        const DHParams * get_link_parameters(const char *name) const;

        /**
         * @brief The get_bus_byname function gets a bus by its name. 
         *
         * @param name [in]; The ID string of the requested bus
         *
         * @return A pointer to the bus configuration or NULL on failure.
         */
        const DeviceBusConfig * get_bus_byname(const char *name) const;

    public: // Access

        /**
         * @brief The get_bus_list function obtains the list of buses.
         *
         * @return The bus list or NULL on failure.
         */
        const BusList & get_bus_list() const { return _busses; }

        /**
         * @brief The get_group_list function obtains the list of groups.
         *
         * @return The group list or NULL on failure.
         */
        const GroupList & get_group_list() const { return _groups; }

        /**
         * @brief The get_chain_list function obtains the list of chains.
         *
         * @return The chain list or NULL on failure.
         */
        const ChainList & get_chain_list() const { return _chains; }

        /**
         * @brief The get_shape_list function obtains the list of shapes.
         *
         * @return The shape list or NULL on failure.
         */
        const ShapeList & get_shape_list() const { return _shapes; }

        /**
         * @brief The find_link function finds a link given a name.
         *
         * @param name [in]; The link ID
         *
         * @return A pointer to the link configuration or NULL on failure.
         */
        LinkConfig * find_link(const char * name) const;

        /**
         * @brief The get_motion_constraints function obtains the robots
         * global motion constraints.
         *
         * @return The motion constraints or NULL on failure.
         */
        const MotionConfig * get_motion_constraints() const;

    public: // Parsing

        /**
         * @brief This function loads configuration data from data source.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result parse_config();

    protected: // Parsing

        /**
         * @brief Use the parse_kinematics function to parse kinematics.
         *
         * @return true if successful.
         */
        bool parse_kinematics();

        /**
         * @brief Use the parse_chain function to parse config chain.
         *
         * @param chain_key [in]; The config key for this config chain
         *
         * @return true if successful.
         */
        bool parse_chain(ConfigKey chain_key);
        
        /**
         * @brief Use the parse_link function to parse config link.
         *
         * @param chain    [out]; The chain containing the link
         * @param link_key [in];  The link configuration key
         *
         * @return true if successful.
         */
        bool parse_link(ChainConfig *chain, ConfigKey link_key);

        /**
         * @brief Use the parse_link_parameters function to parse link
         * parameters.
         *
         * @param link     [out]; The link containing the parameters
         * @param link_key [in];  The link configuration key
         *
         * @return true if successful.
         */
        bool parse_link_parameters(LinkConfig *link, ConfigKey link_key);

        /**
         * @brief This function parses bus.
         *
         * @param bus_key [in]; The bus configuration key
         *
         * @return true if successful.
         */
        bool parse_bus(ConfigKey bus_key);

        /**
         * @brief This function parses dimensions.
         *
         * @return true if successful.
         */
        bool parse_dimensions();

        /**
         * @brief This function parses shape.
         *
         * @param shape_key [in]; The shape configuration key
         *
         * @return true if successful.
         */
        bool parse_shape(ConfigKey shape_key);

        /**
         * @brief This function parses motion constraints.
         * 
         * @param motion_key [in]; The motion configuration key
         *
         * @return true if successful.
         */
        bool parse_motion(ConfigKey motion_key);

        /**
         * @brief This function parses devices.
         *
         * @return true if successful.
         */
        bool parse_devices();

        /**
         * @brief This function parses devices.
         *
         * @param bus        [in/out]; Pointer to the bus the device is on.
         * @param device_key [in]; The device configuration key
         *
         * @return true if successful.
         */
        bool parse_device(DeviceBusConfig *bus, ConfigKey device_key);

        /**
         * @brief This function parses groups.
         *
         * @return true if successful.
         */
        bool parse_groups();

        /**
         * @brief This function parses groups.
         *
         * @param group_key [in]; The group configuration key
         *
         * @return true if successful.
         */
        bool parse_group(ConfigKey group_key);

        /**
         * @brief This function parses position.
         *
         * @param link         [out]; The corresponding link.
         * @param position     [out]; The parsed position vector
         * @param position_key [in];  The position configuration key
         *
         * @return true if successful.
         */
        bool parse_position(LinkConfig **link,
                            Vector3 *position,
                            ConfigKey position_key);
        
        /**
         * @brief This function parses orientation.
         *
         * @param roll  [out]; The parsed roll
         * @param pitch [out]; The parsed pitch
         * @param yaw   [out]; The parsed yaw
         * @param orientation_key [in]; The orientation configuration key
         *
         * @return true if successful.
         */
        bool parse_orientation(double *roll, double *pitch, double *yaw,
                               ConfigKey orientation_key);
    
        /**
         * @brief This function parses position.
         *
         * @param vector_key [in]; The position configuration key
         *
         * @return The position vector.
         */
        Vector3 parse_vector(ConfigKey vector_key);
 
    protected: // Implementation

        BusList      _busses; ///< A list of buses, to which devices are attached 
        GroupList    _groups; ///< A list of device groups 
        ChainList    _chains; ///< A list of chains (each containing a list of links) 
        ShapeList    _shapes; ///< A list of shapes (attached to links) 
        MotionConfig *_motion_config; ///< The robot's global motion constraints
    }; // end class ResourceConfigParser

} // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_RESOURCE_CONFIG_PARSER_HPP
