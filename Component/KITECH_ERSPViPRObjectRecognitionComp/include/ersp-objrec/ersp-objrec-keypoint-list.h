//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-keypoint-list.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_KEYPOINT_LIST_H_
#define __ERSP_OBJREC_KEYPOINT_LIST_H_


typedef struct _ERSP_ObjrecKeypointListNode ERSP_ObjrecKeypointListNode;
typedef struct _ERSP_ObjrecKeypointList ERSP_ObjrecKeypointList;
typedef struct _ERSP_ObjrecFindKeyOptions ERSP_ObjrecFindKeyOptions;

#include "ersp-objrec-keypoint.h"

ERSP_OBJREC_BEGIN_DECLS

struct _ERSP_ObjrecKeypointListNode
{
    ERSP_ObjrecKeypointListNode *prev;
    ERSP_ObjrecKeypointListNode *next;
    ERSP_ObjrecKeypoint         *keypoint;
};

/* note: the keypoints are sorted by ERSP_objrec_keypoint_list_finish()
   into pointer-order, ie node->keypoint < node->next->keypoint. */
struct _ERSP_ObjrecKeypointList
{
    unsigned                ref_count;
    unsigned                width;
    unsigned                height;
    unsigned                count;
    unsigned char           keypoint_type;
    ERSP_ObjrecKeypointListNode *first;
    ERSP_ObjrecKeypointListNode *last;
};

/* Create a new, empty keypoint list */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList *
ERSP_objrec_keypoint_list_new      (unsigned       width,
                                    unsigned       height,
                                    unsigned       keypoint_type);

/* Create a new, empty  keypoint list of a given size */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList *
ERSP_objrec_keypoint_list_new_with_uninitialized
(unsigned       count,
 unsigned       width,
 unsigned       height,
 unsigned       keypoint_type);

/* Append a keypoint to a keypoint-list */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointListNode *
ERSP_objrec_keypoint_list_append   (ERSP_ObjrecKeypointList *,
                                    ERSP_ObjrecKeypoint *);

/* Finish a list. WARNING: a list must be finished before it can be used */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_list_finish (ERSP_ObjrecKeypointList *);

/* Check if a keypoint-list has been finished */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_keypoint_list_is_finished (ERSP_ObjrecKeypointList *);

/* Get the size of the keypoint list */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_list_get_size(ERSP_ObjrecKeypointList *);

/* Add a reference to the keypoint list */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_list_ref (ERSP_ObjrecKeypointList *);

/* Remove a reference to the keypoint list. If the reference count decreases 
   to zero, then the list will be removed from memory. */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_list_unref (ERSP_ObjrecKeypointList *);


/**
   Keypoints extraction functions 
**/

/* Create a find-keypoints options structure */
ERSP_OBJREC_EXPORT ERSP_ObjrecFindKeyOptions *
ERSP_objrec_find_key_options_new (void);

/* Destroy find-keypoints options structure */
ERSP_OBJREC_EXPORT void
ERSP_objrec_find_key_options_destroy (ERSP_ObjrecFindKeyOptions *options);

/* Set find-keypoints option values */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_find_key_set_option (ERSP_ObjrecFindKeyOptions *options, 
                                 const char * option_name, 
                                 ...);

/* Get find-keypoints option values */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_find_key_get_option (const ERSP_ObjrecFindKeyOptions *options, 
                                 const char * option_name, 
                                 void * value);

/* Compute the keypoints of a grayscale image with 8 bit per pixel */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList *
ERSP_objrec_keypoint_list_find_b8     (unsigned        width,
                                       unsigned        height,
                                       const unsigned char  *image_data,
                                       int             rowstride,
                                       const ERSP_ObjrecFindKeyOptions *options);

/* Compute the keypoints of a color RGB image with 8 bit per color channel */ 
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList *
ERSP_objrec_keypoint_list_find_b8_rgb (unsigned        width,
                                       unsigned        height,
                                       const unsigned char  *image_data,
                                       int             rowstride,
                                       const ERSP_ObjrecFindKeyOptions *options);

/* Returns a new keypoint list with only those points inside the defined polygon. */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList * 
ERSP_objrec_keypoint_list_select_polygon(ERSP_ObjrecKeypointList *list,
                                         int num_vertices, double *xy_points);

/* Deletes keypoints in the list that are inside the polygon.  Return value is the
 * number of keypoints deleted. */
ERSP_OBJREC_EXPORT int 
ERSP_objrec_keypoint_list_delete_in_polygon(ERSP_ObjrecKeypointList *list,
                                            int num_vertices, double *xy_points);

ERSP_OBJREC_END_DECLS

#endif
