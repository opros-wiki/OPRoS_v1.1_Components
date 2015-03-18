/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NfsMatcher.h                                                               *|
|* Header file for NfsMatcher module                                          *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NFS_MATCHER_H_INCLUDED
#define NFS_MATCHER_H_INCLUDED

#include "NFMatcher.h"
#include "NfsmMatchDetails.h"
#include "NfsMatcherParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNfsMatcher);

NResult N_API NfsmCopyParameters(HNfsMatcher hDstMatcher, HNfsMatcher hSrcMatcher);

NResult N_API NfsmCreate(HNfsMatcher * pHMatcher);

void N_API NfsmFree(HNfsMatcher hMatcher);

NResult N_API NfsmReset(HNfsMatcher hMatcher);
NResult N_API NfsmVerify(HNfsMatcher hMatcher, const void * template1, NSizeType template1Size,
	const void * template2, NSizeType template2Size, NfsmMatchDetails * * ppMatchDetails, NInt * pScore);
NResult N_API NfsmIdentifyStart(HNfsMatcher hMatcher, const void * templ, NSizeType templSize,
	NfsmMatchDetails * * ppMatchDetails);
NResult N_API NfsmIdentifyNext(HNfsMatcher hMatcher, const void * templ, NSizeType templSize,
	NfsmMatchDetails * pMatchDetails, NInt * pScore);
NResult N_API NfsmIdentifyEnd(HNfsMatcher hMatcher);

#ifndef N_NO_ANSI_FUNC
NResult N_API NfsmGetParameterA(HNfsMatcher hMatcher, NUShort partId, NUInt parameterId, void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NfsmGetParameterW(HNfsMatcher hMatcher, NUShort partId, NUInt parameterId, void * pValue);
#endif
#define NfsmGetParameter N_FUNC_AW(NfsmGetParameter)

#ifndef N_NO_ANSI_FUNC
NResult N_API NfsmSetParameterA(HNfsMatcher hMatcher, NUShort partId, NUInt parameterId, const void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NfsmSetParameterW(HNfsMatcher hMatcher, NUShort partId, NUInt parameterId, const void * pValue);
#endif
#define NfsmSetParameter N_FUNC_AW(NfsmSetParameter)

#ifdef N_CPP
}
#endif

#endif // !NFS_MATCHER_H_INCLUDED
