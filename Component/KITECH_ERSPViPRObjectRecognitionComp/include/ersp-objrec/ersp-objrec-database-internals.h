//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-database-internals.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_DATABASE_INTERNALS_H__
#define __ERSP_OBJREC_DATABASE_INTERNALS_H__

#include "ersp-objrec-database.h"

ERSP_OBJREC_BEGIN_DECLS

/* Retrieve the feature vector length */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_database_get_feature_vector_length (ERSP_ObjrecDatabase *database);

/* --- new database --- */
struct _ERSP_ObjrecDatabaseNewOptions
{
    ersp_objrec_boolean is_external;
    ersp_objrec_boolean supports_merging;
    unsigned            feature_vector_length;
};

#define ERSP_OBJREC_DATABASE_NEW_OPTIONS_DEFAULT                \
  {                                                             \
    0,          /* !external */                                 \
    1,          /* support merging */                           \
    128         /* feature vector length */        \
  }

/* --- adding to the database --- */
struct _ERSP_ObjrecDatabaseAddOptions
{
    ersp_objrec_boolean crop_low_gradients;  /* unsupported */
    ersp_objrec_boolean allow_merging;
    float max_dip;
    float max_outlier_location_amount;
    unsigned max_check;
    unsigned log2_heap_size;
    unsigned n_neighbors;
};

#define ERSP_OBJREC_DATABASE_ADD_OPTIONS_DEFAULT    \
  {                                                 \
    0,  /* crop_low_gradients */                    \
    1,  /* allow merging */                         \
    0.95,                                           \
    0.30, /* max-outlier-location-amount */         \
    400,  /* max-check */                           \
    9,     /* log2 heap size */                      \
    2     /* number of neighbors */                \
  }

struct _ERSP_ObjrecDatabaseMatchOptions
{
    /* whether to allow keypoints to be used in more than one match. */
    ersp_objrec_boolean allow_multiple_matches;

    /* whether to allow models to be matched multiply;
       only used if allow_multiple_matches is set. */
    ersp_objrec_boolean allow_model_repeats;

    /* whether to solve for a complete affine, or just a similarity transformation */
    ersp_objrec_boolean do_solve_affine;

    /* number of neighbors to find.  ignored if !allow_multiple_matches */
    unsigned n_neighbors;

    /* minimum number of keypoints in a match */
    unsigned min_keypoints;

    /* minimum acceptance probability for a match */
    float min_accept_prob;

    /* whether to suppress the coalescing of similar matches.
       ignored if !allow_multiple_matches. */
    ersp_objrec_boolean coalesce_nearly_identical_matches;

    double max_dip;
    unsigned max_check;
    unsigned log2_heap_size;
    unsigned max_distance;

    /* maximum permission ratio between the second-best-keypoint's distance
     * to the best-keypoint's distance.  
     * A small ratio implies that the best keypoint was much better
     * that the second-best, indication that the keypoint is more likely
     * to be the right one.
     *
     * The first number controls who votes,
     * the second number controls whether to use the key in top-down
     * matching.
     */
    float max_dist_ratio;
    float max_dist_ratio_for_extending;

    /* maximum tolerable error in position, as a fraction of the projection
       of the model image */
    float max_outlier_location_amount;

    /* whether to use aadditional keypoints in a match (returns MANY matches) 
       it should be used in conjuntion with a number of nearest neighbors
       bigger than 2. */
    ersp_objrec_boolean insert_additional_keypoints;

    /* the minimum condition number for allow in an affine match
       (small condition numbers mean that the image is "squished"
       in one direction -- since the features are not invariant to such
       squishing, the keypoint-lookups are implausible and the match
       should be discarded.)

       set to 0.0 to suppress the condition number check */
    float min_condition_number;
    /* LG Jan 14 2005 
       the minimum value for the robustly estimated variance of the 
       transform residual. Avoids the outlier removal to be too picky.
       Should correspond to the average error in feature extraction/localization */
    float min_residual_robust_sigma;

    /* the maximum value for the robustly estimated variance of the
     * transform residual. Avoids finding matches that are too lousy. */
    float max_residual_robust_sigma;

    /* consider as outlier a match that has residual this many times larger
       than the estimated residual_robust_sigma */
    float robust_outlier_multiplier_thresh;

    /* whether or not to lock the residual level when looking for outliers */
    ersp_objrec_boolean use_forced_residual_level;

