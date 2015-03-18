//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_VISION_FILTERS_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_FILTERS_HPP


/**
 * @file evolution/core/vision/Filters.hpp
 * @brief filters for matrices
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/


#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/vision/BlockFlow.hpp>
#include <evolution/core/vision/Correlation.hpp>
#include <evolution/core/vision/CovFilter.hpp>
#include <evolution/core/vision/Desample.hpp>
#include <evolution/core/vision/MeanFilter.hpp>
#include <evolution/core/vision/SumFilter.hpp>
#include <evolution/core/vision/StdFilter.hpp>
#include <evolution/core/vision/Unsalt.hpp>

namespace Evolution 
{

    /**
     * @brief This namespace holds various image processing and basic
     * computer vision filters for application to images represented
     * by either an Image object or a Matrix object.
     *
     * @see Image, Matrix
     *
     * @ingroup core_vision
     * @ingroup vision_primitives
     */
    namespace Filters
    {
        ERSP_LOG_NAMESPACE_CATEGORY_DECLARE("Evolution.Core.Vision.Filters", EVOLUTION_EXPORT_VISION);

        /**
         * @brief This detects differences between two frames related to motion.  
         * @param in_now [in]; time now matrix.
         * @param in_then [in]; time then matrix.
         * @param diff_thresh [in]; threshold. higher threshold is a stricter condition for finding motion.
         * @return [out]; A value that encodes the difference between frames.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         **/
        extern EVOLUTION_EXPORT_VISION
        double matrix_difference (const Matrix<double> &in_now, 
                                  const Matrix<double> &in_then, 
                                  double diff_thresh);
    } // end namespace Filters

} // end namespace Evolution

#endif // end INCLUDE_EVOLUTION_CORE_VISION_FILTERS_HPP

