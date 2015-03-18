//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_IMPL_HELPERS_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_IMPL_HELPERS_HPP


/**
 * @file   evolution/core/behavior/BehaviorImplHelpers.hpp
 * @brief  Functions to help with behavior implementations.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::BehaviorImplHelpers
 */


#include <vector>
#include <map>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/MultiValue.hpp>
#include <evolution/core/behavior/BehaviorImpl.hpp>
#include <evolution/core/behavior/SemanticConfigParser.hpp>


namespace Evolution
{

    /**
     * @brief Input string processing typedefs and functions that split
     * configuration strings into tables and vectors for behaviors.
     * @ingroup core_behavior
     */
    typedef std::vector<double> VectorDouble;
    typedef std::vector<String> VectorString;
    typedef std::map<String, double> TableDouble;
    typedef std::map<String, VectorDouble> TableVectorDouble;
    typedef std::vector<VectorDouble> MatrixDouble;
    typedef std::map<String, BehaviorData*> TableData;


    /**
     * @brief Utility class of functions that are helpful when implementing a
     * behavior.
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorImplHelpers
    {
    public: // Types

        /// For convenience.
        typedef BehaviorImpl::BehaviorPortKey    BehaviorPortKey;   ///< Behavior port key
        typedef BehaviorImpl::PortId             PortId;            ///< Port id
        typedef BehaviorImpl::PortInputTable     PortInputTable;    ///< Port input table
        typedef BehaviorImpl::PortInputIterator  PortInputIterator; ///< Port input iterator

    public: // Input data

        /**
         * @brief Get data from source's source_port.
         *
         * Obtains the first data entry for the specific input port, source,
         * and source port.
         *
         * @param input_table [in];  Table to get first input from
         * @param source      [in];  source behavior to get data from
         * @param source_port [in];  PortId of port to get data from
         * @param input_data  [out]; *input_data is *BehaviorData that points to
         *                           the desired data
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result find_input_data (const PortInputTable& input_table,
                                       IBehavior* source,
                                       PortId source_port,
                                       BehaviorData** input_data);

        /**
         * @brief Get first non-null data from input_table.
         *
         * Returns the first non-null data in the table, or NULL if none.
         *
         * @param input_table [in];      Table to get data from
         * @param behavior    [in, opt]; Lets you pass the target behavior
         *                               of the connection, for debugging purposes
         *
         * @return Pointer to the first data in input_table.
         */
        static BehaviorData* get_first_data (const PortInputTable& input_table,
                                             IBehavior *behavior = NULL);

    public: // Configuration

        /**
         * @brief
         * Obtains the configuration information for an input or
         * output port on the given behavior. It is specified either by the
         * port index or name.
         *
         * @param  config      [in];  The behavior configuration information
         * @param  input       [in];  A flag indicating if this is an input or output port
         * @param  port_name   [in];  The name of the port if non-NULL; use index otherwise
         * @param  port_index  [in];  The zero-base index of the port
         * @param  port_config [out]; The returned port configuration information
         *
         * @return  RESULT_SUCCESS, RESULT_NOT_FOUND, or other failure.
         */
        static Result get_port_config (BehaviorConfig& config,
                                       bool input,
                                       const char* port_name,
                                       PortId port_index,
                                       BehaviorPortConfig** port_config);


        /**
         * @brief
         * Obtains the specified semantic type information.
         *
         * @param   parser      [in];  The semantic parser
         * @param   type_name   [in];  The name of the semantic type
         * @param   type_config [out]; The returned type config
         *
         * @return  RESULT_SUCCESS, RESULT_NOT_FOUND, or other failure.
         */
        static Result get_semantic_type (const SemanticConfigParser* parser,
                                         const char* type_name,
                                         SemanticTypeConfig** type_config);

