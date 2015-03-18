//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-option-tables.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


/* flexible option parser generating macros.
 *
 *
 * Define per-type tables of:
 *  - parameter name
 *  - member name
 *  - description.
 *
 * The macros ERSP_OBJREC_OPTION_USER_type()
 * for types FLOAT, INT, UINT and BOOL will be invoked
 * for each option
 */


/* XXX: missing peak-threshold (aka feature-strength) */
/* XXX: feature-quality backward! */
#define ERSP_OBJREC_OPTION_TABLE_FIND_KEYPOINTS                 \
  ERSP_OBJREC_OPTION_USER_FLOAT("max-dip", max_dip,             \
     "maximum dip (feature badness) to accept into the keypoint list") \
  ERSP_OBJREC_OPTION_USER_UINT("max-keypoints", max_keypoints,  \
     "the maximum number of keypoints to return (0 means unlimited)") \
  ERSP_OBJREC_OPTION_USER_UINT("x-index-size", x_index_size,  \
     "number of indices in the x direction") \
  ERSP_OBJREC_OPTION_USER_UINT("y-index-size", y_index_size,  \
     "number of indices in the y direction") \
  ERSP_OBJREC_OPTION_USER_UINT("ori-index-size", ori_index_size,  \
     "number of indices in the orientation dimension") \
  ERSP_OBJREC_OPTION_USER_UINT("magnification-scale-b16", mag_scale_b16,  \
     "magnification of the sampling area from feature-vector, with 16 bits of fraction") \
  ERSP_OBJREC_OPTION_USER_BOOL("upsample", upsample,            \
     "upsample the image before extraction (results in more features)") \
  ERSP_OBJREC_OPTION_USER_BOOL("subsample", subsample,            \
     "subsample the image when computing feature-vectors")      \
  ERSP_OBJREC_OPTION_USER_FLOAT("min-orientation-peak-ratio",   \
                                min_orientation_peak_ratio,     \
     "orientation local:global peak ratio acceptance threshold")\
  ERSP_OBJREC_OPTION_USER_UINT("ori-histogram-smoothing-passes",\
                               ori_histogram_smoothing_passes,  \
          "number of times to smooth the orientation histogram")\
  ERSP_OBJREC_OPTION_USER_BOOL("interpolate-orientation",       \
                               interpolate_orientation,         \
          "interpolate the orientation to sub-ori-bin accuracy") \
  ERSP_OBJREC_OPTION_USER_UINT("max-duplicate-keypoints", maximum_duplicate_keypoints, \
     "maximum number of duplicate keypoints with different orientations to use") \
  ERSP_OBJREC_OPTION_USER_FLOAT("sift-ceiling", sift_ceiling, \
     "float sift vector is ceiling'd to this value") \
  ERSP_OBJREC_OPTION_USER_FLOAT("sift-int-to-float", sift_int_to_float, \
     "scale factor for converting float sift vector to integer")

#define ERSP_OBJREC_OPTION_TABLE_FIND_NN                        \
  ERSP_OBJREC_OPTION_USER_UINT("max-check", max_check,          \
     "the maximum number of keypoints to check (per input keypoint)") \
  ERSP_OBJREC_OPTION_USER_UINT("max-dist-squared", max_dist_squared,\
     "the maximum distance a returned keypoint should be from the target keypoint in feature space")

#define ERSP_OBJREC_OPTION_TABLE_NEW_MODEL                          \
  ERSP_OBJREC_OPTION_USER_BOOL("is_external", is_external,          \
     "whether the database is external")                            \
  ERSP_OBJREC_OPTION_USER_BOOL("supports-merging", supports_merging,  \
     "whether to support merging")                                  

#define ERSP_OBJREC_OPTION_TABLE_ADD_MODEL                      \
  ERSP_OBJREC_OPTION_USER_BOOL("allow-merging", allow_merging,  \
     "whether to merge nearly identical features")              \
  ERSP_OBJREC_OPTION_USER_FLOAT("max-dip", max_dip,             \
     "maximum dip (feature badness) used for training")         \
  ERSP_OBJREC_OPTION_USER_UINT("num-neighbors", n_neighbors,  \
     "number of neighbors to lookup for each keypoint")         \


