/** \file NMemory.h \brief Provides memory management for Neurotechnology components. */

/******************************************************************************\
|*                                                                            *|
|*                           Neurotechnology Core 2.4                         *|
|*                                                                            *|
|* NMemory.h                                                                  *|
|* Header file for NMemory module                                             *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_MEMORY_H_INCLUDED
#define N_MEMORY_H_INCLUDED

#include "NTypes.h"

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief  Allocates memory block.
 */
NResult N_API NAlloc(NSizeType size, void * * ppBlock);
/**
 * \brief  Allocates memory block with all bytes set to zero.
 */
NResult N_API NCAlloc(NSizeType size, void * * ppBlock);
/**
 * \brief  Reallocates memory block.
 */
NResult N_API NReAlloc(void * * ppBlock, NSizeType size);
/**
 * \brief  Deallocates memory block.
 */
void N_API NFree(void * pBlock);

/**
 * \brief  Copies data between memory blocks.
 */
NResult N_API NCopy(void * pDstBlock, const void * pSrcBlock, NSizeType size);
/**
 * \brief  Move data from one memory block to another.
 */
NResult N_API NMove(void * pDstBlock, const void * pSrcBlock, NSizeType size);
/**
 * \brief  Sets bytes of memory block to specified value.
 */
NResult N_API NFill(void * pBlock, NByte value, NSizeType size);
/**
 * \brief  Clears memory block.
 */
#define NClear(pBlock, count) NFill(pBlock, 0, count)
/**
 * \brief  Compares bytes in two memory blocks.
 */
NResult N_API NCompare(const void * pBlock1, const void * pBlock2, NSizeType size, NInt * pResult);

#ifdef N_CPP
}
#endif

#endif // !N_MEMORY_H_INCLUDED
