//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_UNITS_HPP
#define INCLUDE_EVOLUTION_BASE_UNITS_HPP

/**
 * @file   evolution/core/base/Units.hpp
 * @brief  Functions to convert units.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Units
 **/

#include <map>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    // Types.
    /**
     * @brief
     * Enumeration defining unit types.
     * @ingroup core_base
     */
    typedef enum
    {
        UNIT_DISTANCE   = 0, ///< e.g. ft, cm
        UNIT_ANGLE      = 1, ///< e.g. rad, deg
        UNIT_TIME       = 2  ///< e.g. sec, hr
    } UnitType;

    /**
     * @brief
     * A map to hold conversion values from different units.
     * @ingroup core_base
     */
    typedef std::map<String, double> ConversionTable;

    /**
     * @class Units
     * @brief
     * A namespace containing unit handling and conversion methods.
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE Units
    {
    public:

        // Accessors. Default units.

        /**
         * @brief
         * Sets the default units.
         *
         * Example of use:<BR>
         * <BR>
         * // Set the default angle unit to degrees. <BR>
         * Result result = Units::set_default_units(UNIT_ANGLE, "degrees");<BR>
         * 
         * @param type [in]; The unit type to set.
         * @param units [in]; The string identifying the default unit for the specified type.
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        static 
        Result set_default_units (UnitType type, String units);

        /**
         * @brief
         * Gets the default units.
         *
         * Example of use:<BR>
         * <BR>
         * // Get the default angle unit.<BR>
         * String angle_units;<BR>
         * Result result = Units::get_default_units(UNIT_ANGLE, &angle_units);<BR>
         *<BR>
         * The value returned in angle_units should identify the current
         * default angle unit (degrees, radians, etc.).<BR>
         * 
         * @param type [in]; The type of default unit to get.
         * @param output_units [out]; The string identifying the default unit for the specified type.
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        static 
        Result get_default_units (UnitType type, String* output_units);

        // Accessors. Conversion routines.

        /**
         * @brief
         * Converts value associated w/specified units into default units.
         *
         * Example of use:<BR>
         * <BR>
         * // Set default distance units to meter.<BR>
         * Result result = Units::set_default_units(UNIT_DISTANCE, "meter");<BR>
         * if (result != RESULT_SUCCESS) { handle error... }<BR>
         * double conversion_factor;<BR>
         * // Get the conversion factor between cm and the current default<BR>
         * // distance unit (meter).<BR>
         * result = Units::convert_to_default_units(UNIT_DISTANCE, "cm", 1,
         *      &conversion_factor);<BR>
         * // Conversion factor should now be 0.01.<BR>
         * assert(conversion_factor == 0.01);<BR>
         *
         * @param type [in]; The type of unit to convert.
         * @param input_units [in]; The unit to convert from.
         * @param input [in]; The unit value to convert.
         * @param output [out]; The converted value in default units.     
         *
         * @return RESULT_SUCCESS on success, or error code on failure.
         */
        static 
        Result convert_to_default_units (UnitType type, String input_units,
                                         double input,  double* output);

        /**
         * @brief
         * Converts value associated w/default units into specified units
         *
         * Example of use:<BR>
         * <BR>
         * // Set default distance units to meter.<BR>
         * Result result = Units::set_default_units(UNIT_DISTANCE, "meter");<BR>
         * if (result != RESULT_SUCCESS) { handle error... }<BR>
         * double conversion_factor;<BR>
         * // Get the conversion factor from the current default distance<BR>
         * // unit (meter) to the specified unit (cm).<BR>
         * result = Units::convert_to_specified_units(UNIT_DISTANCE, "cm", 1,
         *      &conversion_factor);<BR>
         * // Conversion factor should now be 100.<BR>
         * assert(conversion_factor == 100);<BR>
         *
         * @param type [in]; The type of unit to convert.
         * @param output_units [in]; The unit to which to convert.
         * @param input [in]; The unit value to convert.
         * @param output [out]; The converted value in default units.     
         *
         * @return RESULT_SUCCESS on success, or error code on failure.
         */
        static  
        Result convert_to_specified_units (UnitType type, String output_units,
                                           double input,  double* output);

        /**
         * @brief
         * Convert the first specified units into the second specified units.
         *
         * Example of use:<BR>
         * <BR>
         * double radian_value;<BR>
         * // Convert 60 degrees to radians.<BR>
         * result = Units::convert_units(UNIT_ANGLE, "degree", 60, "radian",
         *      &radian_value);<BR>
         *
         * @param type [in]; The type of unit to convert.
         * @param input_units [in]; The unit to convert from.
         * @param input [in]; The unit value to convert.
         * @param output_units [in]; The unit to convert to.
         * @param output [out]; The converted value in output units.     
         *
         * @return RESULT_SUCCESS on success, or error code on failure.
         */
        static 
        Result convert_units (UnitType type, String input_units, double input,
                              String output_units, double* output);

        /**
         * @brief
         * Validate if a unit string specifies a known unit.
         *
         * @param type [in]; The type of unit to validate.
         * @param input_units [in]; The unit string to validate.
         *
         * @return true if the input_units string describe a unit known to
         * ERSP.
         */
        static 
        bool are_valid_units (UnitType type, String input_units);

    }; // end Units


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_BASE_UNITS_HPP
