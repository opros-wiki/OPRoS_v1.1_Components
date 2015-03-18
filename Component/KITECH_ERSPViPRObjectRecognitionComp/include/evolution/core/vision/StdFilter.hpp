//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/StdFilter.hpp
 * @brief standard deviation filters for images and matrices.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/
#ifndef INCLUDE_EVOLUTION_CORE_VISION_STD_FILTER_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_STD_FILTER_HPP


#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Matrix.hpp>


namespace Evolution 
{

    class Image;

    namespace Filters
    {

        /**
         * @brief This finds all blocks of rsize, csize in source then computes 
         * their standard deviations and  outputs results in dest_matrix.
         *
         * @note This iterates through source, finding each block of size (rsize,csize).  Each
         * block will be rooted at i,j.  Next, it computes the standard deviations for each block,
         * and places the result in dest_matrix[i][j].
         *
         * @param source  [in]; Source matrix wher data is taken from
         * @param rsize   [in]; Row (height) size of blocks
         * @param csize   [in]; Column (width) size of blocks
         * @param dest_matrix [out]; Output matrix, where resultant standard deviations are stored
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result std_filter (const Matrix<Type> &source, 
                                  unsigned rsize, unsigned csize, 
                                  Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;
            if(dest.rsize()-rsize != source.rsize() || 
               dest.csize()-csize != source.csize())
            {
                dest.fast_resize(source.rsize()-rsize+1, source.csize()-csize+1);
            }

            for(unsigned i=0; i< source.rsize()-rsize ; i++)
            {
                for(unsigned j=0; j< source.csize()-csize ; j++)
                {
                    dest[i][j] = source.std(i, j, rsize, csize); 
                }
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief As above, with cached source mean values.
         *
         * @note This iterates through source, finding each block of size (rsize,csize).  Each
         * block will be rooted at i,j.  Next, it computes the standard deviations for each block,
         * and places the result in dest_matrix[i][j].  While computing the
         * standard deviations of each block, instead of computing the mean for a given
         * block, it uses the i,j value in source_mean, to save cpu cycles.
         *
         * This function is most useful when the source_mean is cached across multiple calls to this function.
         *
         * @param source      [in]; Source matrix where data is taken from
         * @param source_mean [in]; Matrix encoding mean value of rsize,csize blocks in source
         * @param rsize       [in]; Row (height) size of blocks
         * @param csize       [in]; Column (width) size of blocks
         * @param dest_matrix [out]; Output matrix where resultant standard deviations are stored
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result std_filter (const Matrix<Type> &source, 
                                  const Matrix<Type> &source_mean, 
                                  unsigned rsize, unsigned csize, 
                                  Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;
            double area = (double) rsize * csize;

            if(dest.rsize()-rsize != source.rsize() || 
               dest.csize()-csize != source.csize())
            {
                dest.fast_resize(source.rsize()-rsize+1, source.csize()-csize+1);
            }

            for(unsigned i = 0; i < source.rsize()-rsize ; i++)
            {
                for(unsigned j = 0; j < source.csize()-csize ; j++)
                {
                    dest[i][j] = source.std(i, j, rsize, csize, 
                                            source_mean[i][j], area); 
                }
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief This finds all blocks of rsize,csize in source and compute their
         * standard deviation, outputting results in dest_matrix.
         *
         * @note This iterates through source, finding each block of size (rsize,csize).  Each
         * block will be rooted at i-rsize/2,j-csize/2. Next, it compute the standard
         * deviation of each block, padding the outside edges of the matrix
         * with zeros, and places the result in dest_matrix[i][j].  The mean of
         * each block will be obtained from source_mean[i][j], a matrix
         * encoding similar mean values for the standard deviation values
         * computed in this function.
         *
         * @param source  [in]; Source matrix. data taken from here.
         * @param source_mean  [in]; Mean matrix, generated by padding the data
         *                 matrix outside with zeros, and running an rsize x
         *                 csize mean filter over it
         * @param rsize   [in]; row (height) size of blocks
         * @param csize   [in]; column (width) size of blocks
         * @param dest_matrix [out]; output matrix
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result std_filter_padded (const Matrix<Type> &source, 
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
                dest.fast_resize(source.rsize(), source.csize());
            }

            for(unsigned i = 0; i < source.rsize() ; i++)
            {
                for(unsigned j = 0; j < source.csize() ; j++)
                {
                    dest[i][j] = source.std_padded(i, j, rsize, csize, 
                                                   source_mean[i][j], area); 
                }
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief This finds all blocks of rsize,csize in input and compute their
         * standard deviation, outputting results in dest_matrix.
         *
         * @note This iterates through input, finding each block of size (rsize,csize).  Each
         * block will be rooted at i-rsize/2,j-csize/2. Next, it compute the standard
         * deviation of each block, padding the outside edges of the matrix
         * with zeros, and places the result in dest_matrix[i][j].  The mean of
         * each block will be obtained from input[i][j], a matrix
         * encoding similar mean values for the standard deviation values
         * computed in this function.
         *
         * @param input   [in]; Source image. data taken from here.
         * @param rsize   [in]; row (height) size of blocks
         * @param csize   [in]; column (width) size of blocks
         * @param output  [out]; output matrix
         * @param channel [in]; Optional.  Giving a value reads a specific channel for data, defaults to grayscale version of image.
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        EVOLUTION_EXPORT_VISION
        Result image_std_filter(const Image &input,
                                int rsize, int csize, 
                                Matrix<double> *output, int channel=-1);

    } // end namespace Filters

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_VISION_STD_FILTER_HPP
