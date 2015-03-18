//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/Correlation.hpp
 * @brief Correlation filters for images and matrices.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Image 
 **/

#ifndef INCLUDE_EVOLUTION_CORE_VISION_CORRELATION_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_CORRELATION_HPP


#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Matrix.hpp>


namespace Evolution 
{

    class Image; ///< Advance declaration, no Image routines used in this file.

    namespace Filters
    {

        /**
         * @brief Return correlation of two matrices of equal size.
         * 
         * @param left [in]; One matrix to use to generate correlation
         * @param right [in]; Another matrix to use to generate correlation
         * @return Returns the correlation of the two matrices, as a Type. 
         * 
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Type correlation (const Matrix<Type> &left, 
                                 const Matrix<Type> &right)
        {
            PRECOND(left.rsize() == right.rsize() && 
                    left.csize() == right.csize());
            PRECOND(left.rsize() == right.rsize() && 
                    left.csize() == right.csize());
            Type sum = (Type) 0;
            double meanval = left.mean() * right.mean() ;
            for(unsigned i = 0; i < left.rsize(); i++)
            {
                for(unsigned j = 0; j < left.csize(); j++)
                {
                    //Sumsquare elementwise both matrices
                    sum += left[i][j]*right[i][j]; 
                }
            }
            //Divide by total area.
            sum /= (left.csize()*left.rsize());
            //Subtract off the mean value.
            sum -= meanval; 
            //Divide away deviations
            sum /= (left.std()*right.std()); 
            return sum;
        }

        /**
         * @brief Return block correlation of two matrices of equal size.
         *
         * Given to matrices, a point in the left matrix, a point in the right
         * matrix, and a width and height of a block to compute for, find the
         * correlation of the two blocks in the matrix
         *
         * @param left  [in]; One matrix to use to generate correlation
         * @param rl    [in]; Row coordinate of point in left matrix
         * @param cl    [in]; Col coordinate of point in left matrix
         * @param right [in]; Another matrix to use to generate correlation
         * @param rr    [in]; Row coordinate of point in right matrix
         * @param cr    [in]; Col coordinate of point in right matrix
         * @param h     [in]; Height of block in 2 matrices
         * @param w     [in]; Width of block in 2 matrices
         * @return Returns the correlation of the two specified blocks.
         * 
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Type correlation (const Matrix<Type> &left,  
                                 unsigned rl, unsigned cl, 
                                 const Matrix<Type> &right, 
                                 unsigned rr, unsigned cr, unsigned h, unsigned w)
        {
            //ensure bounds are proper.
            PRECOND( rl >= 0); PRECOND( (rl+h) <= left.rsize()); 
            PRECOND( rr >= 0); PRECOND( (rr+h) <= right.rsize());
            PRECOND( cl >= 0); PRECOND( (cl+w) <= left.csize());
            PRECOND( cr >= 0); PRECOND( (cr+w) <= right.csize());

            Type sum = (Type) 0;
            double meanval = left.mean(rl, cl, h, w) * right.mean(rr, cr, h, w);
            for(unsigned i = 0;i < h; i++)
            {
                for(unsigned j = 0;j < w; j++)
                {
                    //sumsquare elementwise both matrices
                    sum+=left[i+rl][j+cl]*right[i+rr][j+cr]; 
                }
            }
            //Divide by total area.
            sum/= (w*h); 
            //Subtract off the mean value.
            sum-= meanval; 
            //Divide away deviations
            sum/= (left.std(rl, cl, h, w) * right.std(rr, cr, h, w)); 
            return sum;
        } //End block correlations.

        /**
         * @brief Correlation between the r,c right.rsize,right.csize 
         *        block in left and the entirety of right.
         * 
         * Given two matrices, find the correlation between the right matrix
         * and the block of the left matrix defined at point r,c, of height
         * r.rsize(), and width r.csize()
         *
         * @param left  [in]; The large matrix (equal to or larger than right), use a sub-block to generate correlation.
         * @param right [in]; The small matrix, used to get width and height of block.
         * @param r     [in]; Row position of left at which to start block.
         * @param c     [in]; Col position of left at which to start block.
         * @return Returns the value of the defined block.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Type correlation (const Matrix<Type> &left, 
                                 const Matrix<Type> &right, 
                                 unsigned r, unsigned c)
        {
            return correlation(left, r, c, right, 0, 0, 
                               right.rsize(), right.csize());
        }

        /**
         * @brief Fast version of correlation.  
         *        need source of mean/std values for left/right matrices.
         * 
         * This works like the version correlation (matrix, unsigned, unsigned,
         * matrix, unsigned, unsigned, unsigned, unsigned) above, but it takes
         * cached values for the mean and std of the left and right block.  If
         * your correlation is over many or all blocks between two matrices,
         * this saves the cpu time needed to compute the lstd,lmean,rstd,rmean
         * each call. 
         * 
         * @param left  [in]; One matrix to use to generate correlation
         * @param rl    [in]; Row coordinate of point in left matrix
         * @param cl    [in]; Col coordinate of point in left matrix
         * @param lmean [in]; Mean value of left matrix at rl,cl, with width w and height h 
         * @param lstd  [in]; Std  value of left matrix at rl,cl, with width w and height h 
         * @param right [in]; Another matrix to use to generate correlation
         * @param rr    [in]; Row coordinate of point in right matrix
         * @param cr    [in]; Col coordinate of point in right matrix
         * @param rmean [in]; Mean value of right matrix at rr,cr, with width w and height h 
         * @param rstd  [in]; Std  value of right matrix at rr,cr, with width w and height h  
         * @param h     [in]; Height of block in 2 matrices
         * @param w     [in]; Width  of block in 2 matrices
         * @return Returns the correlation of the two specified blocks.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Type correlation_cached (const Matrix<Type> &left,  
                                        unsigned rl, unsigned cl, 
                                        double lmean, double lstd, 
                                        const Matrix<Type> &right, 
                                        unsigned rr, unsigned cr, 
                                        double rmean, double rstd, 
                                        unsigned h, unsigned w)
        {
            //ensure bounds are proper.
            PRECOND( rl >= 0); PRECOND( (rl+h) <= left.rsize()); 
            PRECOND( rr >= 0); PRECOND( (rr+h) <= right.rsize());
            PRECOND( cl >= 0); PRECOND( (cl+w) <= left.csize());
            PRECOND( cr >= 0); PRECOND( (cr+w) <= right.csize());

            Type sum = (Type) 0;
            double meanval = lmean * rmean;
            for(unsigned i = 0; i < h; i++)
            {
                for(unsigned j = 0; j < w; j++)
                {
                    //Sumsquare elementwise both matrices
                    sum+= left[i+rl][j+cl]*right[i+rr][j+cr]; 
                }
            }
            //Divide by total area.
            sum /= (w*h); 
            //Subtract off the mean value.
            sum -= meanval; 
            //Divide away deviations
            sum /= (lstd * rstd); 
            return sum;
        } //End block correlations.

        /**
         * @brief Fast version of correlation.  
         *        need source of mean/std values for left/right matrices,
         *        and the value of the area of the block.
         *
         * @param left     [in]; One matrix to use to generate correlation
         * @param rl       [in]; Row coordinate of point in left matrix
         * @param cl       [in]; Col coordinate of point in left matrix
         * @param lmean    [in]; Mean value of left matrix at rl,cl, with width w and height h 
         * @param lstd     [in]; Std  value of left matrix at rl,cl, with width w and height h 
         * @param right    [in]; Another matrix to use to generate correlation
         * @param rr       [in]; Row coordinate of point in right matrix
         * @param cr       [in]; Col coordinate of point in right matrix
         * @param rmean    [in]; Mean value of right matrix at rr,cr, with width w and height h 
         * @param rstd     [in]; Std  value of right matrix at rr,cr, with width w and height h  
         * @param h        [in]; Height of block in 2 matrices
         * @param w        [in]; Width  of block in 2 matrices
         * @param new_area [in]; Area of the block (just double(h * w))
         * @return  Returns the correlation of the two specified blocks.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Type correlation_cached (const Matrix<Type> &left,  
                                        unsigned rl, unsigned cl, 
                                        double lmean, double lstd, 
                                        const Matrix<Type> &right, 
                                        unsigned rr, unsigned cr, 
                                        double rmean, double rstd, 
                                        unsigned h, unsigned w, double new_area)
        {
            //Ensure bounds are proper.
            PRECOND( rl >= 0); PRECOND( (rl+h) <= left.rsize()); 
            PRECOND( rr >= 0); PRECOND( (rr+h) <= right.rsize());
            PRECOND( cl >= 0); PRECOND( (cl+w) <= left.csize());
            PRECOND( cr >= 0); PRECOND( (cr+w) <= right.csize());

            Type sum = (Type) 0;
            for(unsigned i = 0; i < h; i++)
            {
                for(unsigned j = 0; j < w; j++)
                {
                    //Sumsquare elementwise both matrices
                    sum+= left[i+rl][j+cl]*right[i+rr][j+cr]; 
                }
            }
            //Divide by total area.
            sum /= (new_area); 
            //Subtract off the mean value.
            sum -= lmean * rmean; 
            //Divide away deviations
            sum /= (lstd * rstd); 
            
            return sum;
        } //End block correlations.

        /**
         * @brief Fast version of block correlation, used for entire matrices at a time.  
         * 
         * This function takes two matrices, left and right.  It then looks at
         * each block of h,w in left (at position i,j, say) and the
         * corresponding block in right.  It computes the correlation of the
         * two blocks against each other, and places the resulting value in
         * dest_matrix[i][j]
         * 
         * @param left  [in]; Left matrix to be used for correlation computation.
         * @param lmean [in]; Mean values of left, in blocks (h,w), with value for i,jth block at i,j. not padded
         * @param lstd  [in]; Std  values of left, in blocks (h,w), with value for i,jth block at i,j. not padded
         * @param right [in]; Right matrix to be used for correlation computation.
         * @param rmean [in]; Mean values of right, in blocks (h,w), with value for i,jth block at i,j. not padded 
         * @param rstd  [in]; Std  values of right, in blocks (h,w), with value for i,jth block at i,j. not padded 
         * @param h     [in]; Height of blocks to use 
         * @param w     [in]; Width of blocks to use
         * @param dest_matrix [out]; the output correlation matrix.
         * @return Returns a result indicating success or a failure condition. 
         * 
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type> 
        inline Result correlation_cached (const Matrix<Type> &left,  
                                          const Matrix<Type> &lmean, 
                                          const Matrix<Type> &lstd, 
                                          const Matrix<Type> &right, 
                                          const Matrix<Type> &rmean, 
                                          const Matrix<Type> &rstd, 
                                          unsigned h, unsigned w, 
                                          Matrix<Type> * dest_matrix)
        {
            PRECOND_RETURN_VALUE (dest_matrix, RESULT_PRECONDITION_ERROR);
            Matrix<Type> & dest = *dest_matrix;
            double new_area = (double) h * w;

            if(dest.rsize() != left.rsize()-h || 
               dest.csize() != left.csize()-w)
            {
                dest.zeros(left.rsize()-h, left.csize()-w);
            }

            double t1=0;//, t2=0;
            for(unsigned i = 0; i < left.rsize()-h ; i++)
            {
                for(unsigned j = 0; j < left.csize()-w ; j++)
                {
                    if(lstd[i][j] * rstd[i][j] > .0000000001)
                    { 
                        t1 = correlation_cached(left, i, j, 
                                                lmean[i][j], 
                                                lstd[i][j],
                                                right, i, j, 
                                                rmean[i][j], 
                                                rstd[i][j],
                                                h, w, new_area);
                        //t2 = correlation (left, i, j, right, i, j, h, w);
                        //ERSP_LOG_DEBUG ("i:%d j:%d t1:%f t2:%f lm:%f lmc:%f ls:%f lsc:%f rm:%f rmc:%f rs:%f rsc:%f",
                        //i, j, t1, t2, lmean[i][j], left.mean(i, j, h, w), 
                        //lstd[i][j], left.std(i, j, h, w), rmean[i][j], 
                        //right.mean(i, j, h, w), rstd[i][j], right.std(i, j, h, w));
                        dest[i][j] = t1;
                    }
                    else
                    {
                        dest[i][j] = 0.0; // @todo Not enough stddev!
                    }
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
         * @param left   [in]; Left source image. data taken from here.
         * @param right   [in]; Right source image. data taken from here.
         * @param w [in]; column (width) size of blocks
         * @param h [in]; row (height) size of blocks
         * @param output  [out]; output matrix
         * @param lchannel [in]; Optional.  Giving a value reads a specific channel of left for data, defaults to grayscale version of image.
         * @param rchannel [in]; Optional.  Giving a value reads a specific channel of right for data, defaults to grayscale version of image.
         * @return Result encoding success or failure condition.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        Result EVOLUTION_EXPORT_VISION correlation(const Image &left, const Image &right,
                                                   int w, int h,
                                                   Matrix<double> *output,
                                                   int lchannel=-1, int rchannel=-1);

    } // end namespace Filters

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_VISION_CORRELATION_HPP
