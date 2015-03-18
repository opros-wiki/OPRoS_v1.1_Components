/** \file NGrayscaleImage.h \brief Provides functionality for managing 8-bit grayscale images.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NGrayscaleImage.h                                                          *|
|* Header file for NGrayscaleImage module                                     *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_GRAYSCALE_IMAGE_H_INCLUDED
#define N_GRAYSCALE_IMAGE_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief  Retrieves value of pixel at the specified coordinates in 8-bit
 *         grayscale image.
 * \param[in]  hImage  Handle to image.
 * \param[in]  x       Specified \a X coordinate of the pixel.
 * \param[in]  y       Specified \a Y coordinate of the pixel.
 * \param[out] pValue  Pointer to #NByte that receives pixel value.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p x is greater than or equal to image width.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td><td>\p y is greater than or equal to image height.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Image pixel format is not #npfGrayscale.</td></tr>
 *          </table>
 * \sa HNImage | NGrayscaleImageSetPixel
 */
NResult N_API NGrayscaleImageGetPixel(HNImage hImage, NUInt x, NUInt y, NByte * pValue);
/**
 * \brief  Sets value of pixel at the specified coordinates in 8-bit grayscale
 *         image.
 * \param[in]  hImage  Handle to image.
 * \param[in]  x       Specified \a X coordinate of the pixel.
 * \param[in]  y       Specified \a Y coordinate of the pixel.
 * \param[in]  value   Specifies new pixel value.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage is #NULL.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p x is greater than or equal to image width.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td><td>\p y is greater than or equal to image height.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Image pixel format is not #npfGrayscale.</td></tr>
 *          </table>
 * \sa HNImage | NGrayscaleImageGetPixel
 */
NResult N_API NGrayscaleImageSetPixel(HNImage hImage, NUInt x, NUInt y, NByte value);

#ifdef N_CPP
}
#endif

#endif // !N_GRAYSCALE_IMAGE_H_INCLUDED
