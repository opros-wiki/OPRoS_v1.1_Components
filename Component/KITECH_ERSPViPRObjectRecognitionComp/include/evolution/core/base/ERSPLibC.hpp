//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    evolution/core/base/ERSPLibC.hpp
 * @brief   Implementation and macro definitions for multi-platform
 *          handling of libc functions
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_base
 * @ingroup core_base_aibo
 *
 * Provides macros for the following libc-like functions:<br>
 * <ul>
 *  <li>ersp_alloca (size_t *size);
 *  <li>ersp_unlink(const char *filename);
 *  <li>ersp_rmdir(const char *dir);
 *  <li>ersp_chdir(const char *dir);
 *  <li>ersp_mkdir(const char *dir, int mode);
 *  <li>ersp_getcwd(char *dir_out, size_t dir_max_len);
 *  <li>ersp_snprintf(char *buf, size_t buf_len, const char *format, ...);
 *  <li>ersp_vsnprintf(char *buf, size_t buf_len, const char *format, va_list args);
 *  <li>ersp_strcasecmp(const char *a, const char *b);
 *  <li>ersp_strncasecmp(const char *a, const char *b, size_t n);
 *  <li>time_t ersp_time(time_t *time_out_optional);
 * </ul><br>
 * <br>
 * See the linux manpages for the functions of the same name (w/o the
 * ersp_ prefix) for full documentation.<br>
 * <br>
 * Contains macros for use with stat().
 * The portable way to use stat() is to check the st_mode
 * member with the S_IS*() macros; that's a little obnoxious
 * on the global namespace so we use ERSP_ST_MODE_IS*() instead:
 *
 * struct stat stat_buf;
 *   if (stat(filename, &stat_buf) < 0)
 *     die();
 *   if (!ERSP_ST_MODE_ISDIR(stat_buf.st_mode))
 *     die();
 * <br>
 *  st_mode testing macros:<br>
 *  <ul>
 *    <li>ERSP_ST_MODE_ISDIR   is a directory
 *    <li>ERSP_ST_MODE_ISREG   is a regular file
 *    <li>ERSP_ST_MODE_ISLINK  is a symlink
 *  </ul>
 *  you can also use (st_mode & 0777) to get "unix-style" permissions,
 *  but on windows, only the first three bits are used.
 * <br>
 * Also provides the following math macros:<br>
 * <ul>
 * <li>ersp_round(value) (using unix ceil)
 * <li>ersp_floorf(value) (using unix fllor)
 * <li>ersp_fabsf(value)  (using unix fabs)
 * <li>ersp_hypot(value)  (using uclibc hypot)
 * </ul><br>
 * Additionally, provides a macro for use with open:<br>
 * <ul>
 * <li>ERSP_O_BINARY: used with open()
 * </ul>
 **/

#ifndef INCLUDE_EVOLUTION_CORE_BASE_ERSP_LIBC_HPP
#define INCLUDE_EVOLUTION_CORE_BASE_ERSP_LIBC_HPP

#include <evolution/core/base/Alloca.hpp>
#include <evolution/core/base/Config.hpp>
#include <evolution/core/base/Export.hpp>

/* For va_list */
#include <stdarg.h>
#include <math.h>

#if EVOLUTION_PLATFORM_WIN32
#include <direct.h>
#include <io.h>
#elif EVOLUTION_HAVE_UNISTD_H  /** !EVOLUTION_PLATFORM_WIN32 */
#include <unistd.h>
#endif /** !EVOLUTION_PLATFORM_WIN32 */

#if !EVOLUTION_PLATFORM_AIBO || defined(GHS)
#include <time.h>
#define ersp_time       time
#endif

#if !defined(GHS)
#include <sys/stat.h>
#endif

/** 
 * @brief Determine if function needs '_' prefix.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#if EVOLUTION_PLATFORM_WIN32
#define _EVOLUTION_LIBC_EXTRA_FUNC(func)    _##func
#else
#define _EVOLUTION_LIBC_EXTRA_FUNC(func)    func
#endif


/** @brief A mechanism to tell the compiler 
 * that the given function has printf() semantics.
 * This is used to get proper format-string mismatch
 * error messages, with -Wall on gcc. */
#if    __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define ERSP_FUNCTION_ATTRIBUTE_PRINTF( format_idx, arg_idx )    \
  __attribute__((format (printf, format_idx, arg_idx)))
