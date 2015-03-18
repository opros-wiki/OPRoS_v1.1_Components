/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NlsMatcher.h                                                               *|
|* Header file for NlsMatcher module                                          *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NLS_MATCHER_H_INCLUDED
#define NLS_MATCHER_H_INCLUDED

#include "NLMatcher.h"
#include "NlsmMatchDetails.h"
#include "NlsMatcherParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNlsMatcher);

NResult N_API NlsmCopyParameters(HNlsMatcher hDstMatcher, HNlsMatcher hSrcMatcher);

NResult N_API NlsmCreate(HNlsMatcher * pHMatcher);

void N_API NlsmFree(HNlsMatcher hMatcher);

NResult N_API NlsmReset(HNlsMatcher hMatcher);
NResult N_API NlsmVerify(HNlsMatcher hMatcher, const void * template1, NSizeType template1Size,
	const void * template2, NSizeType template2Size, NlsmMatchDetails * * ppMatchDetails, NDouble * pScore);
NResult N_API NlsmIdentifyStart(HNlsMatcher hMatcher, const void * templ, NSizeType templSize,
	NlsmMatchDetails * * ppMatchDetails);
NResult N_API NlsmIdentifyNext(HNlsMatcher hMatcher, const void * templ, NSizeType templSize,
	NlsmMatchDetails * pMatchDetails, NDouble * pScore);
NResult N_API NlsmIdentifyEnd(HNlsMatcher hMatcher);

#ifndef N_NO_ANSI_FUNC
NResult N_API NlsmGetParameterA(HNlsMatcher hMatcher, NUShort partId, NUInt parameterId, void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NlsmGetParameterW(HNlsMatcher hMatcher, NUShort partId, NUInt parameterId, void * pValue);
#endif
#define NlsmGetParameter N_FUNC_AW(NlsmGetParameter)

#ifndef N_NO_ANSI_FUNC
NResult N_API NlsmSetParameterA(HNlsMatcher hMatcher, NUShort partId, NUInt parameterId, const void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NlsmSetParameterW(HNlsMatcher hMatcher, NUShort partId, NUInt parameterId, const void * pValue);
#endif
#define NlsmSetParameter N_FUNC_AW(NlsmSetParameter)

#ifdef N_CPP
}
#endif

#endif // !NLS_MATCHER_H_INCLUDED
