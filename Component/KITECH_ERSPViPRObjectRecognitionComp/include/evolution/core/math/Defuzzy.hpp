//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_FUZZY_DEFUZZY_HPP
#define INCLUDE_EVOLUTION_FUZZY_DEFUZZY_HPP

/**
 * @file    evolution/core/math/Defuzzy.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 *
 * Defuzzification methods.
 */

#include <math.h>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/math/MultiValue.hpp>
#include <evolution/core/math/MultiValueFunctions.hpp>


namespace Evolution
{

    /**
     * @brief Defuzzify the multivalue with the MAX method. The
     * multivalue must be 1-dimensional.
     *
     * @param   mv [in]; The multivalue
     * @param   value [out]; The defuzzified value
     * @return  RESULT_SUCCESS or some failure if defuzzification was not possible.
     * @pre     mv.get_dimensionality() == 1
     *
     * @ingroup core_math
     */
    inline Result defuzzify_max (const MultiValue & mv, double &value)
    {
        Interval _iv;
        int i          = 0;
        double _x      = 0; 
        double _y      = 0;
        double _start  = 0; 
        double _step   = 0;
        double _stop   = 0;
        double _max    = 0;
    
        _iv    =  mv.get_interval(0);
        _start = _iv.get_start();
        _step  = _iv.get_step();
        _stop  = _iv.get_stop();
    
        /* for each action/alternative */
        for (_x = _start, i = 0; _x <= _stop ; i++, _x += _step)
        {
            _y = mv (i);
            if (_y > _max)
            {
                _max = _y;
                value = _x;
            }
        }
        return RESULT_SUCCESS;
    }


    /**
     * @brief Defuzzify the multivalue with the centroid method. The
     * multivalue must be 1-dimensional.
     *
     * @param     mv [in]; The multivalue
     * @param     value [out]; The defuzzified value
     * @return  RESULT_SUCCESS or some failure if defuzzification was not possible.
     * @pre     mv.get_dimensionality() == 1
     *
     * @ingroup core_math
     */
    inline Result defuzzify_centroid (const MultiValue & mv, double &value)
    {
        Interval _iv;
        int i          = 0;
        double _x      = 0; 
        double _y      = 0;
        double _start  = 0; 
        double _step   = 0;
        double _stop   = 0;
        double _sum_xy = 0;
        double _sum_y  = 0;
    
        _iv    = mv.get_interval(0);
        _start = _iv.get_start();
        _step  = _iv.get_step();
        _stop  = _iv.get_stop();
    
        /* for each action/alternative */
        for (_x = _start, i = 0; _x <= _stop ; i++, _x += _step)
        {
            _y = mv(i);
            _sum_xy =  (_sum_xy) + (_y * _x);
            _sum_y  = (_sum_y) + (_y);
        }
    
//    std::cout << "SUMXY = " << _sum_xy << std::endl <<  "SUMY = " << _sum_y << std::endl;
        if (_sum_y)
            value = _sum_xy / _sum_y;
        else
            value = 0.0;

        return RESULT_SUCCESS;
    }


    /**
     * @brief Defuzzifies on the median of the set of values in the fuzzy set
     * that share the max value.  The multivalue must be 1 dimensional.
     * 
     * @param     mv [in]; The multivalue
     * @param     value [out]; The defuzzified value
     * @return  RESULT_SUCCESS or some failure if defuzzification was not possible.
     * @pre     mv.get_dimensionality() == 1
     *
     * @ingroup core_math
     */
    inline void defuzzify_median (MultiValue & mv, double &value)
    {
        PRECOND (mv.get_dimensionality () == 1);

        // Compute the global maximum on the set.
        double global_max;
        MultiValue::ConstIterator max_loc = multivalue_max (mv, &global_max);

        // Use the set median if no proper maximum.
        MultiValue::SizeType median_index;  // We have one-dimensional MultiValue.
        if (fabs (global_max) < 1E-10)
        {
            mv.raw_index_to_indices (mv.get_raw_size () / 2, &median_index);
            value = mv.index_to_point (median_index, 0);
            return;
        }

        // Build a set of raw indices that have the maximum value.
        MultiValue::SizeType * max_set =
            new MultiValue::SizeType[mv.get_raw_size ()];
        MultiValue::SizeType max_set_count = 0;
        double *current_data = mv.get_raw_data ();
        for (MultiValue::SizeType current_index = 0;
             current_index < mv.get_raw_size ();
             ++current_index, ++current_data)
        {
            if (fabs (*current_data - global_max) < 1E-10)
            {
                max_set[max_set_count++] = current_index;
            }
        }

        // Find the median of the maximum set.
        if (max_set_count > 2)
        {
            // Determine the index of the median.
            mv.raw_index_to_indices (max_set [max_set_count / 2], &median_index);
        }
        else
        {
            // Use the location of the maximum if it is less than 3 values.
            max_loc.get_current_indices (&median_index);
        }
        value = mv.index_to_point (median_index, 0);
        delete[] max_set;
    } // end defuzzy_median()

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_FUZZY_DEFUZZY_HPP
