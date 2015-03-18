//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  This material contains trade secrets and confidential information
//  of Evolution Robotics, Inc.  Any use, reproduction, disclosure or
//  dissemination is strictly prohibited without the explicit written
//  permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE_CAMERAUTILS_HPP
#define INCLUDE_EVOLUTION_RESOURCE_CAMERAUTILS_HPP


/**
 * @file   evolution/core/resource/CameraUtils.hpp
 * @brief  Use these convenience functions for retrieving a camera from
 * the resource manager.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


#include <evolution/core/resource/ICamera.hpp>

namespace Evolution
{
    /**
     * @brief  Use these convenience functions for retrieving a camera from
     * the resource manager.
     *
     * @ingroup core_vision
     */
    namespace CameraUtils
    {

        /**
         * @brief Gets an ICamera object from the resource management subsystem.
         * Use release_camera () to free the resource.
         *
         * @param interface_id [in]; The name of the camera interface
         * @param camera_interface [out]; The camera interface
         *
         * @ingroup core_vision
         */
        EVOLUTION_EXPORT_RESOURCE
        Result get_camera_by_id (const char *interface_id,
                                 ICamera    **camera_interface);

        /**
         * @brief Releases the ICamera object from the resource
         * management subsystem.
         *
         * @param camera_interface [in]; Pointer to the interface to be released.
         *
         * @ingroup core_vision
         */
        EVOLUTION_EXPORT_RESOURCE
        Result release_camera (ICamera *camera_interface);

        /**
         * @brief Convert camera-relative normalized coordinates
         * into pixel locations, taking into account
         * all the intrinsic parameters of the camera.
         *
         * @param parameters [in]; intrinsic calibration parameters
         * for the camera.
         * @param normalized [in]; array of two doubles, the normalized (x,y)
         * coordinates.  (0,0) is directly in front of the camera
         * while (1,0), (-1,0), (0,1), (0,-1) lie in the center of the
         * sides of a 90-degree viewing frustum.
         * @param pixel [out]; array of two doubles, the (x,y) location on the screen
         * in pixels.
         *
         * @ingroup core_vision
         */
        EVOLUTION_EXPORT_RESOURCE
        void convert_normalized_to_pixel (const CameraParameters *parameters,
                                          const double           *normalized,
                                          double                 *pixel);

        /**
         * @brief Convert camera-relative normalized coordinates
         * into pixel locations, taking into account
         * all the intrinsic parameters of the camera.
         *
         * @param parameters [in]; intrinsic calibration parameters
         * for the camera.
         * @param pixel  [in]; array of two doubles, the (x,y) location on the screen
         * in pixels.
         * @param normalized [out]; array of two doubles, the normalized (x,y)
         * coordinates.  (0,0) is directly in front of the camera
         * while (1,0), (-1,0), (0,1), (0,-1) lie in the center of the
         * sides of a 90-degree viewing frustum.
         *
         * @ingroup core_vision
         */
        EVOLUTION_EXPORT_RESOURCE
        void convert_pixel_to_normalized (const CameraParameters *parameters,
                                          const double           *pixel,
                                          double                 *normalized);

    } // end namespace CameraUtils

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_RESOURCE_CAMERAUTILS_HPP
