//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file evolution/core/vision/BlockFlow.hpp
 * @brief Block flow algorithm for images.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/

#ifndef INCLUDE_EVOLUTION_CORE_VISION_BLOCK_FLOW_HPP
#define INCLUDE_EVOLUTION_CORE_VISION_BLOCK_FLOW_HPP

#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/Vector2.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/resource/Image.hpp>
#include <evolution/core/util/Profiler.hpp>
#include <evolution/core/vision/Correlation.hpp>
#include <evolution/core/vision/MeanFilter.hpp>
#include <evolution/core/vision/StdFilter.hpp>
#include <evolution/core/vision/Desample.hpp>


namespace Evolution
{

    class Image;

    namespace Filters
    {


//Turning profiling on results in a fair bit of logging output, and a fairly
//large slowdown.  Only useful for controlled performance testing.
//#define BLOCK_FLOW_PROFILE 1
#ifdef BLOCK_FLOW_PROFILE
        static Profiler block_flow_prof;

        INIT_PROFILE(block_flow_func,   "block flow function    ");
        INIT_PROFILE(init_rout,         "initialization routines");
        INIT_PROFILE(const_diff_image,  "construct diff image   ");
        INIT_PROFILE(diff_image_std,    "compute diff image cov ");
        INIT_PROFILE(const_vfield,      "construct vfield       ");
        INIT_PROFILE(last_sum_matrix,   "last sum matrix        ");
        INIT_PROFILE(now_sum_matrix,    "now  sum matrix        ");
        INIT_PROFILE(last_cov_matrix,   "last cov matrix        ");
        INIT_PROFILE(now_cov_matrix,    "now  cov matrix        ");
#endif

        /**
         * @brief Wrapper function for block_flow(Matrix,Matrix) to make it take Images.
         * 
         * The flow is computed from last to now.  Blocks of block_r by block_c
         * are compared against all similar blocks for expansion steps to try
         * to decide if that block moved, and if so, how far, and how closely
         * correlated the movement is with the original block.  The outputs are
         * expressed as a matrix of Vector2s, and a matrix of doubles. The
         * Matrix<Vector2> expresses how far each block moved in two
         * dimensions.  The double matrix (dest_correlation) expresses the
         * correlation between the best match block and the original block.
         * The various optional arguments allow one to pass in cached data
         * values, to save cpu cycles in computing the block flow.
         *
         *   last             now          block_r = 2   
         * 1 1 0 0 0 0     0 1 1 0 0 0     block_c = 2       
         * 1 1 0 0 0 0     0 1 1 0 0 0     desample_r = 1        
         * 0 0 0 0 0 0     0 0 0 0 0 0     desample_c = 1        
         * 0 0 0 0 0 0     0 0 0 0 0 0     expansion = 3        
         * 0 0 0 0 0 0     0 0 0 0 0 0             
         * 0 0 0 0 0 0     0 0 0 0 0 0             
         *
         * These inputs would result in:
         * dest_vector:         dest_correlation:
         * (1,0) (0,0) (0,0)    1 0 0
         * (0,0) (0,0) (0,0)    0 0 0
         * (0,0) (0,0) (0,0)    0 0 0
         *
         * @param last           [in]; Matrix encoding last frame of video
         * @param now            [in]; Matrix encoding this frame of video
         * @param block_r        [in]; Row size of blocks used to find flow
         * @param block_c        [in]; Col size of blocks used to find flow
         * @param desample_r     [in]; Row desample size, used to shrink matrix for faster processing.
         * @param desample_c     [in]; Col desample size, used to shrink matrix for faster processing.
         * @param expansion      [in]; Size of expansion area to search for similar blocks, for finding flow.
         *
         * @param diff_threshold [in]; Threshold: covariance of diff image for a block must be > diff_threshold, or ignored
         * @param dest_vector      [out]; Matrix of vectors that encodes block flow for each block_r by block_c block
         * @param dest_correlation [out]; Correlation of the output vectors, used for confidence testing.
         *
         * @param last_mean_in   [inout]; Cache value. When passed in, should
         *    be last frames mean matrix, block_r by block_c.  When finished, it
         *    is set to now_in's mean matrix.  Optional.  Function will compute
         *    the values if not provided.
         * @param last_cov_in    [inout]; Cache value. As last_mean_in, with covariance.
         * @param now_mean_in    [in]; Cache value. As last_mean_in, but no value is stored after completion.
         * @param now_cov_in     [in]; Cache value. As last_cov_in, but no value is stored after completion.
         *
         * @param diff_image_out [out]; Function writes the difference image of last/now matrices here if non-NULL.
         * @param diff_cov_out   [out]; Function writes the covariance of the difference image of last/now matrices here if non-NULL.
         * @param dthr_bool      [out]; If true, forces comparisons of blocks, regardless of diff image covariance.  
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         * 
         */
        Result EVOLUTION_EXPORT_VISION block_flow(const Image &last, const Image &now,
                                                  unsigned block_r,
                                                  unsigned block_c,  //Size of blocks to test flow against
                                                  unsigned desample_r,
                                                  unsigned desample_c, //Amount to desample matrices by
                                                  unsigned expansion,                  //Amount to expand search area for blocks by
                                                  double diff_threshold,          //cov of diff image to exceed or ignore
                                                  Matrix<Vector2> * dest_vector,
                                                  Matrix<double> * dest_correlation, //Output vector field and correlation matrix
                                                  Matrix<double> * last_mean_in = NULL,
                                                  Matrix<double> * last_cov_in = NULL,
                                                  Matrix<double> *  now_mean_in = NULL,
                                                  Matrix<double> *  now_cov_in = NULL, //@todo These are...oddnesses.  if lastmean/cov are passed in non-null,
                                                  Matrix<double> *  diff_image_out = NULL, // Computed difference image
                                                  Matrix<double> *  diff_cov_out   = NULL,
                                                  bool dthr_bool = false); //   Computed diff image covariance

