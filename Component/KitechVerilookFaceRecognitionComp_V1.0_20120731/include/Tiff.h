/** \file Tiff.h \brief Provides functionality for loading and saving images in TIFF format.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* Tiff.h                                                                     *|
|* Header file for Tiff module                                                *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef TIFF_H_INCLUDED
#define TIFF_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/*************\
|*           *|
|* TIFF load *|
|*           *|
\*************/

#ifdef N_DOCUMENTATION

/**
 * \brief  Loads image from TIFF file.
 * \param[in]  szFileName  Points to string that specifies file name.
 * \param[out] pHImage     Pointer to #HNImage that receives handle to loaded
 *                         image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p szFileName or \p pHImage is #NULL.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of file specified by \p szFileName is invalid.
 *                </td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | TiffLoadImageFromMemory
 */
NResult N_API TiffLoadImageFromFile(const NChar * szFileName, HNImage * pHImage);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API TiffLoadImageFromFileA(const NAChar * szFileName, HNImage * pHImage);
#endif
#ifndef N_NO_UNICODE
NResult N_API TiffLoadImageFromFileW(const NWChar * szFileName, HNImage * pHImage);
#endif
#define TiffLoadImageFromFile N_FUNC_AW(TiffLoadImageFromFile)

#endif

/**
 * \brief  Loads image from the memory buffer containing TIFF file.
 * \param[in]  buffer        Pointer to memory buffer.
 * \param[in]  bufferLength  Length of memory buffer.
 * \param[out] pHImage       Pointer to #HNImage that receives handle to loaded
 *                           image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer is #NULL and \p bufferLength is not equal to
 *                zero.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pHImage is #NULL.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of file contained in memory buffer specified by
 *                \p buffer is invalid.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | TiffLoadImageFromFile
 */
NResult N_API TiffLoadImageFromMemory(const void * buffer, NSizeType bufferLength, HNImage * pHImage);

NResult N_API TiffLoadImageFromStream(HNStream hStream, HNImage * pHImage);

#ifdef N_CPP
}
#endif

#endif // !TIFF_H_INCLUDED
