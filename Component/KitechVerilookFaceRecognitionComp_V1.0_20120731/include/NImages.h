/** \file NImages.h \brief Provides library registration and other additional functionality.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NImages.h                                                                  *|
|* Header file for basic NImages library functionality                        *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_IMAGES_H_INCLUDED
#define N_IMAGES_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

#ifndef N_NO_ANSI_FUNC
NResult N_API NImagesGetInfoA(NLibraryInfoA * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImagesGetInfoW(NLibraryInfoW * pValue);
#endif
#define NImagesGetInfo N_FUNC_AW(NImagesGetInfo)

/**
 * \brief  Creates color wrapper for grayscale image.
 * \param[in]  hImage      Handle to image.
 * \param[in]  minColor    Specifies color to be used for black color.
 * \param[in]  maxColor    Specifies color to be used for white color.
 * \param[out] pHDstImage  Pointer to #HNImage that receives handle to created
 *                         image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>Image specified by \p hImage has non-grayscale pixel
 *                format (not #npfGrayscale or #npfMonochrome).</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pHDstImage is #NULL.</td></tr>
 *          </table>
 * \remark  Created image must be deleted using #NImageFree function.
 * \remark  Created image is a thin wrapper for specified grayscale image.
 *          Therefore hImage must not be freed before created image.
 * \remark  Gray values in source image are replaced with according RGB values
 *          from range [minColor, maxColor] in created image.
 * \sa  HNImage | NImageFree
 */
NResult N_API NImagesGetGrayscaleColorWrapper(HNImage hImage, NRgb minColor, NRgb maxColor, HNImage * pHDstImage);

#ifdef N_CPP
}
#endif

#endif // !N_IMAGES_H_INCLUDED
