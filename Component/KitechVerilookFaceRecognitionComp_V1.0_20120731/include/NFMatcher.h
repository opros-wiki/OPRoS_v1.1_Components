/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NFMatcher.h                                                                *|
|* Header file for NFMatcher module                                           *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NF_MATCHER_H_INCLUDED
#define NF_MATCHER_H_INCLUDED

#include "NfmMatchDetails.h"
#include "NFMatcherParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNFMatcher);

NResult N_API NfmCopyParameters(HNFMatcher hDstMatcher, HNFMatcher hSrcMatcher);

NResult N_API NfmCreate(HNFMatcher * pHMatcher);

void N_API NfmFree(HNFMatcher hMatcher);

NResult N_API NfmReset(HNFMatcher hMatcher);
NResult N_API NfmVerify(HNFMatcher hMatcher, const void * template1, NSizeType template1Size,
	const void * template2, NSizeType template2Size, NfmMatchDetails * * ppMatchDetails, NInt * pScore);
NResult N_API NfmIdentifyStart(HNFMatcher hMatcher, const void * templ, NSizeType templSize,
	NfmMatchDetails * * ppMatchDetails);
NResult N_API NfmIdentifyNext(HNFMatcher hMatcher, const void * templ, NSizeType templSize,
	NfmMatchDetails * pMatchDetails, NInt * pScore);
NResult N_API NfmIdentifyEnd(HNFMatcher hMatcher);

#ifndef N_NO_ANSI_FUNC
NResult N_API NfmGetParameterA(HNFMatcher hMatcher, NUInt parameterId, void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NfmGetParameterW(HNFMatcher hMatcher, NUInt parameterId, void * pValue);
#endif
#define NfmGetParameter N_FUNC_AW(NfmGetParameter)

#ifndef N_NO_ANSI_FUNC
NResult N_API NfmSetParameterA(HNFMatcher hMatcher, NUInt parameterId, const void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NfmSetParameterW(HNFMatcher hMatcher, NUInt parameterId, const void * pValue);
#endif
#define NfmSetParameter N_FUNC_AW(NfmSetParameter)

#ifdef N_CPP
}
#endif

#endif // !NF_MATCHER_H_INCLUDED