        /**
         * @brief Obtains the intervals specified for a multivalue port, as
         * specified by its semantic type.
         *
         * This method is useful for a behavior that has a multivalue
         * port and needs to size data for it.
         * @param config [in]; config information for the beahvior being queried.
         * @param input [in]; does nothing.
         * @param port_name [in]; portname to match.
         * @param port_index [in]; index of port to obtain information from.
         * @param intervals [out]; the interval information in question.
         *
         * @return  RESULT_SUCCESS if correctly parsed,
         *          RESULT_NOT_FOUND if no semantic type is found,
         *          or other error.
         */
        static Result get_port_intervals (BehaviorConfig& config,
                                          bool input,
                                          const char* port_name,
                                          PortId port_index,
                                          IntervalVector* intervals);

    public: // Parsing

        /**
         * @brief Parses a set of the Matlab-style intervals of
         * the form: [st:s:sp]
         *
         * @param value     [in];  String containing matlab style interval
         * @param intervals [out]; Actual interval values in an IntervalVector
         *                         object
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_intervals (const char* value,
                                       IntervalVector* intervals);

        /**
         * @brief  Parses a comma-separated list of doubles into a DoubleArray.
         *
         * @param  input  [in];  String in form: "10.2, 1.23, ..."
         * @param  output [out]; DoubleArray holding parsed values
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_double_array (const char* input, DoubleArray* output);

        /**
         * @brief Reads a list of strings into a vector of strings.
         *
         * @param input  [in];  String in form: "first phrase;
         *                      second phrase; ..."
         * @param output [out]; Set to vector containing "first phrase",
         *                      "second phrase", ...
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_vector_string(const char* input, VectorString* output);

        /**
         * @brief Reads a list of doubles into a vector of doubles
         *
         * @param input  [in];   String in form: "10.2; -1.532; 32; ..."
         * @param output [out];  Set to vector containing 10.2, -1.532, 32, ...
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_table_double(const char* input, TableDouble* output);

        /**
         * @brief Reads a pairing of strings/double pairs into a table.
         *
         * @param input  [in];  String in form: "x=1.0; y = -2.0; ..."
         * @param output [out]; Set to table containing x => 1.0, y => -2.0, ...
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_table_vector_double(const char* input,
                                                TableVectorDouble* output);

        /**
         * @brief Reads a list of string => double lists into a matrix table
         *
         * @param input  [in];  String in form: "x = 1, 2, 3; y = -2, -1, 0; ..."
         * @param output [out]; Set to table containing x => {1.2,3}, y => {-2,-1,0}, ...
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_matrix_double(const char* input,
                                          MatrixDouble* output);

        /**
         * @brief Adds a key/value pair entry to a TableData structure and
         * records its key in the ordered list of keys.
         *
         * @param key   [in];  Key of the pair to add
         * @param data  [in];  Data of the pair to add
         * @param table [out]; Table to add key/data pair to. pass in ptr to object
         * @param keys  [out]; Ordered list of keys
         */
        static void record_table_data_entry(const String& key, BehaviorData* data,
                                            TableData* table, VectorString* keys);

