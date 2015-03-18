/** \file NMonochromeImage.h \brief Provides functionality for managing 1-bit monochrome images.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NMonochromeImage.h                                                         *|
|* Header file for NMonochromeImage module                                    *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_MONOCHROME_IMAGE_H_INCLUDED
#define N_MONOCHROME_IMAGE_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief  Retrieves value of pixel at the specified coordinates in 1-bit
 *         monochrome image.
 * \param[in]  hImage  Handle to image.
 * \param[in]  x       Specified \a X coordinate of the pixel.
 * \param[in]  y       Specified \a Y coordinate of the pixel.
 * \param[out] pValue  Pointer to #NBool that receives pixel value.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p x is greater than or equal to image width.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
				  <td>\p y is greater than or equal to image height.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Image pixel format is not #npfMonochrome.</td></tr>
 *          </table>
 * \remark  If pixel value is black then value \p pValue points to receives
 *          #NFalse and if it is white then value receives #NTrue.
 * \sa HNImage | NMonochromeImageSetPixel
 */
NResult N_API NMonochromeImageGetPixel(HNImage hImage, NUInt x, NUInt y, NBool * pValue);
/**
 * \brief  Sets value of pixel at the specified coordinates in 1-bit
 *         monochrome image.
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
 *                <td>Image pixel format is not #npfMonochrome.</td></tr>
 *          </table>
 * \remark  If \p value is #NFalse then pixel will be black and if it is #NTrue
 *          then pixel value will be white.
 * \sa HNImage | NMonochromeImageGetPixel
 */
NResult N_API NMonochromeImageSetPixel(HNImage hImage, NUInt x, NUInt y, NBool value);

#ifdef N_CPP
}
#endif

#endif // !N_MONOCHROME_IMAGE_H_INCLUDED
