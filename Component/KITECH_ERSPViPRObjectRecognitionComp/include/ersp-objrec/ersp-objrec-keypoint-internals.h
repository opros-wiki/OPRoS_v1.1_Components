//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-keypoint-internals.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_KEYPOINT_INTERNALS_H_
#define __ERSP_OBJREC_KEYPOINT_INTERNALS_H_

#include "ersp-objrec-keypoint.h"

ERSP_OBJREC_BEGIN_DECLS

typedef enum
{
    ERSP_OBJREC_KEYPOINT_TYPE_0,
    ERSP_OBJREC_KEYPOINT_TYPE_128,
    ERSP_OBJREC_KEYPOINT_TYPE_36,

    ERSP_OBJREC_KEYPOINT_TYPE_GENERIC   /* NOTE: must be last type */
} ERSP_ObjrecKeypointType;

/* Only will be used in experiments (TYPE_GENERIC) */
#define ERSP_OBJREC_KEYPOINT_FEATURE_VECTOR_MAX 512

/* This array MUST EXACTLY MATCH ERSP_ObjrecKeypointType.  */
static const unsigned global_feature_vector_lengths[4] =
{
    0,
    128,
    36,
    ERSP_OBJREC_KEYPOINT_FEATURE_VECTOR_MAX
};

/* Return the feature vector length */
#define ERSP_objrec_keypoint_get_feature_vector_length(keypoint) \
  ( global_feature_vector_lengths[(keypoint)->keypoint_type] )

/* Return the feature type */
#define ERSP_objrec_keypoint_get_keypoint_type(veclen) \
  ( (veclen == global_feature_vector_lengths[ERSP_OBJREC_KEYPOINT_TYPE_128]) \
     ? ERSP_OBJREC_KEYPOINT_TYPE_128 \
     : (veclen == global_feature_vector_lengths[ERSP_OBJREC_KEYPOINT_TYPE_36])\
     ? ERSP_OBJREC_KEYPOINT_TYPE_36 \
     : ERSP_OBJREC_KEYPOINT_TYPE_GENERIC )

#define ERSP_objrec_keypoint_get_feature_vector(keypoint)       \
  ((unsigned char *) (keypoint) + _ERSP_objrec_keypoint_padded_header_size)
#define ERSP_objrec_keypoint_set_mag_squared(keypoint, mag_squared) \
  do { *_ERSP_objrec_keypoint_mag_squared_ptr(keypoint) = (mag_squared); } while(0)
#define ERSP_objrec_keypoint_get_mag_squared(keypoint) \
  ((unsigned) (*_ERSP_objrec_keypoint_mag_squared_ptr(keypoint)))

#define ERSP_OBJREC_KEYPOINT_SIZEOF(keypoint_type)      \
  ( _ERSP_objrec_keypoint_padded_header_size                    \
    + (((global_feature_vector_lengths[keypoint_type])+3)&(~3)) \
    + 4 /* for mag-squared */ )

#define _ERSP_objrec_keypoint_mag_squared_ptr(keypoint) \
  ((unsigned *) (ERSP_objrec_keypoint_get_feature_vector(keypoint) \
                 + ((ERSP_objrec_keypoint_get_feature_vector_length(keypoint)+3)&(~3))))

/* Keypoint scrambling.

A scrambled keypoint is in an order such that the first N components
is more likely to predict the performance of the distance calculation
on the whole vector.

An unscrambled keypoint is in matrix order: adjacent components
are likely to be correlated.

The input and output buffers MUST NOT overlap */
void ERSP_objrec_keypoint_scramble_feature_vector (unsigned N,
                                                   const unsigned char *input,
                                                   unsigned char *output);
void ERSP_objrec_keypoint_unscramble_feature_vector (unsigned N,
                                                     const unsigned char *input,
                                                     unsigned char *output);

/* internal details (how to align the feature vector) */
#define _ERSP_objrec_keypoint_data_alignment    32
#define _ERSP_objrec_round_to_alignment(size, align)            \
  (((size) + (align) - 1) / (align) * (align))
#define _ERSP_objrec_keypoint_padded_header_size                \
  _ERSP_objrec_round_to_alignment(sizeof (ERSP_ObjrecKeypoint), \
                                  _ERSP_objrec_keypoint_data_alignment)

/* Refresh the magnitude square of the feature descriptor */
void ERSP_OBJREC_EXPORT
ERSP_objrec_keypoint_refresh_mag_squared(ERSP_ObjrecKeypoint *keypoint);

/* unsigned ERSP_objrec_keypoint_get_feature_vector_length(ERSP_ObjrecKeypoint *keypoint) */
/* { */
/*     ERSP_ObjrecKeypointType internal_type = (ERSP_ObjrecKeypointType) keypoint->keypoint_type; */
/*     return global_feature_vector_lengths[internal_type]; */
/* } */

ERSP_OBJREC_END_DECLS

#endif
