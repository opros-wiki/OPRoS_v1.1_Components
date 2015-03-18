/*! \file CameraParams.h
\brief Provides functionality for working with parameters of cameras.
*/


/******************************************************************************\
|*                                                                            *|
|*                     Neurotechnology Camera Manager 1.0                     *|
|*                                                                            *|
|* CameraParams.h                                                             *|
|* Camera parameters definition                                               *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef CAMERA_PARAMS_H_INCLUDED
#define CAMERA_PARAMS_H_INCLUDED

#include "NParameters.h"

#ifdef N_CPP
extern "C"
{
#endif

/** Identifier specifying whether received frame should be mirrored horizontally. #N_TYPE_BOOL type. */
#define CAMERAP_MIRROR_HORIZONTAL  10200 // Bool
/** Identifier specifying whether received frame should be mirrored vertically. #N_TYPE_BOOL type. */
#define CAMERAP_MIRROR_VERTICAL    10201 // Bool

/** Identifier specifying whether automatic camera settings should be used. #N_TYPE_BOOL type. */
#define CAMERAP_AUTOMATIC_SETTINGS 10301 // Bool

#ifdef N_CPP
}
#endif

#endif // !CAMERA_PARAMS_H_INCLUDED
