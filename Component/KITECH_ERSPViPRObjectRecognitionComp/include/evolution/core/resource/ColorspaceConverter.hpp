//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__COLORSPACE_CONVERTER_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__COLORSPACE_CONVERTER_HPP


/**
 * @file   evolution/core/resource/ColorspaceConverter.hpp
 * @brief  Converts between color formats.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ColorspaceConverter
 **/


#include <evolution/core/resource/Colorspace.hpp>


namespace Evolution
{

    /**
     * @class ColorspaceConverter
     *
     * @brief This class provides conversion between color formats.
     *
     * @see Colorspace, ColorFormat
     * @ingroup core_vision
     **/
    class EVOLUTION_EXPORT_RESOURCE ColorspaceConverter
    {
    public: // Types
        /// Function pointer typedef to perform copy between various color spaces
        typedef void ( *CopyFunction ) ( unsigned width,
                                         unsigned height,
                                         const uchar * src, uchar * dst );

    public: // Structors

        /**
         * @brief This will construct a ColorspaceConverter that will convert
         * input_format into output_format.
         *
         * @param input_format  [in]; The initial input format
         * @param output_format [in]; The initial output format
         */
        ColorspaceConverter ( ColorFormat input_format,
                              ColorFormat output_format );

        /**
         * @brief Destructor
         */
        ~ColorspaceConverter (  )
        {
        }

    public:                  // Public interface

        /**
         * @brief This will copy src into dst, converting between color formats.
         *
         * @param  width  [in];  Width in pixels
         * @param  height [in];  Height in pixels
         * @param  src    [in];  Input image buffer
         * @param  dst    [out]; Output image buffer
         *
         * @pre The dst variable must be at least as large as returned by
         * calculate_buffer_size (width, height).
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result operator  (  ) ( unsigned width,
                                unsigned height,
                                const uchar * src, uchar * dst ) const;

        /**
         * This function calculates the size of buffer needed to hold an image
         * with the given dimensions in the current output color format.
         *
         * @param width [in];  Width in pixels
         * @param height [in]; Height in pixels
         *
         * @return The buffer size in bytes.
         */
        size_t calculate_buffer_size ( unsigned width, unsigned height ) const
        {
            return ( ColorspaceUtils::
                     calculate_buffer_size ( width, height, _dst_format ) );
        }

        /**
         * @brief This function returns the bits per pixel of the current
         * output color format.
         *
         * @return The number of bits per plane.
         */
        unsigned calculate_bpp (  ) const
        {
            return ( ColorspaceUtils::calculate_format_bpp ( _dst_format ) );
        }

        /**
         * @brief This function gets the source color format.
         */
        ColorFormat get_source_format (  ) const
        {
            return _src_format;
        }

        /**
         * @brief This function gets the destination color format.
         *
         * @return The destination color format.
         */
        ColorFormat get_destination_format (  ) const
        {
            return _dst_format;
        }

        /**
         * @brief This function sets the source color format.
         *
         * @param format [in]; The new source color format
         */
        void set_source_format ( ColorFormat format );

        /**
         * @brief Set This function sets the destination color format.
         *
         * @param format [in]; The new destination color format
         */
        void set_destination_format ( ColorFormat format );

    public: // Pixel conversion (static methods)

        /**
         * @brief This bounds the pixel value in [0,255].
         *
         * @param val [in]; The pixel value to clamp
         *
         * @return The clamped pixel value.
         */
        static inline uchar bound_pixel_value ( int val )
        {
            return ( val < 0 ? 0 : ( val > 255 ? 255 : val ) );
        }                       // end bound_pixel_value()


        /**
         * @brief This bounds the pixel value in [0,USHRT_MAX].
         *
         * @param val [in]; The pixel value to clamp
         *
         * @return The clamped pixel value.
         */
        static inline unsigned short bound_pixel_value_16 ( int val )
        {
            return ( val < 0 ? 0 : ( val > USHRT_MAX ? USHRT_MAX : val ) );
        }                       // end bound_pixel_value()


        /**
         * @brief This function converts a YUV pixel to RGB.
         *
         * @param y [in]; The pixel's y value
         * @param u [in]; The pixel's u value
         * @param v [in]; The pixel's v value
         * @param r [out]; The converted pixel's red value
         * @param g [out]; The converted pixel's green value
         * @param b [out]; The converted pixel's blue value
         */
        static inline
        void yuv_to_rgb_pixel ( int y, int u, int v,
                                uchar * r, uchar * g, uchar * b )
        {
            // Convert YUV to RGB, round and bound.
            *r = bound_pixel_value ( ( 256 * y + 358 * v + 128 ) >> 8 );
            *g = bound_pixel_value ( ( 256 * y - 88 * u - 183 * v +
                                       128 ) >> 8 );
            *b = bound_pixel_value ( ( 256 * y + 454 * u + 128 ) >> 8 );
        }                       // end yuv_to_rgb_pixel()

