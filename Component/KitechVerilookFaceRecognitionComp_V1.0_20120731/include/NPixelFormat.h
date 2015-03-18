/** \file NPixelFormat.h \brief Provides functionality for managing 24-bit RGB images.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NPixelFormat.h                                                             *|
|* Header file for NPixelFormat module                                        *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_PIXEL_FORMAT_H_INCLUDED
#define N_PIXEL_FORMAT_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief  Calculates number of bytes needed to store line of specified length
 *         of pixels with specified bits per pixel and alignment.
 */
#define NCalcRowSizeEx(bitCount, length, alignment) (((bitCount * length + (alignment << 3) - 1) / (alignment << 3)) * alignment)
/**
 * \brief  Calculates number of bytes needed to store line of specified length
 *         of pixels with specified bits per pixel.
 */
#define NCalcRowSize(bitCount, length) NCalcRowSizeEx(bitCount, length, 1)

/**
 * \brief  Calculates number of bytes needed to store line of specified length
 *         of pixels with specified #NPixelFormat and alignment.
 */
#define NPixelFormatGetRowSizeEx(pixelFormat, length, alignment) NCalcRowSizeEx(NPixelFormatGetBitsPerPixel(pixelFormat), length, alignment)
/**
 * \brief  Calculates number of bytes needed to store line of specified length
 *         of pixels with specified #NPixelFormat.
 */
#define NPixelFormatGetRowSize(pixelFormat, length) NPixelFormatGetRowSizeEx(pixelFormat, length, 1)

/**
 * \brief  Specifies pixel format of each pixel in the image.
 * \remark  Image pixel format is not limited to members of this enumeration.
 *          However only these members are provided for usage with this
 *          product.
 */
enum NPixelFormat_
{
	npfMonochrome = 0x00001001, ///< Each pixel value is stored in 1 bit representing either black or white color.
	npfGrayscale  = 0x00301001, ///< Each pixel value is stored in 8 bits representing 256 shades of gray.
	npfRgb        = 0x00303003  ///< Each pixel value is stored in 24 bits consisting of three 8-bit values representing red, green and blue color components.
};

#ifndef N_PIXEL_FORMAT_EX_H_INCLUDED

/// Specifies pixel format of each pixel in the image.
typedef enum NPixelFormat_ NPixelFormat;

/// Used internally in #NPixelFormatGetBitsPerPixel macro.
NUInt N_API NPixelFormatGetBitsPerPixelFunc(NPixelFormat pixelFormat);
/// Retrieves number of bits used to store a pixel from #NPixelFormat.
#define NPixelFormatGetBitsPerPixel(pixelFormat) NPixelFormatGetBitsPerPixelFunc(pixelFormat)

/// Checks if specified pixel format is valid.
NBool N_API NPixelFormatIsValid(NPixelFormat value);

#endif

/// Represents an RGB color.
typedef struct NRgb_
{
	NByte Red;   ///< Red component value
	NByte Green; ///< Green component value
	NByte Blue;  ///< Blue component value
} N_PACKED NRgb;

/// Makes #NRgb constant with field values provided.
#define NRgbConst(red, green, blue) { red, green, blue }

#ifdef N_CPP
}
#endif

#endif // !N_PIXEL_FORMAT_H_INCLUDED
