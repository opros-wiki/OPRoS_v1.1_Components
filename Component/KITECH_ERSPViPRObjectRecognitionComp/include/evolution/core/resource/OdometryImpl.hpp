//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_ODOMETRY_IMPL_HPP
#define INCLUDE_EVOLUTION_CORE_ODOMETRY_IMPL_HPP


/**
 * @file   evolution/core/resource/OdometryImpl.hpp
 * @brief  A basic implementation of the IOdometry interface.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::IResourceCallback
 * @see    Evolution::OdometryImpl
 */


#include <evolution/core/resource/IOdometry.hpp>
#include <evolution/core/resource/ResourceDriverImpl.hpp>

namespace Evolution
{

    // Forward declarations
    class IResourceCallback;


    /**
     * @class OdometryImpl 
     *
     * @brief This class provides a resource interface to an odometry driver
     * which converts raw motor encoder values to x,y,theta position using
     * motor properties, calibrations, and possibly error correction.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE OdometryImpl : public ResourceDriverImpl,
                                                   public IOdometry
    {
    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param ticket             [in]; The security ticket
         * @param resource_config    [in]; The resource configuration
         * @param resource_container [in]; The resource container
         */
        OdometryImpl (TicketId ticket,
                      const ResourceConfig& resource_config,
                      IResourceContainer& resource_container); 

        /**
         * @brief Destructor.
         */
        virtual ~OdometryImpl () {}

    public: // Queries

        /**
         * @brief The get_position function obtains a position reading
         * with a timestamp.
         *
         * @param  ticket   [in];  The security ticket
         * @param  position [out]; The returned distance reading
         *
         * @return RESULT_SUCCESS on success, or some error value
         */
        virtual Result get_position (TicketId ticket, Pose2DT* position);

        /**
         * @brief The get_odometer_distance function obtains the distance
         * traversed since the last odometer reset. 
         *
         * @param  ticket   [in];  The security ticket
         * @param  distance [out]; The distance traversed since the last
         *                         odometer reset
         *
         * @return RESULT_SUCCESS on success, or some error value
         *
         * @see reset_odometer
         */
        virtual Result get_odometer_distance (TicketId ticket, double* distance);
        
        /**
         * @brief The get_velocities function gets the current velocity and angular velocity.
         *
         * @param  ticket [in];  The security ticket
         * @param  v      [out]; The velocity
         * @param  w      [out]; The angular velocity
         * 
         * @return  RESULT_SUCCESS on success or an error result describing
         * the error.
         */
        virtual Result get_velocities(TicketId ticket, double* v, double* w);

        /**
         * @brief This function gets raw data from the odometer.
         *
         * @param ticket [in];  The security ticket
         * @param data   [out]; Array of doubles specific to the odometry
         * 
         * @return RESULT_SUCCESS on success or an error result describing the error.
         */
        virtual Result get_raw_data(TicketId ticket, DoubleArray* data);

        /**
         * @brief This function gets the odometry covariance.
         *
         * @param ticket     [in];  The security ticket
         * @param covariance [out]; The odometry covariance
         *
         * @return  RESULT_SUCCESS on success or an error result describing
         * the error
         */ 
        virtual Result get_covariance(TicketId ticket, Matrix<double>* covariance);

        /**
         * @brief The read_odometry_error_from_config function reads odometry
         * standard deviations from resource-config file. On error, std_trans
         * and std_rot will be set to internally-defined default values.
         *
         * @param std_trans [out]; Translation standard deviation
         * @param std_rot   [out]; Rotation standard deviation
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_odometry_error_from_config(double *std_trans,
                                               double *std_rot);

        /**
         * @brief This function reads odometry standard deviations
         * from resource-config file, given a ResourceConfig. On
         * error, std_trans and std_rot will be set to
         * internally-defined default values.
         *
         * @note Having this function separate from
         * read_odometry_error_from_config() is awkward, but it needs
         * to be called also from OdometryVSLAM, which inherits from
         * IOdometry but not from OdometryImpl.  This way, the default
         * values and the names of the config file variables are kept
         * in one place.
         *
         * @param resource_config [in]; The resource configuration 
         * @param std_trans       [out]; Translation standard deviation
         * @param std_rot         [out]; Rotation standard deviation
         *
         * @return RESULT_SUCCESS on success or an error result
         */
        static Result
        static_read_odometry_error_from_config(const ResourceConfig &resource_config,
                                               double *std_trans,
                                               double *std_rot);

    public: // Commands

        /**
         * @brief Reset the odometer, i.e. traversed distance resets to 0.
         *
         * @param ticket [in]; The security ticket
         *
         * @return RESULT_SUCCESS on success, or some error value
         *
         * @see get_odometer_distance
         */
        virtual Result reset_odometer (TicketId ticket);

        /**
         * @brief This function resets odometry covariance.
         *
         * @param ticket     [in]; The security ticket
         * @param covariance [in]; The new odometry covariance
         *
         * @return  RESULT_SUCCESS on success or an error result 
         * describing the error.
         */
        virtual Result reset_covariance(TicketId ticket, const Matrix<double>& covariance);

    protected: // Helpers

        /**
         * @brief This function updates odometry covariance.
         */
        void update_covariance(); 

        /**
         * @brief This function does initialization which can't go in the
         * constructor. Currently it is only used to read std_trans and std_rot
         * from config file.
         */
        void initialize();

    protected: // Data
        
        Pose2DT _current_position;   ///< Current robot pose.
        Pose2DT _last_position;      ///< Last robot pose.

        // Covariance
        Matrix<double> _covariance; ///< Current robot pose covariance.
//         double         _last_dth;

        bool _initialized;          ///< Denotes whether the class has been initialized/

    }; // end class OdometryImpl

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_ODOMETRY_IMPL_HPP