        /**
         * @brief This function converts a YUV pixel to RGB.
         *
         * @param y [in]; The pixel's y value
         * @param u [in]; The pixel's u value
         * @param v [in]; The pixel's v value
         * @param r [out]; The converted pixel's red value
         * @param g [out]; The converted pixel's green value
         * @param b [out]; The converted pixel's blue value
         */
        static inline
        void yuv_to_rgb_pixel_float ( int y, int u, int v,
                                      uchar * r, uchar * g, uchar * b )
        {
            // Convert YUV to RGB, round and bound.
            *r = bound_pixel_value ( ( int )
                                     inline_roundf ( y + 1.402f * v ) );
            *g = bound_pixel_value ( ( int )
                                     inline_roundf ( y - 0.344f * u -
                                                     0.714f * v ) );
            *b = bound_pixel_value ( ( int )
                                     inline_roundf ( y + 1.772f * u ) );
        }                       // end yuv_to_rgb_pixel()

        /**
         * @brief This function converts a YCbCr pixel to RGB.
         *
         * @param yy [in]; The pixel's y value
         * @param cb [in]; The pixel's cb value
         * @param cr [in]; The pixel's cr value
         * @param r [out]; The converted pixel's red value
         * @param g [out]; The converted pixel's green value
         * @param b [out]; The converted pixel's blue value
         */

        static inline
        void ycbcr_to_rgb_pixel ( int yy, int cb, int cr,
                                  uchar * r, uchar * g, uchar * b )
        {
            // Convert YCbCr to YUV
            int c = ( ( 298 * ( yy - 16 ) ) >> 8 );
            int d = ( ( 290 * ( cb - 128 ) ) >> 8 );
            int e = ( ( 290 * ( cr - 128 ) ) >> 8 );

            *r = bound_pixel_value ( ( 256 * c + 359 * e ) >> 8 );
            *g = bound_pixel_value ( ( 256 * c - 78 * d - 183 * e ) >> 8 );
            *b = bound_pixel_value ( ( 256 * c + 454 * d ) >> 8 );
        }                       // end ycbcr_to_rgb_pixel()

        /**
         * @brief This function converts a YCbCr pixel to RGB.
         *
         * @param yy [in]; The pixel's y value
         * @param cb [in]; The pixel's cb value
         * @param cr [in]; The pixel's cr value
         * @param r [out]; The converted pixel's red value
         * @param g [out]; The converted pixel's green value
         * @param b [out]; The converted pixel's blue value
         */
        static inline
        void ycbcr_to_rgb_pixel_float ( int yy, int cb, int cr,
                                        uchar * r, uchar * g, uchar * b )
        {
            // Convert YCbCr to YUV
            float y = 1.16438356164f * ( yy - 16 );
            float u = 1.13392857143f * ( cb - 128 );
            float v = 1.13392857143f * ( cr - 128 );
            //std::cerr << "(y,u,v) = (" << y << ", " << u << ", " << v << ")\n";

            // Convert YUV to RGB, round and bound.
            *r = bound_pixel_value ( ( int )
                                     inline_roundf ( y + 1.402f * v ) );
            *g = bound_pixel_value ( ( int )
                                     inline_roundf ( y - 0.344f * u -
                                                     0.714f * v ) );
            *b = bound_pixel_value ( ( int )
                                     inline_roundf ( y + 1.772f * u ) );
        }                       // end ycbcr_to_rgb_pixel()

        /**
         * @brief This function converts an RGB pixel to YUV.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param y [out]; The converted pixel's y value
         * @param u [out]; The converts pixel's u value
         * @param v [out]; The converted pixel's v value
         */
        static inline
        void rgb_to_yuv_pixel ( int r, int g, int b,
                                uchar * y, uchar * u, uchar * v )
        {
            // Convert RGB to YUV
            //*y = bound_pixel_value ( ( (  66 * r + 129 * g +  25 * b + 128) >> 8) );
            //*u = bound_pixel_value ( ( ( -38 * r -  74 * g + 112 * b + 128) >> 8) );
            //*v = bound_pixel_value ( ( ( 112 * r -  94 * g -  18 * b + 128) >> 8) );
            *y = bound_pixel_value ( ( ( 77 * r + 145 * g + 28 * b +
                                         128 ) >> 8 ) );
            *u = bound_pixel_value ( ( ( -43 * r - 85 * g + 128 * b +
                                         128 ) >> 8 ) );
            *v = bound_pixel_value ( ( ( 128 * r - 107 * g - 21 * b +
                                         128 ) >> 8 ) );
        }                       // end rgb_to_yuv_pixel()

