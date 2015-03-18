//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BASE__STRING_UTILS_HPP
#define INCLUDE_EVOLUTION_CORE_BASE__STRING_UTILS_HPP


/**
 * @file    evolution/core/base/StringUtils.hpp
 * @brief   String utilities.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/


#include <assert.h>
#include <ctype.h>
#include <deque>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{
    /// Convenience definition for a list of strings.
    typedef std::deque<String> StringList;

    /**
     * @brief Set of convenience functions used to manipulate strings.
     *
     * @ingroup core_base
     */
    namespace StringUtils
    {

        /**
         * @brief Character class enumeration corresponding to isXXX() functions in ctype.h.
         *
         * @ingroup core_base
         */
        enum CharacterClass
        {
            ALNUM_CHAR,
            ALPHA_CHAR,
            CNTRL_CHAR,
            DIGIT_CHAR,
            GRAPH_CHAR,
            LOWER_CHAR,
            PRINT_CHAR,

            PUNCT_CHAR,
            SPACE_CHAR,
            UPPER_CHAR,
            XDIGIT_CHAR
        };


        /**
         * @brief Calls the appropriate function from ctype.h according to
         * the specified character class.
         *
         * @param char_class [in]; The CharacterClass
         * @param c          [in]; The test value
         *
         * @return 0 on failure.
         *
         * @ingroup core_base
         */
        inline int
        is_character (CharacterClass char_class, int c)
        {
            switch (char_class)
            {
            case ALNUM_CHAR:
                return (isalnum (c));
            case ALPHA_CHAR:
                return (isalpha (c));
            case CNTRL_CHAR:
                return (iscntrl (c));
            case DIGIT_CHAR:
                return (isdigit (c));
            case GRAPH_CHAR:
                return (isgraph (c));
            case LOWER_CHAR:
                return (islower (c));
            case PRINT_CHAR:
                return (isprint (c));
            case PUNCT_CHAR:
                return (ispunct (c));
            case SPACE_CHAR:
                return (isspace (c));
            case UPPER_CHAR:
                return (isupper (c));
            case XDIGIT_CHAR:
                return (isxdigit (c));
            default:
                assert (false);
                return 0;
            }
        } // end is_character()


        /**
         * @brief Splits the given input into a list of strings, according to
         * the given delimiter character.
         *
         * @param   input     [in];  Input string to split (e.g. "foo,bar,hmm")
         * @param   delimiter [in];  Character on which to split (e.g. ',')
         * @param   output    [out]; List of strings split (e.g. "foo", "bar", "hmm")
         *
         * @ingroup core_base
         */
        extern EVOLUTION_EXPORT_BASE void
        split (const char* input, char delimiter, StringList* output);


        /**
         * @brief Splits the given input into a list of strings, according to
         * the given character class.
         *
         * @param   input      [in];  Input string to split (e.g. "foo,bar,hmm")
         * @param   trim_class [in];  Character class on which to split
         * @param   output     [out]; List of strings split (e.g. "foo", "bar", "hmm")
         *
         * @ingroup core_base
         */
        extern EVOLUTION_EXPORT_BASE void
        split (const char* input, CharacterClass trim_class, StringList* output);


        /**
         * @brief Splits on spaces.
         * Splits the given input into a list of strings, using spaces
         * as the delimiter character.
         *
         * @param   input      [in];  Input string to split (e.g. "foo,bar,hmm")
         * @param   output     [out]; List of strings split (e.g. "foo", "bar", "hmm")
         *
         * @ingroup core_base
         */
        inline void
        split (const char* input, StringList* output)
        {
            split (input, SPACE_CHAR, output);
        }


        /**
         * @brief Trims the specified character from the input string,
         * removing all initial, terminal, repeated instances of the
         * character.
         *
         * @param   input     [in];  Old string (e.g. "  foo  bar   ")
         * @param   trim_char [in];  Character (e.g ',') to trim
         * @param   output    [out]; Trimmed string (e.g. "foo bar")
         *
         * @ingroup core_base
         */
        extern EVOLUTION_EXPORT_BASE void
        trim (const char* input, char trim_char, String* output);


        /**
         * @brief
         * Trims characters in the specified character class from the
         * input string, removing all initial, terminal, repeated
         * instances of the characters.
         *
         * @param   input      [in];  Old string (eg. "  foo  bar   ")
         * @param   trim_class [in];  Character class on which to trim
         * @param   output     [out]; Trimmed string (eg. "foo bar")
         *
         * @ingroup core_base
         */
        extern EVOLUTION_EXPORT_BASE void
        trim (const char* input, CharacterClass trim_class, String* output);
 

        /**
         * @brief
         * Trims spaces from the input string, removing all initial, terminal,
         * repeated instances of the character.
         *
         * @param   input      [in];  Old string (eg. "  foo  bar   ")
         * @param   output     [out]; Trimmed string (eg. "foo bar")
         *
         * @ingroup core_base
         */
        inline void
        trim (const char* input, String* output)
        {
            trim (input, SPACE_CHAR, output);
        }


        /**
         * @brief
         * Trims the specified character from the input string, removing all
         * initial, terminal, repeated instances of the characters.
         *
         * @param   input      [in];  Old string (eg. "  foo  bar   ")
         * @param   trim_char [in];  Character class on which to trim
         *
         * @return  A copy of the trimmed String.
         *
         * @ingroup core_base
         */
        inline String
        trim (const char* input, char trim_char)
        {
            String result;
            trim (input, trim_char, &result);
            return (result);
        }


        /**
         * @brief
         * Trims characters in the specified character class from the
         * input string, removing all initial, terminal, repeated
         * instances of the characters.
         *
         * @param   input      [in];  Old string (eg. "  foo  bar   ")
         * @param   trim_class [in];  Character class on which to trim
         *
         * @return  A copy of the trimmed String.
         *
         * @ingroup core_base
         */
        inline String
        trim (const char* input, CharacterClass trim_class)
        {
            String result;
            trim (input, trim_class, &result);
            return (result);
        }


        /**
         * @brief
         * Trims spaces from the input string, removing all initial, terminal,
         * repeated instances of the character.
         *
         * @param   input      [in];  Old string (eg. "  foo  bar   ")
         *
         * @return  A copy of the trimmed String.
         *
         * @ingroup core_base
         */
        inline String
        trim (const char* input)
        {
            String result;
            trim (input, &result);
            return (result);
        }


    } // end namespace StringUtils


} // end namespace Evolution


#endif  //  INCLUDE_EVOLUTION_CORE_BASE__STRING_UTILS_HPP
