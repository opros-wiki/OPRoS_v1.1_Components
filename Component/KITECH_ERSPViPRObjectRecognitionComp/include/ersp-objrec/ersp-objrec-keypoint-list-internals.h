//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-keypoint-list-internals.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_KEYPOINT_LIST_INTERNALS_H_
#define __ERSP_OBJREC_KEYPOINT_LIST_INTERNALS_H_

#include "ersp-objrec-keypoint-internals.h"
#include "ersp-objrec-keypoint-list.h"

ERSP_OBJREC_BEGIN_DECLS

/* Return the feature vector length */
#define ERSP_objrec_keypoint_list_get_feature_vector_length(list) \
  ( global_feature_vector_lengths[(list)->keypoint_type] )

/* feature-finding */
/* This is the function that get called internally by the two 
   keypoint_list_find functions defined in the public API */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList *
ERSP_objrec_keypoint_list_find_b15    (unsigned        width,
                                       unsigned        height,
                                       const short    *image_data,
                                       int             rowstride,
                                       const ERSP_ObjrecFindKeyOptions *options);

struct _ERSP_ObjrecFindKeyOptions
{
    /* whether to upsample */
    ersp_objrec_boolean upsample;

    /* maximum dip of feature to accept */
    double   max_dip;

    /* 0 means "no limit" */
    unsigned max_keypoints;

    /* minimum feature brightness. */
    unsigned peak_threshold;

    /* whether to subsample when extracting the feature-vector */
    ersp_objrec_boolean subsample;

    /* whether to do interpolation in scale space */
    ersp_objrec_boolean do_scale_interpolation;

    /* the feature vector is ori_size * index_x_size * index_y_size */
    unsigned x_index_size;
    unsigned y_index_size;
    unsigned ori_index_size;

    /* reduction factor when moving into bins */
    unsigned mag_scale_b16;

    /* orientation peak acceptance threshold: the minimum ratio between
       the global maximum orientation histogram bin
       and the local maximum orientation histogram bin
       for an orientation to be considered.

       Setting this lower causes more keypoints to be created. */
    float min_orientation_peak_ratio;

    /* number of times to smooth the orientation histogram.
       smoothing allows for better  orientation interpolation,
       as well as suppressing bad orientation maxima. */
    unsigned ori_histogram_smoothing_passes;

    /* whether to find sub-bin keypoint orientation by
       doing parabolic interpolation of the histogram peak. */
    ersp_objrec_boolean interpolate_orientation;

    /* Maximum number of duplicate orientations to use for a given keypoint. */
    unsigned maximum_duplicate_keypoints;

    /* Ceiling for computing sift vector. */
    float sift_ceiling;

    /* Scale factor for converting float sift vectors to integer. */
    float sift_int_to_float;

    /* the four sets of filter coefficients.
       the first index is 0 for the first blur
       of the pyramid and 1 for all subsequent levels.
     
       the second index is 0 for the horizontal blur
       and 1 for the vertical blur.  */
    unsigned short filter_coefficients[2][2][4];


};
#define ERSP_OBJREC_FIND_KEY_DEFAULT_COEFFICIENTS       \
        { 1966, 6881, 14549, 18743 }

#define ERSP_OBJREC_FIND_KEY_OPTIONS_DEFAULTS                 \
        {       1,      /* upsample */                        \
                1.0,    /* max_dip */                         \
                0,      /* max_keypoints */                   \
                327,    /* peak_threshold */                  \
                0,      /* subsample */                       \
                1,      /* do_scale_interpolation */          \
                4,      /* x_index_size */                    \
                4,      /* y_index_size */                    \
                8,      /* ori_index_size */                  \
                4<<16,  /* mag_scale_b16 */                   \
                0.7,    /* min_orientation_peak_ratio */\
                2,      /* histogram smoothing passes */\
                1,      /* interpolate keypoint ori */  \
                3,      /* maximum duplicate orientations */ \
                0.2,    /* sift ceiling */                    \
                512,    /* sift int-to-float */               \
           { { ERSP_OBJREC_FIND_KEY_DEFAULT_COEFFICIENTS,     \
               ERSP_OBJREC_FIND_KEY_DEFAULT_COEFFICIENTS },   \
             { ERSP_OBJREC_FIND_KEY_DEFAULT_COEFFICIENTS,     \
               ERSP_OBJREC_FIND_KEY_DEFAULT_COEFFICIENTS } }  \
        }


