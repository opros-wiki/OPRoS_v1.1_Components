//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BEHAVIOR__SEMANTIC_CONFIG_PARSER_HPP
#define INCLUDE_EVOLUTION_CORE_BEHAVIOR__SEMANTIC_CONFIG_PARSER_HPP


/**
 * @file    evolution/core/behavior/SemanticConfigParser.hpp
 * @brief   Parser of semantic configuration.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::SemanticConfigParser
 **/


#include <list>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Configuration.hpp>
#include <evolution/core/behavior/SemanticConfigTypes.hpp>


namespace Evolution
{


    /**
     * @brief
     * Manages access to behavior semantic data type information.
     *
     * @see     IConfigSource Configuration String
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR SemanticConfigParser : public Configuration
    {
    public: // Structors

        /**
         * @brief
         * Constructor.
         * 
         * @param source [in]; The configuration source to parse.
         */
        SemanticConfigParser (IConfigSource& source);

        /**
         * @brief
         * Destructor.
         */
        virtual ~SemanticConfigParser();
        
    public: // Access
        
        /**
         * @brief
         * Returns a reference to the semantic type table.
         *
         * @return Reference to the semantic type table.
         */
        const SemanticTypeTable& get_semantic_table() const;

        /**
         * @brief
         * Returns the default semantic parser, loading it if necessary.
         *
         * @return A pointer to the default semantic parser.
         */
        static const SemanticConfigParser* get_default_parser ();

    public: // Parsing

        /**
         * @brief
         * Load the configuration data from the data source.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_config();

    protected: // Helpers

        /**
         * @brief
         * Parses a semantic type.
         *
         * @param type_key [in]; The configuration key to the semantic type
         *      configuration.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result parse_semantic_type (ConfigKey type_key);

    protected: // Implementation
        SemanticTypeTable  _semantic_table;   ///< Table of semantic types.
    }; // end class SemanticConfigParser


} // end namespace Evolution


#endif // end INCLUDE_EVOLUTION_CORE_BEHAVIOR__SEMANTIC_CONFIG_PARSER_HPP
