/** \file NImageFormat.h \brief Provides functionality for loading and saving images in format-neutral way.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NImageFormat.h                                                             *|
|* Header file for NImageFormat module                                        *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_IMAGE_FORMAT_H_INCLUDED
#define N_IMAGE_FORMAT_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

/// Handle to the image format.
N_DECLARE_HANDLE(HNImageFormat)

#ifdef N_CPP
}
#endif

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief  Retrieves number of supported image formats.
 * \param[out] pValue  Pointer to #NInt that receives number of supported
 *                     image formats.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatGetFormat
 */
NResult N_API NImageFormatGetFormatCount(NInt * pValue);
/**
 * \brief  Retrieves supported image format with the specified index.
 * \param[in]  index   Specifies zero-based supported image format index to
 *                     retrieve.
 * \param[out] pValue  Pointer to #HNImageFormat that receives image format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less that zero or greater than or equal to
 *                supported image format count. See
 *                #NImageFormatGetFormatCount.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatGetFormatCount
 */
NResult N_API NImageFormatGetFormat(NInt index, HNImageFormat * pValue);

#if N_DOCUMENTATION

/**
 * \brief  Retrieves supported image format registered with file extension of
 *         specified file name and supporting reading/writing as specified.
 * \param[in]  szFileName     Points to string that specifies file name.
 * \param[in]  fileAccess     Specifies that image format should support
 *                            reading, writing or both.
 * \param[out] pHImageFormat  Pointer to #HNImageFormat that receives handle to
 *                            image format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td><td>\p fileAccess value is invalid.
 *                </td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p szFileName or
 *                \p pHImageFormat is #NULL.</td></tr>
 *          </table>
 * \remark  If none of supported image formats that supports reading/writing as
 *          specified by \p fileAccess is registered with file extension of
 *          \p szFileName then handle returned via \p pHImageFormat is #NULL.
 * \sa  HNImageFormat | NFileAccess | NImageFormatGetFormatCount | 
 *      NImageFormatGetFormat
 */
NResult N_API NImageFormatSelect(const NChar * szFileName, NFileAccess fileAccess, HNImageFormat * pHImageFormat);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageFormatSelectA(const NAChar * szFileName, NFileAccess fileAccess, HNImageFormat * pHImageFormat);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageFormatSelectW(const NWChar * szFileName, NFileAccess fileAccess, HNImageFormat * pHImageFormat);
#endif
#define NImageFormatSelect N_FUNC_AW(NImageFormatSelect)

#endif

/**
 * \brief  Retrieves BMP image format.
 * \param[out] pValue  Pointer to #HNImageFormat that receives handle to image
 *                     format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatGetTiff | NImageFormatGetJpeg
 */
NResult N_API NImageFormatGetBmp(HNImageFormat * pValue);
/**
 * \brief  Retrieves TIFF image format.
 * \param[out] pValue  Pointer to #HNImageFormat that receives handle to image
 *                     format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatGetBmp | NImageFormatGetJpeg
 */
NResult N_API NImageFormatGetTiff(HNImageFormat * pValue);
/**
 * \brief  Retrieves JPEG image format.
 * \param[out] pValue  Pointer to #HNImageFormat that receives handle to image
 *                     format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatGetBmp | NImageFormatGetTiff
 */
NResult N_API NImageFormatGetJpeg(HNImageFormat * pValue);

#ifdef N_DOCUMENTATION

/**
 * \brief  Loads image from file of specified image format.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  szFileName    Points to string that specifies file name.
 * \param[out] pHImage       Pointer to #HNImage that receives handle to loaded
 *                           image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat, \p szFileName or \p pHImage is #NULL.
 *                </td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of file specified by \p szFileName is invalid.
 *                </td></tr>
 *            <tr><td>#N_E_NOT_SUPPORTED</td>
 *                <td>Image format specified by \p hImageFormat does not
 *                support reading.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatCanRead | HNImage |
 *      NImageFormatLoadImageFromMemory | NImageFormatSaveImageToFile
 */
NResult N_API NImageFormatLoadImageFromFile(HNImageFormat hImageFormat, const NChar * szFileName, HNImage * pHImage);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageFormatLoadImageFromFileA(HNImageFormat hImageFormat, const NAChar * szFileName, HNImage * pHImage);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageFormatLoadImageFromFileW(HNImageFormat hImageFormat, const NWChar * szFileName, HNImage * pHImage);
#endif
#define NImageFormatLoadImageFromFile N_FUNC_AW(NImageFormatLoadImageFromFile)

#endif

/**
 * \brief  Loads image from the memory buffer containing file of the specified
 *         format.
 * \param[in]  hImageFormat  Handle to image format.
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
 *                <td>\p hImageFormat or \p pHImageFormat is #NULL.</td></tr>
 *            <tr><td>\p buffer is #NULL and \p bufferLength is not equal to
 *                zero.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of the file contained in memory buffer specified
 *                by \p buffer is invalid for the specified image format.
 *                </td></tr>
 *            <tr><td>#N_E_NOT_SUPPORTED</td>
 *                <td>Image format specified by \p hImageFormat does not
 *                support reading.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatCanRead | HNImage |
 *      NImageFormatLoadImageFromFile | NImageFormatSaveImageToMemory
 */
NResult N_API NImageFormatLoadImageFromMemory(HNImageFormat hImageFormat, void * buffer, NSizeType bufferLength, HNImage * pHImage);

NResult N_API NImageFormatLoadImageFromStream(HNImageFormat hImageFormat, HNStream hStream, HNImage * pHImage);

#ifdef N_DOCUMENTATION

