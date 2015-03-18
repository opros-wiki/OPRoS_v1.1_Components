//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_RESOURCE_RANGE_SCAN_HPP
#define INCLUDE_EVOLUTION_RESOURCE_RANGE_SCAN_HPP

/**
 * @file    evolution/core/resource/RangeScan.hpp
 * @brief   Range data from a range sensor
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::RangeScan
 */

#include <math.h>

#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/SphericalPoint.hpp>
#include <evolution/core/math/Vector3.hpp>

// Forward declarations
class ostream;

namespace Evolution
{

    /**
     * @class RangeScan
     * @brief This class provides a set of data from a range sensor reading.  
     * Data values include:
     * @li ID of sensor that generated reading @n
     * @li Maximum range of the sensor's scans @n
     * @li Location of sensor in local Cartesian coordinates @n
     * @li List of polar representations of sensor scans (offset from Location) @n
     * @li Yaw/Pitch angle of sensor scan direction @n
     * @li Distance measured by sensor @n
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE RangeScan : public ObjectImpl
    {
    public:

        /**
         * @brief Interface ID for IObject.
         */
        static const char * const INTERFACE_ID;

        /**
         * @brief Default constructor.
         *
         * @param id        [in, opt]; The ID of the sensor 
         * @param location  [in, opt]; Location of sensor in location
         *                             Cartesian coordinates
         * @param time      [in, opt]; Time when the scan was taken
         * @param max_range [in, opt]; The maximum range of the sensor's scan
         */
        RangeScan (const String id = "",
                   const Vector3 location = Vector3(),
                   Timestamp time = 0,
                   double max_range = -1);

        virtual ~RangeScan();

        /**
         * @brief The obtain_interface function returns an implementation for IObject
         *
         * @param ticket       [in]; Security ticket (a placeholder)
         * @param interface_id [in]; The interface ID of the requested interface.
         *                           This ID should be declared with the
         *                           INTERFACE_ID constant member on the
         *                           interface class.
         * @param interface_ptr [out]; This is the pointer to the interface
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result obtain_interface (TicketId ticket,
                                 const char* interface_id,
                                 void** interface_ptr);

        /**
         * @brief This prints a simple description of the object contents.
         *
         * @param out [out]; The output stream to write to
         */
        void print (std::ostream& out) const;

    public: // data

        String     _id;           ///< Sensor ID tag
        double     _max_range;    ///< Maximum range of sensor, or -1 for no maximum
        Vector3    _location;     ///< Location of sensor in local coordinates
        ScanPoints _scans;        ///< List of scans from sensor
        Timestamp  _time;         ///< Time scan was taken
        double     _theta_spread; ///< Angular spread of sensor.
        double     _rho_spread;   ///< Radius error spread factor of sensor.
    }; // end class RangeScan

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_RESOURCE_RANGE_SCAN_HPP