#define ERSP_OBJREC_OPTION_TABLE_MATCH_MODEL                    \
  ERSP_OBJREC_OPTION_USER_FLOAT("max-dip", max_dip,             \
     "maximum dip (feature badness) used for matching")         \
  ERSP_OBJREC_OPTION_USER_FLOAT("accept-probability", min_accept_prob,\
     "probability threshold for accepting a match")             \
  ERSP_OBJREC_OPTION_USER_BOOL("allow-multiple", allow_multiple_matches,\
     "whether to allow multiple matches to the same model")     \
  ERSP_OBJREC_OPTION_USER_BOOL("allow-model-repeats", allow_model_repeats,\
     "whether to allow the same model to be matched multiple times") \
  ERSP_OBJREC_OPTION_USER_UINT("min-keypoints", min_keypoints,  \
     "minimum number of keypoints to consider a match")         \
  ERSP_OBJREC_OPTION_USER_UINT("max-check", max_check,          \
     "maximum number of keypoints to try in nearest-neighbor lookup")  \
  ERSP_OBJREC_OPTION_USER_UINT("num-neighbors", n_neighbors,  \
     "number of neighbors to lookup for each keypoint")         \
  ERSP_OBJREC_OPTION_USER_FLOAT("max-dist-ratio", max_dist_ratio,\
     "maximum ratio of distance to the nearest neighbour")             \
  ERSP_OBJREC_OPTION_USER_FLOAT("max-dist-ratio-for-extending", max_dist_ratio_for_extending,\
     "maximum ratio of distance to the nearest neighbour when extending a match") \
  ERSP_OBJREC_OPTION_USER_FLOAT("min-condition-number", min_condition_number,\
     "minimum condition number of the affine matrix for accepting the match") \
  ERSP_OBJREC_OPTION_USER_BOOL("insert-additional-keypoints", \
                               insert_additional_keypoints,    \
                               "use additional matching keypoints")\
  ERSP_OBJREC_OPTION_USER_FLOAT("min-residual-robust-sigma", \
                                min_residual_robust_sigma,   \
                                "minimum robust sigma for residual of matching features")\
  ERSP_OBJREC_OPTION_USER_FLOAT("max-residual-robust-sigma", \
                                max_residual_robust_sigma,   \
                                "maximum robust sigma for residual of matching features")\
  ERSP_OBJREC_OPTION_USER_FLOAT("robust-outlier-multiplier-thresh", \
                                robust_outlier_multiplier_thresh,   \
                                "remove as outlier matches that exceed the residual robust sigma by this factor")\
  ERSP_OBJREC_OPTION_USER_BOOL("use-forced-residual-level", use_forced_residual_level,\
     "whether or not to force the use of a fixed value of the residual robust sigma for outlier rejection") \
  ERSP_OBJREC_OPTION_USER_FLOAT("forced-residual-level", \
                                forced_residual_level,   \
                                "a specified value of the residual robust sigma to use for outlier rejection")\
  ERSP_OBJREC_OPTION_USER_BOOL("use-single-model-databases", use_single_model_databases, \
"whether or not to use single model databases in extended matching") \
  ERSP_OBJREC_OPTION_USER_BOOL("use-nonlinear-affine-estimation", use_nonlinear_affine_estimation,\
     "whether or not to use a nonlinear affine transform estimator instead of the least squares estimator") \
  ERSP_OBJREC_OPTION_USER_FLOAT("nonlinearity-scale", \
                                nonlinearity_scale,   \
                                "defines the scale at which the nonlinearity occurs")\
          

/*   XXX: may need some of these match and add parameters...
     FLOAT_PARAM("outlier-loc-range", outlier_loc_range)
     FLOAT_PARAM("min-vec-cosine", min_vec_cosine)
     FLOAT_PARAM("bin-range", bin_range)
     FLOAT_PARAM("ori-range", ori_range)
     FLOAT_PARAM("outlier-ori", outlier_ori)
     FLOAT_PARAM("scale-range", scale_range)
     FLOAT_PARAM("location-range", location_range)
     FLOAT_PARAM("pose-prior", pose_prior)
     FLOAT_PARAM("outlier-scale", outlier_scale)
     FLOAT_PARAM("max-merge-residual", outlier_scale)   [sic]
*/


/* various other aliases, for back compability */
#define ERSP_OBJREC_OPTION_TABLE_FIND_KEYPOINTS__ALIASES        \
  ERSP_OBJREC_OPTION_USER_FLOAT("feature-quality", max_dip,     \
     "alias to max-dip")                                        \
  ERSP_OBJREC_OPTION_USER_UINT("max-num-features", max_keypoints,\
     "alias to max-keypoints")                                  \
  ERSP_OBJREC_OPTION_USER_BOOL("use-upsampling-method", upsample,\
     "alias to upsample")                                       \
  ERSP_OBJREC_OPTION_USER_BOOL("double", upsample,              \
     "alias to upsample")

#define ERSP_OBJREC_OPTION_TABLE_ADD_MODEL__ALIASES             \
  ERSP_OBJREC_OPTION_USER_FLOAT("feature-quality", max_dip,     \
     "alias to max-dip")                                        \
            
#define ERSP_OBJREC_OPTION_TABLE_MATCH_MODEL__ALIASES           \
  ERSP_OBJREC_OPTION_USER_FLOAT("feature-quality", max_dip,     \
     "alias to max-dip")                                        \
  ERSP_OBJREC_OPTION_USER_UINT("min-keys", min_keypoints,       \
     "alias to min-keypoints")                                  \
  ERSP_OBJREC_OPTION_USER_FLOAT("accept-prob", min_accept_prob, \
     "alias to accept-probability")                             \
  ERSP_OBJREC_OPTION_USER_FLOAT("detection-threshold", min_accept_prob,\
     "alias to accept-probability")                             \
  ERSP_OBJREC_OPTION_USER_FLOAT("max-dist-ratio-ext", max_dist_ratio_for_extending,\
     "alias to max-dist-ratio-for-extending")


