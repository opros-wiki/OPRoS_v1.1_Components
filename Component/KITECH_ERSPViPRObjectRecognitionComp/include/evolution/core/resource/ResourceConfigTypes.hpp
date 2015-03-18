//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__RESOURCE_CONFIG_TYPES_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__RESOURCE_CONFIG_TYPES_HPP


/**
 * @file    evolution/core/resource/ResourceConfigTypes.hpp
 * @brief   Contains various resource configuration types.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::BaseResourceConfig
 * @see Evolution::ShapeConfig
 * @see Evolution::ShapeRectConfig
 * @see Evolution::ResourceConfig
 * @see Evolution::DeviceConfig
 **/


#include <string>
#include <map>
#include <list>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/EulerAngles.hpp>
#include <evolution/core/math/Kinematics.hpp>


namespace Evolution
{


    /**
     * @brief  The default resource library if not specified, is '__internal'.
     **/
    extern EVOLUTION_EXPORT_RESOURCE const char* const DEFAULT_RESOURCE_LIBRARY;


    // Forward declarations.
    class ChainConfig;
    struct DeviceGroupConfig;
    class ResourceConfigParser;

    /**
     * @brief Converts a string to a double with simple expression parsing.
     * Expressions can be simple arithmetic operations with pi.
     *
     * @param   str  [in]; The string of the requested bus.
     *
     * @returns        the double value of the string.
     * @ingroup  core_resource
     */
    extern double EVOLUTION_EXPORT_RESOURCE hw_atof(const char * str);

    /**
     * @brief DeviceGroupList is a list of DeviceGroupConfig pointers.
     *
     * @ingroup core_resource
     */
    typedef std::list<DeviceGroupConfig*> DeviceGroupList;

    class LinkConfig;

    /**
     * @class BaseResourceConfig 
     *
     * @brief The BaseResourceConfig class encapsulates the common 
     * functionalities and data of all resource configuration types.
     *
     * Each resource configuration object contains configuration 
     * data for a single resource as parsed from a configuration 
     * repository, such as the resource-config.xml file.  Each
     * resource configuration object is usually associated with a
     * configuration key (type ConfigKey) which points to where the
     * configuration data of the resource is stored in the configuration
     * repository.
     * 
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE BaseResourceConfig 
    {
    public:

        /**
         * @brief Constructor
         *
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        BaseResourceConfig(ResourceConfigParser& parser, ConfigKey key = 0); 
    
        /**
         * @brief Destructor
         */
        virtual ~BaseResourceConfig();
       
        /**
         * @brief This function sets a configuration key to the 
         * configuration data.
         *
         * @param key [in]; The key to the configuration data.
         */
        void set_key(ConfigKey key);
 
        /**
         * @brief Returns the id of the resource.
         *
         * @param id [out]; The id of the resource whose configuration
         * data is stored in this resource configuration object.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        virtual Result get_id(String* id) const;              
        
        /**
         * @brief Returns a resource parameter value as a string.
         *
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_parameter(const char* name, String* value) const;

        /**
         * @brief Returns a parameter value as a double.
         *
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_parameter(const char* name, double* value) const;

        /**
         * @brief Returns the parameter value as an int.
         * 
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_parameter(const char* name, int* value) const;

        /**
         * @brief Returns a resource parameter value as an unsigned integer.
         *
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_parameter(const char* name, unsigned* value) const;

        /**
         * @brief Returns a resource parameter value as a long integer.
         *
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_parameter(const char* name, long* value) const;

        /**
         * @brief Returns a resource parameter value as an unsigned long 
         * integer.
         *
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_parameter(const char* name, unsigned long* value) const;

        /**
         * @brief Returns a resource parameter value as a boolean.
         *
         * @param name [in]; The parameter's name
         * @param value [out]; The parameter's value
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result get_parameter(const char* name, bool* value) const;

        /**
         * @brief Returns a resource parameter value specified as a
         * series of comma-separated numbers.
         *
         * @param name [in]; The parameter's name
         * @param buffer [out]; The buffer containing the numbers in the series.
         * @param size [out]; The size of the buffer.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  get_series_parameter(const char* name, long* buffer, unsigned int* size) const;

        /**
         * @brief Sets the value of a resource parameter as a string.
         *
         * @param name [in]; The name of the parameter to set.
         * @param value [in]; The resource parameter value to set.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  set_parameter(const char* name, const char* value);
            
        /**
         * @brief Set configuration key to the specification data for a 
         * resource.
         *  
         * @param spec_key [in]; The resource specification key
         */
        void    set_spec_key(const ConfigKey& spec_key);

