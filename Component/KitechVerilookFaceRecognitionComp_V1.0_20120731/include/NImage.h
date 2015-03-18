/** \file NImage.h \brief Provides functionality for managing images. */

/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Images 2.3                         *|
|*                                                                            *|
|* NImage.h                                                                   *|
|* Header file for NImage module                                              *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_IMAGE_H_INCLUDED
#define N_IMAGE_H_INCLUDED

#include "NPixelFormat.h"

#ifdef N_CPP
extern "C"
{
#endif

/// Handle to the image.
N_DECLARE_HANDLE(HNImage)

#ifdef N_CPP
}
#endif

#include "NImageFormat.h"

#ifdef N_CPP
extern "C"
{
#endif

typedef enum NImageRotateFlipType_ // Flags
{
	nirftRotateNone = 0,
	nirftRotate90 = 1,
	nirftRotate180 = 2,
	nirftRotate270 = 3,
	nirftFlipNone = 0,
	nirftFlipX = 4,
	nirftFlipY = 8,
	nirftFlipXY = nirftFlipX | nirftFlipY,
	nirftNone = nirftRotateNone | nirftFlipNone
} NImageRotateFlipType;

NResult N_API NImageCopy(HNImage hSrcImage, NUInt left, NUInt top, NUInt width, NUInt height, HNImage hDstImage, NUInt dstLeft, NUInt dstTop);

/**
 * \brief  Creates an image wrapper for specified pixels with specified pixel
 *         format, size, stride and resolution.
 * \param[in]  pixelFormat     Specifies pixel format of the image.
 * \param[in]  width           Specifies width of the image.
 * \param[in]  height          Specifies height of the image.
 * \param[in]  stride          Specifies stride of the image.
 * \param[in]  horzResolution  Specifies horizontal resolution in pixels per
 *                             inch of the image.
 * \param[in]  vertResolution  Specifies vertical resolution in pixels per
 *                             inch of the image.
 * \param[in]  pixels          Points to memory block containing pixels for the
 *                             image.
 * \param[in]  ownsPixels      Specifies whether pixels will be automatically
 *                             deleted with the image (if set to #NTrue).
 * \param[out] pHImage         Pointer to #HNImage that receives handle to
 *                             created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td>
 *                <td>\p pixelFormat has invalid value.</td></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
				  <td>\p stride is less than minimal value for specified pixel
 *                format and width.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pixels or \p pHImage is #NULL.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p width or \p height is zero.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
				  <td>\p horzResolution or \p vertResolution is less than zero.
 *                </td></tr>
 *          </table>
 * \remark  For more information on image stride see #NImageGetStride function.
 * \remark  Format of memory block \p pixels points to must be the same as
 *          described in #NImageGetPixels function.
 * \remark  Created image must be deleted using NImageFree function.
 * \remark  \p pixels must not be deleted during lifetime of the image. If
 *          \p ownsPixels is #NTrue then pixels will be automatically deleted
 *          with the image.
 * \remark  \p horzResolution and \p vertResolution can be zero if resolution
 *          is not applicable for the image.
 * \sa  HNImage | NImageFree | NImageCreate | NImageCreateFromData |
 *      NImageGetStride | NImageGetPixels
 */
NResult N_API NImageCreateWrapper(NPixelFormat pixelFormat,
	NUInt width, NUInt height, NSizeType stride, NFloat horzResolution, NFloat vertResolution,
	void * pixels, NBool ownsPixels, HNImage * pHImage);

NResult N_API NImageCreateWrapperForPart(NPixelFormat pixelFormat,
	NUInt width, NUInt height, NSizeType stride, NFloat horzResolution, NFloat vertResolution,
	void * pixels, NUInt left, NUInt top, HNImage * pHImage);

NResult N_API NImageCreateWrapperForImagePart(HNImage hSrcImage, NUInt left, NUInt top, NUInt width, NUInt height,
	HNImage * pHImage);

NResult N_API NImageCreateWrapperForImagePartEx(HNImage hSrcImage, NUInt left, NUInt top, NUInt width, NUInt height,
	NFloat horzResolution, NFloat vertResolution,
	HNImage * pHImage);

/**
 * \brief  Creates an image with specified pixel format, size, stride and
 *         resolution.
 * \param[in]  pixelFormat     Specifies pixel format of the image.
 * \param[in]  width           Specifies width of the image.
 * \param[in]  height          Specifies height of the image.
 * \param[in]  stride          Specifies stride of the image.
 * \param[in]  horzResolution  Specifies horizontal resolution in pixels per
 *                             inch of the image.
 * \param[in]  vertResolution  Specifies vertical resolution in pixels per
 *                             inch of the image.
 * \param[out] pHImage         Pointer to #HNImage that receives handle to
 *                             created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td>
 *                <td>\p pixelFormat has invalid value.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td><td>\p stride is not zero and is less than minimal value for
 *                specified pixel format and width.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pHImage is #NULL.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p width or \p height is zero.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td><td>\p horzResolution or \p vertResolution is less than zero.
 *                </td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>There was not enough memory.</td></tr>
 *          </table>
 * \remark  If \p stride is zero then image stride is automatically calculated.
 *          For more information on image stride see #NImageGetStride function.
 * \remark  Created image must be deleted using #NImageFree function.
 * \remark  \p horzResolution and \p vertResolution can be zero if resolution
 *          is not applicable for the image.
 * \sa  HNImage | NImageFree | NImageCreateWrapper | NImageCreateFromData |
 *      NImageCreateFromImage | NImageCreateFromFile | NImageClone |
 *      NImageGetStride
 */
NResult N_API NImageCreate(NPixelFormat pixelFormat,
	NUInt width, NUInt height, NSizeType stride, NFloat horzResolution, NFloat vertResolution,
	HNImage * pHImage);
/**
 * \brief  Creates an image wrapper for specified pixels with specified pixel
 *         format, size, stride and resolution.
 * \param[in]  pixelFormat     Specifies pixel format of the image.
 * \param[in]  width           Specifies width of the image.
 * \param[in]  height          Specifies height of the image.
 * \param[in]  stride          Specifies stride of the image. Can be zero.
 * \param[in]  horzResolution  Specifies horizontal resolution in pixels per
 *                             inch of the image.
 * \param[in]  vertResolution  Specifies vertical resolution in pixels per
 *                             inch of the image.
 * \param[in]  srcStride       Specifies stride of pixels to be copied to the
 *                             image.
 * \param[in]  srcPixels       Points to memory block containing pixels that
 *                             will be copied to the new image.
 * \param[out] pHImage         Pointer to #HNImage that receives handle to
 *                             created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="3">#N_E_ARGUMENT</td>
 *                <td>\p pixelFormat has invalid value.</td></tr>
 *            <tr><td rowspan="3">#N_E_ARGUMENT</td>
				  <td>\p stride is less than minimal value for specified pixel
 *                format and width.</td></tr>
 *            <tr><td rowspan="3">#N_E_ARGUMENT</td>
				  <td>\p srcStride is less than minimal value for specified
 *                pixel format and width.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p srcPixels or \p pHImage is #NULL.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p width or \p height is zero.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_OUT_OF_RANGE</td>
				  <td>\p horzResolution or \p vertResolution is less than zero.
 *                </td></tr>
 *          </table>
 * \remark  If \p stride is zero then image stride is automatically calculated.
 *          For more information on image stride see #NImageGetStride function.
 * \remark  Format of memory block \p srcPixels points to must be the same as
 *          described in #NImageGetPixels function, only stride is equal to
 *          \p srcStride.
 * \remark  Created image must be deleted using NImageFree function.
 * \remark  \p pixels must not be deleted during lifetime of the image. If
 *          \p ownsPixels is #NTrue then pixels will be automatically deleted
 *          with the image.
 * \remark  \p horzResolution and \p vertResolution can be zero if resolution
 *          is not applicable for the image.
 * \sa  HNImage | NImageFree | NImageCreate | NImageCreateWrapper |
 *      NImageGetStride | NImageGetPixels
 */
NResult N_API NImageCreateFromData(NPixelFormat pixelFormat,
	NUInt width, NUInt height, NSizeType stride, NFloat horzResolution, NFloat vertResolution,
	NSizeType srcStride, const void * srcPixels, HNImage * pHImage);

NResult N_API NImageCreateFromDataPart(NPixelFormat pixelFormat,
	NUInt width, NUInt height, NSizeType stride, NFloat horzResolution, NFloat vertResolution,
	NSizeType srcStride, const void * srcPixels, NUInt left, NUInt top, HNImage * pHImage);

/**
 * \brief  Creates an image from specified image with specified pixel format
 *         and stride.
 * \param[in]  pixelFormat  Specifies pixel format of the image.
 * \param[in]  stride       Specifies stride of the image. Can be zero.
 * \param[in]  hSrcImage    Handle to image used as source for the image.
 * \param[out] pHImage      Pointer to #HNImage that receives handle to
 *                          created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td>
 *                <td>\p pixelFormat has invalid value.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td><td>\p stride is less than minimal value for specified pixel
 *                format and width.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hSrcImage or \p pHImage is #NULL.</td></tr>
 *          </table>
 * \remark  If \p stride is zero then image stride is automatically calculated.
 *          For more information on image stride see #NImageGetStride function.
 * \remark  Created image must be deleted using NImageFree function.
 * \remark  \p horzResolution and \p vertResolution can be zero if resolution
 *          is not applicable for the image.
 * \sa  HNImage | NImageFree | NImageCreate | NImageCreateFromImageEx |
 *      NImageClone | NImageGetStride
 */
NResult N_API NImageCreateFromImage(NPixelFormat pixelFormat,
	NSizeType stride, HNImage hSrcImage, HNImage * pHImage);
/**
 * \brief  Creates an image from specified image with specified pixel format,
 *         stride and resolution.
 * \param[in]  pixelFormat     Specifies pixel format of the image.
 * \param[in]  stride          Specifies stride of the image. Can be zero.
 * \param[in]  horzResolution  Specifies horizontal resolution in pixels per
 *                             inch of the image.
 * \param[in]  vertResolution  Specifies vertical resolution in pixels per
 *                             inch of the image.
 * \param[in]  hSrcImage       Handle to image used as source for the image.
 * \param[out] pHImage         Pointer to #HNImage that receives handle to
 *                             created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td>
 *                <td>\p pixelFormat has invalid value.</td></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT</td><td>\p stride is less than minimal value for specified pixel
 *                format and width.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hSrcImage or \p pHImage is #NULL.</td></tr>
 *          </table>
 * \remark  If \p stride is zero then image stride is automatically calculated.
 *          For more information on image stride see #NImageGetStride function.
 * \remark  Created image must be deleted using NImageFree function.
 * \remark  \p horzResolution and \p vertResolution can be zero if resolution
 *          is not applicable for the image.
 * \sa  HNImage | NImageFree | NImageCreate | NImageCreateFromImageEx |
 *      NImageClone | NImageGetStride
 */
NResult N_API NImageCreateFromImageEx(NPixelFormat pixelFormat,
	NSizeType stride, NFloat horzResolution, NFloat vertResolution, HNImage hSrcImage, HNImage * pHImage);

NResult N_API NImageCreateFromImagePart(NPixelFormat pixelFormat,
	NSizeType stride, HNImage hSrcImage,
	NUInt left, NUInt top, NUInt width, NUInt height, HNImage * pHImage);

NResult N_API NImageCreateFromImagePartEx(NPixelFormat pixelFormat,
	NSizeType stride, NFloat horzResolution, NFloat vertResolution, HNImage hSrcImage,
	NUInt left, NUInt top, NUInt width, NUInt height, HNImage * pHImage);

#ifdef N_DOCUMENTATION

/**
 * \brief  Creates (loads) an image from file of specified format.
 * \param[in]  szFileName    Points to string that specifies file name.
 * \param[in]  hImageFormat  Handle to the image format of the file. Can be
 *                           #NULL.
 * \param[out] pHImage       Pointer to #HNImage that receives handle to
 *                           created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p szFileName or \p pHImage is #NULL.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Format of file specified by \p szFileName is invalid for
 *                specified image format.</td></tr>
 *            <tr><td rowspan="3">#N_E_NOT_SUPPORTED</td>
 *                <td>\p hImageFormat is #NULL and none of supported image
 *                formats is registered with file extension of \p szFileName.
 *                </td></tr>
 *            <tr><td rowspan="3">#N_E_NOT_SUPPORTED</td>
				  <td>\p hImageFormat is #NULL and image format registered with
 *                file extension of \p szFileName does not support reading.</td>
			  <tr><td rowspan="3">#N_E_NOT_SUPPORTED</td>
				  <td>Image format specified by \p hImageFormat does not
 *                support reading.</td></tr>
 *          </table>
 * \remark  If \p hImageFormat is #NULL image format is selected by file
 *          extension of \p szFileName.
 * \remark  Created image must be deleted using NImageFree function.
 * \sa  HNImage | NImageFree | NImageCreate | NImageFormatCanRead
 */
NResult N_API NImageCreateFromFile(const NChar * szFileName, HNImageFormat hImageFormat, HNImage * pHImage);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageCreateFromFileA(const NAChar * szFileName, HNImageFormat hImageFormat, HNImage * pHImage);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageCreateFromFileW(const NWChar * szFileName, HNImageFormat hImageFormat, HNImage * pHImage);
#endif
#define NImageCreateFromFile N_FUNC_AW(NImageCreateFromFile)

#endif

/**
 * \brief  Deletes the image. After the image is deleted the specified handle
 *         is no longer valid.
 * \param[in]  hImage  Handle to the image.
 * \remark  If \p hImage is #NULL does nothing.
 * \sa  HNImage | NImageCreate
 */
void N_API NImageFree(HNImage hImage);

/**
 * \brief  Creates a new image that is a copy of specified image.
 * \param[in]  hImage         Handle to the image.
 * \param[out] pHClonedImage  Pointer to #HNImage that receives handle to
 *                            created image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pHClonedImage is #NULL.</td></tr>
 *          </table>
 * \remark  Created image must be deleted using #NImageFree function.
 * \sa  HNImage | NImageFree | NImageCreate
 */
NResult N_API NImageClone(HNImage hImage, HNImage * pHClonedImage);

#ifdef N_DOCUMENTATION

/**
 * \brief  Saves the image to the file of specified format.
 * \param[in]  hImage        Handle to the image.
 * \param[in]  szFileName    Pointer to the string that specifies file name.
 * \param[in]  hImageFormat  Handle to the image format of the file. Can be
 *                           #NULL.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p szFileName is #NULL.</td></tr>
 *            <tr><td rowspan="3">#N_E_NOT_SUPPORTED</td>
 *                <td>\p hImageFormat is #NULL and none of supported image
 *                formats is registered with file extension of \p szFileName.
 *                </td></tr>
 *            <tr><td rowspan="3">#N_E_NOT_SUPPORTED</td><td>\p hImageFormat is #NULL and image format registered with
 *                file extension of \p szFileName does not support writing.
 *                </td></tr>
 *            <tr><td rowspan="3">#N_E_NOT_SUPPORTED</td><td>Image format specified by \p hImageFormat does not
 *                support writing.</td></tr>
 *          </table>
 * \remark  If \p hImageFormat is #NULL image format is selected by the file
 *          extension of \p szFileName.
 * \sa  HNImage | NImageCreateFromFile | NImageFormatCanWrite
 */
NResult N_API NImageSaveToFile(HNImage hImage, const NChar * szFileName, HNImageFormat hImageFormat);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API NImageSaveToFileA(HNImage hImage, const NAChar * szFileName, HNImageFormat hImageFormat);
#endif
#ifndef N_NO_UNICODE
NResult N_API NImageSaveToFileW(HNImage hImage, const NWChar * szFileName, HNImageFormat hImageFormat);
#endif
#define NImageSaveToFile N_FUNC_AW(NImageSaveToFile)

#endif

NResult N_API NImageFlipHorizontally(HNImage hImage);
NResult N_API NImageFlipVertically(HNImage hImage);
NResult N_API NImageFlipDiagonally(HNImage hImage);
NResult N_API NImageRotateFlip(HNImage hImage, NImageRotateFlipType rotateFlipType, HNImage * pHResultImage);
NResult N_API NImageCrop(HNImage hImage, NUInt left, NUInt top, NUInt width, NUInt height, HNImage * pHResultImage);

/**
 * \brief  Retrieves pixel format of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NPixelFormat that receives pixel format of
 *                     the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImage | NPixelFormat
 */
NResult N_API NImageGetPixelFormat(HNImage hImage, NPixelFormat * pValue);
/**
 * \brief  Retrieves width of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NUInt that receives width of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImage | NImageGetHeight | NImageGetStride
 */
NResult N_API NImageGetWidth(HNImage hImage, NUInt * pValue);
/**
 * \brief  Retrieves height of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NUInt that receives height of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \sa  HNImage | NImageGetWidth | NImageGetStride
 */
NResult N_API NImageGetHeight(HNImage hImage, NUInt * pValue);
/**
 * \brief  Retrieves stride (size of one row) of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NSizeType that receives stride of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \remark  Stride (size of one row) of the image depends on image pixel format
 *          and width. It can not be less than value obtained with
 *          #NPixelFormatGetRowSize macro with arguments obtained with
 *          #NImageGetPixelFormat and #NImageGetWidth functions.
 * \sa  HNImage | NPixelFormatGetRowSize | NImageGetPixelFormat |
 *      NImageGetHeight | NImageGetSize
 */
NResult N_API NImageGetStride(HNImage hImage, NSizeType * pValue);
/**
 * \brief  Retrieves size of memory block containing pixels of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NSizeType that receives size of memory block
 *                     containing pixels of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \remark  Size of memory block containing image pixels is equal to image
 *          height multiplied by image stride. For more information see
 *          #NImageGetHeight and #NImageGetStride functions.
 * \sa  HNImage | NImageGetHeight | NImageGetStride
 */
NResult N_API NImageGetSize(HNImage hImage, NSizeType * pValue);
/**
 * \brief  Retrieves horizontal resolution of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NFloat that receives horizontal resolution
 *                     in pixels per inch of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \remark  Horizontal resolution equal to zero means that it is not applicable
 *          for the image.
 * \sa  HNImage | NImageGetVertResolution
 */
NResult N_API NImageGetHorzResolution(HNImage hImage, NFloat * pValue);
/**
 * \brief  Retrieves vertical resolution of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to #NFloat that receives vertical resolution in
 *                     pixels per inch of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \remark  Vertical resolution equal to zero means that it is not applicable
 *          for the image.
 * \sa  HNImage | NImageGetHorzResolution
 */
NResult N_API NImageGetVertResolution(HNImage hImage, NFloat * pValue);
/**
 * \brief  Retrieves pointer to memory block containing pixels of the image.
 * \param[in]  hImage  Handle to the image.
 * \param[out] pValue  Pointer to void * that receives pointer to memory block
 *                     containing pixels of the image.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hImage or \p pValue is #NULL.</td></tr>
 *          </table>
 * \remark  Memory block containing image pixels is organized as image height
 *          rows following each other in top-to-bottom order. Each row occupies
 *          image stride bytes and is organized as image width pixels following
 *          each other in right-to-left order. Each pixel is described by image
 *          pixel format.
 * \remark  For more information see #NImageGetPixelFormat, #NImageGetWidth,
 *          #NImageGetHeight, #NImageGetStride, and #NImageGetSize functions.
 * \sa  HNImage | NImageGetPixelFormat | NImageGetWidth | NImageGetHeight |
 *      NImageGetStride | NImageGetSize
 */
NResult N_API NImageGetPixels(HNImage hImage, void * * pValue);

#ifdef N_CPP
}
#endif

#endif // !N_IMAGE_H_INCLUDED
