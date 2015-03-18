//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-database.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_DATABASE_H__
#define __ERSP_OBJREC_DATABASE_H__

typedef struct _ERSP_ObjrecDatabase ERSP_ObjrecDatabase;
typedef struct _ERSP_ObjrecModel ERSP_ObjrecModel;
typedef struct _ERSP_ObjrecMatchList ERSP_ObjrecMatchList;
typedef struct _ERSP_ObjrecMatchKeypoint ERSP_ObjrecMatchKeypoint;
typedef struct _ERSP_ObjrecDatabaseNewOptions ERSP_ObjrecDatabaseNewOptions;
typedef struct _ERSP_ObjrecDatabaseAddOptions ERSP_ObjrecDatabaseAddOptions;
typedef struct _ERSP_ObjrecDatabaseMatchOptions ERSP_ObjrecDatabaseMatchOptions;

#include "ersp-objrec-keypoint-list.h"

ERSP_OBJREC_BEGIN_DECLS

/** 
    Functions to create, destroy, retrieve info about a database
**/

/* create and destroy database option data structure */ 
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabaseNewOptions *
ERSP_objrec_database_new_options_new (void);
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_new_options_destroy (ERSP_ObjrecDatabaseNewOptions *options);

/* Set values of new_options */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_new_set_option (ERSP_ObjrecDatabaseNewOptions *options, 
                                     const char * option_name, 
                                     ...);
/* Get values of new_options */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_new_get_option (const ERSP_ObjrecDatabaseNewOptions *options, 
                                     const char * option_name, 
                                     void * value);

/* creating a database */
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabase *
ERSP_objrec_database_new           (const ERSP_ObjrecDatabaseNewOptions *);

/* remove entry from database */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_remove        (ERSP_ObjrecDatabase *database,
                                    ERSP_ObjrecModel    *model);
/* Retrieve user data */
ERSP_OBJREC_EXPORT void *
ERSP_objrec_model_get_user_data    (ERSP_ObjrecModel    *model);

/* Destroy database */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_destroy       (ERSP_ObjrecDatabase *database);

/* Get first model in database */
ERSP_OBJREC_EXPORT ERSP_ObjrecModel *
ERSP_objrec_database_get_first     (ERSP_ObjrecDatabase *database);

/* Get next model in database */
ERSP_OBJREC_EXPORT ERSP_ObjrecModel *
ERSP_objrec_model_get_next         (ERSP_ObjrecModel    *model);

/* Retrieve database containing a model */
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabase *
ERSP_objrec_model_get_database     (ERSP_ObjrecModel    *model);

/* Retrieve the KeypointList of a specific model */
/* WARNING: the keypoint is not necessarily the same one
   you passed to ERSP_objrec_database_add().
   This operation may be expensive.
   You must unref the keypoint-list.  */
ERSP_OBJREC_EXPORT ERSP_ObjrecKeypointList *
ERSP_objrec_model_get_keypoints      (ERSP_ObjrecModel *model);

/* Retrieve the current number of models in database */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_database_get_num_models(ERSP_ObjrecDatabase *database);

/* Retrieve total number of keys in database */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_database_get_num_keys  (ERSP_ObjrecDatabase *database);

/* Retrieve the keypoint type */
ERSP_OBJREC_EXPORT unsigned
ERSP_objrec_database_get_keypoint_type (ERSP_ObjrecDatabase *database);


/**
   Functions to add a model to the database
**/

/* Create add_options data structure and initialize to the defaul values */ 
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabaseAddOptions *
ERSP_objrec_database_add_options_new (void);

/* Destroy a add_options data structure */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_add_options_destroy (ERSP_ObjrecDatabaseAddOptions *options);

/* Set values of add_options */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_add_set_option (ERSP_ObjrecDatabaseAddOptions *options, 
                                     const char * option_name, 
                                     ...);
/* Get values of add_options */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_add_get_option (const ERSP_ObjrecDatabaseAddOptions *options, 
                                     const char * option_name, 
                                     void * value);

/* Add a new model to a database */
ERSP_OBJREC_EXPORT ERSP_ObjrecModel *
ERSP_objrec_database_add           (ERSP_ObjrecDatabase *database,
                                    ERSP_ObjrecKeypointList *kp_list,
                                    void           *user_data,
                                    const ERSP_ObjrecDatabaseAddOptions*);

/* Force rebuilding the database */ 
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_maybe_rebuild (ERSP_ObjrecDatabase *database);