        /**
         * @brief Computes the block flow matrix between the two input matrices, with optimizations and data caching..
         *
         * The flow is computed from last to now.  Blocks of block_r by block_c
         * are compared against all similar blocks for expansion steps to try
         * to decide if that block moved, and if so, how far, and how closely
         * correlated the movement is with the original block.  The outputs are
         * expressed as a matrix of Vector2s, and a matrix of doubles. The
         * Matrix<Vector2> expresses how far each block moved in two
         * dimensions.  The double matrix (dest_correlation) expresses the
         * correlation between the best match block and the original block.
         * The various optional arguments allow one to pass in cached data
         * values, to save cpu cycles in computing the block flow.
         *
         * @param last_in        [in]; Matrix encoding last frame of video
         * @param now_in         [in]; Matrix encoding this frame of video
         * @param block_r        [in]; Row size of blocks used to find flow
         * @param block_c        [in]; Col size of blocks used to find flow
         * @param desample_r     [in]; Row desample size, used to shrink matrix for faster processing.
         * @param desample_c     [in]; Col desample size, used to shrink matrix for faster processing.
         * @param expansion      [in]; Size of expansion area to search for similar blocks, for finding flow.
         *
         * @param diff_threshold [in]; Threshold: covariance of diff image for a block must be > diff_threshold, or ignored
         * @param dest_vector      [out]; Matrix of vectors that encodes block flow for each block_r by block_c block
         * @param dest_correlation [out]; Correlation of the output vectors, used for confidence testing.
         *
         * @param last_mean_in   [inout]; Cache value. When passed in, should
         *    be last frames mean matrix, block_r by block_c.  When finished, it
         *    is set to now_in's mean matrix.  Optional.  Function will compute
         *    the values if not provided.
         * @param last_cov_in    [inout]; Cache value. As last_mean_in, with covariance.
         * @param now_mean_in    [in]; Cache value. As last_mean_in, but no value is stored after completion.
         * @param now_cov_in     [in]; Cache value. As last_cov_in, but no value is stored after completion.
         *
         * @param diff_image_out [out]; Function writes the difference image of last/now matrices here if non-NULL.
         * @param diff_cov_out   [out]; Function writes the covariance of the difference image of last/now matrices here if non-NULL.
         * @param dthr_bool      [out]; If true, forces comparisons of blocks, regardless of diff image covariance.  
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type>
        inline Result block_flow (const Matrix<Type> &last_in,
                                  const Matrix<Type> &now_in, //Flow is from last to now.
                                  unsigned block_r,
                                  unsigned block_c,  //Size of blocks to test flow against
                                  unsigned desample_r,
                                  unsigned desample_c, //Amount to desample matrices by
                                  unsigned expansion,                  //Amount to expand search area for blocks by
                                  double diff_threshold,          //cov of diff image to exceed or ignore
                                  Matrix<Vector2> * dest_vector,
                                  Matrix<double> * dest_correlation, //Output vector field and correlation matrix
                                  Matrix<double> * last_mean_in = NULL,
                                  Matrix<double> * last_cov_in = NULL,
                                  Matrix<double> *  now_mean_in = NULL,
                                  Matrix<double> *  now_cov_in = NULL, //@todo these are...oddnesses.  if lastmean/cov are passed in non-null,
                                  Matrix<double> *  diff_image_out = NULL, // Computed difference image
                                  Matrix<double> *  diff_cov_out   = NULL,
                                  bool dthr_bool = false) //   Computed diff image covariance
            //they'll be replaced by now's mean/cov matrix at the end of
            //the computation.  now mean/cov won't be altered.
        {
            unsigned i=0, j=0;
            Result result = RESULT_SUCCESS;

#ifdef BLOCK_FLOW_PROFILE
            START_PROFILE(block_flow_prof, block_flow_func );
            START_PROFILE(block_flow_prof, init_rout );
#endif
            PRECOND_RETURN_VALUE (last_in.rsize() == now_in.rsize(),
                                  RESULT_PRECONDITION_ERROR);
            PRECOND_RETURN_VALUE (last_in.csize() == now_in.csize(),
                                  RESULT_PRECONDITION_ERROR);
            PRECOND_RETURN_VALUE (dest_vector && dest_correlation,
                                  RESULT_PRECONDITION_ERROR);

            //Calculate size needed for dest_vector and dest_correlation
            Matrix<Vector2> & vfield = *dest_vector;       //Vector field
            Matrix<double>  & cfield = *dest_correlation;  //Correlation field

            //@todo desampling!

            Matrix<Type> last;
            Matrix<Type> now;
            if(desample_r > 0 && desample_c > 0)
            {
                desample(last_in, desample_r, desample_c, &last);
                desample(now_in,  desample_r, desample_c, &now);
            }


            //Size output appropriately
            vfield.fast_resize(last.rsize()/block_r,
                               last.csize()/block_c);
            cfield.fast_resize(last.rsize()/block_r,
                               last.csize()/block_c);
#ifdef BLOCK_FLOW_PROFILE
            END_PROFILE(block_flow_prof, init_rout);
#endif

            //Declare variables. All will be read from cache (passed in) or generated.
            Matrix<Type> diff_image;
            Matrix<double>  diff_cov;
            Matrix<Type> now_mean;
            Matrix<Type> now_cov;
            Matrix<Type> last_mean;
            Matrix<Type> last_cov;

            //Construct difference image.
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, const_diff_image );
#endif
                if(diff_image_out != NULL)
                {
                    diff_image = now_in;
                    diff_image -= last_in;
                    *diff_image_out = diff_image;
                }
                else
                {
                    diff_image = now_in;
                    diff_image -= last_in;
                }
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, const_diff_image);
#endif
            }

            //Construct difference image cov
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, diff_image_std );
#endif
                diff_cov.fast_resize(cfield.rsize(), cfield.csize());
                for(i = 0 ; i < vfield.rsize() ; i++)
                {
                    for(j = 0 ; j < vfield.csize() ; j++)
                    {
                        //Not faster to cache. Disparate blocks only.
                        diff_cov[i][j] = diff_image.mean(i*block_r*desample_r,
                                                         j*block_c*desample_c,
                                                         block_r*desample_r,
                                                         block_c*desample_c);
                    }
                }
                if(diff_cov_out != NULL)
                {
                    *diff_cov_out = diff_cov;
                }

#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, diff_image_std);
#endif
            }

            //Construct mean matrix of last
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, last_sum_matrix );
#endif
                if(!last_mean_in || !(last_mean_in->rsize() == (last.rsize()-block_r+1) &&
                                      last_mean_in->csize() == (last.csize()-block_c+1)))
                {
                    result |= mean_filter (last, block_r, block_c, &last_mean);
                }
                else
                {
                    last_mean = *last_mean_in;
                }
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, last_sum_matrix);
#endif
            }

            //Construct mean matrix of now
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, now_sum_matrix );
#endif
                if( !now_mean_in || !(now_mean_in->rsize() == (now.rsize()-block_r+1) &&
                                      now_mean_in->csize() == (now.csize()-block_c+1)))
                {
                    result |= mean_filter (now, block_r, block_c, &now_mean);
                }
                else
                {
                    now_mean = *now_mean_in;
                }
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, now_sum_matrix);
#endif
            }

            //Construct cov matrix of last
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, last_cov_matrix );
#endif
                if(last_cov_in &&  ((last_cov_in->rsize() + block_r == (last.rsize() )) &&  //subtraction move to other
                                    (last_cov_in->csize() + block_c == (last.csize() ))))   //side
                {
                    last_cov = *last_cov_in;
                }
                else
                {
                    result |= std_filter (last, last_mean, block_r, block_c, &last_cov);
                }
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, last_cov_matrix);
#endif
            }

            //Construct cov matrix of now
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, now_cov_matrix );
#endif
                if( !now_cov_in || !(now_cov_in->rsize() == (now.rsize()-block_r+1) &&
                                     now_cov_in->csize() == (now.csize()-block_c+1)))
                {
                    result |= std_filter (now, now_mean, block_r, block_c, &now_cov);  //cache, production
                }
                else
                {
                    now_cov = *now_cov_in;
                }
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, now_cov_matrix);
#endif
            }


            //Construct field vectors
            Result inner_result = RESULT_SUCCESS;
            {
#ifdef BLOCK_FLOW_PROFILE
                START_PROFILE(block_flow_prof, const_vfield );
#endif
                //Vector2 tvector;
                int r = 0, c = 0; //These are NOT unsigned ints.
                unsigned counter=0;
                unsigned lasti=0, lastj=0;
                double correlation=0;
                for(i = 0 ; i < vfield.rsize() ; i++)
                {
                    for(j = 0 ; j < vfield.csize() ; j++)
                    {
                        if((fabs(diff_cov[i][j]) > diff_threshold) || dthr_bool)
                        {
                            inner_result = block_flow_point ( //Check below for this function
                                last,      now,      // Flow is from last to now
                                last_mean, now_mean, // Mean mats of above
                                last_cov,  now_cov,  // cov  mats of above
                                i, j,                // Row/index to search from
                                block_r, block_c, //Size of blocks to test flow against
                                expansion, //Amount to expand search area for blocks by
                                &r, &c, // Row, col of the best match
                                &correlation);

                            cfield[i][j] = correlation;  // Correlation of the best match
                            vfield[i][j][0] = c;
                            vfield[i][j][1] = r;
                            lasti = r;
                            lastj = c;
                            counter ++ ;
                        }
                        else //vfield _NOT_ initialized to a known state to reduce cpu cycles.
                        {
                            vfield[i][j][0] = 0.0;
                            vfield[i][j][1] = 0.0;
                            cfield[i][j]    = 0.0;
                        }
                    }
                }
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, const_vfield);
#endif
            }

            //Output profiler things.
            {
#ifdef BLOCK_FLOW_PROFILE
                END_PROFILE(block_flow_prof, block_flow_func);
                String reportings;
                block_flow_prof.report(reportings);
                std::cerr << reportings << std::endl;
#endif
            }

            if(now_mean_in) //Save some cpu next cycle. Don't allocate if not there.
            {
                (*now_mean_in) = now_mean;
            }
            if(now_cov_in)
            {
                (*now_cov_in) = now_cov;
            }
            if(last_mean_in) //Save some cpu next cycle.
            {
                (*last_mean_in) = last_mean;
            }
            if(last_cov_in)
            {
                (*last_cov_in) = last_cov;
            }

            return (result | inner_result);
        }

        /**
         * @brief Helper function for block_flow, above.
         *
         * Finds the best match for a given block with block r/c and expansion parameters.
         *
         * @param last           [in]; Matrix encoding last frame of video
         * @param now            [in]; Matrix encoding this frame of video
         * @param last_mean      [in]; Cache value. When passed in, should
         * @param last_cov       [in]; Cache value. As last_mean_in, with covariance.
         * @param now_mean       [in]; Cache value. As last_mean_in, but no value is stored after completion.
         * @param now_cov        [in]; Cache value. As last_cov_in, but no value is stored after completion.
         *                       
         * @param r              [in]; row index
         * @param c              [in]; col index
         *                       
         * @param block_r        [in]; row block size
         * @param block_c        [in]; col block size
         * @param expansion      [in]; area to search for good matches.
         * @param r_match        [out]; best row match
         * @param c_match        [out]; best col match
         * @param correlation_in [out]; correlation of the best match.
         *
         * @ingroup core_vision
         * @ingroup vision_primitives
         */
        template <class Type>
        inline Result block_flow_point (const Matrix<Type> &last,
                                        const Matrix<Type> &now,  // Flow is from last to now.
                                        const Matrix<Type> &last_mean,
                                        const Matrix<Type> &now_mean, // Mean mats of above
                                        const Matrix<Type> &last_cov,
                                        const Matrix<Type> &now_cov,  // cov  mats of above
                                        unsigned r, unsigned c, // Row/index to search from.
                                        unsigned block_r, unsigned block_c, //Size of blocks to test flow against.
                                        unsigned expansion, //Amount to expand search area for blocks by.
                                        int *r_match, int *c_match, // row, col of the best match
                                        double *correlation_in) // Correlation_in of the best match
        {
            //variables
            double max_corr=0, tcorr=0, new_area = block_r * block_c;
            int bestr=0, bestc=0;

            unsigned rowadj = r * block_r;
            unsigned coladj = c * block_c;

            if(( rowadj + block_r + 1) > last.rsize () )
            {
                rowadj = last.rsize () - block_r - 1;  //Safe for unsigned subtract
            }
            if(( coladj + block_c + 1 ) > last.csize () )
            {
                coladj = last.csize () - block_c - 1; //Safe for unsigned subtract
            }

            int botr = (signed)rowadj - (signed)expansion;
            int topr = (signed)rowadj + (signed)expansion + (signed)block_r;
            int botc = (signed)coladj - (signed)expansion;
            int topc = (signed)coladj + (signed)expansion + (signed)block_c;

            //bounds check
            if( botr < 0 )
            {
                botr = 0;
            }
            if( botc < 0 )
            {
                botc = 0;
            }
            if( (topr + block_r + 1) > last.rsize () )
            {
                topr = last.rsize () - block_r - 1; //Large enough, subtract ok.
            }
            if( (topc + block_c + 1) > last.csize () )
            {
                topc = last.csize () - block_c - 1; //Large enough, subtract ok.
            }



            //Compute
            int i=0, j=0;
            for(i = botr ; i < topr ; i++)
            {
                for(j = botc ; j < topc ; j++)
                {
                    tcorr = correlation_cached (last, rowadj, coladj, last_mean[rowadj][coladj], last_cov[rowadj][coladj],
                                                now , i, j, now_mean[i][j],  now_cov[i][j],
                                                block_r, block_c, new_area);
                    if(tcorr > max_corr)
                    {
                        bestr = i - (signed)rowadj;
                        bestc = j - (signed)coladj;
                        max_corr = tcorr;
                    }
                }
            }
            *correlation_in = max_corr;
            *r_match = bestr;
            *c_match = bestc;

            return RESULT_SUCCESS;
        }
    } // end namespace Filters

} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_VISION_BLOCK_FLOW_HPP
