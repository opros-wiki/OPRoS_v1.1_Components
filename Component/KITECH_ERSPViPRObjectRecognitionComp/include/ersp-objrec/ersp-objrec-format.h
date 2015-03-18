//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-format.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_FORMAT_INCLUDED_H_
#define __ERSP_OBJREC_FORMAT_INCLUDED_H_

#include "ersp-objrec-io.h"
#include "ersp-objrec-keypoint-list.h"
#include "ersp-objrec-database.h"

/* --- magic --- */
#define ERSP_OBJREC_MAGIC_3_0                "EVOM"
#define ERSP_OBJREC_MAGIC_3_1_KEYPOINT       "\000\144\102\113"
#define ERSP_OBJREC_MAGIC_3_1_KEYPOINT_LIST  "\000\113\120\114"
#define ERSP_OBJREC_MAGIC_3_1_DATABASE       "\000\117\104\102"

ERSP_OBJREC_BEGIN_DECLS

/* --- represent on-disk structures --- */
typedef struct
{
    ERSP_ObjrecKeypointList *list;
    ERSP_ObjrecKeypoint **keypoint_array;
    unsigned n_annotations;
    unsigned *annotation_types;
    unsigned *annotation_lengths;
    unsigned *annotation_versions;
    void **annotations;
} ERSP_Objrec_3_1_KeypointListInfo;

/* private */
typedef struct _ERSP_Objrec_3_1_MergeKeyListElementPool ERSP_Objrec_3_1_MergeKeyListElementPool;

typedef struct 
{
    unsigned model_index;
    unsigned keypoint_index;
} ERSP_Objrec_3_1_MergeKeyListElement;

typedef struct
{
    unsigned n_elements;
    ERSP_Objrec_3_1_MergeKeyListElement *elements;
} ERSP_Objrec_3_1_MergeKeyListInfo;

typedef struct
{
    unsigned n_models;
    ERSP_Objrec_3_1_KeypointListInfo **models;
    unsigned n_merge_key_lists;
    ERSP_Objrec_3_1_MergeKeyListInfo *merge_key_lists;
    ERSP_Objrec_3_1_MergeKeyListElementPool *_trash;
} ERSP_Objrec_3_1_DatabaseInfo;

/************************************************************************/
/*                           deserialization                            */
/************************************************************************/

ERSP_OBJREC_EXPORT ERSP_Objrec_3_1_KeypointListInfo *
ERSP_objrec_3_1_deserialize_kp_list    (ERSP_ObjrecReader *reader,
                                        int                expect_magic);

ERSP_OBJREC_EXPORT ERSP_ObjrecKeypoint *
ERSP_objrec_3_1_deserialize_kp         (ERSP_ObjrecReader *reader,
                                        ERSP_ObjrecKeypoint *input);

ERSP_OBJREC_EXPORT ERSP_Objrec_3_1_DatabaseInfo *
ERSP_objrec_3_1_deserialize_database   (ERSP_ObjrecReader *reader,
                                        int                expect_magic);

ERSP_OBJREC_EXPORT void
ERSP_objrec_3_1_keypoint_list_info_free (ERSP_Objrec_3_1_KeypointListInfo *);
ERSP_OBJREC_EXPORT void
ERSP_objrec_3_1_database_info_free      (ERSP_Objrec_3_1_DatabaseInfo *);

ERSP_OBJREC_EXPORT ERSP_Objrec_3_1_KeypointListInfo *
ERSP_objrec_3_1_keypoint_list_info_from_keypoint_list (const ERSP_ObjrecKeypointList *kp_list);

/************************************************************************/
/*                            serialization                             */
/************************************************************************/

ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_3_1_serialize_kp_list (ERSP_ObjrecWriter *writer,
                                   const ERSP_Objrec_3_1_KeypointListInfo *keypoint);

ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_3_1_serialize_kp      (ERSP_ObjrecWriter *writer,
                                   const ERSP_ObjrecKeypoint *keypoint);

ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_3_1_serialize_database(ERSP_ObjrecWriter *writer,
                                   const ERSP_Objrec_3_1_DatabaseInfo *database);

