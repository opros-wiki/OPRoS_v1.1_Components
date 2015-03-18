//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file     evolution/core/resource/Colorspace.hpp
 * @brief    This class provides an interface for Colorspace definition and conversion.
 * @author   Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/


#ifndef INCLUDE_EVOLUTION_CORE__RESOURCE_COLORSPACE_HPP
#define INCLUDE_EVOLUTION_CORE__RESOURCE_COLORSPACE_HPP


#include <deque>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/MathUtils.hpp>


namespace Evolution
{


    /**
     * @brief This a list of Colorspaces.
     *
     * @ingroup core_vision 
     * @ingroup core_vision_aibo
     **/
    enum Colorspace
    {
        COLORSPACE_UNSPECIFIED = 0x00,                         ///< Unspecified type.
        COLORSPACE_GRAYSCALE   = 1 << (sizeof(int) * 8 - 2),   ///< Grayscale.
        COLORSPACE_RGB         = 1 << (sizeof(int) * 8 - 3),   ///< RGB format.
        COLORSPACE_YUV         = 1 << (sizeof(int) * 8 - 4),   ///< YUV format.
        COLORSPACE_YCBCR       = 1 << (sizeof(int) * 8 - 5),   ///< YCbCr format.
        COLORSPACE_VWX         = 1 << (sizeof(int) * 8 - 6),   ///< VWX format.
        COLORSPACE_MASK        = (COLORSPACE_GRAYSCALE |
                                  COLORSPACE_YCBCR     |
                                  COLORSPACE_YUV       |
                                  COLORSPACE_RGB       |
                                  COLORSPACE_VWX)
    };


    /**
     * @brief This is a list of color format definitions.
     *
     * @ingroup core_vision 
     * @ingroup core_vision_aibo
     **/    
    enum ColorFormat
    {
        COLOR_FORMAT_UNSPECIFIED = COLORSPACE_UNSPECIFIED,     ///< Default value.

        // YCbCr
        COLOR_FORMAT_IYUV      = COLORSPACE_YCBCR | 0x00,      ///< Planar YUV 4:2:2
        COLOR_FORMAT_IYU1      = COLORSPACE_YCBCR | 0x01,      ///< Packed YUV 4:1:1 horizontal (UYYVYY)
        COLOR_FORMAT_IYU2      = COLORSPACE_YCBCR | 0x02,      ///< Packed YUV 4:4:4 (UYV)
        COLOR_FORMAT_UYVY      = COLORSPACE_YCBCR | 0x03,      ///< Packed YUV 4:2:2 horizontal
        COLOR_FORMAT_YVU9      = COLORSPACE_YCBCR | 0x04,      ///< Planar YUV 4:1:1 (Y, V, U)
        COLOR_FORMAT_YV12      = COLORSPACE_YCBCR | 0x05,      ///< Planar YUV 4:2:2 (Y, U, V)
        COLOR_FORMAT_I420      = COLORSPACE_YCBCR | 0x06,      ///< Same as IYUV

        // YUV
        // This is an invented format, using values that are actually
        // in the YUV colorspace, as opposed to the YCbCr colorspace
        // used by all the above alleged YUV formats from the cameras.
        COLOR_FORMAT_YUV       = COLORSPACE_YUV | 0x00,        ///< Real YUV planar format (not YCbCr) 4:4:4

        // RGB
        COLOR_FORMAT_RGB24     = COLORSPACE_RGB | 0x00,  ///< RGB 3bpp interleaved.
        COLOR_FORMAT_RGB32     = COLORSPACE_RGB | 0x01,  ///< 4-byte aligned 24-bit RGB

