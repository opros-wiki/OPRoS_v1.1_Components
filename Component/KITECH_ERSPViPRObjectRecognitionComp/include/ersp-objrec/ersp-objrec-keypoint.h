//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-keypoint.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_KEYPOINT_H_
#define __ERSP_OBJREC_KEYPOINT_H_


/* This structure describes a keypoint that has been found in an image.  */
#include "ersp-objrec-common.h"
#include "ersp-objrec-lock.h"
ERSP_OBJREC_BEGIN_DECLS

typedef struct _ERSP_ObjrecKeypoint ERSP_ObjrecKeypoint;

struct _ERSP_ObjrecKeypoint
{
    /* Number of referers to this feature vector. */
    unsigned ref_count;

    /* Position of the keypoint. */
    float x, y;

    /* Pixel location of keypoint (in the image the feature was
       found in; y=orig_y*scale and x=orig_x*scale.) */
    int orig_x, orig_y;

    /* Scale (relative to input image). */
    float scale;

    /* Orientation in radians (-PI to PI). */
    float ori;

    /* Orientation in bins */
    unsigned char ori_bin;

    /* Which type of keypoint this is. Stored as unsigned char for
     * saving space */
    unsigned char keypoint_type;

    /* maximum ratio of gradient of peak with its surrounding pixels */
    float dip;
};

/* Allocate a keypoint of a given type (default type = 1 -- other types 
are unsupported) */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypoint *ERSP_objrec_keypoint_alloc (unsigned type);

/* Free keypoint memory */
ERSP_OBJREC_EXPORT void            _ERSP_objrec_keypoint_free  (ERSP_ObjrecKeypoint *);

/* Compute distances between the keypoint descriptors */
ERSP_OBJREC_EXPORT unsigned        ERSP_objrec_keypoints_distance (const ERSP_ObjrecKeypoint *a,
                                                                   const ERSP_ObjrecKeypoint *b);

#ifdef _WIN32
ERSP_OBJREC_EXPORT ERSP_ObjrecLock keypoint_ref_lock;
#else
extern ERSP_ObjrecLock keypoint_ref_lock;
#endif

#define ERSP_OBJREC_LOG_REF_COUNT 0

#ifndef ERSP_OBJREC_LOG_REF_COUNT
#define ERSP_OBJREC_LOG_REF_COUNT 1
#endif

#if ERSP_OBJREC_LOG_REF_COUNT
#define ERSP_objrec_keypoint_ref(keypoint) \
    (                                      \
        (keypoint)->ref_count++,                                        \
        fprintf( stderr, "logged_add_ref %p, after ref_count = %d [%s:%d]\n", \
                 (keypoint), (keypoint)->ref_count, __FILE__, __LINE__ ), \
        keypoint \
        )

#define ERSP_objrec_keypoint_unref(keypoint) \
    { \
        fprintf( stderr, "logged_remove_ref %p, before ref_count = %d [%s:%d]\n", \
                 (keypoint), (keypoint)->ref_count, __FILE__, __LINE__ ); \
        if (--(keypoint)->ref_count == 0)                              \
            _ERSP_objrec_keypoint_free (keypoint); \
    }
#else // ERSP_OBJREC_LOG_REF_COUNT

#if ERSP_OBJREC_INLINE_KEYPOINT_REF

/* Increase the reference counter for the the keypoint */
ERSP_OBJREC_INLINE ERSP_ObjrecKeypoint *
ERSP_objrec_keypoint_ref (ERSP_ObjrecKeypoint *keypoint)
{
    ERSP_objrec_lock(&keypoint_ref_lock);
    keypoint->ref_count++;
    ERSP_objrec_unlock(&keypoint_ref_lock);
    return keypoint;
}
/* Decrease the reference counter for the the keypoint. If the reference 
counter is zero, the keypoint will be deallocated.*/
ERSP_OBJREC_INLINE void
ERSP_objrec_keypoint_unref (ERSP_ObjrecKeypoint *keypoint)
{
ERSP_objrec_lock(&keypoint_ref_lock);
    if (--keypoint->ref_count == 0)
        _ERSP_objrec_keypoint_free (keypoint);
ERSP_objrec_unlock(&keypoint_ref_lock);
}
#else /* !ERSP_OBJREC_INLINE_KEYPOINT_REF */
#define ERSP_objrec_keypoint_unref _ERSP_objrec_keypoint_unref
#define ERSP_objrec_keypoint_ref _ERSP_objrec_keypoint_ref
#endif

#endif // ERSP_OBJREC_LOG_REF_COUNT

void _ERSP_objrec_keypoint_unref (ERSP_ObjrecKeypoint *keypoint);
ERSP_ObjrecKeypoint *_ERSP_objrec_keypoint_ref (ERSP_ObjrecKeypoint *keypoint);


ERSP_OBJREC_END_DECLS

#endif
