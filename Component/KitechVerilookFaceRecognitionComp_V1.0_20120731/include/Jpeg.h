/** \file Jpeg.h \brief Provides functionality for loading and saving images in JPEG format. */


/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* Jpeg.h                                                                     *|
|* Header file for Jpeg module                                                *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef JPEG_H_INCLUDED
#define JPEG_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/*************\
|*           *|
|* JPEG load *|
|*           *|
\*************/

#ifdef N_DOCUMENTATION

/**
 * \brief  Loads image from JPEG file.
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
 * \sa  HNImage | JpegLoadImageFromMemory | JpegSaveImageToFile
 */
NResult N_API JpegLoadImageFromFile(const NChar * szFileName, HNImage * pHImage);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API JpegLoadImageFromFileA(const NAChar * szFileName, HNImage * pHImage);
#endif
#ifndef N_NO_UNICODE
NResult N_API JpegLoadImageFromFileW(const NWChar * szFileName, HNImage * pHImage);
#endif
#define JpegLoadImageFromFile N_FUNC_AW(JpegLoadImageFromFile)

#endif

/**
 * \brief  Loads image from the memory buffer containing JPEG file.
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
 *            <tr><td rowspan="2">#N_E_ARGUMENT_NULL</td><td>\p pHImage is #NULL.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of file contained in memory buffer specified by
 *                \p buffer is invalid.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | JpegLoadImageFromFile | JpegSaveImageToMemory
 */
NResult N_API JpegLoadImageFromMemory(const void * buffer, NSizeType bufferLength, HNImage * pHImage);

NResult N_API JpegLoadImageFromStream(HNStream hStream, HNImage * pHImage);

/*************\
|*           *|
|* JPEG save *|
|*           *|
\*************/

#define JPEG_DEFAULT_QUALITY 75 ///< Specifies default JPEG quality

#ifdef N_DOCUMENTATION

/**
 * \brief  Saves image to file in JPEG format.
 * \param[in]  hImage      Handle to image.
 * \param[in]  quality     Specifies quality of JPEG compression.
 * \param[in]  szFileName  Points to string that specifies file name.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p szFileName is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p quality is less than 0 or greater than 100.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | JpegSaveImageToMemory | JpegLoadImageFromFile
 */
NResult N_API JpegSaveImageToFile(HNImage hImage, NInt quality, const NChar * szFileName);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API JpegSaveImageToFileA(HNImage hImage, NInt quality, const NAChar * szFileName);
#endif
#ifndef N_NO_UNICODE
NResult N_API JpegSaveImageToFileW(HNImage hImage, NInt quality, const NWChar * szFileName);
#endif
#define JpegSaveImageToFile N_FUNC_AW(JpegSaveImageToFile)

#endif

/**
 * \brief  Saves image to the memory buffer in JPEG format.
 * \param[in]  hImage         Handle to image.
 * \param[in]  quality        Specifies quality of JPEG compression.
 * \param[out] pBuffer        Pointer to void * that receives pointer to
 *                            allocated memory buffer.
 * \param[out] pBufferLength  Pointer to #NSizeType that receives size of
 *                            allocated memory buffer.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage, \p pBuffer or \p pBufferLength is #NULL.
 *                </td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>There was not enough memory to allocate memory buffer.
 *                </td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p quality is less than 0 or greater than 100.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \remark  Memory buffer allocated by this function must be deallocated by
 *          #NFree function when it is no longer needed.
 * \sa  HNImage | JpegSaveImageToFile | JpegLoadImageFromMemory
 */
NResult N_API JpegSaveImageToMemory(HNImage hImage, NInt quality, void * * pBuffer, NSizeType * pBufferLength);

NResult N_API JpegSaveImageToStream(HNImage hImage, NInt quality, HNStream hStream);

#ifdef N_CPP
}
#endif

#endif // !JPEG_H_INCLUDED
