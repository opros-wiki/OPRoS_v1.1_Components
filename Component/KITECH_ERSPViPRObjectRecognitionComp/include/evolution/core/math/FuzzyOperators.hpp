//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_FUZZY_OPERATORS_HPP
#define INCLUDE_EVOLUTION_FUZZY_OPERATORS_HPP


/**
 * @file    evolution/core/math/FuzzyOperators.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @brief   Fuzzy logic functions.  And, or, not, etc.
 *
 * Common fuzzy inference operators.
 *
 * @see  The fuzzy library README.
 */


#include <float.h>
#include <vector>


namespace Evolution
{

    /**
     * @brief
     * Aggregates the given fuzzy rules using the MAX operator.
     * @param rule_count [in]; number of rules
     * @param rule_set [in]; value of rules, as double *
     * @return aggregated value.
     * @ingroup core_math
     */
    inline double
    fuzzy_aggregate (unsigned long rule_count, const double *rule_set)
    {
        //std::cout << "AGGREGATE METHOD" << std::endl;
        double max = rule_set[0];
        for (unsigned long i = 1; i < rule_count; i++)
        {
            if (rule_set[i] > max)
            {
                max = rule_set[i];
            }
        }
        return max;
    } // end fuzzy_aggregate()

    /**
     * @brief
     * Aggregates the given vector of fuzzy rules using the MAX
     * operator.
     * @param rule_set [in]; the vector encoding the rules.
     * @return aggregated value.
     *
     * @ingroup core_math
     */
    inline double fuzzy_aggregate (const std::vector < double >&rule_set)
    {
        unsigned long rule_count = rule_set.size ();
        double max = rule_set[0];
        for (unsigned long i = 1; i < rule_count; i++)
        {
            if (rule_set[i] > max)
            {
                max = rule_set[i];
            }
        }
        return max;
    } // end fuzzy_aggregate()

    /**
     * @brief
     * Aggregates the given vector of fuzzy rules using the AVERAGE
     * operator.
     * @param rule_set [in]; the vector encoding the rules.
     * @return aggregated value.
     *
     * @ingroup core_math
     */
    inline double
    fuzzy_aggregate_average (const std::vector < double >&rule_set)
    {
        unsigned long rule_count = rule_set.size ();
        if (rule_count == 0)
        {
            return 0.0;
        }

        double value = rule_set[0];
        for (unsigned long i = 1; i < rule_count; i++)
        {
            value += rule_set[i];
        }
        return (value / (double) rule_count);
    } // end fuzzy_aggregate_average()

    ////////////////////////////////////////////////////////////
    // Fuzzy inferencing operators  
    ////////////////////////////////////////////////////////////

    /** 
     * @brief  Minimum.
     *
     * @param x [in]; x value
     * @param y [in]; y value
     * @return min(x,y)
     * @ingroup core_math
     */
    inline double fuzzy_min (double x, double y)
    {
        return (double) ((x <= y) ? (x) : (y));
    } // end fuzzy_min()

    /** 
     * @brief  Maximum.
     *
     * @param x [in]; x value
     * @param y [in]; y value
     * @return max(x,y)
     * @ingroup core_math
     */
    inline double fuzzy_max (double x, double y)
    {
        return (double) ((x >= y) ? (x) : (y));
    } // end fuzzy_max()

    /** 
     * @brief  Fuzzy NOT operator.
     * @param x [in]; x value
     * @return (1.0 - x)
     *
     * @ingroup core_math
     */
    inline double fuzzy_not (double x)
    {
        return (1.0 - x);
    } // end fuzzy_not()

    /** 
     * @brief  Fuzzy OR operator.
     * @param x [in]; x value
     * @param y [in]; y value
     * @return max(x,y)
     *
     * @ingroup core_math
     */
    inline double fuzzy_or (double x, double y)
    {
        return fuzzy_max (x, y);
    } // end fuzzy_or()

    /** 
     * @brief  Fuzzy AND operator.
     * @param x [in]; x value
     * @param y [in]; y value
     * @return min(x,y)
     *
     * @ingroup core_math
     */
    inline double fuzzy_and (double x, double y)
    {
        return fuzzy_min (x, y);
    } // end fuzzy_and()

    /** 
     * @brief  Fuzzy PRODUCT operator.  
     * @param x [in]; x value
     * @param y [in]; y value
     * @return x * y
     *
     * @ingroup core_math
     */
    inline double fuzzy_product (double x, double y)
    {
        return (x * y);
    } // end fuzzy_product()

    /** 
     * @brief Fuzzy IFTHEN operator. 
     * @param condition   [in]; x value
     * @param consequence [in]; y value
     * @return min(condition, consequence)
     *
     * @ingroup core_math
     */
    inline double fuzzy_ifthen (double condition, double consequence)
    {
        return fuzzy_min (condition, consequence);
    } // end fuzzy_ifthen()

} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_FUZZY_OPERATORS_HPP
