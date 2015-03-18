//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BASE__LOGGING_HPP
#define INCLUDE_EVOLUTION_CORE_BASE__LOGGING_HPP


/**
 * @file   evolution/core/base/Logging.hpp
 * @brief  Logging utilities.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


#include <evolution/core/base/Config.hpp>
#include <evolution/core/base/LoggingCategory.hpp>
#include <evolution/core/base/LoggingTypes.hpp>
#include <evolution/core/base/Platform.hpp>

#if !EVOLUTION_PLATFORM_AIBO
#include <evolution/core/base/SimpleProfiler.hpp>
#endif


/// Declares a logging category. It is to be used in a .cpp implementation file.
#define ERSP_LOG_CATEGORY(cat) \
    static const char *_ersp_log_category_name = (cat); \
    static Evolution::LoggingCategory *_ersp_log = 0

#ifndef DOXYGEN_INTERNAL

/**
 * @brief
 * Declares a static class logging category.
 *
 * This is necessary for inline functions declared in the .hpp
 * file. The declaration should be used inside the class in the header
 * file, while the implementation, ERSP_LOG_CLASS_CATEGORY_IMPLEMENT,
 * should be in the .cpp file.
 *
 * To use this class logging category, you must call the special set
 * of CLASS macros, such as ERSP_LOG_CLASS_DEBUG().
 * @ingroup core_base
 * @ingroup core_base_aibo
 **/
#define ERSP_LOG_CLASS_CATEGORY_DECLARE(klass, name) \
    static const char* klass##__ersp_log_category_name; \
    static Evolution::LoggingCategory* klass##__ersp_log

#define ERSP_LOG_CLASS_CATEGORY_IMPLEMENT(klass, name) \
    const char *klass::klass##__ersp_log_category_name = (name); \
    Evolution::LoggingCategory *klass::klass##__ersp_log = (0)


// For example, if you wish for a class to have a static logging category,
// for use with inline functions, then you can use
// these macros to declare the category in the .hpp file
// and write the data in the .cpp file.
#define ERSP_LOG_NAMESPACE_CATEGORY_DECLARE(name, LNK) \
    extern LNK const char *_ersp_log_category_name; \
    extern LNK Evolution::LoggingCategory *_ersp_log

#define ERSP_LOG_NAMESPACE_CATEGORY_IMPLEMENT(class, name) \
    const char *class::_ersp_log_category_name = (name); \
    Evolution::LoggingCategory *class::_ersp_log = (0)


#define ERSP_GET_LOG_CATEGORY() \
    (_ersp_log \
     ? _ersp_log \
     : (_ersp_log=&Evolution::LoggingCategory::get_instance(_ersp_log_category_name)))

