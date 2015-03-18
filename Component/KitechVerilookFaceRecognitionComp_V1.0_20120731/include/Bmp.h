/** \file Bmp.h \brief Provides functionality for loading and saving images in BMP format.*/

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* Bmp.h                                                                      *|
|* Header file for Bmp module                                                 *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#include "NImage.h"

#ifdef N_CPP
extern "C"
{
#endif

/************\
|*          *|
|* BMP load *|
|*          *|
\************/

#ifdef N_DOCUMENTATION

/**
 * \brief  Loads image from BMP file.
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
 * \sa  HNImage | BmpLoadImageFromMemory | BmpLoadImageFromHBitmap |
 *      BmpSaveImageToFile
 */
NResult N_API BmpLoadImageFromFile(const NChar * szFileName, HNImage * pHImage);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API BmpLoadImageFromFileA(const NAChar * szFileName, HNImage * pHImage);
#endif
#ifndef N_NO_UNICODE
NResult N_API BmpLoadImageFromFileW(const NWChar * szFileName, HNImage * pHImage);
#endif
#define BmpLoadImageFromFile N_FUNC_AW(BmpLoadImageFromFile)

#endif

/**
 * \brief  Loads image from the memory buffer containing BMP file.
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
 *            <tr><td>\p pHImage is #NULL.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of file contained in memory buffer specified by
 *                \p buffer is invalid.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | BmpLoadImageFromFile | BmpLoadImageFromHBitmap |
 *      BmpSaveImageToMemory
 */
NResult N_API BmpLoadImageFromMemory(const void * buffer, NSizeType bufferLength, HNImage * pHImage);

NResult N_API BmpLoadImageFromStream(HNStream hStream, HNImage * pHImage);

#if defined(N_WINDOWS) || defined(N_DOCUMENTATION)
/**
 * \brief  Loads image from Windows HBITMAP.
 * \note  This function is available on Windows only.
 * \param[in]  handle   Pointer to memory buffer.
 * \param[out] pHImage  Pointer to #HNImage that receives handle to loaded
 *                      image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p handle or \p pHImage is #NULL.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | BmpLoadImageFromFile | BmpLoadImageFromMemory |
 *      BmpSaveImageToHBitmap
 */
NResult N_API BmpLoadImageFromHBitmap(NHandle handle, HNImage * pHImage);
#endif

/************\
|*          *|
|* BMP save *|
|*          *|
\************/

#ifdef N_DOCUMENTATION

/**
 * \brief  Saves image to file in BMP format.
 * \param[in]  hImage      Handle to image.
 * \param[in]  szFileName  Points to string that specifies file name.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p szFileName is #NULL.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | BmpSaveImageToMemory | BmpSaveImageToHBitmap |
 *      BmpLoadImageFromFile
 */
NResult N_API BmpSaveImageToFile(HNImage hImage, const NChar * szFileName);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API BmpSaveImageToFileA(HNImage hImage, const NAChar * szFileName);
#endif
#ifndef N_NO_UNICODE
NResult N_API BmpSaveImageToFileW(HNImage hImage, const NWChar * szFileName);
#endif
#define BmpSaveImageToFile N_FUNC_AW(BmpSaveImageToFile)

#endif

/**
 * \brief  Saves image to the memory buffer in BMP format.
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
 *                <td>\p hImage, \p pBuffer or \p pBufferLength is #NULL.
 *                </td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>There was not enough memory to allocate memory buffer.
 *                </td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \remark  Memory buffer allocated by this function must be deallocated by
 *          #NFree function when it is no longer needed.
 * \sa  HNImage | BmpSaveImageToFile | BmpSaveImageToHBitmap |
 *      BmpLoadImageFromMemory
 */
NResult N_API BmpSaveImageToMemory(HNImage hImage, void * * pBuffer, NSizeType * pBufferLength);

NResult N_API BmpSaveImageToStream(HNImage hImage, HNStream hStream);

#if defined(N_WINDOWS) || defined(N_DOCUMENTATION)
/**
 * \brief  Saves image Windows HBITMAP.
 * \note  This function is available on Windows only.
 * \param[in]  hImage   Handle to image.
 * \param[in]  pHandle  Pointer to #NHandle that receives handle to created
 *                      Windows HBITMAP.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pHandle is #NULL.</td></tr>
 *          </table>
 * \remark  This is a low-level function and can be changed in future version
 *          of the library.
 * \sa  HNImage | BmpSaveImageToFile | BmpSaveImageToMemory |
 *      BmpLoadImageFromHBitmap
 */
NResult N_API BmpSaveImageToHBitmap(HNImage hImage, NHandle * pHandle);
#endif

#ifdef N_CPP
}
#endif

#endif // !BMP_H_INCLUDED
