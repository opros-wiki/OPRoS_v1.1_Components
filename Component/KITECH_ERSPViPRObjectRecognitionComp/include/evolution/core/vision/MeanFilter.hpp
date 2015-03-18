//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/MeanFilter.hpp
 * @brief mean filters for images and matrices.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/
#ifndef INCLUDE_EVOLUTION_CORE_VISION_MEAN_FILTER_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_MEAN_FILTER_HPP


#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Matrix.hpp>

namespace Evolution 
{
    class Image;

    namespace Filters
    {

        /**
         * @brief Find all blocks of rsize,csize in source, compute their mean, output results in dest_matrix
         *
         * Iterate through source. find each block of size (rsize,csize).  Each
         * block will be rooted at i,j.  Compute the mean of each block,
         * and place the result in dest_matrix[i][j].
         *
         * @param source  [in]; Source matrix. Data taken from here.
         * @param rsize   [in]; Row (height) size of blocks.
         * @param csize   [in]; Column (width) size of blocks.
         * @param dest_matrix [out]; Output matrix, where resultant means are stored.
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result mean_filter (const Matrix<Type> &source, 
                                   int rsize, int csize, 
                                   Matrix<Type> * dest_matrix)
        {
            int x=0, y=0;
            double temp0=0, temp1=0;
            double *oldvec = NULL;
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;

            dest.zeros(source.rsize(), source.csize());//-rsize+1);
            for(y = 0;(unsigned)y < dest.rsize(); y++) //First column. Sum rows.
            { 
                dest[y][0] = source.sum(y,0,1,csize);
            }

            dest.rsize();

            double safe_val = 0;
            ASSERT(dest.csize() + 1 > (unsigned)csize);
            unsigned col_max = dest.csize() + 1 - (unsigned)csize ; //Should always be valid unsigned subtract at this point.
            for(x = 1;(unsigned)x < col_max; x++) 
            { //build colum 2 -> column max. build additively. bounds check.
                y=0;
                for( y = 0; (unsigned)y < dest.rsize(); y++) 
                {
                    if(x-1 < 0 )
                    {
                        safe_val = 0;
                    }
                    else
                    {
                        safe_val = dest[y][x-1];
                    }
                    dest[y][x] = safe_val - source[y][x-1] + source[y][x+csize-1];
                    //ERSP_LOG_INFO ("x:%d y:%d e:%f l:%f sub:%f add:%f %d", 
                    //x, y, dest[y][x], safe_val , 
                    //source[y][x-1] , 
                    //source[y][x+csize-1], 
                    //x+csize-1);
                }
                y=0;
            }
        
            //Preserve the precondense values for the top row.
            oldvec = new Type[source.csize()];
            //We now have a matrix where dest[y][x] = source.sum_vector(x,y,csize,1);
            //To make dest[y][x] = source.sum_vector(x,y,csize,rsize), for each element,
            //dest[y0][x0] = dest[y0][x0-1] 

            for(x = 0;(unsigned) x < source.csize(); x++) 
            {
                oldvec[x] = dest[0][x];
                dest[0][x] = dest.sum(0,x,rsize,1); //sum_vector down, create first row.
            }
            //_log.warn("Construct row two->max");
            for(x = 0;(unsigned)x < dest.csize(); x++) 
            {
                temp1 = oldvec[x];
                for(y = 1; (unsigned)y < dest.rsize() + 1 - rsize; y++) 
                {
                    temp0 = dest[y][x];
                    if((unsigned)y+rsize-1 >= dest.rsize())
                    {
                        safe_val = 0;
                    }
                    else
                    {
                        safe_val = dest[y+rsize-1][x]; 
                    }
                    //dest[y][x] = dest[y-1][x] - temp1 + elem_bounded(x,y+rsize-1); 
                    dest[y][x] = dest[y-1][x] - temp1 + safe_val;
                    //ERSP_LOG_DEBUG ("x:%d y:%d e:%f e-1:%f t0:%f t1sub:%f add:%f max:%i source.rsize():%i rsize:%i %f=(%f - %f + %f)", 
                    //x, y, dest[y][x], dest[y-1][x], 
                    //temp0, temp1, safe_val, 
                    //source.rsize()-rsize+1, 
                    //source.rsize(), rsize, 
                    //dest[y][x], dest[y-1][x], 
                    //temp1, safe_val);
                    temp1 = temp0;
                }
            }
            delete[] oldvec;
            double area = rsize * csize;
            int rbound = dest.rsize() + 1 - rsize;
            int cbound = dest.csize() + 1 - csize;
            for(x = 0; x < cbound; x++) 
            {
                for(y = 0; y < rbound; y++) 
                {
                    dest[y][x] /= area;
                }
            }
            dest.resize(dest.rsize() + 1 - rsize, dest.csize() + 1 - csize);
       
            return RESULT_SUCCESS;
        }

        /**
         * @brief Find all blocks of rsize,csize in source, compute their mean, output results in dest_matrix
         *
         * Iterate through source. Find each block of size (rsize,csize).  Each
         * block will be rooted at i-rsize/2,j-csize/2.  Compute the mean of
         * each block, padding the outside edges of the matrix with zeros, and
         * place the result in dest_matrix[i][j].
         *
         * @param source  [in]; Source matrix. data taken from here.
         * @param rsize   [in]; Row (height) size of blocks.
         * @param csize   [in]; Column (width) size of blocks.
         * @param dest_matrix [out]; Output matrix, where resultant means are stored.
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result mean_filter_padded (const Matrix<Type> &source, 
                                          int rsize, int csize, 
                                          Matrix<Type> * dest_matrix)
        {
            int x=0, y=0;
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;
            Matrix<Type>   temp;

            dest.zeros(source.rsize(), source.csize());//-rsize+1);
            temp.zeros(source.rsize(), source.csize());//-rsize+1);

            for(y = 0;(unsigned)y < dest.rsize(); y++) //first column. sum_padded rows.
            { 
                temp[y][0] = source.sum_padded(y,0,1,csize);
                //ERSP_LOG_DEBUG ("x:%d y:%d e:%f", 0, y, dest[y][0]);
            }

//@todo Ranges out of bounds give zero, not 0th element.
            double safe_val = 0;
            double sub = 0;
            double add = 0;
            for(x = 1;(unsigned)x < dest.csize(); x++) 
            { //build colum 2 -> column max. build additively. bounds check.
                for(y = 0 ; (unsigned)y < dest.rsize(); y++) 
                {
                    //base value
                    safe_val = ( x-1 < 0 )                      ? 0 : temp[y][x-1];
                    //sub value                                            
                    sub =      ( x-csize/2-1 < 0)                 ? 0 : source[y][x-csize/2-1];
                    //add value                                         
                    add =      ( (unsigned)x+csize/2 >= source.csize()) ? 0 : source[y][x+csize/2];
                    //dest[y][x] = safe_val - source[y][x-csize/2] + source[y][x+csize/2+1];
                    temp[y][x] = safe_val - sub + add;
                    //ERSP_LOG_DEBUG ("x:%d y:%d e:%f l:%f sub:%f add:%f %d", 
                    //x, y, temp[y][x], safe_val , 
                    //source[y][x-1] , 
                    //source[y][x+csize-1], 
                    //x+csize-1);
                }
            }
                                                                             
            //dest = temp;
                                                                                              
            for(x = 0; (unsigned)x < source.csize(); x++) 
            {
                dest[0][x] = temp.sum_padded(0,x,rsize,1); //sum_vector down, create first row.
                //ERSP_LOG_DEBUG ("x:%d y:%d e:%f", x, 0, temp[0][x]);
            }
                                                                                              
            //_log.warn("Construct row two->max");
            for(x = 0;(unsigned)x < dest.csize() ; x++) 
            {
                for(y = 1; (unsigned)y < dest.rsize() ; y++) 
                {
                    //base value
                    safe_val = ( y-1 < 0 )                        ? 0 : dest[y-1][x];
                    //sub value                                            
                    sub =      ( y-rsize/2-1 < 0)                 ? 0 : temp[y-rsize/2-1][x];
                    //add value                                         
                    add =      ( (unsigned)y+rsize/2 >= source.rsize())   ? 0 : temp[y+rsize/2][x];
                    dest[y][x] = safe_val - sub + add;
                }
            }
                                                                                                   
            double area = rsize * csize;
            for(x = 0; (unsigned)x < dest.csize() ; x++) 
            {
                for(y = 0; (unsigned)y < dest.rsize() ; y++) 
                {
                    dest[y][x] /= area;
                }
            }

            return RESULT_SUCCESS;
        }

        /**
         * @brief Find all blocks of rsize,csize in input, compute their mean, output results in dest_matrix
         *
         * Iterate through input. Find each block of size (rsize,csize).  Each
         * block will be rooted at i-rsize/2,j-csize/2.  Compute the mean of
         * each block, padding the outside edges of the matrix with zeros, and
         * place the result in dest_matrix[i][j].
         *
         * @param input       [in]; Source image. data taken from here.
         * @param rsize       [in]; Row (height) size of blocks.
         * @param csize       [in]; Column (width) size of blocks.
         * @param output [out]; Output matrix, where resultant means are stored.
         * @param channel     [in]; Optional.  Giving a value reads a specific channel for data, defaults to grayscale version of image.
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        EVOLUTION_EXPORT_VISION
        Result image_mean_filter(const Image &input,
                                 int rsize, int csize, 
                                 Matrix<double> *output, int channel=-1);

    } // end namespace Filters

} // end namespace Evolution
#endif // INCLUDE_EVOLUTION_CORE_VISION_MEAN_FILTER_HPP