#else
#define ERSP_FUNCTION_ATTRIBUTE_PRINTF( format_idx, arg_idx )
#endif




// Public interface.
/** 
 * @def ersp_unlink
 * @brief See man page for unlink().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/** 
 * @def ersp_rmdir
 * @brief See man page for rmdir().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/** 
 * @def ersp_mkdir
 * @brief See man page for mkdir().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/** 
 * @def ersp_chdir
 * @brief See man page for chdir().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/** 
 * @def ersp_getcwd
 * @brief See man page for getcwd().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/** 
 * @def ersp_alloca
 * @brief See man page for alloca().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/**
 * @def ersp_stat
 * @brief See man page for stat().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/**
 * @def ersp_finite
 * @brief See man page for finite().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/**
 * @def _ersp_snprintf
 * @brief See man page for snprintf().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
/**
 * @def _ersp_vsnprintf
 * @brief See man page for vsnprintf().
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define ersp_alloca     alloca
#define ersp_unlink     _EVOLUTION_LIBC_EXTRA_FUNC(unlink)
#define ersp_rmdir      _EVOLUTION_LIBC_EXTRA_FUNC(rmdir)
#define ersp_mkdir      Platform::mkdir
#define ersp_chdir      _EVOLUTION_LIBC_EXTRA_FUNC(chdir)
#define ersp_getcwd     _EVOLUTION_LIBC_EXTRA_FUNC(getcwd)
#define ersp_stat       _EVOLUTION_LIBC_EXTRA_FUNC(stat)
#define ersp_finite     _EVOLUTION_LIBC_EXTRA_FUNC(finite)
#define _ersp_snprintf  _EVOLUTION_LIBC_EXTRA_FUNC(snprintf)
#define _ersp_vsnprintf _EVOLUTION_LIBC_EXTRA_FUNC(vsnprintf)
#define ersp_write      _EVOLUTION_LIBC_EXTRA_FUNC(write)
#define ersp_strdup      _EVOLUTION_LIBC_EXTRA_FUNC(strdup)
#define ersp_putenv      _EVOLUTION_LIBC_EXTRA_FUNC(putenv)

/** 
 * @brief A safe version of sprintf().   This takes a buffer length.
 * Unlike most snprintf() implementations, this ensures that the
 * string buffer is NULL-terminated, even if that means loosing
 * the (length-1)th character of your string.
 * @ingroup core_base
 * @ingroup core_base_aibo
*/
int EVOLUTION_EXPORT_BASE
ersp_snprintf (char *buf, unsigned max_len,
               const char *format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(3,4);
/** 
 * @brief See man page for vsnprintf().
 * This version ensures that the string-buffer is NULL-terminated.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
int EVOLUTION_EXPORT_BASE
ersp_vsnprintf (char *buf, unsigned max_len,
                const char *format, va_list args);


#if EVOLUTION_PLATFORM_WIN32
/** 
 * @brief See windows help for _stricmp().
 * @ingroup core_base
 */
#define ersp_strcasecmp    _stricmp
/** 
 * @brief See windows help for _strnicmp().
 * @ingroup core_base
 */
#define ersp_strncasecmp   _strnicmp
#else
/** 
 * @brief See man page for strcasecmp().
 * @ingroup core_base
 */
#define ersp_strcasecmp    strcasecmp
/**
 * @brief See man page for strncasecmp().
 * @ingroup core_base
 */
#define ersp_strncasecmp   strncasecmp
#endif

/** 
 * @brief Flag for use with ersp_open to define a file as binary.
 * Flag passed on to libc open() ie: open (...,O_RDONLY | ERSP_O_BINARY)
 * Any other use might certainly lead to problems (ie: O_RDONLY & ERSP_O_BINARY)
 * @see ersp_open
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#if EVOLUTION_PLATFORM_WIN32
#define ERSP_O_BINARY O_BINARY
#else
#define ERSP_O_BINARY 0x00
#endif


/**
 * @brief Test for use with stat.st_mode to determine if a file is a regular file.
 * Test for use with stat.st_mode to determine if a file is a regular file.
 * @param mode [in]: stat.st_mode 
 * @ingroup core_base
 */
#ifdef S_ISREG
#define ERSP_ST_MODE_ISREG(mode)    S_ISREG(mode)
#elif defined(_S_IFREG)
#define ERSP_ST_MODE_ISREG(mode)    (((mode) & _S_IFREG) == _S_IFREG)
#elif defined(S_IFREG)
#define ERSP_ST_MODE_ISREG(mode)    (((mode) & S_IFREG) == S_IFREG)
#else
#define ERSP_ST_MODE_ISREG(mode)    0
#endif

/**
 * @brief Test for use with stat.st_mode to determine if a file is a directory.
 * @param mode [in]: stat.st_mode 
 * @ingroup core_base
 */
#ifdef S_ISDIR
#define ERSP_ST_MODE_ISDIR(mode)    S_ISDIR(mode)
#elif defined(_S_IFDIR)
#define ERSP_ST_MODE_ISDIR(mode)    (((mode) & _S_IFDIR) == _S_IFDIR)
#elif defined(S_IFDIR)
#define ERSP_ST_MODE_ISDIR(mode)    (((mode) & S_IFDIR) == S_IFDIR)
#else 
#define ERSP_ST_MODE_ISDIR(mode)    0
#endif

/**
 * @brief Test for use with stat.st_mode to determine if a file is a symbolic link.
 * @param mode [in]: stat.st_mode 
 * @ingroup core_base
 */
#ifdef S_ISLINK
#define ERSP_ST_MODE_ISLINK(mode)   S_ISLINK(mode)
#else
#define ERSP_ST_MODE_ISLINK(mode)   0
#endif

// This section is for compatibility with the Sony Aibo compiler.
#if EVOLUTION_PLATFORM_AIBO
#ifdef GHS
#undef ersp_strncasecmp
#undef ersp_strcasecmp
#include <stdarg.h>
int ersp_strncasecmp(const char *, const char *b, size_t maxlen);
int ersp_strcasecmp(const char *, const char *b);
int _ersp_vsnprintf(char *buf, size_t len, const char *format, va_list args);
int _ersp_snprintf(char *buf, size_t len, const char *format, ...);
#undef ersp_stat
#undef ersp_unlink
#include <OFS.h>
#define ersp_stat OFS::stat
#define ersp_open OFS::open
#define ersp_close OFS::close
#define ersp_read OFS::read
#define ersp_unlink OFS::unlink
#else /** !GHS: need to define this macros to re-use the same code with gcc */
#define ersp_open open
#define ersp_close close
#define ersp_read read
#endif  /** GHS */
// The gcc version of OPENR does not have time at all
// The GHS version of OPENR has the header file but no implementation
// Thus, we use GetSystemTime() in both cases...
#undef ersp_time
#include <MCOOP.h>
static inline time_t ersp_time(time_t *out)
{
    SystemTime sys_time;
    GetSystemTime(&sys_time);
    if (out)
        *out = sys_time.seconds;
    return sys_time.seconds;
}
#endif //AIBO

/**
 * @def ersp_floorf
 * @brief Return the floor for a given decimal.
 * @param value [in]: Decimal value to return integer floor of.
 * @returns Floor of decimal value.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */

/**
 * @brief Return the absolute value for a given signed decimal.
 * @param value [in]: Signed value to return absolute value of.
 * @returns Absolute value of signed value.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
// Certain functions are not defined for uClibC, used for some
// embedded systems.

#ifdef EVOLUTION_C_LIBRARY_UCLIBC

#define ersp_fabsf(value) ((float) fabs(value))
#define ersp_floorf(value) ((float) floor(value))

#else

#define ersp_fabsf(value)  fabs(value)
#define ersp_floorf(value)  floor(value)

#endif // EVOLUTION_C_LIBRARY_UCLIBC

/** 
 * @brief Uses unix ceil() function to perform rounding.
 * ceil(value - 0.5) rounds a value to the nearest whole number.
 * See unix man page for ceil() for more information.
 * @param value [in]: Decimal value (float, double, etc.) to round.
 * @returns Rounded decimal value.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define ersp_round(value)  ceil(value - 0.5)

/* We've found problems in the hypot calls (both in uclibc, and on
 * glibc running on a MIPS board).  The problem manifests itself as
 * hypot(0.0,0.0) = nan!  So, replacing for now with sqrt.  This, however,
 * doesn't properly handle potential overflow problems that may occur.
 */
//TODO: Should have unit tests to test all the functions in here that we can...
static inline double ersp_hypot (double a, double b)
{
    return sqrt(a*a + b*b);
}
//#define ersp_hypot(a,b) sqrt(a*a + b*b) 

#endif // INCLUDE_EVOLUTION_CORE_BASE_ERSP_LIBC_HPP
