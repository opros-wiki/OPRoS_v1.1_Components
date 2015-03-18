//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file   ersp-objrec/ersp-objrec-simple-database-load.h
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_DATABASE_SIMPLE_SAVE_H__
#define __ERSP_OBJREC_DATABASE_SIMPLE_SAVE_H__

#include "ersp-objrec-common.h"

ERSP_OBJREC_BEGIN_DECLS

ERSP_OBJREC_EXPORT ersp_objrec_boolean
ERSP_objrec_database_simple_save(ERSP_ObjrecDatabase *to_save,
                                 const char *save_filename);

ERSP_OBJREC_EXPORT ERSP_ObjrecDatabase *
ERSP_objrec_database_simple_load(const char *db_filename);

ERSP_OBJREC_END_DECLS

#endif
