//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_TYPES_HPP
#define INCLUDE_EVOLUTION_BASE_TYPES_HPP


/**
 * @file    evolution/core/base/Types.hpp
 * @brief   Basic types.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_base_aibo
 * @ingroup core_base
 */


#if EVOLUTION_PLATFORM_AIBO
#include <iostream.h>
#else
#include <iostream>
#endif
#include <evolution/core/base/ERSPLibC.hpp>
#include <evolution/core/base/Export.hpp>


#ifdef EVOLUTION_PLATFORM_WIN32
#pragma warning(disable: 4251 4275)  // STL export warning.
#pragma warning(disable: 4267)  // Conversion of size_t to unsigned int.
#endif  // EVOLUTION_PLATFORM_WIN32

#ifdef EVOLUTION_PLATFORM_LINUX
#include <sys/types.h>
#endif //EVOLUTION_PLATFORM_LINUX

#include <deque>
#include <string>


namespace Evolution
{


    /**
     * @brief A function result.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef unsigned long Result;

    /**
     * @brief Placeholder for security tickets.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef unsigned long TicketId;

    /**
     * @brief Constant indicating no security.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    const TicketId NO_TICKET = 0;

    /**
     * @brief ID for the registration of an object in a container.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef unsigned long RegistrationId;

    /**
     * @brief ID for a callback instance.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef size_t CallbackId;

    /*
     * @brief  Evolution String class.  Currently implemented as a 
     * typedef std::string.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef std::string   String;

    /**
     * @brief Handy list of Strings...
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef std::deque<String>  StringList;

    /**
     * @brief Timestamping time. May be changed to larger resolution type.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef double Timestamp;

    /**
     * @brief Defining uchar as an abbreviation for unsigned char.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef unsigned char uchar;

    /**
     * @brief Defining ulong as an abbreviation for unsigned long.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
#ifndef ulong
    typedef unsigned long int ulong;
#endif
    /**
     * @brief Infinite value.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    EVOLUTION_EXPORT_BASE extern const unsigned long ERSP_INFINITE;

    /**
     * @class ColorTriplet
     * @brief Triplet of color values.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    struct ColorTriplet
    {
        uchar _data[3]; ///< Color triplet value, 8 bits per color, 24 bits per pixel

        /**
         * @brief Constructor.
         *
         * @param val [in]; The initial value of the color triplet
         */
        explicit ColorTriplet (uchar val = 0)
        {
            _data[0] = _data[1] = _data[2] = val;
        }

        /**
         * @brief Array subscript operator.
         *
         * @param index [in]; An array index
         *
         * @return A reference to the value at the index.
         */
        uchar& operator[] (int index)
        {
            return _data[index];
        }

        /**
         * @brief Const array subscript operator.
         *
         * @param index [in]; An array index
         *
         * @return A const reference to the value at the index.
         */
        const uchar& operator[] (int index) const
        {
            return _data[index];
        }
        /**
         * @brief Print the contents of the ColorTriplet to the specified 
         *        stream.
         *
         * @return The ostream object.
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::ColorTriplet & vec)
        {
            return (out << "( " << vec[0] << ", " << vec[1] << ", " << vec[2]
                    << " )");
        }
    };

    /**
     * @brief 64-bit integers
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
#if __GNUC__ > 2
    typedef long long ersp_int64;
    typedef unsigned long long ersp_uint64;
#elif defined(WIN32)
    typedef __int64 ersp_int64;
    typedef unsigned __int64 ersp_uint64;
#elif defined(GHS)
    typedef long ersp_int64;
    typedef unsigned long ersp_uint64;
#else
#error  I do not know how to get a 64-bit integer on your platform.
#endif


} // end namespace Evolution

#ifndef DOXYGEN_INTERNAL
#ifndef ulong
typedef Evolution::ulong ulong;
#endif
#endif // DOXYGEN_INTERNAL


#endif  // INCLUDE_EVOLUTION_BASE_TYPES_HPP
