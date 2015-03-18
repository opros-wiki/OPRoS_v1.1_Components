/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NMatcher.h                                                                 *|
|* Header file for NMatcher module                                            *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_MATCHER_H_INCLUDED
#define N_MATCHER_H_INCLUDED

#include "NfsMatcher.h"
#include "NlsMatcher.h"
#include "NMMatchDetails.h"
#include "NMatcherParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNMatcher);

#ifndef N_NO_ANSI_FUNC
NResult N_API NMGetInfoA(NLibraryInfoA * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NMGetInfoW(NLibraryInfoW * pValue);
#endif
#define NMGetInfo N_FUNC_AW(NMGetInfo)

NBool N_API NMIsRegistered(void);
NResult N_API NMCopyParameters(HNMatcher hDstMatcher, HNMatcher hSrcMatcher);

NResult N_API NMCreate(HNMatcher * pHMatcher);

void N_API NMFree(HNMatcher hMatcher);

NResult N_API NMReset(HNMatcher hMatcher);
NResult N_API NMVerify(HNMatcher hMatcher, const void * template1, NSizeType template1Size,
	const void * template2, NSizeType template2Size, NMMatchDetails * * ppMatchDetails, NInt * pScore);
NResult N_API NMIdentifyStart(HNMatcher hMatcher, const void * templ, NSizeType templSize,
	NMMatchDetails * * ppMatchDetails);
NResult N_API NMIdentifyNext(HNMatcher hMatcher, const void * templ, NSizeType templSize,
	NMMatchDetails * pMatchDetails, NInt * pScore);
NResult N_API NMIdentifyEnd(HNMatcher hMatcher);

#ifndef N_NO_ANSI_FUNC
NResult N_API NMGetParameterA(HNMatcher hMatcher, NUShort partId, NUInt parameterId, void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NMGetParameterW(HNMatcher hMatcher, NUShort partId, NUInt parameterId, void * pValue);
#endif
#define NMGetParameter N_FUNC_AW(NMGetParameter)

#ifndef N_NO_ANSI_FUNC
NResult N_API NMSetParameterA(HNMatcher hMatcher, NUShort partId, NUInt parameterId, const void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NMSetParameterW(HNMatcher hMatcher, NUShort partId, NUInt parameterId, const void * pValue);
#endif
#define NMSetParameter N_FUNC_AW(NMSetParameter)

#ifdef N_CPP
}
#endif

#endif // !N_MATCHER_H_INCLUDED
