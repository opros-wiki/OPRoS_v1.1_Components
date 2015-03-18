//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-io.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_IO_H_
#define __ERSP_OBJREC_IO_H_

#include "ersp-objrec-common.h"
#ifdef ERSP_OBJREC_SUPPORT_STDIO_FILE
#include <stdio.h>
#endif

ERSP_OBJREC_BEGIN_DECLS

typedef struct _ERSP_ObjrecReader ERSP_ObjrecReader;
typedef struct _ERSP_ObjrecWriter ERSP_ObjrecWriter;

typedef enum {
    ERSP_OBJREC_IO_SUCCESS,
    ERSP_OBJREC_IO_ERROR,
    ERSP_OBJREC_IO_EOF,

    ERSP_OBJREC_IO_BAD_FORMAT,
    ERSP_OBJREC_IO_TOO_SHORT,
    ERSP_OBJREC_IO_OUT_OF_MEMORY,
    ERSP_OBJREC_IO_INVALID,
    ERSP_OBJREC_IO_FULL /* out of storage media */
} ERSP_ObjrecIOStatus;

ERSP_OBJREC_EXPORT const char *
ERSP_objrec_io_status_string (ERSP_ObjrecIOStatus status);

struct _ERSP_ObjrecReader
{
    ERSP_ObjrecIOStatus (*read)    (ERSP_ObjrecReader *reader,
                                    void              *buffer,
                                    size_t             buffer_size,
                                    size_t            *n_bytes_read_out);
    void                (*destroy) (ERSP_ObjrecReader *reader);
};

struct _ERSP_ObjrecWriter
{
    ERSP_ObjrecIOStatus (*write)   (ERSP_ObjrecWriter *writer,
                                    const void        *buffer,
                                    size_t             buffer_size);
    void                (*destroy) (ERSP_ObjrecWriter *writer);
};

#ifdef ERSP_OBJREC_SUPPORT_STDIO_FILE
ERSP_OBJREC_EXPORT ERSP_ObjrecReader *
ERSP_objrec_reader_from_stdio_fp (FILE *fp);
ERSP_OBJREC_EXPORT ERSP_ObjrecWriter *
ERSP_objrec_writer_from_stdio_fp (FILE *fp);
#endif

ERSP_OBJREC_EXPORT ERSP_ObjrecReader *
ERSP_objrec_reader_from_filename (const char *filename);
ERSP_OBJREC_EXPORT ERSP_ObjrecWriter *
ERSP_objrec_writer_from_filename (const char *filename);


ERSP_OBJREC_EXPORT ERSP_ObjrecReader *
ERSP_objrec_reader_from_memory   (const void *data,
                                  unsigned    length,
                                  void       *destroy_data,
                                  void (*destroy) (void *destroy_data));

/* Create a writer which will write to memory and automatically
 * expand its buffer as needed. */
ERSP_OBJREC_EXPORT ERSP_ObjrecWriter *
ERSP_objrec_writer_for_memory (void **data, size_t *size);

/* NOTE: partial reads must only occur at the end of the file. */
ERSP_OBJREC_EXPORT ERSP_ObjrecIOStatus
ERSP_objrec_reader_read          (ERSP_ObjrecReader *reader,
                                  void              *buffer,
                                  size_t             buffer_size,
                                  size_t            *n_bytes_read_out);
ERSP_OBJREC_EXPORT void
ERSP_objrec_reader_destroy       (ERSP_ObjrecReader *reader);

ERSP_OBJREC_EXPORT ERSP_ObjrecIOStatus
ERSP_objrec_writer_write         (ERSP_ObjrecWriter *writer,
                                  const void        *buffer,
                                  size_t             buffer_size);
ERSP_OBJREC_EXPORT void
ERSP_objrec_writer_destroy       (ERSP_ObjrecWriter *writer);

ERSP_OBJREC_END_DECLS

#endif /* __ERSP_OBJREC_IO_H_ */
