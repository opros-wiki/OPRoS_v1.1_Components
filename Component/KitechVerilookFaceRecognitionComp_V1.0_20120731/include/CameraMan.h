/*! \file CameraMan.h
\brief Provides management of cameras. 
*/


/******************************************************************************\
|*                                                                            *|
|*                     Neurotechnology Camera Manager 1.0                     *|
|*                                                                            *|
|* CameraMan.h                                                                *|
|* Header file for Camera Manager module                                      *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef CAMERA_MAN_H_INCLUDED
#define CAMERA_MAN_H_INCLUDED

#include "Camera.h"

#ifdef N_CPP
extern "C"
{
#endif

#ifdef N_DOCUMENTATION

/**
 * \brief  Gets information about the library.
 * \param[out] *pValue  Pointer to #NLibraryInfo that receives information
 *                      about the library.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pValue is #NULL.</td></tr>
 *          </table>
 * \sa NLibraryInfo
 *
 * Gets information about the library as title, product name, company,
 * copyright message, and version.
 */
NResult N_API CameraManGetInfo(NLibraryInfo *pValue);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API CameraManGetInfoA(NLibraryInfoA *pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API CameraManGetInfoW(NLibraryInfoW *pValue);
#endif
#define CameraManGetInfo N_FUNC_AW(CameraManGetInfo)

#endif

/**
 * \brief  Initializes CameraMan library.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_FAILED</td>
 *                <td>Unspecified error has occurred.</td></tr>
 *          </table>
 * \sa  CameraManUninitialize | CameraManGetCameraCount | CameraManGetCamera |
 *      CameraManGetCameraByID
 *
 * Initializes the library by creating a snapshot of all availbale cameras on
 * the system that can be later accessed and used.
 */
NResult N_API CameraManInitialize(void);

/**
 * \brief  Uninitializes CameraMan library
 * \sa  CameraManInitialize
 */
void N_API CameraManUninitialize(void);

/**
 * \brief  Retrieves the number of available cameras.
 * \param[out] pValue  Pointer to #NInt that receives number of cameras.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>CameraMan library was not initialized.</td></tr>
 *          </table>
 * \sa CameraManInitialize | CameraManGetCamera | CameraManGetCameraByID
 */
NResult N_API CameraManGetCameraCount(NInt *pValue);

/**
 * \brief  Retrieves the camera at the specified index.
 * \param[in]  index      Index of camera to retrieve.
 * \param[out] *pHCamera  Points to #HCamera that receives handle to camera.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pHCamera is #NULL.</td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>CameraMan library was not initialized.</td></tr>
 *          </table>
 * \sa CameraManInitialize | HCamera
 */
NResult N_API CameraManGetCamera(NInt index, HCamera *pHCamera);

#ifdef N_DOCUMENTATION

/**
 * \brief  Retrieves the camera with the specified identifier.
 * \param[in]  szID       Points to string that specifies the camera.
 * \param[out] *pHCamera  Points to #HCamera that receives handle to camera.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pHCamera is #NULL.</td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>CameraMan library was not initialized.</td></tr>
 *          </table>
 * \sa CameraManInitialize | HCamera
 */
NResult N_API CameraManGetCameraByID(const NChar *szID, HCamera *pHCamera);

#else

#ifndef N_NO_ANSI_FUNC
NResult N_API CameraManGetCameraByIDA(const NAChar *szID, HCamera *pHCamera);
#endif
#ifndef N_NO_UNICODE
NResult N_API CameraManGetCameraByIDW(const NWChar *szID, HCamera *pHCamera);
#endif
#define CameraManGetCameraByID N_FUNC_AW(CameraManGetCameraByID)

#endif

#ifdef N_CPP
}
#endif

#endif // !CAMERA_MAN_H_INCLUDED
