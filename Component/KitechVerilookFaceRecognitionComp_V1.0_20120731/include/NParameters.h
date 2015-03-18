/** \file NParameters.h \brief Provides functionality for working with parameters for Neurotechnology components. */

/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NParameters.h                                                              *|
|* Header file for NParameters module                                         *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_PARAMETERS_H_INCLUDED
#define N_PARAMETERS_H_INCLUDED

#include "NCore.h"

#ifdef N_CPP
extern "C"
{
#endif

// Type ids

#define N_TYPE_BYTE      1 ///< Specifies that parameter type is #NByte.
#define N_TYPE_SBYTE     2 ///< Specifies that parameter type is #NSByte.
#define N_TYPE_USHORT    3 ///< Specifies that parameter type is #NUShort.
#define N_TYPE_SHORT     4 ///< Specifies that parameter type is #NShort.
#define N_TYPE_UINT      5 ///< Specifies that parameter type is #NUInt.
#define N_TYPE_INT       6 ///< Specifies that parameter type is #NInt.
#ifndef N_NO_INT_64
#define N_TYPE_ULONG     7 ///< Specifies that parameter type is #NULong.
#define N_TYPE_LONG      8 ///< Specifies that parameter type is #NLong.
#endif
#define N_TYPE_BOOL     10 ///< Specifies that parameter type is #NBool.
#define N_TYPE_CHAR     20 ///< Specifies that parameter type is #NChar.
#ifndef N_NO_FLOAT
#define N_TYPE_FLOAT    30 ///< Specifies that parameter type is #NFloat.
#define N_TYPE_DOUBLE   31 ///< Specifies that parameter type is #NDouble.
#endif
#define N_TYPE_STRING  100 ///< Specifies that parameter type is nullterminated string of NChar.

// Parameter id codes

/**
 * \brief  Specifies that type id (#NInt value, one of N_TYPE_XXX) of the
 *         parameter should be retrieved.
 */
#define N_PC_TYPE_ID 1

/**
 * \brief  Makes parameter id.
 * \param  code   One of N_PC_XXX.
 * \param  index  Reserved, must be zero.
 * \param  id     One of the parameter ids provided by a Neurotechnology 
 *                module.
 */
#define NParameterMakeId(code, index, id) ((NUInt)(((code) << 24) | (((index) & 0xFF) << 16) | ((id) & 0xFFFF)))

#ifdef N_CPP
}
#endif

#endif // !N_PARAMETERS_H_INCLUDED