/**
 * \brief  Saves image to the file in the specified format.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  hImage        Handle to image.
 * \param[in]  szFileName    Points to string that specifies file name.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat, \p hImage or \p szFileName is #NULL.
 *                </td></tr>
 *            <tr><td>#N_E_NOT_SUPPORTED</td>
 *                <td>Image format specified by \p hImageFormat does not
 *                support writing.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatCanWrite | HNImage |
 *      NImageFormatSaveImageToMemory | NImageFormatLoadImageFromFile
 */
NResult N_API NImageFormatSaveImageToFile(HNImageFormat hImageFormat, HNImage hImage, const NChar * szFileName);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageFormatSaveImageToFileA(HNImageFormat hImageFormat, HNImage hImage, const NAChar * szFileName);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageFormatSaveImageToFileW(HNImageFormat hImageFormat, HNImage hImage, const NWChar * szFileName);
#endif
#define NImageFormatSaveImageToFile N_FUNC_AW(NImageFormatSaveImageToFile)

#endif

/**
 * \brief  Saves image to the memory buffer in the specified format.
 * \param[in]  hImageFormat   Handle to image format.
 * \param[in]  hImage         Handle to image.
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
 *                <td>\p hImageFormat, \p hImage, \p pBuffer or
 *                \p pBufferLength is #NULL.</td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>There was not enough memory to allocate memory buffer.
 *                </td></tr>
 *            <tr><td>#N_E_NOT_SUPPORTED</td>
 *                <td>Image format specified by \p hImageFormat does not
 *                support writing.</td></tr>
 *          </table>
 * \remark  Memory buffer allocated by this function must be deallocated by
 *          #NFree function when it is no longer needed.
 * \sa  HNImageFormat | NImageFormatCanWrite | HNImage |
 *      NImageFormatSaveImageToFile | NImageFormatLoadImageFromMemory
 */
NResult N_API NImageFormatSaveImageToMemory(HNImageFormat hImageFormat, HNImage hImage, void * * pBuffer, NSizeType * pBufferLength);

NResult N_API NImageFormatSaveImageToStream(HNImageFormat hImageFormat, HNImage hImage, HNStream hStream);

#ifdef N_DOCUMENTATION

/**
 * \brief  Retrieves name of the image format.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  pValue        Pointer to the string that receives name of the
 *                           image format. Can be #NULL.
 * \return  If the function succeeds and \p pValue is #NULL, the return value
 *          is length of the string (not including NULL-terminator) \p pValue
 *          should point to.
 * \return  If the function succeeds and \p pValue is not #NULL, the return
 *          value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat
 */
NResult N_API NImageFormatGetName(HNImageFormat hImageFormat, NChar * pValue);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageFormatGetNameA(HNImageFormat hImageFormat, NAChar * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageFormatGetNameW(HNImageFormat hImageFormat, NWChar * pValue);
#endif
#define NImageFormatGetName N_FUNC_AW(NImageFormatGetName)

#endif

#ifdef N_DOCUMENTATION

/**
 * \brief  Retrieves default extension of the image format.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  pValue        Pointer to the string that receives default file
 *                           extension of the image format. Can be #NULL.
 * \return  If the function succeeds and \p pValue is #NULL, the return value
 *          is length of the string (not including NULL-terminator) \p pValue
 *          should point to.
 * \return  If the function succeeds and \p pValue is not #NULL, the return
 *          value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat
 */
NResult N_API NImageFormatGetDefaultFileExtension(HNImageFormat hImageFormat, NChar * pValue);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageFormatGetDefaultFileExtensionA(HNImageFormat hImageFormat, NAChar * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageFormatGetDefaultFileExtensionW(HNImageFormat hImageFormat, NWChar * pValue);
#endif
#define NImageFormatGetDefaultFileExtension N_FUNC_AW(NImageFormatGetDefaultFileExtension)

#endif

#ifdef N_DOCUMENTATION

/**
 * \brief  Retrieves file filter of the image format.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  pValue        Pointer to the string that receives file filter
 *                           of the image format. Can be #NULL.
 * \return  If the function succeeds and \p pValue is #NULL, the return value
 *          is length of the string (not including NULL-terminator) \p pValue
 *          should point to.
 * \return  If the function succeeds and \p pValue is not #NULL, the return
 *          value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat
 */
NResult N_API NImageFormatGetFileFilter(HNImageFormat hImageFormat, NChar * pValue);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageFormatGetFileFilterA(HNImageFormat hImageFormat, NAChar * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageFormatGetFileFilterW(HNImageFormat hImageFormat, NWChar * pValue);
#endif
#define NImageFormatGetFileFilter N_FUNC_AW(NImageFormatGetFileFilter)

#endif

/**
 * \brief  Retrieves a value indicating whether the image format supports
 *         reading.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  pValue        Pointer to #NBool that receives value indicating
 *                           whether the image format supports reading.
 * \return  If the function succeeds, the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat or \p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatCanWrite
 */
NResult N_API NImageFormatCanRead(HNImageFormat hImageFormat, NBool * pValue);
/**
 * \brief  Retrieves a value indicating whether the image format supports
 *         writing.
 * \param[in]  hImageFormat  Handle to image format.
 * \param[in]  pValue        Pointer to #NBool that receives value indicating
 *                           whether the image format supports writing.
 * \return  If the function succeeds, the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImageFormat or \p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImageFormat | NImageFormatCanRead
 */
NResult N_API NImageFormatCanWrite(HNImageFormat hImageFormat, NBool * pValue);

#ifdef N_CPP
}
#endif

#endif // !N_IMAGE_FORMAT_H_INCLUDED
