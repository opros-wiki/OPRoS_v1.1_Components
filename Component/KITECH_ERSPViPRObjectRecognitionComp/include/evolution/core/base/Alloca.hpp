//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_CORE_PLATFORM_ALLOCA_HPP
#define INCLUDE_EVOLUTION_CORE_PLATFORM_ALLOCA_HPP

/** 
 * @file evolution/core/base/Alloca.hpp
 * @brief Define alloca() portably
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */

/* XXX: check this at autoconf time!!! */
#ifdef EVOLUTION_PLATFORM_LINUX
#define HAVE_ALLOCA_H    1     ///< Variable defined for conditional compilation
#endif

#ifdef  __GNUC__
/* GCC does the right thing */
# undef alloca
# define alloca(size)   __builtin_alloca (size)
#elif defined (HAVE_ALLOCA_H)
/* a native and working alloca.h is there */
# include <alloca.h>
#else /* !__GNUC__ && !HAVE_ALLOCA_H */
# ifdef _MSC_VER
#  include <malloc.h>
#  define alloca _alloca
# else /* !_MSC_VER */
#  ifdef _AIX
#pragma alloca
#  else /* !_AIX */
#   ifndef alloca /* predefined by HP cc +Olibcalls */
#    ifdef __cplusplus
extern "C" {
#    endif
    /**
     * @brief Platform dependent alloca(). See your platform's man page or 
     * help file for more info. Note that it is highly unsupportable.
     *
     * @returns Pointer to stack frame allocated memory.
     * @see ersp_alloca
     */
    char *alloca ();
#    ifdef __cplusplus
}
#    endif
#   endif /* !alloca */
#  endif /* !_AIX */
# endif /* !_MSC_VER */
#endif /* !__GNUC__ && !HAVE_ALLOCA_H */

#endif // INCLUDE_EVOLUTION_CORE_PLATFORM_ALLOCA_HPP
