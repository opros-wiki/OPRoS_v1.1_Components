//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-malloc.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_MALLOC_H_
#define __ERSP_OBJREC_MALLOC_H_

#include "ersp-objrec-common.h"
#include <stddef.h>
ERSP_OBJREC_BEGIN_DECLS

#ifdef USE_WINDOWS_LEAKCHECK
#include <stdlib.h>
#include <crtdbg.h>

#define ersp_objrec_malloc malloc
#define ersp_objrec_free free
#define ersp_objrec_realloc realloc
#define ersp_objrec_calloc calloc

#else

ERSP_OBJREC_EXPORT extern void *(*ersp_objrec_malloc) (size_t size);
ERSP_OBJREC_EXPORT extern void (*ersp_objrec_free) (void *ptr);
ERSP_OBJREC_EXPORT extern void *(*ersp_objrec_realloc) (void *ptr, size_t size);
ERSP_OBJREC_EXPORT extern void *(*ersp_objrec_calloc) (size_t size0, size_t size1);
ERSP_OBJREC_EXPORT extern ersp_objrec_boolean ersp_objrec_malloc_failure_is_fatal;

#endif

ERSP_OBJREC_EXPORT void ERSP_objrec_enable_malloc_debugging (ersp_objrec_boolean do_tracking);

#define ERSP_OBJREC_FREE_SET_NULL(x) {ersp_objrec_free(x); (x) = 0;}


ERSP_OBJREC_END_DECLS

#endif
