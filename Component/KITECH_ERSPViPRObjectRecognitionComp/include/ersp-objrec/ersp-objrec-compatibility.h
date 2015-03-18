//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-compatibility.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_COMPATIBILITY_H_
#define __ERSP_OBJREC_COMPATIBILITY_H_

#include "ersp-objrec-database.h"
#include "ersp-objrec-io.h"

ERSP_OBJREC_BEGIN_DECLS

/* --- Load modelsets from version 3.0 and before --- */
typedef struct _ERSP_Objrec_3_0_Model ERSP_Objrec_3_0_Model;
struct _ERSP_Objrec_3_0_Model
{
    ERSP_ObjrecKeypointList *keypoint_list;

    char *label;
    float distance;
    char *units;
    unsigned model_id;
    unsigned group;
    unsigned width;
    unsigned height;
};

/* Handle information from a newly loaded model. */
typedef void   (*ERSP_Objrec_3_0_HandleModel)    (ERSP_Objrec_3_0_Model *model,
                                                  void                  *converter_data);

/* Returns whether to save this model. */
typedef int    (*ERSP_Objrec_3_0_ModelDowngrade) (void                  *model_data,
                                                  ERSP_Objrec_3_0_Model *model_out,
                                                  void                  *converter_data);

ERSP_OBJREC_EXPORT ERSP_ObjrecIOStatus
ERSP_objrec_compatibility__load_modelset_3_0(ERSP_ObjrecReader    *reader,
                                             ersp_objrec_boolean   expect_magic,
                                             ERSP_Objrec_3_0_HandleModel converter,
                                             void                 *converter_data);
ERSP_OBJREC_EXPORT ERSP_ObjrecIOStatus
ERSP_objrec_compatibility__save_modelset_3_0(ERSP_ObjrecWriter    *writer,
                                             ERSP_ObjrecDatabase  *database,
                                             unsigned              file_format_version,
                                             ERSP_Objrec_3_0_ModelDowngrade converter,
                                             void                 *converter_data);

#define ERSP_OBJREC_COMPATIBILITY__KEYPOINT_SERIALIZED_SIZE (12 * 4 + 128)
ERSP_OBJREC_EXPORT ERSP_ObjrecIOStatus
ERSP_objrec_compatibility__load_keypoint    (ERSP_ObjrecReader    *reader,
                                             ERSP_ObjrecKeypoint  *keypoint_out);

ERSP_OBJREC_EXPORT ERSP_ObjrecIOStatus
ERSP_objrec_compatibility__save_keypoint    (ERSP_ObjrecWriter    *writer,
                                             const ERSP_ObjrecKeypoint *keypoint);
ERSP_OBJREC_END_DECLS

#endif
