//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/behavior/FuzzyBehavior.hpp
 * @brief   Useful macros to help with a behavior using fuzzy logic.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 *
 * These macros alleviate the need to write fuzzy behaviors,
 * which, by their very nature, require numerous membership set
 * parameters and accessor methods to get and set them. With these
 * macros, it is trivial to declare these methods and data
 * members, and to access them.
 *
 *
 * To declare a fuzzy variable for your behavior class:
 *
 * @code
 * DECLARE_FUZZY_VARIABLE (high_speed);
 * @endcode
 *
 * [put this at the end of the class declaration]
 *
 *
 * To set the membership values for the variables, use
 * FUZZY_MEMBERSHIP_SET(). Within the class itself:
 *
 * @code
 * FUZZY_MEMBERSHIP_SET (high_speed, 25.0, 10.0);
 * @endcode
 *
 * From outside the class:
 *
 * @code
 * fuzzy_behavior.FUZZY_MEMBERSHIP_SET (high_speed, 25.0, 10.0);
 * @endcode
 *
 *
 * To get the membership values for the variables, use
 * FUZZY_MEMBERSHIP_GET():
 *
 * @code
 * double high_t, high_w;
 * fuzzy_behavior.FUZZY_MEMBERSHIP_GET (high_speed, high_t, high_w);
 * @endcode
 *
 *
 * From inside the class, you can do this more efficiently with
 * FUZZY_MEMBER:
 *
 * @code
 * fuzzy_high (velocity, FUZZY_MEMBER_T(high_speed), FUZZY_MEMBER_W(high_speed));
 * @endcode
 *
 * [remember fuzzy_high() is standard fuzzy function]
 *
 *
 * @ingroup core_behavior
 */


////////////////////////////////////////////////////////////
// Accessor methods
////////////////////////////////////////////////////////////

/**
 * @brief
 * Declares and defines a get_XXX accessor method for the specified
 * fuzzy variable.
 *
 * The method defined by this macro should be used through the
 * FUZZY_MEMBERSHIP_GET macro.
 *
 * Generally, this macro does not need to be used directly;
 * DECLARE_FUZZY_VARIABLE should be used instead.
 *
 * @see  FUZZY_MEMBERSHIP_GET, DECLARE_FUZZY_VARIABLE
 *
 * @ingroup core_behavior
 */
#define DECLARE_FUZZY_MEMBERSHIP_GET(xxx) \
    void get_fuzzy_##xxx##_membership (double& t, double& w) const \
    {  t = _fuzzy_##xxx##_t; w = _fuzzy_##xxx##_w; }


/**
 * @brief
 * Calls the get_XXX accessor method for the specified fuzzy variable,
 * as defined by DECLARE_FUZZY_MEMBERSHIP_GET.
 *
 * @see  DECLARE_FUZZY_MEMBERSHIP_GET
 *
 * @ingroup core_behavior
 */
#define FUZZY_MEMBERSHIP_GET(xxx, t, w)  get_fuzzy_##xxx##_membership (t, w)


/**
 * @brief
 * Declares and defines a set_XXX accessor method for the specified
 * fuzzy variable.
 *
 * The method defined by this macro should be used through the
 * FUZZY_MEMBERSHIP_SET macro.
 *
 * Generally, this macro does not need to be used directly;
 * DECLARE_FUZZY_VARIABLE should be used instead.
 *
 * @see  FUZZY_MEMBERSHIP_SET, DECLARE_FUZZY_VARIABLE
 *
 * @ingroup core_behavior
 */
#define DECLARE_FUZZY_MEMBERSHIP_SET(xxx) \
    void set_fuzzy_##xxx##_membership (double t, double w) \
    {  _fuzzy_##xxx##_t = t; _fuzzy_##xxx##_w = w; }


/**
 * @brief
 * Calls the set_XXX accessor method for the specified fuzzy variable,
 * as define by DECLARE_FUZZY_MEMBERSHIP_SET.
 *
 * @see  DECLARE_FUZZY_MEMBERSHIP_SET
 *
 * @ingroup core_behavior
 */
#define FUZZY_MEMBERSHIP_SET(xxx, t, w)  set_fuzzy_##xxx##_membership (t, w)



////////////////////////////////////////////////////////////
// Member data
////////////////////////////////////////////////////////////

/**
 * @brief
 * Defines the member data within a class for the fuzzy variable of the
 * given name.
 *
 * The member data defined by this macro for the fuzzy set's t and w
 * can be accessed through the FUZZY_MEMBER_T and FUZZY_MEMBER_W
 * macros, respectively.
 *
 * Generally, this macro does not need to be used directly;
 * DECLARE_FUZZY_VARIABLE should be used instead.
 *
 * @see DECLARE_FUZZY_VARIABLE, FUZZY_MEMBER_T, FUZZY_MEMBER_W
 *
 * @ingroup core_behavior
 */
#define DECLARE_FUZZY_MEMBER_DATA(xxx) \
   double  _fuzzy_##xxx##_t;  /**< Start point for fuzzy set 'xxx' */ \
   double  _fuzzy_##xxx##_w;  /**< Width for fuzzy set 'xxx' */


/**
 * @brief
 * For the specified fuzzy variable, accesses the t parameter of the
 * fuzzy set, from the member data defined by
 * DECLARE_FUZZY_MEMBER_DATA.
 *
 * @see  DECLARE_FUZZY_MEMBER_DATA
 *
 * @ingroup core_behavior
 */
#define FUZZY_MEMBER_T(xxx)  (_fuzzy_##xxx##_t)


/**
 * @brief
 * For the specified fuzzy variable, accesses the t parameter of the
 * fuzzy set, from the member data defined by
 * DECLARE_FUZZY_MEMBER_DATA.
 *
 * @see  DECLARE_FUZZY_MEMBER_DATA
 *
 * @ingroup core_behavior
 */
#define FUZZY_MEMBER_W(xxx)  (_fuzzy_##xxx##_w)



////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////

/**
 * @brief
 *
 * Creates a fuzzy variable within a fuzzy behavior class, declaring
 * and defining membership accessor methods and member data for the
 * variable.
 *
 * This macro uses DECLARE_FUZZY_MEMBERSHIP_GET,
 * DECLARE_FUZZY_MEMBERSHIP_SET, DECLARE_FUZZY_MEMBER_DATA.
 *
 * @see DECLARE_FUZZY_MEMBERSHIP_GET, DECLARE_FUZZY_MEMBERSHIP_SET,
 *      DECLARE_FUZZY_MEMBER_DATA
 *
 * @ingroup core_behavior
 */
#define DECLARE_FUZZY_VARIABLE(xxx) \
public: \
   DECLARE_FUZZY_MEMBERSHIP_GET(xxx) \
   DECLARE_FUZZY_MEMBERSHIP_SET(xxx) \
protected: \
   DECLARE_FUZZY_MEMBER_DATA(xxx)
