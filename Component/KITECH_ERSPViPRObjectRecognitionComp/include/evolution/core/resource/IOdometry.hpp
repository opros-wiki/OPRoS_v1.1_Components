//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IODOMETRY_HPP
#define INCLUDE_EVOLUTION_CORE_IODOMETRY_HPP


/**
 * @file    evolution/core/resource/IOdometry.hpp
 * @brief  Contains a class to define a resource interface to odometry drivers
 * to convert raw values to x/y/theta positions
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IOdometry
 * @see Evolution::Pose2DT
 */


#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Pose2DT.hpp>

namespace Evolution
{

    // Forward declarations
    class IResourceCallback;

    /**
     * @interface IOdometry 
     *
     * @brief This class defines a resource interface to an odometry driver
     * which converts raw motor encoder values to x,y,theta position using
     * motor properties, calibrations, and possibly error correction.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IOdometry 
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.IOdometry"

    public: // Structors

        /**
         * @brief Constructor
         */
        IOdometry () {}

        /**
         * @brief Destructor
         */
        virtual ~IOdometry () {}

    public: // Queries

        /**
         * @brief The get_position function obtains a calibrated position
         * reading with no error
         * estimation (for now).
         *
         * @param ticket   [in];  the security ticket
         * @param position [out]; the returned distance reading
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result get_position (TicketId ticket, Pose2DT* position) = 0;

        /**
         * @brief The get_odometer_distance function obtains the distance
         * traversed since the last odometer reset. 
         *
         * @param ticket   [in];  The security ticket
         * @param distance [out]; The distance traversed since the last
         *                        odometer reset
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @see reset_odometer
         */
        virtual Result get_odometer_distance (TicketId ticket, double* distance) = 0;
        
        /**
         * @brief The get_velocities function gets the current velocity and
         * angular velocity.
         *
         * @param   ticket  [in];    The security ticket
         * @param   v       [out];   The velocity
         * @param   w       [out];   The angular velocity
         * 
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result get_velocities(TicketId ticket, double* v, double* w) = 0;

        /**
         * The get_raw_data function gets raw data from the odometer.  The
         * output format is a double array.  However, the values and order
         * of values in this array will be specific to the raw data available
         * from the particular underlying odometry hardware and therefore 
         * is not defined here.
         *
         * @param   ticket  [in];    The security ticket
         * @param   data    [out];   Array of doubles specific to the odometry
         * 
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result get_raw_data(TicketId ticket, DoubleArray* data) = 0;

        /**
         * @brief The get_covariance function gets the odometry covariance.
         *
         * @param ticket     [in];  The security ticket
         * @param covariance [out]; The odometry covariance
         *
         * @return RESULT_SUCCESS on success or an error result.
         */ 
        virtual Result get_covariance(TicketId ticket, Matrix<double>* covariance) = 0;

        /**
         * @brief The read_odometry_error_from_config function 
         * reads odometry standard deviations from resource-config
         * file.  On error, std_trans and std_rot will be set to
         * internally-defined default values.
         * 
         * Read odometry standard deviations from resource-config
         * file.  The parameters read are "std_dev_translation" and
         * "std_dev_rotation".  On error, the return values of
         * std_trans and std_rot will be set to internally-defined
         * default values.
         *
         * @param std_trans [out]; The estimated standard deviation of
         *                         the translational component of an odometry
         *                         reading
         * @param std_rot [out]; The estimated standard deviation of
         *                       the rotational component of an odometry reading
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result read_odometry_error_from_config(double *std_trans,
                                                       double *std_rot) = 0;
    public: // Commands

        /**
         * @brief The reset_odometer function sets traversed distance to 0.
         *    This can be implemented as either a hardware reset if possible
         *    or just an internal software representation of odometry reset.
         *
         * @param ticket [in]; The security ticket
         *
         * @return RESULT_SUCCESS on success, or some error value.
         *
         * @see get_odometer_distance
         */
        virtual Result reset_odometer (TicketId ticket) = 0;

        /**
         * @brief The reset_covariance function resets odometry covariance
         *
         * @param ticket     [in]; The security ticket
         * @param covariance [in]; The new odometry covariance
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result reset_covariance(TicketId ticket, const Matrix<double>& covariance) = 0;

    }; // end class IOdometry

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_IODOMETRY_HPP