        /**
         * @brief This function converts an RGB pixel to YUV.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param y [out]; The converted pixel's y value
         * @param u [out]; The converts pixel's u value
         * @param v [out]; The converted pixel's v value
         */
        static inline
        void rgb_to_yuv_pixel_float ( int r, int g, int b,
                                      uchar * y, uchar * u, uchar * v )
        {
            // Convert RGB to YUV
            *y = bound_pixel_value ( ( int )
                                     inline_roundf ( 0.2990f * r +
                                                     0.5870f * g +
                                                     0.1140f * b ) );
            *u = bound_pixel_value ( ( int )
                                     inline_roundf ( -0.1687f * r -
                                                     0.3313f * g +
                                                     0.5000f * b ) );
            *v = bound_pixel_value ( ( int )
                                     inline_roundf ( 0.5000f * r -
                                                     0.4187f * g -
                                                     0.0813f * b ) );
        }                       // end rgb_to_yuv_pixel()

        /**
         * @brief This function converts an RGB pixel to YCbCr.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param yy [out]; The converted pixel's yy value
         * @param cb [out]; The converts pixel's cb value
         * @param cr [out]; The converted pixel's cr value
         */
        static inline
        void rgb_to_ycbcr_pixel ( int r, int g, int b,
                                  uchar * yy, uchar * cb, uchar * cr )
        {
//            *yy = ( ( ( 66 * r + 129 * g + 25 * b + 128) >> 8) + 16);
//            *cb = ( (-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
//            *cr = ( ( 112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

            *yy =
                bound_pixel_value ( ( ( ( ( 77 * r + 145 * g + 28 * b +
                                            128 ) >> 8 ) * 220 ) >> 8 ) +
                                    16 );
            *cb =
                bound_pixel_value ( ( ( ( ( -43 * r - 85 * g + 128 * b +
                                            128 ) >> 8 ) * 226 ) >> 8 ) +
                                    128 );
            *cr =
                bound_pixel_value ( ( ( ( ( 128 * r - 107 * g - 21 * b +
                                            128 ) >> 8 ) * 226 ) >> 8 ) +
                                    128 );
        } // end rgb_to_ycbcr_pixel()

        /**
         * @brief This function converts an RGB pixel to YCbCr.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param yy [out]; The converted pixel's yy value
         * @param cb [out]; The converts pixel's cb value
         * @param cr [out]; The converted pixel's cr value
         */
        static inline
        void rgb_to_ycbcr_pixel_float ( int r, int g, int b,
                                        uchar * yy, uchar * cb, uchar * cr )
        {
            // Convert RGB to YUV
            float y = 0.2990f * r + 0.5870f * g + 0.1140f * b;
            float u = -0.1687f * r - 0.3313f * g + 0.5000f * b;
            float v = 0.5000f * r - 0.4187f * g - 0.0813f * b;
            //std::cerr << "(y,u,v) = (" << y << ", " << u << ", " << v << ")\n";

            // Convert YUV to YCbCr, round and bound.
            *yy =
                bound_pixel_value ( ( int )
                                    inline_roundf ( 0.85882352941f * y +
                                                    16 ) );
            *cb =
                bound_pixel_value ( ( int )
                                    inline_roundf ( 0.88188976378f * u +
                                                    128 ) );
            *cr =
                bound_pixel_value ( ( int )
                                    inline_roundf ( 0.88188976378f * v +
                                                    128 ) );
        } // end rgb_to_ycbcr_pixel()

        /**
         * @brief This function converts an RGB pixel to grayscale.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param gray [out]; The converted pixel's gray value
         */
        static inline
        void rgb_to_grayscale_pixel_float ( int r, int g, int b, uchar * gray )
        {
            // Convert, round and bound.
            *gray = bound_pixel_value ( ( int )
                                        inline_roundf ( 0.2990f * r +
                                                        0.5870f * g +
                                                        0.1140f * b ) );


        } // end rgb_to_ycbcr_pixel()

