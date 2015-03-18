/** \file NProcessorInfo.h 
\brief Provides functionality for getting processor information. 
*/

/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NProcessorInfo.h                                                           *|
|* Header file for NProcessorInfo module                                      *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_PROCESSOR_INFO_H_INCLUDED
#define N_PROCESSOR_INFO_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

#if defined(N_WINDOWS) && !defined(WINCE)

#define N_PROCESSOR_INFO_MAX_VENDOR_NAME_LENGTH 13
#define N_PROCESSOR_INFO_MAX_MODEL_NAME_LENGTH  48

typedef enum NProcessorVendor_
{
	npvUnknown = 0,
	npvIntel = 1,
	npvUmc = 2,
	npvAmd = 3,
	npvCyrix = 4,
	npvNexGen = 5,
	npvCentaur = 6,
	npvRiseTechnology = 7,
	npvSiS = 8,
	npvTransmeta = 9,
	npvNationalSemiconductor = 10
} NProcessorVendor;

#ifndef N_NO_ANSI_FUNC
NResult N_API NProcessorInfoGetVendorNameA(NAChar * szValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NProcessorInfoGetVendorNameW(NWChar * szValue);
#endif
#define NProcessorInfoGetVendorName N_FUNC_AW(NProcessorInfoGetVendorName)

NResult N_API NProcessorInfoGetVendor(NProcessorVendor * pValue);
NResult N_API NProcessorInfoGetModelInfo(NInt * pFamily, NInt * pModel, NInt * pStepping);

#ifndef N_NO_ANSI_FUNC
NResult N_API NProcessorInfoGetModelNameA(NAChar * szValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NProcessorInfoGetModelNameW(NWChar * szValue);
#endif
#define NProcessorInfoGetModelName N_FUNC_AW(NProcessorInfoGetModelName)

NBool N_API NProcessorInfoIsMmxSupported();
NBool N_API NProcessorInfoIsSseSupported();
NBool N_API NProcessorInfoIsSse2Supported();
NBool N_API NProcessorInfoIsSse3Supported();
NBool N_API NProcessorInfoIs3DNowSupported();

#endif // N_WINDOWS && !WINCE

#ifdef N_CPP
}
#endif

#endif // !N_PROCESSOR_INFO_H_INCLUDED
