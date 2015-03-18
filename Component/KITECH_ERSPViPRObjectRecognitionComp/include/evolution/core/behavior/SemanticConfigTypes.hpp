//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BEHAVIOR_SEMANTIC_CONFIG_TYPES_HPP
#define INCLUDE_EVOLUTION_CORE_BEHAVIOR_SEMANTIC_CONFIG_TYPES_HPP


/**
 * @file    evolution/core/behavior/SemanticConfigTypes.hpp
 * @brief   Semantic configuration types.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::SemanticTypeConfig
 */


#include <map>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IConfigSource.hpp>


namespace Evolution
{

    class SemanticConfigParser;

    /**
     * @brief Class for getting information about defined semantic
     * behavior data types.
     *
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR SemanticTypeConfig
    {
    public: // Structors

        /**
         * @brief Constructor
         *
         * @param parser [in];   The parser object to use for parsing
         *                       the semantic configuration
         * @param key [in, opt]; An optional key to the semantic information
         */
        SemanticTypeConfig(SemanticConfigParser& parser, ConfigKey key = 0);

        /**
         * @brief Destructor
         */
        virtual ~SemanticTypeConfig();
       
    public: // Accessors

        /** 
         * @brief Set the configuration key of this config.
         * 
         * @param key [in]; The configuration key to set
         */
        void set_key (ConfigKey key);

        /**
         * @brief Get the name of this semantic type.
         * 
         * @param name [out]; The returned semantic type name
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_name (String* name) const;

        /**
         * @brief Get the data type of this semantic type.
         * 
         * @param data_type [out]; The data type of the semantic type
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_data_type (String* data_type) const;

        /**
         * @brief Get the data size of this semantic type.
         * 
         * @param data_size [out]; The data size of the semantic type
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_data_size (String* data_size) const;

        /**
         * @brief Get the display name.
         * 
         * @param display_name [out]; The returned display name of the
         *                            semantic type.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_display_name (String* display_name) const;

        /** 
         * @brief Get the description.
         * 
         * @param description [out]; The returned description of the 
         *                           semantic type.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        
        virtual Result get_description (String* description) const;

        /**
         * @brief Obtains the root parser.
         * 
         * @return A reference to the root parser of the semantic type.
         */ 
        virtual const SemanticConfigParser& get_parser () const;
    
    protected: // Implementation

        SemanticConfigParser&   _parser; ///< Reference to the parser object.
        ConfigKey               _key;    ///< The configuration key encapsulated by this config.
    }; // end class SemanticType Config

    /**
     * @brief Yes, it's a list of semantic types.
     * @ingroup core_behavior
     */
    typedef std::map<String, SemanticTypeConfig*> SemanticTypeTable;

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_BEHAVIOR_SEMANTIC_CONFIG_TYPES_HPP