#define ERSP_GET_LOG_CLASS_CATEGORY(klass) \
    (klass##__ersp_log \
     ? klass##__ersp_log \
         : (klass##__ersp_log=&Evolution::LoggingCategory::get_instance(klass##__ersp_log_category_name)))


// Sets the priority of the specified category.
#define ERSP_LOG_PRIORITY(cat,pri)                                     \
    do{                                                                \
        Evolution::LoggingCategory::get_instance(cat).set_priority(Evolution::LoggingPriority::pri);   \
    }while(0)

#define ERSP_LOG_SET_ROOT_PRIORITY(pri)                    \
    Evolution::LoggingCategory::set_root_priority(Evolution::LoggingPriority::pri)

  /* Define _ERSP_LOG_OBJECT_* and _ERSP_LOG_CLASS_OBJECT_*
   * where '*' may be one of:
   *    SIMPLE: no prefix/suffix
   *    LAST_CATEGORY: suffixed with last component of category name.
   *    FILE_LINE: the filename and line number.
   */
#define _ERSP_LOG_OBJECT_SIMPLE() \
    (*ERSP_GET_LOG_CATEGORY())
#define _ERSP_LOG_OBJECT_LAST_CATEGORY() \
    Evolution::LoggingCategoryPostfixed(ERSP_GET_LOG_CATEGORY())
#define _ERSP_LOG_OBJECT_FILE_LINE() \
    Evolution::LoggingCategoryPostfixed(ERSP_GET_LOG_CATEGORY(), \
                                        "%s:%d", __FILE_BASE__, __LINE__)

#define _ERSP_LOG_CLASS_OBJECT_SIMPLE(klass) \
    (*ERSP_GET_LOG_CLASS_CATEGORY(klass))
#define _ERSP_LOG_CLASS_OBJECT_LAST_CATEGORY(klass) \
    Evolution::LoggingCategoryPostfixed(ERSP_GET_LOG_CLASS_CATEGORY(klass), \
                                        "%s:%d", __FILE_BASE__, __LINE__)
#define _ERSP_LOG_CLASS_OBJECT_FILE_LINE(klass) \
    Evolution::LoggingCategoryPostfixed(ERSP_GET_LOG_CLASS_CATEGORY(klass), \
                                        "%s:%d", __FILE_BASE__, __LINE__)


// ERSP_DEBUG_VAR() logging.
// This can be used for variables that otherwise will show up
// as unused in release builds.
// There is an empty definition below in the non-debug section
#if defined(EVOLUTION_DEBUG_LOGGING)
#define ERSP_DEBUG_VAR(var)            var
#else
#define ERSP_DEBUG_VAR(var)
#endif

// Select the type of logging to do.  FILE_LINE in DEBUG mode,
// and LAST_CATEGORY in release mode.
#ifdef EVOLUTION_DEBUG_LOGGING
#define _ERSP_LOG_OBJECT()              _ERSP_LOG_OBJECT_FILE_LINE()
#define _ERSP_LOG_CLASS_OBJECT(klass)   _ERSP_LOG_CLASS_OBJECT_FILE_LINE(klass)
#else
#define _ERSP_LOG_OBJECT()              _ERSP_LOG_OBJECT_SIMPLE()
#define _ERSP_LOG_CLASS_OBJECT(klass)   _ERSP_LOG_CLASS_OBJECT_SIMPLE(klass)
#endif

/* Define ERSP_LOG_{VERBOSE,DEBUG} and ERSP_LOG_CLASS_{VERBOSE,DEBUG}. */
#ifdef EVOLUTION_DEBUG_LOGGING
# define ERSP_LOG_VERBOSE                _ERSP_LOG_OBJECT().verbose
# define ERSP_LOG_DEBUG                  _ERSP_LOG_OBJECT().debug
# define ERSP_LOG_CLASS_VERBOSE(klass)   _ERSP_LOG_CLASS_OBJECT(klass).verbose
# define ERSP_LOG_CLASS_DEBUG(klass)     _ERSP_LOG_CLASS_OBJECT(klass).debug
#else  /* !EVOLUTION_DEBUG_LOGGING */
# ifdef __GNUC__
#  define ERSP_LOG_VERBOSE(format...)
#  define ERSP_LOG_DEBUG(format...)
# elif defined(EVOLUTION_PLATFORM_WIN32)
#  pragma warning (disable:4002)
   // Windows doesn't support variadic-arguments to macros.
   // However, it does allow you the pass in arguments to
   // a macro that doesn't declare itself as not taking any,
   // albeit with a warning.
   //
   // This trick is highly non-portable,
   // but seems to work... for now.
#  define  ERSP_LOG_VERBOSE()
#  define  ERSP_LOG_DEBUG()
#else   /* !GNUC, !WINDOWS */
    // Support systems which don't seem to have an
    // appropriate preprocessor workaround.
#undef ERSP_DEBUG_VAR
#define ERSP_DEBUG_VAR(var) var
   static inline void ERSP_LOG_VERBOSE(const char *format, ...) { }
   static inline void ERSP_LOG_DEBUG(const char *format, ...) { }
# endif  /* !__GNUC__ && !WINDOWS */
# define ERSP_LOG_CLASS_VERBOSE(klass) ERSP_LOG_VERBOSE
# define ERSP_LOG_CLASS_DEBUG(klass) ERSP_LOG_DEBUG
#endif  /* end of clause defining DEBUG and VERBOSE */

/* Debug ERSP_LOG_{INFO,WARN,ERROR,FATAL} */
#define ERSP_LOG_INFO                   _ERSP_LOG_OBJECT().info
#define ERSP_LOG_WARN                   _ERSP_LOG_OBJECT().warn
#define ERSP_LOG_ERROR                  _ERSP_LOG_OBJECT().error
#define ERSP_LOG_FATAL                  _ERSP_LOG_OBJECT().fatal
#define ERSP_LOG_CLASS_INFO(klass)      _ERSP_LOG_CLASS_OBJECT(klass).info
#define ERSP_LOG_CLASS_WARN(klass)      _ERSP_LOG_CLASS_OBJECT(klass).warn
#define ERSP_LOG_CLASS_ERROR(klass)     _ERSP_LOG_CLASS_OBJECT(klass).error
#define ERSP_LOG_CLASS_FATAL(klass)     _ERSP_LOG_CLASS_OBJECT(klass).fatal


/* We want ERSP_LOG_TRACE to include the a brief hint
 * about what it's talking about... */
#ifndef EVOLUTION_DEBUG_LOGGING
#define ERSP_LOG_TRACE                  _ERSP_LOG_OBJECT_LAST_CATEGORY().trace
#define ERSP_LOG_CLASS_TRACE(klass)     _ERSP_LOG_CLASS_OBJECT_LAST_CATEGORY(klass).trace
#else
#define ERSP_LOG_TRACE                  _ERSP_LOG_OBJECT().trace
#define ERSP_LOG_CLASS_TRACE(klass)     _ERSP_LOG_CLASS_OBJECT(klass).trace
#endif

#if !EVOLUTION_PLATFORM_AIBO
#define ERSP_LOG_USEPROFILER                    USESIMPLEPROFILER
#define ERSP_LOG_OPENPROFILER                   OPENSIMPLEPROFILER
#define ERSP_LOG_CLOSEPROFILER                  CLOSESIMPLEPROFILER
#define ERSP_LOG_STARTPROFILERSECTION           STARTSIMPLEPROFILERSECTION
#define ERSP_LOG_ENDPROFILERSECTION             ENDSIMPLEPROFILERSECTION
#define ERSP_LOG_STARTDERIVEDPROFILERSECTION    STARTDERIVEDSIMPLEPROFILERSECTION
#define ERSP_LOG_ENDDERIVEDPROFILERSECTION      ENDDERIVEDSIMPLEPROFILERSECTION

EXTERNSIMPLEPROFILER
#endif  /* !EVOLUTION_PLATFORM_AIBO */


#endif // DOXYGEN_INTERNAL

#endif  // INCLUDE_EVOLUTION_CORE_BASE__LOGGING_HPP
