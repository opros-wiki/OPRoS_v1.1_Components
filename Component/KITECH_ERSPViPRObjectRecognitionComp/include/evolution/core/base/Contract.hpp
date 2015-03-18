//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_CONTRACT_HPP
#define INCLUDE_EVOLUTION_BASE_CONTRACT_HPP


/**
 * @file   evolution/core/base/Contract.hpp
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @brief  Assertion macros to implement Design By Contract in C/C++.
 *
 * ASSERT_OR_XXX MACROS
 *
 * These macros execute a simple assertion for debug builds, and allow
 * graceful error handling in release builds. The description for each
 * macro details the behaviour in a release build.
 *
 * As these are macros, the usual caveats apply concerning arguments
 * with side effects. The one exception to this rule is the set of
 * XXX_EXPR_XXX macros, which are intended to set a status/error code
 * upon an error event. Because these macros throw assertions in debug
 * builds, and because such a side effect is directly related to the
 * error event, this usage is safe and appropriate.
 *
 *
 * DESIGN BY CONTRACT MACROS
 *
 * These macros are simply aliases for the above ASSERT_OR_XXX macros,
 * named to indicate their use for clarity. The intended usage in a function
 * is as follows:
 * @code
 *   void foo()
 *   {
 *     PRECOND_XXX ...
 *     INVARIANT_XXX ...
 *
 *     Function body
 *
 *     INVARIANT_XXX ...
 *     POSTCOND_XXX ...
 *   }
 * @endcode
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */

#include <assert.h>



////////////////////////////////////////////////////////////
// Assertions
////////////////////////////////////////////////////////////