/* --- for serializing an existing ERSP_Objrec_3_1_Database --- */
typedef ersp_objrec_boolean (*ERSP_Objrec_3_1_DatabaseInfoAnnotator)
    (ERSP_Objrec_3_1_KeypointListInfo *info,
     void                        *user_data,
     void                        *anno_data);

ERSP_OBJREC_EXPORT ERSP_Objrec_3_1_DatabaseInfo *
ERSP_objrec_3_1_database_info_from_db (ERSP_ObjrecDatabase *database,
                                       ERSP_Objrec_3_1_DatabaseInfoAnnotator annotator,
                                       void                             *anno_data);

typedef ersp_objrec_boolean (*ERSP_Objrec_3_1_DatabaseInfoCreateUserData)
    (ERSP_Objrec_3_1_KeypointListInfo *info,
     void                       **user_data_out,
     void                        *creator_data);

ERSP_OBJREC_EXPORT ERSP_ObjrecDatabase *
ERSP_objrec_3_1_database_info_to_db (ERSP_Objrec_3_1_DatabaseInfo *database,
                                     ERSP_Objrec_3_1_DatabaseInfoCreateUserData creator,
                                     void *creator_data);

ERSP_OBJREC_EXPORT int
ERSP_objrec_3_1_kp_list_info_annotate (ERSP_Objrec_3_1_KeypointListInfo *list,
                                       unsigned type,   /* XXX: need 32-bit int */
                                       size_t length,
                                       const void *data);

/* --- simple interface --- */
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabase *
ERSP_objrec_3_1_database_load (const char *filename,
                               ERSP_Objrec_3_1_DatabaseInfoCreateUserData creator,
                               void *creator_data);

/* Similar to ERSP_objrec_3_1_database_load but loads a list of files without
 * rebuilding the tree for each one. */
ERSP_OBJREC_EXPORT ERSP_ObjrecDatabase *
ERSP_objrec_3_1_database_load_list_fast (int num_files, const char **filenames,
                                         ERSP_Objrec_3_1_DatabaseInfoCreateUserData creator,
                                         void *creator_data);

ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_3_1_database_save (ERSP_ObjrecDatabase *database,
                               const char *filename,
                               ERSP_Objrec_3_1_DatabaseInfoAnnotator annotator,
                               void *anno_data);

/* --- Annotations --- */
typedef enum
{
    /* generic definitions */
    ERSP_OBJREC_3_1_ANNOTATION_LABEL = 0x30300000,
    /* definitions for evolution-core */
    ERSP_OBJREC_3_1_ANNOTATION_DISTANCE = 0x30300001,
    /* definitions for lanehawk */
    ERSP_OBJREC_3_1_ANNOTATION_OBJECT_WIDTH = 0x30300010,
    ERSP_OBJREC_3_1_ANNOTATION_VIPR_RULE = 0x30300011,
    ERSP_OBJREC_3_1_ANNOTATION_REPORT_AS = 0x30300012,
    /* definitions for correlation */
    ERSP_OBJREC_3_1_ANNOTATION_THUMB_X = 0x30300013,
    ERSP_OBJREC_3_1_ANNOTATION_THUMB_Y = 0x30300014,
    ERSP_OBJREC_3_1_ANNOTATION_THUMB_DATA = 0x30300015,
    ERSP_OBJREC_3_1_ANNOTATION_THUMB_DOWNSAMP_RATIO = 0x30300016
} ERSP_Objrec_3_1_AnnotationType;

typedef struct
{
    double distance;
    char units[9];        /* must be NUL terminated */
} ERSP_Objrec_3_1_Distance;

#define ERSP_OBJREC_3_1_ANNOTATION_DISTANCE_SIZE        16
ERSP_OBJREC_EXPORT void
ERSP_objrec_3_1_distance_pack   (const ERSP_Objrec_3_1_Distance *dist,
                                 char *packed_data_out);
ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_3_1_distance_unpack (const char *packed_data,
                                 unsigned packed_data_length,
                                 ERSP_Objrec_3_1_Distance *dist_out);

/* the annotation for a LABEL is just a string */

ERSP_OBJREC_END_DECLS

#endif /* __ERSP_OBJREC_FORMAT_INCLUDED_H_ */
