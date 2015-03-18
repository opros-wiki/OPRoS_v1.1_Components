//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/Desample.hpp
 * @brief Resampling filters for matrices.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/
#ifndef INCLUDE_EVOLUTION_CORE_VISION_DESAMPLE_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_DESAMPLE_HPP


#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Matrix.hpp>

namespace Evolution 
{
    namespace Filters
    {

        /**
         * @brief This sets variable dest to be a downsampled version of the source variable.
         * 
         * @param source      [in]; source data for downsampling.
         * @param rfactor     [in]; factor to downsample by, rows.
         * @param cfactor     [in]; factor to downsample by, cols.
         * @param dest_matrix [in]; output, downsampled matrix.
         * @return Result indicating success or failure.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result desample (const Matrix<Type> &source, 
                                unsigned rfactor, unsigned cfactor, 
                                Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_INVALID_ARGUMENT);
            if(cfactor == 1 && rfactor == 1)
            {
                *dest_matrix = source;
                return RESULT_SUCCESS;
            }
            unsigned oc_iter = (unsigned)( source.csize ()/ (double)cfactor); 
            unsigned or_iter = (unsigned)( source.rsize()/ (double)rfactor);
            double new_area = (double)(cfactor * rfactor);
            Matrix<Type> &dest = *dest_matrix;
            dest.zeros(or_iter, oc_iter);

            for(unsigned i = 0 ; i < or_iter ; i++)
            {
                for(unsigned j = 0 ; j < oc_iter ; j++)
                {
                    for(unsigned rs = (rfactor*i) ; rs < (rfactor*(i+1)) ; rs++)
                    {        
                        for(unsigned is = (cfactor*j) ; is < (cfactor*(j+1)) ; is++)
                        {
                            dest[i][j] += source[rs][is];
                        }
                    }
                    dest[i][j] /= new_area;
                }
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief This sets dest to an upsampled version of the matrix.
         * 
         * @param source      [in]; source data for upsampling.
         * @param rfactor     [in]; factor to upsample by, rows.
         * @param cfactor     [in]; factor to upsample by, cols.
         * @param dest_matrix [in]; output, upsampled matrix.
         * @return Result indicating success or failure.
         * 
         * @ingroup vision_primitives
         * @ingroup core_vision
         */
        template <class Type> 
        inline Result upsample (const Matrix<Type> &source, 
                                unsigned rfactor, unsigned cfactor, 
                                Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_INVALID_ARGUMENT);
            if(cfactor == 1 && rfactor == 1)
            {
                *dest_matrix = source;
                return RESULT_SUCCESS;
            }
            unsigned oc_iter=source.csize()*cfactor;
            unsigned or_iter=source.rsize()*rfactor; //output xl/yl
            Matrix<Type> &dest = *dest_matrix;

            dest.zeros(or_iter, oc_iter);
            for(unsigned i = 0; i < source.rsize(); i++)
            {
                for(unsigned j = 0; j < source.csize(); j++)
                { 
                    for(unsigned rs = 0; rs < rfactor; rs++)
                    {        
                        for(unsigned is = 0; is < cfactor; is++)
                        {
                            dest[i*rfactor+rs][j*cfactor+is] = source[i][j];
                        }
                    }
                }
            }

            return RESULT_SUCCESS; 
        }

    } // end namespace Filters

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_VISION_DESAMPLE_HPP