/**
 * @brief Just for completeness. Parameter <b>s</b> is the boolean
 * condition to assert on.
 *
 * @param s [in]; The boolean condition to assert on.
 *        
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef ASSERT
#define ASSERT(s)               assert (s)
#endif


/**
 * @brief Assert in debug builds; return from routine in release builds.
 *
 * @param s [in]; The boolean condition to assert on.
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define ASSERT_OR_RETURN(s)    assert(s) 
#else
#define ASSERT_OR_RETURN(s)    if ( !(s) ) { return; }
#endif


/**
 * @brief Assert in debug builds; return value in release builds
 *
 * @param s [in]; The boolean condition to assert on.
 * @param v [in]; The value to return if the assertion fails.
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define ASSERT_OR_RETURN_VALUE(s,v)    assert(s) 
#else
#define ASSERT_OR_RETURN_VALUE(s,v)    if ( !(s) ) { return v; }
#endif


/**
 * @brief Assert in debug builds; goto label in release builds. 
 * In C, unfortunately, it is a common pattern that you allocate 
 * several resources, then run into an error and need to perform 
 * cleanup. Using goto is the most fail-safe way out.  
 *
 * @param s [in]: The boolean condition to assert on.
 * @param l [in]: The label to go to if the assertion fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define ASSERT_OR_GOTO(s,l)    assert(s) 
#else
#define ASSERT_OR_GOTO(s,l)    if ( !(s) ) { goto l; }
#endif


/**
 * @brief Assert in debug builds; evaluate expression in release builds.
 *
 * @param s [in]: The boolean condition to assert on.
 * @param e [in]: The expression to evaluate if the assertion fails.
 * 
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define ASSERT_OR_EXPR(s,e)    assert(s) 
#else
#define ASSERT_OR_EXPR(s,e)    if ( !(s) ) { e; }
#endif


/**
 * @brief Assert in debug builds; evaluate expression and then goto label in release
 * builds. The intended use is to set an error flag and then jump to cleanup 
 * code.
 *
 * @param s [in]: The boolean condition to assert on.
 * @param e [in]: The expression to evaluate if the assertion fails.
 * @param l [in]: The label to go to if the assertion fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define ASSERT_OR_EXPR_GOTO(s,e,l)    assert(s) 
#else
#define ASSERT_OR_EXPR_GOTO(s,e,l)    if ( !(s) ) { e; goto l; }
#endif



////////////////////////////////////////////////////////////
// Design By Contract
////////////////////////////////////////////////////////////

/**
 * @brief A preconditon assert.
 * 
 * @param s [in]: The boolean pre-condition.
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define PRECOND(s)            assert(s)

/**
 * @brief Assert in debug builds; return from routine in release builds.
 *
 * @param s [in]: The boolean pre-condition.
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define PRECOND_RETURN(s)        ASSERT_OR_RETURN(s)

/**
 * @brief Assert in debug builds; return value in release builds.
 *
 * @param s [in]: The boolean pre-condition.
 * @param v [in]; The value to return if the pre-condition fails.
 * 
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define PRECOND_RETURN_VALUE(s,v)    ASSERT_OR_RETURN_VALUE(s,v)

/**
 * @brief Assert in debug builds; goto label in release builds.
 *
 * @param s [in]: The boolean pre-condition.
 * @param l [in]: The label to go to if the pre-condition fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define PRECOND_GOTO(s,l)        ASSERT_OR_GOTO(s,l)

/**
 * @brief Assert in debug builds; evaluate expression in release builds.
 *
 * @param s [in]: The boolean pre-condition.
 * @param e [in]: The expression to evaluate if the pre-condition fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define PRECOND_EXPR(s,e)        ASSERT_OR_EXPR(s,e)

/**
 * @brief Assert in debug builds; evaluate expression and then goto label in release
 * builds. The intended use is to set an error flag and then jump to cleanup 
 * code.
 *
 * @param s [in]: The boolean pre-condition.
 * @param e [in]: The expression to evaluate if the pre-condition fails.
 * @param l [in]: The label to go to if the pre-condition fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define PRECOND_EXPR_GOTO(s,e,l)    ASSERT_OR_EXPR_GOTO(s,e,l)


/**
 * @brief A postcondition assert.
 *
 * @param s [in]; The boolean post-condition.
 * 
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define POSTCOND(s)            assert(s)

/**
 * @brief Assert in debug builds; return from routine in release builds.
 *
 * @param s [in]; The boolean post-condition.
 * 
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define POSTCOND_RETURN(s)        ASSERT_OR_RETURN(s)

/**
 * @brief Assert in debug builds; return value in release builds.
 *
 * @param s [in]: The boolean post-condition.
 * @param v [in]; The value to return if the post-condition fails.
 * 
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define POSTCOND_RETURN_VALUE(s,v)    ASSERT_OR_RETURN_VALUE(s,v)

/**
 * @brief Assert in debug builds; goto label in release builds.
 *
 * @param s [in]: The boolean post-condition.
 * @param l [in]: The label to go to if the post-condition fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define POSTCOND_GOTO(s,l)        ASSERT_OR_GOTO(s,l)

/**
 * @brief Assert in debug builds; evaluate expression in release builds.
 *
 * @param s [in]: The boolean post-condition.
 * @param e [in]: The expression to evaluate if the post-condition fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define POSTCOND_EXPR(s,e)        ASSERT_OR_EXPR(s,e)

/**
 * @brief Assert in debug builds; evaluate expression and then goto label in release
 * builds. The intended use is to set an error flag and then jump to cleanup 
 * code.
 *
 * @param s [in]: The boolean post-condition.
 * @param e [in]: The expression to evaluate if the post-condition fails.
 * @param l [in]: The label to go to if the post-condition fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define POSTCOND_EXPR_GOTO(s,e,l)    ASSERT_OR_EXPR_GOTO(s,e,l)


/**
 * @brief An invariant assert.
 *
 * @param s [in]; The invariant condition.
 * 
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define INVARIANT(s)            assert(s)

/**
 * @brief Assert in debug builds; return from routine in release builds.
 *
 * @param s [in]; The invariant condition.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define INVARIANT_RETURN(s)        ASSERT_OR_RETURN(s)

/**
 * @brief Assert in debug builds; return value in release builds.
 *
 * @param s [in]; The invariant condition.
 * @param v [in]; The value to return if the invariant fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define INVARIANT_RETURN_VALUE(s,v)    ASSERT_OR_RETURN_VALUE(s,v)

/**
 * @brief Assert in debug builds; goto label in release builds.
 *
 * @param s [in]; The invariant condition.
 * @param l [in]; The label to go to if the invariant fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define INVARIANT_GOTO(s,l)        ASSERT_OR_GOTO(s,l)

/**
 * @brief Assert in debug builds; evaluate expression in release builds.
 *
 * @param s [in]; The invariant condition.
 * @param e [in]; The expression to evaluate if invariant fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define INVARIANT_EXPR(s,e)        ASSERT_OR_EXPR(s,e)

/**
 * @brief Assert in debug builds; evaluate expression and then goto label in release
 * builds. The intended use is to set an error flag and then jump to cleanup 
 * code.
 *
 * @param s [in]; The invariant condition.
 * @param e [in]; The expression to evaluate if invariant fails.
 * @param l [in]; The label to go to if the invariant fails.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define INVARIANT_EXPR_GOTO(s,e,l)    ASSERT_OR_EXPR_GOTO(s,e,l)


/**
 * @brief Designed for a C++ class that has an invariant function returning a bool.
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define CLASS_INVARIANT                 INVARIANT(invariant ())


/**
 * @brief Logical implication (=>). Useful for assertions.
 *
 * @param p [in]; Implication condition.
 * @param q [in]; What p implies.
 *
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#define IMPLIES(p,q)   ((p) ? (q) : true)


/**
 * @brief Declares a variable to save the old value of a variable. Used only
 * in debug builds.
 *
 * @param T [in]; The type of the old value.
 * @param x [in]; The name of the old value.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define DECLARE_OLD_VALUE(T,x)   T _dbc_old_##x = x;
#else
#define DECLARE_OLD_VALUE(T,x)
#endif


/**
 * @brief References the old value of a variable, which must have been
 * declared with DECLARE_OLD_VALUE. Used only in debug builds.
 *
 * @param x [in]; The name of the old value.
 * @ingroup core_base
 * @ingroup core_base_aibo
 */
#ifndef NDEBUG
#define OLD_VALUE(x)  (_dbc_old_##x)
#else
#define OLD_VALUE(x)
#endif


#endif  // INCLUDE_EVOLUTION_BASE_CONTRACT_HPP
