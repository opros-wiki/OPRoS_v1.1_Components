//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_PARSE_UTILS_HPP
#define INCLUDE_EVOLUTION_MATH_PARSE_UTILS_HPP


/**
 * @file    evolution/core/math/ParseUtils.hpp
 * @brief   Functions to help parsing strings.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_math
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Matrix.hpp>


namespace Evolution
{

    /**
     * @brief
     * Input string processing typedefs and functions that split
     * configuration strings into tables and vectors for behaviors.
     * @ingroup core_math
     */
    namespace ParseUtils
    {

        /**
         * @brief Trims extra spaces from a string.
         * @param input [in]; Old string (eg. "  foo  bar   ")
         * @returns Trimmed string (eg. "foo bar")
         */
        extern EVOLUTION_EXPORT_MATH String trim_spaces(String& input);

        /**
         * @brief  Parses a comma-separated list of doubles into a DoubleArray.
         * @param  input  [in];  String in form: "10.2, 1.23, ..."
         * @param  output [out]; DoubleArray holding parsed values.
         *
         * @return Result indicating success or failure
         **/
        extern EVOLUTION_EXPORT_MATH Result parse_double_array (const char* input, 
                                                                DoubleArray* output);

        /**
         * @brief Converts the contents of a DoubleArray 
         *        into a comma-separated string.
         * @param input [in]; DoubleArray for conversion.
         * @param str [out]; String in the form "1.0, 2.3, ..."
         *
         * @return Result indicating success or failure
         */
        extern EVOLUTION_EXPORT_MATH Result double_array_to_string (DoubleArray & input, 
                                                                    String * str);

        /**
         * @brief  Parses a comma-separated list of doubles 
         *         into a Vector<double>.
         * @param  input  [in];  String in form: "10.2, 1.23, ..."
         * @param  output [out]; Vector<double> holding parsed values.
         *
         * @return Result indicating success or failure
         **/
        extern EVOLUTION_EXPORT_MATH Result parse_vector_double (const char* input, 
                                                                 Vector<double>* output);

        /**
         * @brief Converts the contents of a vector into a 
         *        comma-separated string.
         * @param input [in]; Vector input for conversion.
         * @param str [out]; String in the form "1.0, 2.3, ..."
         *
         * @return Result indicating success or failure
         */
        extern EVOLUTION_EXPORT_MATH Result vector_double_to_string (Vector<double>&  input, 
                                                                     String * str);

        /**
         * @brief Converts the contents of a matrix into a 
         *        comma-separated string (the matrix is stored row-wise).
         * @param input [in]; Matrix input for conversion.
         * @param str [out]; String in the form "1.0, 2.3, ..."
         *
         * @return Result indicating success or failure
         */
        extern EVOLUTION_EXPORT_MATH Result matrix_double_to_string (Matrix<double>& input, 
                                                                     String * str);

    } // end namespace ParseUtils.

} // end namespace Evolution.

#endif  // INCLUDE_EVOLUTION_MATH_PARSE_UTILS_HPP