        /**
         * @brief This function converts an RGB pixel to grayscale.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param gray [out]; The converted pixel's gray value
         */
        static inline
        void rgb_to_grayscale_pixel ( int r, int g, int b, uchar * gray )
        {
            // Convert, round and bound.
            *gray = bound_pixel_value ( (77 * r + 150 * g + 29 * b) >> 8); // approximately!
        } // end rgb_to_grayscale_pixel()

        /**
        * @brief This function converts an RGB pixel to grayscale 16.
        *
        * @param r [in]; The pixel's red value
        * @param g [in]; The pixel's green value
        * @param b [in]; The pixel's blue value
        * @param gray [out]; The converted pixel's gray value
        */
        static inline
        void rgb_to_grayscale_pixel_16 ( int r, int g, int b,
                                         unsigned short * gray )
        {
            // Convert, round and bound.
            *gray = bound_pixel_value_16 ( (38* r + 75 * g + 15 * b) ); //approxmately!
        } // end rgb_to_grayscale_pixel()

        /**
         * @brief This function converts an RGB pixel to VXW.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param v [out]; The converted pixel's v value
         * @param x [out]; The converts pixel's x value
         * @param w [out]; The converted pixel's w value
         */
        static inline
        void rgb_to_vwx_pixel_float (int r, int g, int b,
                                     uchar* v, uchar* x, uchar* w)
        {
            *v = (uchar) ((r + g + b )/3); //check inter-data type errors. 
            *w = (uchar) ((r - (g + b)/2)/2);
            *x = (uchar) inline_roundf ((0.86602540378443864676f * (g - b))/2);
        }

        /**
         * @brief This function converts an RGB pixel to VXW.
         *
         * @param r [in]; The pixel's red value
         * @param g [in]; The pixel's green value
         * @param b [in]; The pixel's blue value
         * @param v [out]; The converted pixel's v value
         * @param x [out]; The converts pixel's x value
         * @param w [out]; The converted pixel's w value
         */
        static inline
        void rgb_to_vwx_pixel (int r, int g, int b, uchar* v, uchar* x, uchar* w)
        {
            *v = (uchar) (( r + g + b ) / 3 ); //check inter-data type errors. 
            *w = (uchar) (( r - (( g + b ) / 2 )) / 2  );
            //*x = (uchar) (221 * (( g - b ) / 2)) / 256;
            //*v = (uchar) ((r + g + b )/3); //check inter-data type errors. 
            //*w = (uchar) ((r - (g + b)/2)/2);
            *x = (uchar) inline_roundf ((0.86602540378443864676f * (g - b))/2);
        } 

        /**
         * @brief This function converts an RGB pixel to VXW.
         *
         * @param v [in]; The pixel's v value
         * @param w [in]; The pixel's w value
         * @param x [in]; The pixel's x value
         * @param r [out]; The converted pixel's red value
         * @param g [out]; The converted pixel's green value
         * @param b [out]; The converted pixel's blue value
         */
        static inline
        void vwx_to_rgb_pixel_float (int v, int w, int x, uchar* r, uchar* g, uchar* b)
        {
            *r = (uchar) ((v + w + x )/3); //check inter-data type errors. 
            *g = (uchar) ((v - (w + x)/2)/2);
            *b = (uchar) inline_roundf ((0.86602540378443864676f * (w - x))/2);
        }

        /**
         * @brief This function converts an RGB pixel to VXW.
         *
         * @param v [in]; The pixel's v value
         * @param w [in]; The pixel's w value
         * @param x [in]; The pixel's x value
         * @param r [out]; The converted pixel's red value
         * @param g [out]; The converted pixel's green value
         * @param b [out]; The converted pixel's blue value
         */
        static inline
        void vwx_to_rgb_pixel (int v, int w, int x, uchar* r, uchar* g, uchar* b)
        {
            *r = (uchar) (( v + w + x ) / 3); //check inter-data type errors. 
            *g = (uchar) (( v - (( w + x ) >> 1 )) >> 1);
            // used to be (221 * ( w - x )) >> 9
            *b = (uchar) ((221 * ( w - x )) >> 9);
        } 


