//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  This material contains trade secrets and confidential information
//  of Evolution Robotics, Inc.  Any use, reproduction, disclosure or
//  dissemination is strictly prohibited without the explicit written
//  permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef __EVOLUTION_CORE_VISION_OPENCV_HPP_
#define __EVOLUTION_CORE_VISION_OPENCV_HPP_

/**
 * @file   evolution/core/vision/OpenCV.hpp
 * @brief  Wraps opencv image create and free functionality.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/

#include <evolution/core/base/Config.hpp>

#include <evolution/core/resource/Image.hpp>

#ifdef EVOLUTION_PLATFORM_WIN32
#include <cxtypes.h>
#else
#if defined(OPENCV_VERSION) && (OPENCV_VERSION >= 97)
#include <opencv/cxtypes.h>
#endif
#include <opencv/cvtypes.h>
#endif

namespace Evolution 
{

    /**
     * @brief
     * Manage an OpenCV IplImage based on parameters from an Evolution::Image.
     *
     * @ingroup core_vision
     * @ingroup vision_primitives
     *
     * Wrappers for OpenCV image create/free functionality.
     *
     */

    /**
     * @brief
     * Creates the image, see details for important information on the copy parameter.
     *
     * @see evolution_opencv_image_free()
     *
     * @param   ersp_image_in [in]; The source image data
     * @param   must_free_data_out [out]; Implies a deep copy was made.
     * @param   must_copy_data [in]; Implies a deep copy of image data.
     * @param   grayscale [in]; Means whether a grayscale image is OK.
     *
     * @return  An IplImage, or NULL if something goes wrong.
     *
     * The 'must_free_data_out' Boolean should be checked and sent to an 
     * obligatory call to evolution_opencv_image_free. Even if must_copy_data
     * is set to false, an implicit conversion (say from RGB to Grayscale)
     * might force a call to cvCreateImage.
     *
     * The Camera class re-uses the Images it creates, so you must set
     * must_copy_data to true when using with camera-captured images.
     * 
     * Even if must_copy_data is set to false, cases where a grayscale image
     * are to be made from a format such as RGB will still require a complete
     * image creation, so must_free_data_out should be passed to 
     * evolution_opencv_image_free.
     *
     * @ingroup core_vision
     * @ingroup vision_primitives
     **/
    EVOLUTION_EXPORT_VISION
    IplImage *evolution_opencv_image_create   (Evolution::Image *ersp_image_in,
                                               bool             *must_free_data_out,
                                               bool              must_copy_data = true,  
                                               bool              grayscale = false);

    /**
     * @brief
     * Frees an IplImage created using the evolution_opencv_image_create call.
     *
     * @see evolution_opencv_image_create()
     *
     * @param   image [in]; A pointer to the IplImage.
     * @param   must_free_data [in]; If true, delete header and image
     *
     * This function should be called with the must_free_data_out value from
     * evolution_opencv_image_create to handle implicit data conversions and other
     * needs to free the IplImage's raw data.
     *
     * Some cases may only require the header to be deleted, such as when used
     * with a Camera-class captured Image.
     *
     * @ingroup core_vision
     * @ingroup vision_primitives
     **/
    EVOLUTION_EXPORT_VISION
    void      evolution_opencv_image_free     (IplImage         *image,
                                               bool              must_free_data);
} // end namespace Evolution

#endif
