//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_ILASER_SENSOR_HPP
#define INCLUDE_EVOLUTION_CORE_ILASER_SENSOR_HPP


/**
 * @file    evolution/core/resource/ILaserSensor.hpp
 * @brief   This file defines a class to provide an interface to a laser sensor.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::ILaserSensor
 */


#include <stddef.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/math/PointSet.hpp>
#include <evolution/core/resource/ISpatialSensor.hpp>


namespace Evolution
{


    /**
     * @interface ILaserSensor 
     *
     * @brief This class defines an interface to a laser spatial sensor. 
     * A laser spatial sensor is a laser that senses
     * objects in three-dimensional space.
     *
     * @note Interface ID = "Evolution.ILaserSensor"
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ILaserSensor : public ISpatialSensor
    {
    public: // Types

        class SpatialSensorParms;

    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.ILaserSensor"

    public: // Structors

        /**
         * @brief Empty constructor
         */
        ILaserSensor () {}

        /**
         * @brief Empty destructor
         */
        virtual ~ILaserSensor () {}

    public: // Commands

        /**
         * Obtains different kinds of laser data from the scanner
         *
         * @param  ticket [in]; The security ticket
         * @param  distances [out]; The set of distances or NULL for don't compute.
         *       Distances will be MAXDOUBLE for readings beyond the range
         *       of the scanner and -1 if an error occured.
         * @param  points [out]; the set of points or NULL for don't compute
         */
        virtual Result get_laser_data (TicketId ticket,
                                       DoubleArray** distances = NULL,
                                       PointSet** points = NULL)
            = 0;

    public: // Type implementation
    

    }; // end class ILaserSensor


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_ILASER_SENSOR_HPP

