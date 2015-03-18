//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE_RASTER_MAP_HPP
#define INCLUDE_EVOLUTION_RESOURCE_RASTER_MAP_HPP


/**
 * @file   evolution/core/resource/RasterMap.hpp
 * @brief  A class encapsulating a raster map.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::RasterMap
 */


#include <evolution/core/base/Types.hpp>

namespace Evolution
{


    /**
     * @class RasterMap
     *
     * @brief This class defines a type to store range information. 
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE RasterMap
    {
    public: // Constants

        /**
         * @brief RGB/BGR Color model
         */
        enum ColorModel
        {
            NO_MODEL,
            BGR_24,
            RGB_24,
            MODEL_COUNT
        };

    public: // Structors

        /**
         * @brief Constructor.
         */
        RasterMap ();

        /**
         * @brief Constructor.
         *
         * @param model  [in]; The color model representation
         * @param width  [in]; Width of the rater map
         * @param height [in]; Height of the raster map
         */
        RasterMap (ColorModel model,
                   unsigned long width,
                   unsigned long height);

        /**
         * @brief Copy constructor.
         *
         * @param rhs [in]; The RasterMap to copy from
         */
        RasterMap (const RasterMap & rhs);

        /**
         * @brief Destructor
         */
        virtual ~RasterMap ();

    public: // Operators

        /**
         * @brief RasterMap assignment operator
         *
         * @param rhs [in]; The RasterMap to copy from
         *
         * @return A reference to the copied object.
         */
        const RasterMap & operator= (const RasterMap & rhs);

    public: // Helpers

        /**
         * @brief Change color model
         *
         * @param model  [in]; The new ColorModel
         * @param width  [in]; The new width of the Rastermap
         * @param height [in]; The new height of the Rastermap
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result change (ColorModel model,
                               unsigned long width,
                               unsigned long height);

        /**
         * @brief Set rastermap data.
         *
         * @param data [in]; The data to copy from 
         */
        virtual void set_data (const unsigned char *data);

        /**
         * @brief Return a copy of the ColorModel.
         *
         * @return A copy of the ColorModel.
         */
        const ColorModel get_color_model () const
        {
            return _color_model;
        }

        /**
         * @brief Get width of rastermap.
         *
         * @return The width of the rastermap.
         */
        unsigned long get_width () const
        {
            return _width;
        }

        /**
         * @brief Get height of rastermap.
         *
         * @return The height of the rastermap.
         */
        unsigned long get_height () const
        {
            return _height;
        }

        /**
         * @brief Get data size (in bytes) of the rastermap.
         *
         * @return The size (in bytes) of the rastermap.
         */
        unsigned long get_data_size () const
        {
            return _data_size;
        }

        /**
         * @brief Get rastermap data.
         *
         * @return The raw rastermap data buffer.
         */
        unsigned char *get_data () const
        {
            return _data;
        }

    protected: // Helpers

        /**
         * @brief Allocates rastermaps.
         *
         * @param width          [in]; Width of the rater map
         * @param height         [in]; Height of the raster map
         * @param color_planes   [in]; Number of bit planes
         * @param bits_per_plane [in]; Bits per color plane
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result allocate (unsigned long width,
                                 unsigned long height,
                                 unsigned long color_planes,
                                 unsigned long bits_per_plane);

    protected: // Data

        ColorModel    _color_model;    ///< Color model of rastermap
        unsigned long _width;          ///< Width of rastermap
        unsigned long _height;         ///< Height of rastermap
        Timestamp     _timestamp;      ///< Timestamp 
        unsigned long _data_size;      ///< Data size of ratermap
        unsigned char *_data;          ///< Rastermap data
        unsigned char _color_planes;   ///< Color planes of rastermap
        unsigned char _bits_per_plane; ///< Bits per plane of rastermap

    }; // end class RasterMap

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_RESOURCE_RASTER_MAP_HPP
