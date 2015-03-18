//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-keypoint-table.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_KEYPOINT_TABLE_H_
#define __ERSP_OBJREC_KEYPOINT_TABLE_H_

#include "ersp-objrec-keypoint.h"

ERSP_OBJREC_BEGIN_DECLS

typedef struct _ERSP_ObjrecKeypointTable ERSP_ObjrecKeypointTable;
typedef struct _ERSP_ObjrecKeypointInfo ERSP_ObjrecKeypointInfo;
typedef struct _ERSP_ObjrecKeypointLookupOptions ERSP_ObjrecKeypointLookupOptions;
typedef struct _ERSP_ObjrecKeypointTableOptions ERSP_ObjrecKeypointTableOptions;
struct _ERSP_ObjrecKeypointInfo
{
    ERSP_ObjrecKeypoint *keypoint;
    unsigned             dist_squared;
    void                *user_data;
};

/* Create keypoint table options */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointTableOptions *
ERSP_objrec_keypoint_table_options_new (void);

/* Destroy keypoint table options */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_table_options_destroy (ERSP_ObjrecKeypointTableOptions *options);

/* Create a keypoint table */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointTable *
ERSP_objrec_keypoint_table_new    (const ERSP_ObjrecKeypointTableOptions *options,
                                   void *db);

/* Create keypoint lookup options */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointLookupOptions *
ERSP_objrec_keypoint_lookup_options_new (void);

/* Destroy keypoint lookup options */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_lookup_options_destroy (ERSP_ObjrecKeypointLookupOptions *options);

/* Insert keypoint in keypoint table */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_table_insert (ERSP_ObjrecKeypointTable *table,
                                   ERSP_ObjrecKeypoint      *keypoint,
                                   void                     *user_data);
typedef ersp_objrec_boolean
(*ERSP_ObjrecKeypointTableTest)   (ERSP_ObjrecKeypoint      *keypoint,
                                   void                     *user_data,
                                   void                     *test_data);
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_table_remove_pattern
(ERSP_ObjrecKeypointTable *table,
 ERSP_ObjrecKeypoint      *keypoint,
 ERSP_ObjrecKeypointTableTest test_func,
 void                     *test_data);

/* Remove keypoint from table */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_table_remove (ERSP_ObjrecKeypointTable *table,
                                   ERSP_ObjrecKeypoint      *keypoint,
                                   void                     *user_data);

/* Look-up table with a keypoint for the closest max_results keypoints.*/
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_table_lookup (ERSP_ObjrecKeypointTable *table,
                                   ERSP_ObjrecKeypoint      *search,
                                   unsigned             max_results,
                                   ERSP_ObjrecKeypointInfo  *results,
                                   const ERSP_ObjrecKeypointLookupOptions*);



/* Look-up table for an exact match */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_table_lookup_exact(ERSP_ObjrecKeypointTable *table,
					ERSP_ObjrecKeypoint      *search,
					unsigned             max_results,
					ERSP_ObjrecKeypointInfo  *results);

/* Retrieve the number of keypoints in the table */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_table_count  (ERSP_ObjrecKeypointTable *table);

/* Destroy keypoint table */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_table_destroy(ERSP_ObjrecKeypointTable *table);

/* Forces a re-organization of the table */
ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_table_maybe_refresh (ERSP_ObjrecKeypointTable *);

/* Retrieve the feature vector lenght of the keypoints in the table */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_keypoint_table_get_feature_vector_length (ERSP_ObjrecKeypointTable *);


/* Iterate the hash table, for examining or for modifying (the
   return value becomes the new keypoint data) */
typedef void *
(*ERSP_ObjrecKeypointTableIterFunc) (ERSP_ObjrecKeypoint *keypoint,
                                     void                *keypoint_data,
                                     void                *iter_data);

ERSP_OBJREC_EXPORT void
ERSP_objrec_keypoint_table_foreach (ERSP_ObjrecKeypointTable *table,
                                    ERSP_ObjrecKeypointTableIterFunc func,
                                    void                     *iter_data);

ERSP_OBJREC_END_DECLS

#endif