#if 0
/* These predicate routines need to be tested before including.  The polygon routine
   does not work.  */
/* --- KeypointCreationPredicates:
   selecting a keypoint subset before the feature-extraction phase --- */

typedef struct _ERSP_ObjrecKeypointCreationPredicate ObjrecKeypointCreationPredicate;
typedef struct ERSP_ObjrecKeypointCreationPredicate ERSP_ObjrecKeypointCreationPredicate;


ERSP_ObjrecKeypointCreationPredicate *
ERSP_objrec_keypoint_creation_predicate_new_polygon_union (unsigned n_polygons,
                                                           const unsigned *polygon_nsides,
                                                           double *xy_pairs);
/* 'op' may be OR, AND, NOT, XOR. */
ERSP_ObjrecKeypointCreationPredicate *
ERSP_objrec_keypoint_creation_predicate_new_boolean       (const char *op,
                                                           ERSP_ObjrecKeypointCreationPredicate *a,
                                                           ERSP_ObjrecKeypointCreationPredicate *b);

/* Predicate returns TRUE for:
   A * x*x + B * x*y + C * y*y + D * x + E * y + F <= 0 */
ERSP_ObjrecKeypointCreationPredicate *
ERSP_objrec_keypoint_creation_predicate_new_quadric       (double A,
                                                           double B,
                                                           double C,
                                                           double D,
                                                           double E,
                                                           double F);
void
ERSP_objrec_keypoint_creation_predicate_destroy (ERSP_ObjrecKeypointCreationPredicate *predicate);

/* Make a circle, by transforming
   (x - x0)^2 + (y - y0)^2 <= radius^2
   into the quadric form above. */
#define ERSP_objrec_keypoint_creation_predicate_new_circle(x0,y0,radius) \
  ERSP_objrec_keypoint_creation_predicate_new_quadric(1, 0, 1, -2.0 * (x0), -2.0 * (y0), \
                                                      (x0)*(x0) + (y0)*(y0) + (radius)*(radius))
#endif

/* For initializing the blur coefficients. */
ERSP_OBJREC_EXPORT void
ERSP_objrec_compute_gaussian_coeffients_symmetric7 (double width,
                                                    unsigned short *coeffs_out);

#define ERSP_objrec_find_key_options_init_gaussians(options, first_h, first_v, \
                                                    subsequent_h, subsequent_v)\
  do {                                                             \
   _ERSP_OBJREC_FIND_KEY_INIT_FILTER(options, 0, 0, first_h);      \
   _ERSP_OBJREC_FIND_KEY_INIT_FILTER(options, 0, 1, first_v);      \
   _ERSP_OBJREC_FIND_KEY_INIT_FILTER(options, 1, 0, subsequent_h); \
   _ERSP_OBJREC_FIND_KEY_INIT_FILTER(options, 1, 1, subsequent_v); \
  }while(0)

/* Only for internal interface users */
ERSP_OBJREC_EXPORT void
ERSP_objrec_key_init (void);

ERSP_OBJREC_END_DECLS

/* internal use only */
#define _ERSP_OBJREC_FIND_KEY_INIT_FILTER(options, isfirst, isv, wid)   \
  ERSP_objrec_compute_gaussian_coeffients_symmetric7(                   \
          (options)->filter_coefficients[isfirst][isv],                 \
          (wid)                                                         \
  )

#endif