        // Mono.
        COLOR_FORMAT_GRAYSCALE    = COLORSPACE_GRAYSCALE   | 0x00, ///< Friendly name for grayscale.
        COLOR_FORMAT_Y800         = COLOR_FORMAT_GRAYSCALE,        ///< Single-byte Luminance (mono)
        COLOR_FORMAT_GRAYSCALE_16 = COLOR_FORMAT_GRAYSCALE | 0x01, ///< Friendly name for grayscale with 16 bpp.
        COLOR_FORMAT_Y1600        = COLOR_FORMAT_GRAYSCALE_16,     ///< Two-byte Luminance (mono)     

        // VWX
        COLOR_FORMAT_VWX       = COLORSPACE_VWX | 0x00,  ///< VWX 3bpp interleaved.

        // Number of distinct color values (e.g COLOR_FORMAT_GRAYSCALE,
        // COLOR_FORMAT_GRAYSCALE_16, COLOR_FORMAT_Y800 and COLOR_FORMAT_Y1600
        // count as 1).
        COLOR_FORMAT_COUNT     = 14
    };


    /**
     * @brief This is  a list of color formats.
     */
    typedef std::deque<ColorFormat> ColorFormatList;


    /**
     * @class ColorFormatDesc
     * @brief This is a description of a color format.
     */
    struct EVOLUTION_EXPORT_RESOURCE ColorFormatDesc
    {
        ColorFormat  format;         ///< Color format.
        Colorspace   space;          ///< Colorspace
        const char*  name;           ///< Description
        unsigned     bpp;            ///< Bits per pixel
        bool         planar;         ///< Planar layout?
        unsigned     x_sampling[3];  ///< X sampling period for 3 color values, ordered as RGB or YUV.
        unsigned     y_sampling[3];  ///< Y sampling period for 3 color values, ordered as RGB or YUV.
    }; // end struct ColorFormatDesc
 

    /**
     * @brief This is a list of criteria for matching a color format.
     */
    enum ColorMatchCriterion
    {
        COLOR_MATCH_EXACT  = 0x00,   ///< Match the format exactly
        COLOR_MATCH_SPACE  = 0x01,   ///< Match the color space
        COLOR_MATCH_BPP    = 0x02,   ///< Match the bits per pixel
        COLOR_MATCH_LAYOUT = 0x04    ///< Match the layout, packed vs. planar
    };


    /**
     * @brief  These are colorspace utilities.
     *
     * @ingroup core_vision
     * @ingroup core_vision_aibo
     **/
    namespace ColorspaceUtils
    {


        ////////////////////////////////////////////////////////////
        // Queries
        ////////////////////////////////////////////////////////////

        /** 
         * @brief This function computes the colorspace of the specified color format.
         *
         * Computes the colorspace of the specified color format.
         * @param format [in]; ColorFormat to find the Colorspace of.
         * @return Colorspace that corresponds to the ColorFormat
         */
        inline Colorspace
        get_format_colorspace (ColorFormat format)
        {
            return Colorspace(format & COLORSPACE_MASK);
        } // end get_format_colorspace()

        /**
         * @brief This looks up the description of the color format.
         *
         * Looks up the description of the color format.
         * @param format [in]; ColorFormat to find description of.
         * @return Description of the ColorFormat
         */
        EVOLUTION_EXPORT_RESOURCE
        const ColorFormatDesc* get_format_description (ColorFormat format);

        /**
         * @brief This returns the name of the color format.
         *
         * Returns the name of the color format.
         * @param format [in]; ColorFormat to find name of
         * @return name of the ColorFormat passed in
         */
        inline const char*
        format_to_string (ColorFormat format)
        {
            const ColorFormatDesc* desc = get_format_description (format);
            return (desc ? desc->name : "Unknown format");
        }

        /**
         * @brief This parses the string name into a format code, returning
         * COLOR_FORMAT_UNSPECIFIED if not found.
         *
         * @note In general, string names are the XXX portion of the
         * COLOR_FORMAT_XXX.
         * @param str [in]; string to match with a ColorFormat
         * @return ColorFormat associated with the input string.
         **/
        EVOLUTION_EXPORT_RESOURCE
        ColorFormat string_to_format (const char* str);

