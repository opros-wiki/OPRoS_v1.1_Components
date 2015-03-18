/*! \file Camera.h
\brief Provides functionality for working with cameras.
*/

/******************************************************************************\
|*                                                                            *|
|*                     Neurotechnology Camera Manager 1.0                     *|
|*                                                                            *|
|* Camera.h                                                                   *|
|* Header file for Camera module                                              *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <NImage.h>

#include "CameraParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HCamera);


#ifdef N_DOCUMENTATION

/**
 * \brief  Gets associated device identifier.
 * \param[in]  hCamera  Handle to the camera object.
 * \param[out] pValue   Pointer to string that receives camera identifier.
 * \return  If the function succeeds and \p pValue is #NULL, the return value
 *          is length of the string (not including the NULL-terminator)
 *          \p pValue should point to.
 * \return  If the function succeeds and \p pValue is not #NULL, the return
 *          value is #N_OK
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p hCamera is #NULL.</td></tr>
 *          </table>
 */
NResult N_API CameraGetID(HCamera hCamera, NAChar *pValue);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API CameraGetIDA(HCamera hCamera, NAChar *pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API CameraGetIDW(HCamera hCamera, NWChar *pValue);
#endif
#define CameraGetID N_FUNC_AW(CameraGetID)

#endif

#ifdef N_DOCUMENTATION

/**
 * \brief  Retrieves value of the parameter.
 * \param[in]  hCamera      Handle to the camera object.
 * \param[in]  parameterId  Identifier of the parameter to retrieve.
 * \param[out] pValue       Pointer to a variable that receives parameter
 *                          value.
 * \return  If the function succeeds and \p parameterId specifies
 *          #N_TYPE_STRING type parameter and \p pValue is #NULL, the return
 *          value is length of the string (not including NULL-terminator)
 *          \p pValue should point to.
 * \return  If the function succeeds and \p pValue is not #NULL, the return
 *          value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td rowspan="2">#N_E_ARGUMENT_NULL</td>
 *                <td>\p parameterId specifies a non-static parameter and
 *                \p hCamera is #NULL.</td></tr>
 *            <tr><td>\p parameterId specifies a non-#N_TYPE_STRING type
 *                parameter and \p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_FAILED</td>
 *                <td>Unspecified error has occurred.</td></tr>
 *            <tr><td>#N_E_PARAMETER</td>
 *                <td>\p parameterId is invalid.</td></tr>
 *          </table>
 * \remark  The following values can be used for \p parameterId:
 *          - #CAMERAP_MIRROR_HORIZONTAL
 *          - #CAMERAP_MIRROR_VERTICAL
 *          - #CAMERAP_AUTOMATIC_SETTINGS
 *          To learn the type of parameter pass a value obtained with
 *          #NParameterMakeId macro using #N_PC_TYPE_ID code, the parameter
 *          identifier via \p parameterId and pointer to #NInt that will
 *          receive one of the N_TYPE_XXX via \p pValue parameter.
 *          \p hCamera can be #NULL in this case.
 * \sa  HCamera | CameraSetParameter
 *
 * Retrieves value of the specified parameter of the specified camera
 * object.
 */
NResult N_API CameraGetParameter(HCamera hCamera, NUInt parameterId, void *pValue);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API CameraGetParameterA(HCamera hCamera, NUInt parameterId, void *pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API CameraGetParameterW(HCamera hCamera, NUInt parameterId, void *pValue);
#endif
#define CameraGetParameter N_FUNC_AW(CameraGetParameter)
#ifndef N_NO_ANSI_FUNC

#endif

#ifdef N_DOCUMENTATION

/**
 * \brief  Sets value of the parameter.
 * \param[in]  hCamera      Handle to the camera object. Can be #NULL if
 *                          setting static parameter value.
 * \param[in]  parameterId  Identifier of the parameter to set.
 * \param[in]  pValue       Pointer to the parameter value to set.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p pValue points to a value that is out of range.</td>
 *                </tr>
 *            <tr><td>#N_E_PARAMETER</td>
 *                <td>\p parameterId is invalid.</td></tr>
 *            <tr><td>#N_E_PARAMETER_READ_ONLY</td>
 *                <td>\p parameterId specifies a read-only parameter.</td></tr>
 *          </table>
 * \remark  The following values can be used for \p parameterId:
 *          - #CAMERAP_MIRROR_HORIZONTAL
 *          - #CAMERAP_MIRROR_VERTICAL
 *          - #CAMERAP_AUTOMATIC_SETTINGS
 *          To learn the type of parameter pass a value obtained with
 *          #NParameterMakeId macro using #N_PC_TYPE_ID code, the parameter
 *          identifier via \p parameterId and pointer to #NInt that will
 *          receive one of the N_TYPE_XXX via \p pValue parameter.
 *          \p hCamera can be #NULL in this case.
 * \sa  HCamera | CameraGetParameter
 *
 * Sets value of the specified parameter of the specified camera object.
 */
NResult N_API CameraSetParamete(HCamera hCamera, NUInt parameterId, const void *pValue);

#else

NResult N_API CameraSetParameterA(HCamera hCamera, NUInt parameterId, const void *pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API CameraSetParameterW(HCamera hCamera, NUInt parameterId, const void *pValue);
#endif
#define CameraSetParameter N_FUNC_AW(CameraSetParameter)

#endif

/**
 * \brief  Copies parameter values from one camera object to another.
 * \param[in]  hDstCamera  Handle to the destination camera object.
 * \param[in]  hSrcCamera  Handle to the source camera object.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hDstCamera or \p hSrcCamera is #NULL.</td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>\p hDstCamerais capturing.</td></tr>
 *          </table>
 * \sa  HCamera
 *
 * Copies parameters values from one camera object to another.
 */
NResult N_API CameraCopyParameters(HCamera hDstCamera, HCamera hSrcCamera);

// structures

/**
 * \brief  Camera video format.
 * \sa  CameraGetAvailableVideoFormats | CameraGetVideoFormat
 *
 * Camera video format identified by frame width, height and rate. Can be
 * initialized manually or retrieved by library functions.
 */
typedef struct CameraVideoFormat_
{
	NInt FrameWidth;   ///< Frame width.
	NInt FrameHeight;  ///< Frame height.
	NFloat FrameRate;  ///< Frame rate.
} CameraVideoFormat;

// capturing

/**
 * \brief  Gets available video formats.
 * \param[in]  hCamera            Handle to camera object.
 * \param[out] pVideoFormatCount  Pointer to #NInt that receives number of
 *                                available video formats.
 * \param[out] pArVideoFormats    Pointer to array of #CameraVideoFormat of all
 *                                available video formats.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera, \p pVideoFormatCount or \p pArVideoFormats is
 *                    #NULL.</td></tr>
 *          </table>
 * \remark  The buffer for \p pArVideoFormats variable is allocated inside the
 *          function and must be deleted by the user using #NFree function from
 *          NCore library.
 *
 * Gets available video formats for the camera.
 */
NResult N_API CameraGetAvailableVideoFormats(HCamera hCamera,
	NInt *pVideoFormatCount, CameraVideoFormat **pArVideoFormats);

/**
 * \brief  Gets current video format.
 * \param[in]  hCamera       Handle to camera.
 * \param[out] pVideoFormat  Pointer to #CameraVideoFormat that receives
 *                           current video format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera or \p pVideoFormat is #NULL.</td></tr>
 *          </table>
 *
 * Gets video format that will be used to initialize camera on capturing.
 */
NResult N_API CameraGetVideoFormat(HCamera hCamera, CameraVideoFormat *pVideoFormat);

/**
 * \brief  Sets capturing video format.
 * \param[in]  hCamera      Handle to the camera object.
 * \param[in]  pVideoFormat Pointer to #CameraVideoFormat that indicates
 *                          demanded video format.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera or \p pVideoFormat is #NULL.</td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>Capturing is already started.</td></tr>
 *          </table>
 *
 * Sets capturing video format that was received by
 * #CameraGetAvailableVideoFormats function call or initialized manually.
 */
NResult N_API CameraSetVideoFormat(HCamera hCamera, CameraVideoFormat *pVideoFormat);

/**
 * \brief  Checks camera status.
 * \param[in]  hCamera  Handle to the camera object.
 * \param[out] pValue   Pointer to #NBool that receives value indicating
 *                      whether camera is already capturing
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera or \p pValue is #NULL.</td></tr>
 *          </table>
 *
 * Checks whether camera is already capturing.
 */
NResult N_API CameraIsCapturing(HCamera hCamera, NBool *pValue);

/**
 * \brief  Starts capturing.
 * \param[in]  hCamera  Handle to the camera object
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera is #NULL.</td></tr>
 *            <tr><td>#N_E_FAILED</td>
 *                <td>Unspecified error has occurred.</td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>The camera is already capturing.</td></tr>
 *          </table>
 *
 * Starts capturing with the specified camera.
 */
NResult N_API CameraStartCapturing(HCamera hCamera);

/**
 * \brief  Stops capturing.
 * \param[in]  hCamera  Handle to the camera object
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera is #NULL.</td></tr>
 *          </table>
 *
 * Stops capturing with the specified camera.
 */
NResult N_API CameraStopCapturing(HCamera hCamera);

/**
 * \brief  Gets current frame from the capture device.
 * \param[in]  hCamera  Handle to the camera object.
 * \param[out] pHImage  Pointer to #HNImage that receives handle to created
 *                      #NImage object that contains current frame from the
 *                      camera.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hCamera or \p pHImage is #NULL.</td></tr>
 *          </table>
 *
 * Gets current frame from the specified capture device.
 */
NResult N_API CameraGetCurrentFrame(HCamera hCamera, HNImage *pHImage);

#ifdef N_CPP
}
#endif

#endif // !CAMERA_H_INCLUDED
