#ifndef __DEBUG_MACRO_H__
#define __DEBUG_MACRO_H__

#include <string>

#if !defined(BOOST_CURRENT_FUNCTION_HPP_INCLUDED) && !defined(BOOST_CURRENT_FUNCTION)

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))

# define BOOST_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__DMC__) && (__DMC__ >= 0x810)

# define BOOST_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)

# define BOOST_CURRENT_FUNCTION __FUNCSIG__

#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))

# define BOOST_CURRENT_FUNCTION __FUNCTION__

#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)

# define BOOST_CURRENT_FUNCTION __FUNC__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)

# define BOOST_CURRENT_FUNCTION __func__

#else

# define BOOST_CURRENT_FUNCTION "(unknown)"

#endif

#endif

#define TO_STRING_FOR_DEBUG_MESSAGE_(X) #X
#define TO_STRING_FOR_DEBUG_MESSAGE(X) TO_STRING_FOR_DEBUG_MESSAGE_(X)
#define DEBUG_MESSAGE(_MESSAGE_) std::string(BOOST_CURRENT_FUNCTION).append("-> ").append(_MESSAGE_).append(" << ").append(__FILE__).append("(").append(TO_STRING_FOR_DEBUG_MESSAGE(__LINE__)).append(")\r\n")

#endif
