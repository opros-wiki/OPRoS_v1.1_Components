/** \file NGeometry.h 
\brief Defines gometry primitives used in Neurotechnology components
*/

/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NGeometry.h                                                                *|
|* Geometry types definition                                                  *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_GEOMETRY_H_INCLUDED
#define N_GEOMETRY_H_INCLUDED

#include "NTypes.h"

#ifdef N_CPP
extern "C"
{
#endif

/// Structure defining point coordinates in 2D space.
typedef struct NPoint_
{
	NInt X; ///< Point coordinate on \a X axis.
	NInt Y; ///< Point coordinate on \a Y axis.
} NPoint;

/// Structure defining rectangle size.
typedef struct NSize_
{
	NInt Width;
	NInt Height;
} NSize;

/// Structure defining a rectangle figure in 2D space.
typedef struct NRect_
{
	NInt X;      ///< Upper left rectangle corner coordinate on \a X axis.
	NInt Y;      ///< Upper left rectangle corner coordinate on \a Y axis.
	NInt Width;  ///< Rectangle width.
	NInt Height; ///< Rectangle height.
} NRect;

#ifdef N_CPP
}
#endif

#endif // !N_GEOMETRY_H_INCLUDED
