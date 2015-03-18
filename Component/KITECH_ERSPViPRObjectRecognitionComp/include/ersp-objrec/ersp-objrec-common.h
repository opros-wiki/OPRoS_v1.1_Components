//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    ersp-objrec/ersp-objrec-common.h
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#ifndef __ERSP_OBJREC_COMMON_H_
#define __ERSP_OBJREC_COMMON_H_

/* XXX: um, i think C99 supports 'inline' in C at some level. */
#if defined(__GNUC__)
#define ERSP_OBJREC_INLINE static inline
#elif defined(_WIN32)
#define ERSP_OBJREC_INLINE static __inline
#ifdef USE_WINDOWS_LEAKCHECK
#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
#endif
#else
#define ERSP_OBJREC_INLINE static
#endif


#ifndef ERSP_OBJREC_INLINE_KEYPOINT_REF
#define ERSP_OBJREC_INLINE_KEYPOINT_REF 1
#endif

/* Macros to deal with windows DLL export/import madness.
 * Basically, on windows you need to "import" library
 * symbols, but whilst building the library you must "export" them.
 *
 * Of course, the headers are the same so you have to differentiate
 * the export tags from the namespace etc etc.
 *
 * Anyways, the result is:
 *  (1) defined BUILDING_ERSP_OBJREC_ITSELF when building
 *  (2) tag every exported function with ERSP_OBJREC_EXPORT
 */
#if defined(_WIN32)
#ifdef BUILDING_ERSP_OBJREC_ITSELF
#define ERSP_OBJREC_EXPORT   __declspec(dllexport)
#else
#define ERSP_OBJREC_EXPORT   __declspec(dllimport)
#endif
#else /* !_WIN32 */
#define ERSP_OBJREC_EXPORT
#endif /* !_WIN32 */

/* Guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
# define ERSP_OBJREC_BEGIN_DECLS  extern "C" {
# define ERSP_OBJREC_END_DECLS    }
#else
# define ERSP_OBJREC_BEGIN_DECLS
# define ERSP_OBJREC_END_DECLS
#endif


#define ERSP_OBJREC_SUPPORT_STDIO_FILE          1
#undef  ERSP_OBJREC_SUPPORT_UNISTD_FD                   /* not implemented yet */
#ifdef  __cplusplus
# define ERSP_OBJREC_SUPPORT_IOSTREAM           1
#else   /* !__cplusplus */
# undef  ERSP_OBJREC_SUPPORT_IOSTREAM
#endif  /* !__cplusplus */

#ifdef EVOLUTION_PLATFORM_AIBO
#undef ERSP_OBJREC_SUPPORT_STDIO_FILE
#define ERSP_OBJREC_SUPPORT_STDIO_FILE 0
#define ERSP_OBJREC_USE_STDIO_FILE 0
#define ERSP_OBJREC_USE_IOSTREAM   0
#define ERSP_OBJREC_IO_USE_WHOLE_SLAB_OF_MEMORY 1
#endif

/* Choose an implementation to use. */
#if !defined(ERSP_OBJREC_USE_IOSTREAM) && !defined(ERSP_OBJREC_USE_STDIO_FILE)
#define ERSP_OBJREC_USE_STDIO_FILE 1
#endif

#define ersp_objrec_boolean     int


#endif
