//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/CovFilter.hpp
 * @brief Covariance filters for images and matrices.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/
#ifndef INCLUDE_EVOLUTION_CORE_VISION_COV_FILTER_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_COV_FILTER_HPP


#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Matrix.hpp>

namespace Evolution 
{
    class Image;

    namespace Filters
    {

        /**
         * @brief Find all blocks of rsize,csize in source, compute their covariance, output results in dest_matrix
         *
         * Iterate through source. find each block of size (rsize,csize).  Each
         * block will be rooted at i,j.  Compute the covariance for each block,
         * and place the result in dest_matrix[i][j].
         *
         * @param source  [in]; Source matrix. data taken from here.
         * @param rsize   [in]; Row (height) size of blocks
         * @param csize   [in]; Column (width) size of blocks
         * @param dest_matrix [out]; Output matrix, where resultant covariances are stored.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result cov_filter (const Matrix<Type> &source, 
                                  unsigned rsize, unsigned csize, 
                                  Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;
            if(dest.rsize() != source.rsize() || 
               dest.csize() != source.csize())
            {
                dest.fast_resize(source.rsize()-rsize+1, source.csize()-csize+1);
            }

            for(unsigned i = 0; i < source.rsize()-rsize ; i++)
            {
                for(unsigned j = 0; j < source.csize()-csize ; j++)
                {
                    dest[i][j] = source.cov(i, j, rsize, csize); 
                }
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief As above, with cached source mean values.
         *
         * Iterate through source. Find each block of size (rsize,csize).  Each
         * block will be rooted at i,j.  Compute the covariance for each block,
         * and place the result in dest_matrix[i][j].  While computing the
         * covariance of each block, instead of computing the mean for a given
         * block, use the i,j value in source_mean, to save cpu cycles.
         *
         * This function is most useful when the source_mean is cached across multiple calls to this function.
         *
         * @param source  [in]; Source matrix. Data taken from here.
         * @param source_mean  [in]; Matrix encoding mean value of rsize,csize blocks in source.
         * @param rsize   [in]; Row (height) size of blocks.
         * @param csize   [in]; Column (width) size of blocks.
         * @param dest_matrix [out]; Output matrix, where resultant covariances are stored.
         * @return Result indicating success or failure.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result cov_filter (const Matrix<Type> &source, 
                                  const Matrix<Type> &source_mean, 
                                  unsigned rsize, unsigned csize, 
                                  Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;
            double area = (double) rsize * csize;

            if(dest.rsize() != source.rsize() || 
               dest.csize() != source.csize())
            {
                dest.fast_resize(source.rsize()-rsize+1, source.csize()-csize+1);
            }

            for(unsigned i = 0; i < source.rsize()-rsize ; i++)
            {
                for(unsigned j = 0; j < source.csize()-csize ; j++)
                {
                    dest[i][j] = source.cov(i, j, rsize, csize, 
                                            source_mean[i][j], area); 
                }
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief Finds the point by point covariance in rsize by
         * csize blocks, and puts it in output.
         * 
         * @see CovFilter::cov_filter
         * 
         * @param input   [in]; Input data for generating covariance matrices.
         * @param rsize   [in]; Row (height) size of blocks.
         * @param csize   [in]; Column (width) size of blocks.
         * @param output [out]; Output matrix, where resultant covariances are stored.
         * @param channel [out]; defaults to using grayscale of image.  Otherwise, takes the specified channel.
         * @return Result indicating success or failure.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        Result EVOLUTION_EXPORT_VISION image_cov_filter(const Image &input,
                                                        int rsize, int csize, 
                                                        Matrix<double> *output, int channel=-1);
    } // end namespace Filters

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_VISION_COV_FILTER_HPP