    /* the value of the residual level (residual_robust_sigma) to force
       outlier rejection to use */
    float forced_residual_level;

    /* wheter to do grouping (and sorting) of initial matches */
    ersp_objrec_boolean group_initial_matches;

    /* whether to use a separate, single-model DB for each initial match models to
       do the extended matching, or to build a one model with all the intial matching
       models. Separate models may give more thorough results, but will take longer 
       to process because a k-D search has to be done for each separately
    */
    ersp_objrec_boolean use_single_model_databases;

    /* whether to use a non-linear, gradient descent-based 
       estimator of the affine transform instead of a least squares estimator.
       The non-linear estimator is more robust to outliers, resulting in matches
       with more features with lower residual.*/
    ersp_objrec_boolean use_nonlinear_affine_estimation;

    /* the scale at which the nonlinearity acts.
       recall the nonlinear function is g(x) = log(1 + abs(x))*sign(x)
       scale factor is used to divide x, so that nonlinearity scaled is
       g(x) = log(1 + abs(x/scale)) * sign(x)
       scale should be postiive, non-zero.
       Scale should be set proportionally to the expected residual of a good match
       (LG TODO: what is the fudge factor, if rms residual is 2.0, what should scale be ?) */
    float nonlinearity_scale;

    /* an initial estimate of the affine transform to use 
     * with the nonlinear estimator */
    /* Note that, the affine transform is represented in objrec's internal, awkward
     * vector representation.
     *
     The conventional representation would be
     [Px  =  [Axx Axy Tx] [Mx
     Py]    [Ayx Ayy Ty]  My
     1      [ 0   0   1]   1]
     where the transform is stored as a vector as 
     [Axx Axy Tx Ayx Ayy Ty] (ie, the first two rows of the homogeneous representation.
     0   1   2  3   4   5

     Unfortunately, objrec's internal representation is
     [Py] = [Ayy Ayx]  [My] + [Ty]       (note the reversal of x and y coords ..)
     [Px]   [Axy Axx]  [Mx]   [Tx]
     where the transform is stored as a vector as
     [Ayy Ayx Axy Axx Ty Tx]
     0   1   2   3   4  5

     So store an affine transform like this :
     [Ayy Ayx Axy Axx Ty Tx]

     (Note that make_match converts to standard representation (as of Feb 9 2005))
    */
    float affine_init[6];
};

#define ERSP_OBJREC_DATABASE_MATCH_OPTIONS_DEFAULT                  \
  {                                                                 \
    0,              /* don't allow-multiple-matches */              \
    0,              /* suppress repeat matches to the same model */ \
    1,              /* do-solve-affine */                           \
    2,              /* 2 neighbors if the user allows mult matches */\
    4,              /* minimum keypoints in a match */              \
    0.95,           /* minimum acceptance probability for a match */\
    1,              /* coalesce_nearly_identical_matches */         \
    0.95            /* max-dip */,                                  \
    200,            /* max-check, for keypoint finding */           \
    9,              /* log2 heap size */                            \
    255*255*128,    /* max-distance, for keypoint finding */        \
    0.9,            /* max-dist-ratio                     */        \
    0.95,           /* max-dist-ratio-for-extending       */        \
    0.15,           /* max-outlier-location-amount */               \
    0,              /* don't use additional keypoints in matching */ \
    0.1,            /* minimum condition number for affine matches */\
    40.0,           /* minimum robust sigma for residual of matching features */\
    40.0,           /* maximum robust sigma for residual of matching features */\
    40.0,           /* robust outlier multiplier threshold*/        \
    0,              /* use_forced_residual_level */                 \
    1.0,            /* forced_residual_level */                     \
    0,              /* group_initial_matches */                     \
    0,              /* use_single_model_databases */                \
    0,              /* use_nonlinear_affine_estimation */           \
    1.0,            /* nonlinearity_scale */                        \
    {1.0, 0.0, 0.0, 1.0, 0.0, 0.0}, /* default affine transform initialization */ \
  }

#define ERSP_objrec_database_get_user_data(database, model)     \
  ERSP_objrec_model_get_user_data(model)
#define ERSP_objrec_database_get_next(database, model)  \
  ERSP_objrec_model_get_next(model)

