//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_MATH_UTILS_HPP
#define INCLUDE_EVOLUTION_MATH_MATH_UTILS_HPP


/**
 * @file    evolution/core/math/MathUtils.hpp
 * @brief   Math helper functions
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_math
 * @ingroup core_math_aibo
 */


#include <math.h>
#include <float.h>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @brief Definition of Error Check Constants.
     **/
//#define ERROR_PERCENTAGE std::numeric_limits<double>::epsilon() @todo
#define ERROR_PERCENTAGE  1e-7
    /**
     * @brief Zero_eplison is for those numbers near zero.
     **/
#define ZERO_EPSILON      1e-13

    /**
     * @brief  Double equal checks because of double epsilon there is a
     *         need for these functions.
     *
     *         Checks first to see if both are within ZERO_EPSILON of
     *         zero (both effectively zero).  If this fails,
     *         then checks to see whether they are within
     *         ERROR_PERCENTAGE of the first argument.
     */

    /**
     * @brief Double_equal returns true if a,b are almost equal. 
     * @param  a [in]; Double to compare with b.
     * @param  b [in]; Double to compare with a.
     * @return True if a and b are double_equal.
     */
    EVOLUTION_EXPORT_MATH bool double_equal(double a,double b);

    /**
     * @brief Double_equal_array returns true if all elements in a,b are almost equal.
     * @param a [in]; Array of doubles to compare, elementwise against b
     * @param b [in]; Array of doubles to compare, elementwise against a
     * @return True if a and b are double_equal.
     */
    EVOLUTION_EXPORT_MATH bool double_array_equal(DoubleArray& a,DoubleArray& b);

    /** 
     * @brief Double_not_equal returns true if a, b are not close enough.
     * @param  a [in]; Double to compare with b.
     * @param  b [in]; Double to compare with a.
     * @return True if a and b are not double_equal.
     */
#define double_not_equal(a,b)   (!double_equal((a),(b))) 

    /** 
     * @brief Double_cut_equal returns true if difference between a,b is 
     *        less than c.
     * @param  a [in]; Double to compare with b.
     * @param  b [in]; Double to compare with a.
     * @param  c [in]; Threshold on the maximum difference between a and b..
     * @return True if (fabs ((a) - (b)) < (c))
     */
#define double_cut_equal(a,b,c) (fabs ((a) - (b)) < (c))

#ifndef ABS
    /** 
     * @brief Computes absolute value of the input.
     * @param  a [in]; Input Value
     * @return Absolute value of the input
     */
#define ABS(a)    (((a) < (0)) ? (-(a)) : (a))
#endif

#ifndef MAX
    /** 
     * @brief Computes maximum value of two double precission numbers.
     * @param  A [in]; Double to compare with B.
     * @param  B [in]; Double to compare with A.
     * @return Maximum value between a and b
     */
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#endif

#ifndef MIN
    /** 
     * @brief Computes minimum value of two double precission numbers.
     * @param  A [in]; Double to compare with B.
     * @param  B [in]; Double to compare with A.
     * @return Minimum value between a and b
     */
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#endif

#ifndef POW2
    /** 
     * @brief Computes the square of the input.
     * @param  X [in]; Input Value
     * @return Square of X: (X*X)
     */
#define POW2(X) ((X)*(X))
#endif

   /** 
    * @brief Converts mathematical expressions that include "PI" to Float.
    * @param str [in]; String encoding a mathematical expression that can include 'PI' instead of '3.1415etc'.
    * @return Double value of evaluated string
    */
    EVOLUTION_EXPORT_MATH double
    parse_pi_expressions (const char * str);

    // Definition of log and exp constants.
    /**
     * @brief Definition of the log(0.0).
     **/
#define LOG_ZERO     -1.0e10
    /**
     * @brief Minimum argument for exp() = log(MIN_LOG_ARG)
     **/
#define MIN_EXP_ARG  -690.776
    /**
     * @brief Minimum argument for log() = exp(MIN_EXP_ARG).
     **/
#define MIN_LOG_ARG   1e-300

    /** 
     * @brief Safe log computation
     * This log function will not kill log by giving it an argument less than the minimum log argument.
     * @param x [in]; Input value
     * @return LOG_ZERO if value of x is too small, otherwise log(x).
     */
    inline double evo_log (double x)
    {
        if (x < MIN_LOG_ARG)
        {
            return LOG_ZERO;
        }
        else
        {
            return (log (x));
        }
    }

    /** 
     * @brief Safe exponential.
     * This exp function will not kill exp by giving it less than the minimum argument to exp.
     * @param x [in]; Input value
     * @return 0 if value of x is too small, otherwise exp(x).
     */
    inline double evo_exp (double x)
    {
        if (x < MIN_EXP_ARG)
        {
            return 0.0;
        }
        else
        {
            return (exp (x));
        }
    }

    /** 
     * @brief Safe log addition.
     * This log function will properly do log_add, with no chance of killing log or exp in the process.
     * @param x [in]; Value to add to y
     * @param y [in]; Value to add to x
     * @return LOG_ZERO if log_add would otherwise result in an invalid value, otherwise log_add(x,y)
     */
    inline double evo_log_add (double x, double y)
    {
        double z;
        if (x >= y)
        {
            z = x + evo_log (1.0 + evo_exp (y - x));
        }
        else
        {
            z = y + evo_log (1.0 + evo_exp (x - y));
        }

        if (z <= LOG_ZERO)
        {
            return LOG_ZERO;
        }
        else
        {
            return z;
        }
    }