/* -- Re-enable model that was disabled */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_enable_model(ERSP_ObjrecDatabase *database,
                                  ERSP_ObjrecModel    *model);

/* --- Disable model without removing it */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_disable_model(ERSP_ObjrecDatabase *database,
                                   ERSP_ObjrecModel    *model);


/**
   Querying the database

**/
struct _ERSP_ObjrecMatchList
{
    /* List of matching pairs. */
    ERSP_ObjrecMatchKeypoint *common_features;

    /* number of elements in the above list */
    unsigned n_common_features;

    /* Residual error of least-squares fit. */
    float residual;

    /* Matching model (same as cluster->key2->model) */
    ERSP_ObjrecModel *model;

    /* Affine parameter solution.   This is a 2x3 matrix
       giving the mapping from the model image's coordinates
       (the image that was trained on) to the coordinates of the 
       image we are attempting to query. */
    float affine[6];

    /* x,y of center of match in tested image */
    float center[2];

    /* 4 vertices of where the original image would be projected. */
    float vertices[4][2];

    /* likelihood that this match was correct */
    float probability;

    /* condition number for the affine transformation */
    float affine_condition_number;

    /* number of keypoints found initially */
    unsigned n_initial_keypoints;

    unsigned is_affine : 1;

    /* Experimental and not supported yet */
    /* This is called match_group_mask because it used to be a bit field.  Now
       it is just a number. */
    unsigned int match_group_mask;

    /* Experimental and not supported yet */
    /* a count of how many image mask pixels the matched features of this
     * match occupied. Used to group the matches geometrically, and also
     * may be useful in ranking the matches */
    unsigned int mask_occupancy;

    /* Linked list of matches */
    ERSP_ObjrecMatchList *next;

    /* private */
    void *_first_hash_entry;
};

/* Create a match_option data structure and initialize to the default values */
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabaseMatchOptions *
ERSP_objrec_database_match_options_new (void);

/* Destroy match_option data structure */
ERSP_OBJREC_EXPORT void
ERSP_objrec_database_match_options_destroy (ERSP_ObjrecDatabaseMatchOptions *options);

/* Set values of match options */ 
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_match_set_option (ERSP_ObjrecDatabaseMatchOptions *options, 
                                       const char * option_name, 
                                       ...);
/* Get values of match options */
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_match_get_option (const ERSP_ObjrecDatabaseMatchOptions *options, 
                                       const char * option_name, 
                                       void * value);

/* Query the database with a keypoint list */
ERSP_OBJREC_EXPORT ERSP_ObjrecMatchList *
ERSP_objrec_database_match         (ERSP_ObjrecDatabase     *database,
                                    ERSP_ObjrecKeypointList *keypoints,
                                    const ERSP_ObjrecDatabaseMatchOptions *);


struct _ERSP_ObjrecDatabaseLookupInfo
{
    ERSP_ObjrecKeypoint *keypoint;
    unsigned             dist_squared;
    void *model_data;
};

typedef struct _ERSP_ObjrecDatabaseLookupInfo ERSP_ObjrecDatabaseLookupInfo;

/* Look in the database for keypoints that are close to the specified
 * keypoint (neighbor lookup).  The array results should be allocated
 * to max_results.
 */
ERSP_OBJREC_EXPORT int
ERSP_objrec_database_keypoint_lookup(ERSP_ObjrecDatabase *,
                                     ERSP_ObjrecKeypoint *,
                                     void *options,
                                     int max_results,
                                     ERSP_ObjrecDatabaseLookupInfo *results);

/* Free memory for one match_lists */
ERSP_OBJREC_EXPORT void
ERSP_objrec_match_list_free_1 (ERSP_ObjrecMatchList *match_list);

/* Free memory for an entire list of match_list objects */ 
ERSP_OBJREC_EXPORT void
ERSP_objrec_match_list_free (ERSP_ObjrecMatchList *match_list);


/* An entry in a hash table that represents a match between two keys. */
struct _ERSP_ObjrecMatchKeypoint
{
    ERSP_ObjrecKeypoint *match_key;    /* was 'key1': comes from new image */
    ERSP_ObjrecKeypoint *model_key;    /* was 'key2': comes from existing model */
    int topdown;            /* Was the match created top-down during verification. */
};

ERSP_OBJREC_EXPORT ersp_objrec_boolean 
ERSP_objrec_database_check_neighbor_disabled(void *database, void *kp_info);


ERSP_OBJREC_END_DECLS

#endif
