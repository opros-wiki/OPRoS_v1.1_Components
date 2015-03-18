/** \file NRgbImage.h \brief Provides functionality for working with image pixel format.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NRgbImage.h                                                                *|
|* Header file for NRgbImage module                                           *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_RGB_IMAGE_H_INCLUDED
#define N_RGB_IMAGE_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief  Retrieves value of pixel at the specified coordinates in 24-bit
 *         RGB image.
 * \param[in]  hImage  Handle to image.
 * \param[in]  x       Specified \a X coordinate of the pixel.
 * \param[in]  y       Specified \a Y coordinate of the pixel.
 * \param[out] pValue  Pointer to #NRgb that receives pixel value.
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
 *                <td>Image pixel format is not #npfRgb.</td></tr>
 *          </table>
 * \remark  
 * \sa HNImage | NRgbImageSetPixel
 */
NResult N_API NRgbImageGetPixel(HNImage hImage, NUInt x, NUInt y, NRgb * pValue);
/**
 * \brief  Sets value of pixel at the specified coordinates in 24-bit
 *         RGB image.
 * \param[in]  hImage  Handle to image.
 * \param[in]  x       Specified \a X coordinate of the pixel.
 * \param[in]  y       Specified \a Y coordinate of the pixel.
 * \param[in]  pValue  Pointer no #NRgb that specifies new pixel value.
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
 *                <td>Image pixel format is not #npfRgb.</td></tr>
 *          </table>
 * \sa HNImage | NRgbImageGetPixel
 */
NResult N_API NRgbImageSetPixel(HNImage hImage, NUInt x, NUInt y, const NRgb * pValue);

#ifdef N_CPP
}
#endif

#endif // !N_RGB_IMAGE_H_INCLUDED
