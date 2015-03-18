//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/Unsalt.hpp
 * @brief Unsalt function filter for Images and Matricies.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/
#ifndef INCLUDE_EVOLUTION_CORE_VISION_UNSALT_FILTER_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_UNSALT_FILTER_HPP


#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/vision/SumFilter.hpp>

namespace Evolution 
{
    class Image;

    namespace Filters
    {

        /**
         * @brief This sets dest to the unsalted version of source.
         * @note For the salt_threshold, 17 is *often* a good value.
         *
         * This function attempts to get rid of 'salt' noise, random high
         * intensity pixels that are very different from background.  It does
         * this by checking median values vs. groups of pixels to find pixels
         * that are too large.
         *
         * @param source_in [in]; data to unsalt
         * @param rsize [in]; rowsize of blocks
         * @param csize [in]; colsize of blocks
         * @param salt_threshold [in]; Value used in thresholding salt.
         * @param dest_matrix [out]; matrix less salt.
         * @param constant [in]; Optional.  A larger constant is more selective.
         * @ingroup core_vision
         * @ingroup vision_primitives
         **/
        template <class Type> 
        inline Result unsalt_filter (const Matrix<Type> &source_in, 
                                     unsigned rsize, unsigned csize, 
                                     double salt_threshold, 
                                     Matrix<Type> * dest_matrix, 
                                     double constant=3.0)
        {
            if(dest_matrix == NULL)
            {
                return RESULT_PRECONDITION_ERROR;
            }
            Matrix<double> source = source_in;

            unsigned wid=9, hei=9;
            
            double median = source.median ();
            double threshold = median * constant;
            source.threshold (threshold, 0, 1);

            Matrix<double> source_unsalted;
            Filters::sum_filter_padded (source, wid, hei, &source_unsalted);
            source_unsalted.threshold(salt_threshold, 0, 1);

            for (unsigned i = 0; i < source.rsize(); i++)
            {
                for (unsigned j = 0; j < source.csize(); j++)
                {
                    source_unsalted[i][j] = source_unsalted[i][j] * source[i][j];
                }
            }
            
            *dest_matrix = source_unsalted;
            return RESULT_SUCCESS;
        }

        /**
         * @brief This sets dest to the unsalted version of source.
         * @note For the salt_threshold, 17 is *often* a good value.
         *
         * This function attempts to get rid of 'salt' noise, random high
         * intensity pixels that are very different from background.  It does
         * this by checking median values vs. groups of pixels to find pixels
         * that are too large.
         *
         * @param source [in]; data to unsalt
         * @param rsize [in]; rowsize of blocks
         * @param csize [in]; colsize of blocks
         * @param salt_threshold [in]; Value used in thresholding salt.
         * @param dest [out]; source Image less salt pixels.
         * @param constant [in]; Optional.  A larger constant is more selective.
         * @ingroup core_vision
         * @ingroup vision_primitives
         **/
        EVOLUTION_EXPORT_VISION
        Result unsalt_filter (const Image source, unsigned rsize, unsigned csize, double salt_threshold,
                              Image * dest, double constant=3.0);
    } // end namespace Filters

} // end namespace Evolution
#endif // INCLUDE_EVOLUTION_CORE_VISION_UNSALT_FILTER_HPP