    public: // Helpers

        /**
         * @brief Use the parse_orientation_params function to parse 
         * given orientation as a set of Euler angles.
         *
         * @param orientation [out]; The orientation configuration data
         * as Euler angles.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  parse_orientation_params(EulerAngles* orientation) const;

        /**
         * @brief Use the parse_position_params function to parse 
         * given position.
         *
         * @param position [out]; The position configuration information
         * parsed into a vector.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  parse_position_params(Vector3* position) const;
    
    protected: // Implementation
        ResourceConfigParser&   _parser;   ///< Resource config parser reference
        ConfigKey               _key;      ///< Resource config key
        ConfigKey               _spec_key; ///< Resource config specific key
    };

    /**
     * @class LinkConfig 
     *
     * @brief The LinkConfig class defines Link structure.  Each link 
     * represents a single "rigid body" on the
     * robot.  The blocks of a robot form a chain, the forward kinematics of
     * which can be solved using the Denavit-Hartenberg parameters.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE LinkConfig : public BaseResourceConfig 
    {
    public:
    
        /**
         * @brief Constructor
         *
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        LinkConfig(ResourceConfigParser& parser, ConfigKey key = 0); 
       
        /**
         * @brief Destructor
         */
        virtual ~LinkConfig();
        
        ChainConfig *       chain;  ///< The chain of which this link is part.
        LinkConfig *        prev;   ///< Previous link
        DHParams            param;  ///< The D-H parameters of the link.
    }; // end struct LinkConfig

    /**
     * @brief List of LinkConfig. Defined as a typedef for convenience.
     * @ingroup core_resource
     */
    typedef std::list<LinkConfig*> LinkList;


    /**
     * @class ChainConfig 
     *
     * @brief The ChainConfig class defines the chain structure 
     * (chains of links).
     * 
     * @see Evolution::LinkConfig
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ChainConfig : public BaseResourceConfig 
    {
    public:
    
        /**
         * @brief Constructor
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */ 
        ChainConfig(ResourceConfigParser& parser, ConfigKey key = 0);
        
        /**
         * @brief Destructor
         */
        virtual ~ChainConfig();

        LinkList            links;  ///< The links that compose the chain.
    }; // end struct ChainConfig


    /**
     * @brief This is a list of the types of shapes a block can have.
     *
     * @ingroup core_resource
     */
    enum ShapeTypeConfig     
    {
        SHAPE_NONE, ///< No shape                   
        SHAPE_RECT  ///< Rectangular shape (unique height, width, length).
    };


    /**
     * @class ShapeConfig 
     *
     * @brief The ShapeConfig class defines the shape structure 
     * parameters used by all shapes.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ShapeConfig : public BaseResourceConfig 
    {
    public:
    
        /**
         * @brief Constructor
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        ShapeConfig(ResourceConfigParser& parser, ConfigKey key = 0);
        
        /**
         * @brief Destructor
         */
        virtual ~ShapeConfig();

        /**
         * @brief Returns the shape type.
         *
         * @return The shape type.
         */ 
        ShapeTypeConfig     get_type();

        /**
         * @brief Parse the shape configuration data.
         *
         * @return true if the parse is successful.
         */
        virtual bool        parse();
        
        /**
         * @brief Parse the shape position configuration data.
         *
         * @return true if the parse is successful.
         */
        virtual bool        parse_position();

        /**
         * @brief Parse the shape orientation configuration data.
         *
         * @return true if the parse is successful.
         */
        virtual bool        parse_orientation(); 
        
        /**
         * @brief This function returns the link frame to 
         * which shape is attached.
         *
         * @return The link to which the shape is attached
         */
        LinkConfig *        get_link() const { return (_link); }

        /**
         * @brief The get_position function provides translation from 
         * the block's joint to center of shape.
         *
         * @return The translation as a vector.
         */
        const Vector3&      get_position() const { return (_position); }

        /**
         * @brief The get_orientation function returns rotation from 
         * block's joint to shape orientation.
         *
         * @return The orientation as Euler angles.
         */
        const EulerAngles&  get_orientation() const { return (_orientation); }

        /**
         * @brief The get_rotation function returns rotation given as a matrix.
         *
         * @return The rotation as a matrix.
         */
        const Matrix3&      get_rotation() const { return (_rotation); }

    protected:
        LinkConfig *  _link;         ///< Link config pointer
        Vector3       _position;     ///< Position 
        Matrix3       _rotation;     ///< Rotation matrix.
        EulerAngles   _orientation;  ///< Orientation (roll,pitch,yaw) of device.
    }; // end struct ShapeConfig


    /**
     * @class ShapeRectConfig 
     *
     * @brief The ShapeRectConfig class defines parameters specific to 
     * the rectangular shape.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ShapeRectConfig : public ShapeConfig 
    {
    public:

        /**
         * @brief Constructor
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        ShapeRectConfig(ResourceConfigParser& parser, ConfigKey key = 0);
        
        /**
         * @brief Destructor
         */
        virtual ~ShapeRectConfig();
   
        /**
         * @brief This method returns the dimensions of a rectangular shape.
         *
         * @param xl [out]; The size of the rectangular shape along the x 
         * axis.
         * @param yl [out]; The size of the rectangular shape along the y 
         * axis.
         * @param zl [out]; The size of the rectangular shape along the z 
         * axis.
         * 
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result      get_dimensions(double* xl, double* yl, double* zl) const;
        
        /**
         * @brief Use this function to parse the orientation.
         *
         * @return true if the parse is successful.
         */
        virtual bool    parse();

        /**
         * @brief Use this function to parse the attributes.
         *
         * @return true if the parse is successful.
         */
        virtual bool    parse_attributes();

    protected:
        double    _lx;            ///< Length in the x direction.
        double    _ly;            ///< Length in the y direction.
        double    _lz;            ///< Length in the z direction.
    }; // end struct ShapeRectConfig

    /**
    * @class MotionConfig
    *
    * @brief The MotionConfig class defines parameters specific to 
    * the motion constraints.
    *
    * @ingroup core_resource
    */
    class EVOLUTION_EXPORT_RESOURCE MotionConfig : public BaseResourceConfig 
    {
    public:

        /**
        * @brief Constructor
        * 
        * @param parser [in]; The parser to use for parsing the resource
        * configuration data.
        *
        * @param key [in]; The configuration key to the resource 
        * configuration data.
        */
        MotionConfig(ResourceConfigParser &parser, ConfigKey key = 0);

        /**
        * @brief Destructor
        */
        virtual ~MotionConfig();

        /**
        * @brief Use this function to parse the orientation.
        *
        * @return true if the parse is successful.
        */
        virtual bool parse();

    public:

        double _lin_vel;      ///< Linear velocity
        double _max_lin_vel;  ///< Max linear velocity
        double _lin_acc;      ///< Linear acceleration
        double _max_lin_acc;  ///< Max linear acceleration
        double _ang_vel;      ///< Angular velocity
        double _max_ang_vel;  ///< Max angular velocity
        double _ang_acc;      ///< Angular acceleration
        double _max_ang_acc;  ///< Max angular acceleration
    }; // end struct ShapeRectConfig

    /**
     * @class ResourceConfig 
     *
     * @brief The ResourceConfig class is a base class for all resource 
     * configuration types related to devices and buses.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ResourceConfig : public BaseResourceConfig 
    {
    public:
        /**
         * @brief Constructor
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        ResourceConfig(ResourceConfigParser& parser, ConfigKey key = 0);
     
        /**
         * @brief Destructor
         */
        virtual ~ResourceConfig();
    
        /**
         * @brief Use this function to get a device driver for a resource.
         *
         * @param driver [out];  The type of the driver for the resource.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        virtual Result get_driver(String* driver) const;

        /**
         * @brief Use this function to get a library for a resource.
         * 
         * @param library [out];  The name of the library containing
         * the resource's driver implementation.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        virtual Result get_library(String* library) const;

    }; // end struct ResourceConfig


    /**
     * @class DeviceConfig 
     *
     * @brief The DeviceConfig class encapsulates the resource configuration 
     * data common to all device types.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE DeviceConfig : public ResourceConfig 
    {
    public:

        /**
         * @brief Constructor
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        DeviceConfig(ResourceConfigParser& parser, ConfigKey key = 0);

        /**
         * @brief Destructor
         */
        virtual ~DeviceConfig();
        
    public: 
        /**
         * @brief Parses the resource configuration data for the device.
         *
         * @return true if successful.
         */
        virtual bool        parse();

        /**
         * @brief Parses the position configuration data for the device.
         *
         * @return true if successful.
         */
        virtual bool        parse_position();

        /**
         * @brief Parses the orientation configuration data for the device.
         *
         * @return true if successful.
         */
        virtual bool        parse_orientation();

        /**
         * @brief Parses the unit types configuration data for the device.
         *
         * @return true if successful.
         */
        virtual bool        parse_unit_type();
        
        /**
         * @brief Parses the link parameter configuration data for the device.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result  parse_link_params(); 
        
        /**
         * @brief The get_link function returns the link frame to which 
         * device is attached.
         *
         * @return the link configuration data for the device's link.
         */
        LinkConfig *        get_link() const;

        /**
         * @brief The get_position function returns the translation from 
         * the block's joint to the device.
         *
         * @return The translation as a vector.
         */
        const Vector3&      get_position() const { return (_position); }

        /**
         * @brief The get_orientation function returns the rotation from 
         * block's joint to the device's orientation.
         *
         * @return The rotation as Euler angles.
         */
        const EulerAngles&  get_orientation() const { return (_orientation); }

        /**
         * @brief The get_rotation function returns rotation from the
         * block's joint to the device's orientation as a matrix.
         *
         * @return the rotation as a matrix.
         */
        const Matrix3&      get_rotation() const { return (_rotation); }
        
        /**
         * @brief The get_unit_type function returns the unit type.
         * 
         * @param unit_type [out]; The unit type used by the device.
         *
         * @return RESULT_SUCCESS on success, or an error code on failure.
         */
        Result              get_unit_type(String *unit_type);

        /**
         * @brief Use the set_bus_id function to set the bus id.
         *
         * @param bus_id [in]; The id of the device's bus.
         */
        void                set_bus_id(const char* bus_id);

        /**
         * @brief Use the get_bus_id function to get the bus id.
         *
         * @return The id of the device's bus.
         */
        const char*         get_bus_id() const;

    protected: 
        String      _bus_id;       ///< Bus id
        LinkConfig* _link;         ///< Link to which device is attached.
        Vector3     _position;     ///< Translation of device's position from block's joint.
        Matrix3     _rotation;     ///< Transformation that represents rotation
        EulerAngles _orientation;  ///< Orientation (roll,pitch,yaw) of device.
        String      _unit_type;    ///< Type of units to return from device.

    }; // end struct DeviceConfig


    /**
     * @brief  A list of DeviceConfig objects.
     *
     * @ingroup core_resource
     */
    typedef std::list<DeviceConfig*> DeviceList;

    /**
     * @class DeviceBusConfig 
     *
     * @brief This class is used to represent communications methods
     * that may be shared by multiple devices where some bus-level
     * initialization or setup is needed before one can communicate
     * directly to the devices.  E.g., a serial port to which multiple
     * sensors are connected may need to be opened and configured
     * before being used.
     *
     * @ingroup core_resource
     */
    struct EVOLUTION_EXPORT_RESOURCE DeviceBusConfig : public ResourceConfig 
    {
        /**
         * @brief Constructor.
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        DeviceBusConfig(ResourceConfigParser& parser, ConfigKey key = 0);
        
        /**
         * @brief Destructor.
         */
        virtual ~DeviceBusConfig(); 

        DeviceList    devices;        ///< List of devices on bus.
    };


    /**
     * @class DeviceGroupConfig 
     *
     * @brief Encapsulates the configuration data for a group of devices
     *
     * @ingroup core_resource
     */
    struct EVOLUTION_EXPORT_RESOURCE DeviceGroupConfig : public ResourceConfig 
    {
    
        /**
         * @brief Constructor
         * 
         * @param parser [in]; The parser to use for parsing the resource
         * configuration data.
         *
         * @param key [in]; The configuration key to the resource 
         * configuration data.
         */
        DeviceGroupConfig(ResourceConfigParser& parser, ConfigKey key = 0);
       
        /**
         * @brief Destructor
         */
        virtual ~DeviceGroupConfig(); 

        DeviceList       devices;       ///< List of devices in the group.
        DeviceGroupList  groups;        ///< List of groups in the group.
    };
} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE__RESOURCE_CONFIG_TYPES_HPP
