//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVO_MATH_MULTI_VALUE_FUNCTIONS_HPP
#define INCLUDE_EVO_MATH_MULTI_VALUE_FUNCTIONS_HPP


/**
 * @file    evolution/core/math/MultiValueFunctions.hpp
 * @brief   General functions operating on MultiValue objects.
 * @see Evolution::Multivalue
 */


#include <float.h>
#include <evolution/core/math/MultiValue.hpp>


namespace Evolution
{

    /**
     * @brief Computes the maximum value, returning an iterator that
     * points to the first occurrence of that value.
     *
     * From the iterator, the dimensional indices can be determined.
     *
     * @param mv [in]; MultiValue from which to compute the maximum value.
     * @param max_value [out]; Maximum value
     *
     * @return Iterator that points to the first occurrence of the
     * maximum value.
     *
     * @ingroup core_math
     */
    inline MultiValue::ConstIterator 
    multivalue_max (const MultiValue& mv, double* max_value)
    {
        const double* current_data = mv.get_raw_data ();
        const double* max_data = current_data;
        *max_value = *max_data;
        for (MultiValue::SizeType raw_index = 0; 
             raw_index < mv.get_raw_size ();
             ++raw_index, ++current_data)
        {
            if (*current_data > *max_data)
            {
                max_data = current_data;
                *max_value = *max_data;
            }
        }
        return (MultiValue::ConstIterator ((MultiValue&) mv, (double*)max_data));
    } // end multivalue_max()

} // end namespace Evolution

#endif // INCLUDE_EVO_MATH_MULTI_VALUE_FUNCTIONS_HPP
