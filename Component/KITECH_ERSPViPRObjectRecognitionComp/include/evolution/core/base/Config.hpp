/* Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * Any reproduction is strictly prohibited without the explicit
 * written permission of Evolution Robotics, Inc.  All rights reserved.
 */


/* Platforms */
/* #undef EVOLUTION_PLATFORM_LINUX    1 */
#define EVOLUTION_PLATFORM_WIN32    1
/* #undef EVOLUTION_PLATFORM_INTEL32  1 */

/* Debugging */
/* #undef EVOLUTION_DEBUG_LOGGING  1 */

/* Modules */
/* #undef EVOLUTION_MODULE_APPS         1 */
/* #undef EVOLUTION_MODULE_SAMPLE_CODE  1 */

/* Threading */
/* #undef EVOLUTION_SUPPORT_POSIX_THREADS  1 */

/* uclibc compiler */
/* #undef EVOLUTION_C_LIBRARY_UCLIBC  1 */

/* Libtool */
/* #undef EVOLUTION_LIBTOOL_TAGC  1 */

/* Headers */
/* #undef EVOLUTION_HAVE_DLFCN_H   1 */
/* #undef EVOLUTION_HAVE_LIMITS_H  1 */
/* #undef EVOLUTION_HAVE_UNISTD_H  1 */

/* Functions */
/* #undef EVOLUTION_HAVE_PTHREAD_YIELD  1 */

/* NCurses library for cursor control */
/* #undef EVOLUTION_HAVE_NCURSES 1 */

/* Which version of ERSP is this? */
#ifndef EVOLUTION_VERSION
#define EVOLUTION_VERSION "3.1.6001-20080110114606"
#endif