#ifndef SWAP
    /** 
     * @brief Swaps the contents of the input arguments.
     * @param  A [in]; Double to be swapped with B.
     * @param  B [in]; Double to be swapped with A.
     */
#define SWAP(A,B) {double t=(A);(A)=(B);(B)=t;}
#endif

    /**
     * @brief Finds the kth smallest element in the array.
     * 
     * @param a [in]; Input array
     * @param n [in]; Number of elements
     * @param k [in]; Rank k
     *
     * @return Kth smallest element
     *
     * @note The incoming array, a, is likely to be altered (elements
     * rearranged). Copy it to a tmp array before calling it if you 
     * want to preserve it.
     */
    EVOLUTION_EXPORT_MATH double 
    kth_smallest(double a[], int n, int k);

    // Conversion functions.
    /**
     * @brief Bounds an angle value in the range [-pi, +pi].
     * @param theta [in]; Input angle, any value
     * @return Input angle bound to [-pi, +pi].
     */
    inline double 
    angle_bound(const double theta)
    {
        int beta = (int)(theta / (2 * M_PI));
        double alpha = theta - ((double)beta * 2 * M_PI);
        
        if(alpha > M_PI)
        {
            alpha -= 2 * M_PI;
        }
        else if(alpha < -M_PI)
        {
            alpha += 2 * M_PI;
        }
        
        return alpha;
    }

    /**
     * @brief Angle difference between a and b.  Returns diff in [-pi, +pi]
     *  such that angle_bound(diff + b) = angle_bound(a).
     * @param a [in]; Input angle, any value
     * @param b [in]; Input angle, any value
     * @return Angle distance between a and b, bounded in [-pi, +pi]
     */
    template <class T>
    inline double angle_delta(T a, T b)
    {
        return angle_bound(a - b);
    }

    /**
     * @brief Changes the angle representation from radians to degrees.
     * @param theta [in]; Input angle, any value, in radians
     * @return Value of theta in degrees
     */
    template <class T>
    inline double rad_to_deg(T theta)
    {
        return theta*(180.0/M_PI);
    }

    /**
     * @brief Change the angle representation from degrees to radians.
     * @param theta [in]; Input angle, any value, in degrees
     * @return Value of theta in radians
     */
    template <class T>
    inline double deg_to_rad(T theta)
    {
        return theta*(M_PI/180);
    }

    /** 
     * @brief Returns true if angles are the same with different k's in
     *        alpha + k * 2 * PI.
     * @param alpha [in]; Input angle, any value, in radians
     * @param beta  [in]; Input angle, any value, in radians
     * @return Boolean value indicating equality or lack thereof
     */
    inline bool angle_equal(double alpha, double beta)
    {
        return (double_cut_equal(sin(alpha), sin(beta), 1e-7) && 
                double_cut_equal(cos(alpha), cos(beta), 1e-7));
    }


    /** 
     * @brief Rounding.
     * @param x [in]; Double value to round.
     * @return Rounded double value of x
     */
    inline double inline_round (double x)
    {
        long i = (long) x;
        double diff = x - i;
        if ((diff < 0.5) && (diff > -0.5))
        {
            return (i);
        }
        else
        {
            return (diff < 0 ? i - 1 : i + 1);
        }
    } // End inline_round().


    /** 
     * @brief Rounding
     * @param x [in]; Float value to round
     * @return Rounded float value of x
     */
    inline float inline_roundf (float x)
    {
        int i = (int) x;
        float diff = x - i;
        if ((diff < 0.5) && (diff > -0.5))
        {
            return ((float) i);
        }
        else
        {
            return ((float)(diff < 0 ? i - 1 : i + 1));
        }
    } // End inline_roundf().

} // End namespace Evolution.

#ifdef EVOLUTION_PLATFORM_WIN32
// Win32 doesn't believe in rounding. @todo
///EVOLUTION_EXPORT_MATH double round  (double x) { return Evolution::inline_round  (x); } @todo
///EVOLUTION_EXPORT_MATH float  roundf (float  x) { return Evolution::inline_roundf (x); } @todo
EVOLUTION_EXPORT_MATH double round  (double x);
EVOLUTION_EXPORT_MATH float  roundf (float  x);
#endif

#endif // end INCLUDE_EVOLUTION_MATH_MATH_UTILS_HPP