        /**
         * @brief This function converts color values from one format to another. 
         *
         * @note The caller MUST provide sufficient output variables for the format.
         *
         * @param src_space [in]; The source color space
         * @param i1 [in]; The pixel byte to be converted
         * @param i2 [in]; The pixel byte to be converted
         * @param i3 [in]; The pixel byte to be converted
         * @param dest_space [in]; The destination color space
         * @param o1 [out]; The converted pixel byte
         * @param o2 [out]; The converted pixel byte
         * @param o3 [out]; The converted pixel byte
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        static inline
        Result convert_pixel ( Colorspace src_space,
                               uchar i1, uchar i2, uchar i3,
                               Colorspace dest_space,
                               uchar * o1, uchar * o2 = NULL, uchar * o3 =
                               NULL )
        {
            PRECOND ( ( src_space != COLORSPACE_UNSPECIFIED )
                      && ( dest_space != COLORSPACE_UNSPECIFIED ) );
            PRECOND ( dest_space & COLORSPACE_GRAYSCALE
                      ? ( o1 != NULL ) : ( o1 && o2 && o3 ) );

            // Check for equivalent color formats.
            if ( src_space == dest_space )
            {
                if ( dest_space == COLORSPACE_GRAYSCALE )
                {
                    *o1 = i1;
                }
                else
                {
                    *o1 = i1;
                    *o2 = i2;
                    *o3 = i3;
                }
            }

            // YUV
            else if ( src_space == COLORSPACE_YCBCR )
            {
                if ( dest_space == COLORSPACE_RGB )
                {
                    ycbcr_to_rgb_pixel ( i1, i2, i3, o1, o2, o3 );
                }
                else if ( dest_space == COLORSPACE_GRAYSCALE )
                {
                    *o1 = i1;
                }
                else
                {
                    return ( RESULT_INVALID_ARGUMENT );
                }
            }

            // RGB
            else if ( src_space == COLORSPACE_RGB )
            {
                if ( dest_space == COLORSPACE_YCBCR )
                {
                    rgb_to_ycbcr_pixel ( i1, i2, i3, o1, o2, o3 );
                }
                else if ( dest_space == COLORSPACE_GRAYSCALE )
                {
                    rgb_to_grayscale_pixel ( i1, i2, i3, o1 );
                }
                else if (dest_space == COLORSPACE_VWX)
                {
                    rgb_to_vwx_pixel (i1, i2, i3, o1, o2, o3);
                }
                else
                {
                    return ( RESULT_INVALID_ARGUMENT );
                }
            }

            // grayscale
            else if ( src_space == COLORSPACE_GRAYSCALE )
            {
                if ( dest_space == COLORSPACE_YCBCR )
                {
                    *o1 = i1;
                    *o2 = 0;
                    *o3 = 0;
                }
                else if ( dest_space == COLORSPACE_RGB )
                {
                    *o1 = i1;
                    *o2 = i1;
                    *o3 = i1;
                }
                else
                {
                    return ( RESULT_INVALID_ARGUMENT );
                }
            }
            else if (src_space == COLORSPACE_VWX)
            {
                if (dest_space == COLORSPACE_RGB)
                {
                    vwx_to_rgb_pixel (i1, i2, i3, o1, o2, o3);
                }
                else
                {
                    return (RESULT_INVALID_ARGUMENT);
                }
            }
            else
            {
                return ( RESULT_INVALID_ARGUMENT );
            }
            return ( RESULT_SUCCESS );
        }                       // end convert_pixel()

    public:                  // Buffer conversion (to real YUV)

        /**
         * @brief This function converts IYU1 to YUV.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu1_to_yuv ( unsigned width, unsigned height,
                                  const uchar * src,
                                  uchar * dst );

        /**
         * @brief This function converts IYU2 to YUV.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu2_to_yuv ( unsigned width, unsigned height,
                                  const uchar * src,
                                  uchar * dst );

        /**
         * @brief This function converts UYVY to YUV.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void uyvy_to_yuv ( unsigned width, unsigned height,
                                  const uchar * src,
                                  uchar * dst );

        /**
         * @brief This function converts Planar YUV 4:1:1 (Y, V, U) to  YUV 4:4:4.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yvu9_to_yuv ( unsigned width, unsigned height,
                                  const uchar * src,
                                  uchar * dst );

        /**
         * @brief This function converts Planar YUV 4:2:2 (Y, U, V) to YUV 4:4:4.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yv12_to_yuv ( unsigned width, unsigned height,
                                  const uchar * src,
                                  uchar * dst );

        /**
         * @brief This function converts RGB24 to YUV.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb24_to_yuv ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

        /**
         * @brief This function converts Aligned RGB32 to YUV.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb32_to_yuv ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

        /**
         * @brief This function converts single-byte Luminance channel (mono) to YUV.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void y800_to_yuv ( unsigned width, unsigned height,
                                  const uchar * src,
                                  uchar * dst );

    public:                  // Buffer conversion (to RGB24)

        /**
         * @brief This function converts IYUV = I420 to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyuv_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts IYU1 to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu1_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts IYU2 to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu2_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts UYVY to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void uyvy_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts Planar YUV 4:1:1 (Y, V, U) to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yvu9_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts Planar YUV 4:2:2 (Y, U, V) to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yv12_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts YUV to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yuv_to_rgb24 ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

        /**
         * @brief This function converts RGB24 to RGB24 simple copy
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb24_to_rgb24 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

        /**
         * @brief This function converts Aligned RGB32 to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb32_to_rgb24 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

        /**
         * @brief This function converts Single-byte Luminance channel (mono) to RGB24.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void y800_to_rgb24 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

    public:                  // Buffer conversion (to RGB32)

        /**
         * @brief This function converts IYUV = I420 to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyuv_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );
        /**
         * @brief This function converts IYU1 to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu1_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts IYU2 to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu2_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts UYVY to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void uyvy_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts Planar YUV 4:1:1 (Y, V, U) to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yvu9_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts Planar YUV 4:2:2 (Y, U, V) to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void yv12_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts RGB24 to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb24_to_rgb32 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

        /**
         * @brief This function converts RGB32 to RGB32 simple copy.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb32_to_rgb32 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

        /**
         * @brief This function converts Single-byte Luminance channel (mono) to RGB32.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void y800_to_rgb32 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts RGB32 to VWX simple copy.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void vwx_to_rgb24 (unsigned width, unsigned height,
                                  const uchar* src,
                                  uchar* dst);

    public: // Buffer conversion (to Y800)

        /**
         * @brief This function converts IYU1 to Y800.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu1_to_y800 ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

        /**
         * @brief This function converts IYU2 to Y800.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void iyu2_to_y800 ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

        /**
         * @brief This function converts UYVY to Y800.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void uyvy_to_y800 ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

        /**
         * @brief This function converts RGB24 to Y800.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb24_to_y800 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts RGB32 to Y800.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb32_to_y800 ( unsigned width, unsigned height,
                                    const uchar * src,
                                    uchar * dst );

        /**
         * @brief This function converts Y800 to Y800 simple copy.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void y800_to_y800 ( unsigned width, unsigned height,
                                   const uchar * src,
                                   uchar * dst );

    public: // Buffer conversion (to Y1600)

        /**
         * @brief This function converts RGB24 to Y1600 (grayscale 16 bpp).
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb24_to_y1600 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

        /**
         * @brief This function converts RGB32 to Y1600 (grayscale 16 bpp).
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb32_to_y1600 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

        /**
         * @brief This function converts Y800 to Y1600 (grayscale 16 bpp).
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void y800_to_y1600 (unsigned width, unsigned height,
                                   const uchar* src,
                                   uchar* dst);

        /**
         * @brief This function converts Y1600 to Y1600 simple copy.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void y1600_to_y1600 ( unsigned width, unsigned height,
                                     const uchar * src,
                                     uchar * dst );

    public: // Buffer conversion (to VWX)
        
        /**
         * @brief This function converts RGB24 to VWX.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src    [in]; The source data buffer
         * @param dst    [out]; The converted data
         */
        static void rgb24_to_vwx (unsigned width, unsigned height,
                                  const uchar* src,
                                  uchar* dst);

    public: // Buffer conversion (general)

        /**
         * @brief This function converts Converts from the source buffer to
         * the destination buffer.
         *
         * @note The caller MUST provide sufficient space for the output
         * data buffer.
         *
         * @param width  [in]; The width of the source data
         * @param height [in]; the height of the source data
         * @param src_format [in]; The source color format
         * @param src_buffer [in]; The source data buffer
         * @param dst_format [in]; The destination color format
         * @param dst_buffer [out]; The converted data
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        static Result
        convert_buffer ( unsigned width, unsigned height,
                         ColorFormat src_format, const uchar * src_buffer,
                         ColorFormat dst_format, uchar * dst_buffer );

    protected: // Implementation

        ColorFormat  _src_format;     ///< Source color format.
        ColorFormat  _dst_format;     ///< Destination color format.
        CopyFunction _copy_function;  ///< Copy function.
    }; // end class ColorspaceConverter

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_RESOURCE__COLORSPACE_CONVERTER_HPP