        /**
         * @brief This function computes the bits per pixel in the specified color format.
         *
         * Computes the bits per pixel in the specified color format.
         * @param format [in]; Color format to find bpp of.
         * @return Number of bits per pixel for format.
         */
        inline unsigned
        calculate_format_bpp (ColorFormat format)
        {
            switch (format)
            {
            case COLOR_FORMAT_IYUV:         return 12;
            case COLOR_FORMAT_YV12:         return 12;
            case COLOR_FORMAT_IYU1:         return 12;
            case COLOR_FORMAT_I420:         return 12;
            case COLOR_FORMAT_YVU9:         return  9;
            case COLOR_FORMAT_IYU2:         return 24;
            case COLOR_FORMAT_UYVY:         return 16;
            case COLOR_FORMAT_YUV:          return 24;
            case COLOR_FORMAT_RGB24:        return 24;
            case COLOR_FORMAT_RGB32:        return 32;
            case COLOR_FORMAT_GRAYSCALE:    return  8;
            case COLOR_FORMAT_GRAYSCALE_16: return 16;
            case COLOR_FORMAT_VWX:          return 24;
            default:
            {
                const ColorFormatDesc* desc = get_format_description (format);
                return (desc ? desc->bpp : 0);
            }
            } // end switch
        } // end calculate_format_bpp()


        /**
         * @brief This function calculates the size of buffer needed to hold an image with the
         * given dimensions and colorspace.
         *
         * @param  width  [in]; width in pixels
         * @param  height [in]; height in pixels
         * @param  format [in]; requested color format
         * @return Size of buffer that will hold width/heigh/format
         */
        inline size_t
        calculate_buffer_size (unsigned width, unsigned height, ColorFormat format)
        {
            const ColorFormatDesc* desc = get_format_description (format);
            if (desc == NULL)
            {
                return 0;
            }
            // Calculate alignment of dimensions.
            unsigned width_align = 1, height_align = 1;
            for (short i = 0; i < 3; ++i)
            {
                if (desc->x_sampling[i] > width_align)
                {
                    width_align = desc->x_sampling[i];
                }
            }
            for (short j = 0; j < 3; ++j)
            {
                if (desc->y_sampling[j] > height_align)
                {
                    height_align = desc->y_sampling[j];
                }
            }

            // Round up dimensions as necessary.
            unsigned width_mod  = width  % width_align;
            unsigned height_mod = height % height_align;
            if (width_mod)
            {
                width += width_align - width_mod;
            }
            if (height_mod)
            {
                height += height_align - height_mod;
            }
            return (width * height * desc->bpp / 8);
        } // end calculate_buffer_size()


        /**
         * @brief This finds all matching formats according to the search.
         *
         * Finds all matching formats according to the search
         * criteria, a set of ColorMatchCriterion values.
         *
         * @note If search_criteria == COLOR_MATCH_EXACT, only the exact
         * format will be returned (not extremely useful, but here for
         * completeness; exact matching is needed in other contexts).
         *
         * In any case, the search_format value will always be the
         * first entry in the returned match_list.
         *
         * @param  search_format   [in]; format to search with
         * @param  search_criteria [in]; ColorMatchCriterion value
         * @param  match_list      [out]; list of matching ColorFormats
         **/
        EVOLUTION_EXPORT_RESOURCE 
        void find_matching_format (ColorFormat search_format,
                                   unsigned search_criteria,
                                   ColorFormatList* match_list);

        /**
         * @brief This obtains a list of all formats.
         *
         * Get a list of all ColorFormats in ERSP as configured..
         *
         * @param  format_list [out]; list of all ColorFormat types in ERSP
         **/
        EVOLUTION_EXPORT_RESOURCE
        void get_all_formats (ColorFormatList* format_list);
        
    } // end namespace ColorspaceUtils


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE__RESOURCE_COLORSPACE_HPP