        /**
         * @brief Reads a list of string => behavior data lists into a data table
         *
         * @param input [in]; String in form: 'x = 1, 2, 3; y = "foo"; z = true'
         * @param table [out]; table to write data sets to
         * @param keys  [out]; ordered list of keys
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_table_data (const char* input,
                                        TableData* table,
                                        VectorString* keys);

        /**
         * Parses a string with an avoidance method.
         *
         * Currently accepted values are:
         *    NO_AVOIDANCE           => NO_AVOIDANCE
         *    FUZZY_AVOIDANCE        => FUZZY_AVOIDANCE
         *    STRAIGHT_ONLY          => STRAIGHT_ONLY
         *
         * @param input     [in];  String with name of avoidance method
         * @param avoidance [out]; Enum encoding the avoidance method of
         *                         the string
         *
         * @see  AvoidanceMethod
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_avoidance_method (const char* input, long* avoidance);

        /**
         * @brief
         * Takes BehaviorData represented as a string and parses it into a
         * BehaviorDataWriter.
         *
         * Data represented as a string is type ambiguous, so we have to make
         * certain assumptions. parse_behavior_data supports the following
         * types: boolean, enum, double, double array, and string. Use the
         * following format for specifying each type:
         * <ul>
         * <li> boolean: "\true" or "\false" only. If you leave out the leading
         * backslash it will be parsed as a string. </li>
         * <li> enum: "\enum(value)" only where value is a valid long integer.
         * If you leave out the "\enum(" it will be parsed as a double. </li>
         * <li> double: "value" or "\double(value)" where value is a valid
         * double as determined by the std::atof function. The explicit
         * "\double(" is not necessary but may be used. </li>
         * <li> double array: "value,value,..." or
         * "\doublearray(value,value,...)", a comma-delimited list of
         * valid double values as determined by std::atof. The explicit
         * "\doublearray(" is not necessary but it may be used in the
         * case of a type ambiguity (e.g., if you would like to parse "3.14"
         * as a one element double array).</li>
         * <li> string: "string" or "\string(string)" where string can
         * contain any characters. The explicit "\string(" is not necessary
         * but may be used in the case of a type ambiguity (eg, if you would
         * like to parse "3.14" as a string. If any <code>value</code> cannot
         * be parsed as any other of the above types then it will default to
         * being parsed as a string). </li>
         * </ul>
         * And remember that the backslash is the escape character, so
         * representing one backslash in a quoted string looks like "\\" in
         * code.
         *
         * @param     value [in];  The value, represented as a string
         * @param     data  [out]; The value, encapsulated in a
         *                         BehaviorDataWriter
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result parse_generic_parameter (const char * value,
                                               BehaviorDataWriter * data);

    protected: // Helpers for parse_generic_parameter()

        /**
         * @brief
         * Helper that checks to see if the string passed in can be parsed as a
         * double.
         *
         * @param value [in];  The string to be checked
         *
         * @return True if it can be parsed as a double, false if not.
         *
         * @pre Value points to a valid string.
         */
        static bool check_for_double(const char * value);

        /**
         * @brief
         * Helper that checks to see if the string passed in can be parsed as a
         * hexidecimal value. Values are considered hexidecimal if they are of
         * the form: 0(x|X)(nnnn) where nnnn is a series of one to eight digits
         * within the hexidecimal range (0-9,a-f).
         *
         * @param value [in];  The string to be checked
         *
         * @return True if it can be parsed as a hexidecimal value, false if not.
         *
         * @pre Value points to a valid string.
         */
        static bool check_for_hexidecimal(const char * value);

        /**
         * @brief
         * Helper that parses the input value as an enum.
         *
         * @param     value [in];  The value, represented as a string
         * @param     data  [out]; The value, encapsulated in a BehaviorDataWriter
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         **/
        static Result parse_generic_enum(const char * value,
                                         BehaviorDataWriter * data);

        /**
         * @brief
         * Helper that parses the input value as an string.
         *
         * @param     value [in];  The value, represented as a string
         * @param     data  [out]; The value, encapsulated in a BehaviorDataWriter
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         */
        static Result parse_generic_string(const char * value,
                                           BehaviorDataWriter * data);
        /**
         * @brief
         * Helper that parses the input value as double.
         *
         * @param     value [in];  The value, represented as a string
         * @param     data  [out]; The value, encapsulated in a BehaviorDataWriter
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         */
        static Result parse_generic_double(const char * value,
                                           BehaviorDataWriter * data);
        /**
         * @brief
         * Helper that parses the input value as double array.
         *
         * @param     value [in];  The value, represented as a string
         * @param     data  [out]; The value, encapsulated in a BehaviorDataWriter
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         */
        static Result parse_generic_double_array(const char * value,
                                                 BehaviorDataWriter * data);
        /**
         * @brief
         * Helper that parses the input value an image.
         *
         * @param     value [in];  The image filename, represented as a string
         * @param     data  [out]; The image, encapsulated in a BehaviorDataWriter
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         */
        static Result parse_generic_image(const char * value,
                                          BehaviorDataWriter * data);
        /**
         * @brief
         * Helper that guesses how the input value should be parsed.
         *
         * @param     value [in];  The value, represented as a string
         * @param     data  [out]; The value, encapsulated in a BehaviorDataWriter
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     Value points to a valid string.
         * @pre     Data is initialized to a valid BehaviorDataWriter *.
         */
        static Result parse_generic_default(const char * value,
                                            BehaviorDataWriter * data);

    }; // end BehaviorImplHelpers


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_IMPL_HPP
