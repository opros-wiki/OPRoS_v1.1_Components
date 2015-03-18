//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RANGE_DATA_HPP
#define INCLUDE_EVOLUTION_CORE_RANGE_DATA_HPP


/**
 * @file    evolution/core/resource/RangeData.hpp
 * @brief   Encapsulate a data structure describing the detection
 *          of some entity at a certain range.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::RangeData
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Vector3.hpp>

namespace Evolution
{


    /**
     * @class RangeData
     *
     * @brief This class provides a type to store range information. 
     *
     * @ingroup core_resource
     * @deprecated  Please use a DoubleArray of 5 values instead of this class.
     */
    class EVOLUTION_EXPORT_RESOURCE RangeData
    {
    public:                  // Structors

        /**
         * @brief Constructor
         */
        RangeData (  ):x ( 0 ), y ( 0 ), z ( 0 ), d ( 0 ), timestamp ( 0 )
        {
        }

    public:                  // Data
        double x;               ///< X Range data
        double y;               ///< Y Range data
        double z;               ///< Z range data
        double d;               ///< Depth range data
        Timestamp timestamp;    ///< Timestamp of the data
    };                          // end class RangeData


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_RANGE_DATA_HPP
