/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NLMatcher.h                                                                *|
|* Header file for NLMatcher module                                           *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NL_MATCHER_H_INCLUDED
#define NL_MATCHER_H_INCLUDED

#include "NLMatcherParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNLMatcher);

NResult N_API NlmCopyParameters(HNLMatcher hDstMatcher, HNLMatcher hSrcMatcher);

#ifndef N_NO_ANSI_FUNC
NResult N_API NlmGetParameterA(HNLMatcher hMatcher, NUInt parameterId, void *pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NlmGetParameterW(HNLMatcher hMatcher, NUInt parameterId, void *pValue);
#endif
#define NlmGetParameter N_FUNC_AW(NlmGetParameter)

#ifndef N_NO_ANSI_FUNC
NResult N_API NlmSetParameterA(HNLMatcher hMatcher, NUInt parameterId, const void *pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NlmSetParameterW(HNLMatcher hMatcher, NUInt parameterId, const void *pValue);
#endif
#define NlmSetParameter N_FUNC_AW(NlmSetParameter)

NResult N_API NlmCreate(HNLMatcher *pHMatcher);
NResult N_API NlmReset(HNLMatcher hMatcher);
void N_API NlmFree(HNLMatcher hMatcher);

NResult N_API NlmVerify(HNLMatcher hMatcher,
	const void *pTemplate1, NSizeType template1Size, const void *pTemplate2, NSizeType template2Size,
	NDouble *pScore);

NResult N_API NlmIdentifyStart(HNLMatcher hMatcher,
	const void *pTemplate, NSizeType templateSize);
NResult N_API NlmIdentifyNext(HNLMatcher hMatcher,
	const void *pTemplate, NSizeType templateSize,
	NDouble *pScore);
NResult N_API NlmIdentifyEnd(HNLMatcher hMatcher);

#ifdef N_CPP
}
#endif

#endif // !NL_MATCHER_H_INCLUDED
