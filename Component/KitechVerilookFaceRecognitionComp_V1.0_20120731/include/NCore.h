/** \file NCore.h \brief Provides infrastructure/basic functionality for Neurotechnology components.*/

/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NCore.h                                                                    *|
|* Header file for basic NCore library functionality                          *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_CORE_H_INCLUDED
#define N_CORE_H_INCLUDED

#include "NTypes.h"
#include "NErrors.h"
#include "NMemory.h"
#include "NStream.h"
#include "NLibraryInfo.h"

#ifdef N_CPP
extern "C"
{
#endif

#ifndef N_NO_ANSI_FUNC
NResult N_API NCoreGetInfoA(NLibraryInfoA * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NCoreGetInfoW(NLibraryInfoW * pValue);
#endif
#define NCoreGetInfo N_FUNC_AW(NCoreGetInfo)

#ifdef N_CPP
}
#endif

#endif // !N_CORE_H_INCLUDED