/* --- support for external databases --- */
#include "ersp-objrec-keypoint-table.h"  /* for ERSP_ObjrecKeypointInfo !!! */

/* An external database is one where the NNTree is not within this tree.

Basically, that means you must do NN lookups to do matching,
and you must write to the NN tree on success.
   
To create an external database, pass in ERSP_OBJREC_DATABASE_FLAG_EXTERNAL
to ERSP_objrec_database_new(). */

/* Caller passes in the result of querying the nearest-neighbors from
   each keypoint in the image to recognize.
   
   The 'keypoints' do not have to have feature vectors. */
ERSP_OBJREC_EXPORT ERSP_ObjrecMatchList *
ERSP_objrec_database_external_match  (ERSP_ObjrecDatabase     *database,
                                      ERSP_ObjrecKeypointList *keypoint_list,
                                      unsigned                 neighbors_per_keypoint,
                                      ERSP_ObjrecKeypointInfo *kp_infos,
                                      const ERSP_ObjrecDatabaseMatchOptions *);

/* If merging, the caller must pass result of a lookup in the KeypointTable.
   Otherwise, all the caller must do is
   add nn_tree_data_out[0..n_keypoints-1] into the nn tree.
   
   Therefore, neighbors_per_keypoint, kp_infos, and keypoints
   are only used if you set the merging flag in the AddOptions. */
ERSP_OBJREC_EXPORT ERSP_ObjrecModel *
ERSP_objrec_database_external_add    (ERSP_ObjrecDatabase     *database,
                                      ERSP_ObjrecKeypointList *keypoint_list,
                                      unsigned                 neighbors_per_keypoint,
                                      ERSP_ObjrecKeypointInfo *kp_infos,
                                      void                    *user_data,
                                      void                   **nn_tree_data_out,
                                      ERSP_ObjrecDatabaseAddOptions *options);

#define ERSP_objrec_database_external_remove(database, model) \
        ERSP_objrec_database_remove(database, model)

/* Merge-key-list information (used to implement file loading) */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_merge_keys      (ERSP_ObjrecDatabase *database,
                                      unsigned             n_to_merge,
                                      ERSP_ObjrecKeypoint **merge_keys);
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypoint *
ERSP_objrec_database_merge_key_next  (ERSP_ObjrecDatabase *database,
                                      ERSP_ObjrecKeypoint *key);
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypoint *
ERSP_objrec_database_merge_key_prev  (ERSP_ObjrecDatabase *database,
                                      ERSP_ObjrecKeypoint *key);
ERSP_OBJREC_EXPORT unsigned
ERSP_new_merge_key_list_for_keypoint(ERSP_ObjrecDatabase *database,
                                     ERSP_ObjrecKeypoint *key,
                                     ERSP_ObjrecKeypoint **merge_list);
ERSP_OBJREC_EXPORT void
ERSP_delete_merge_key_list_for_keypoint(ERSP_ObjrecKeypoint **merge_list);


ERSP_OBJREC_EXPORT void
ERSP_objrec_database_disable_model(ERSP_ObjrecDatabase *database,
                                   ERSP_ObjrecModel    *model);

ERSP_OBJREC_EXPORT void
ERSP_objrec_database_enable_model(ERSP_ObjrecDatabase *database,
                                  ERSP_ObjrecModel    *model);

/* Set up callback function to destroy memory allocated in the
   user data for each model. */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_set_user_data_destroyer(ERSP_ObjrecDatabase *database,
                                             void (*destroyer)(void *));
ERSP_OBJREC_EXPORT ERSP_ObjrecMatchList *
ERSP_objrec_database_match_extended (ERSP_ObjrecDatabase *db, 
                                     ERSP_ObjrecKeypointList *test_keypoints, 
                                     const ERSP_ObjrecDatabaseMatchOptions *db_match_options,
                                     ERSP_ObjrecDatabaseMatchOptions *extended_match_options,
                                     ERSP_ObjrecMatchList **initial_matches_returned);

/* Computes a convex hull from the input points.  The return value is the number
 * of output points. The output points are written over the input points. Both input
 * and output points are in the form (x0,y0,x1,y1...).
*/
ERSP_OBJREC_EXPORT int 
ERSP_ObjrecComputeConvexHull(int num_points, float *input_points);


ERSP_OBJREC_END_DECLS

#endif  //__ERSP_OBJREC_DATABASE_INTERNALS_H__
