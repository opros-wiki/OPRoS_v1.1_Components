/******************************************************************************\
|*                                                                            *|
|*                           VeriLook Matcher 3.2                             *|
|*                                                                            *|
|* VLMatcher.h                                                                *|
|* Obsolete, use NLMatcher.h instead                                          *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef VL_MATCHER_H_INCLUDED
#define VL_MATCHER_H_INCLUDED

#include "VLMatcherParams.h"
#include "NLMatcher.h"

#ifdef N_CPP
extern "C"
{
#endif

#define HVLMatcher HNLMatcher

#define VlmGetInfo        NlmGetInfo
#define VlmGetInfoA       NlmGetInfoA
#define VlmGetInfoW       NlmGetInfoW
#define VlmIsRegistered   NlmIsRegistered
#define VlmCopyParameters NlmCopyParameters
#define VlmGetParameter   NlmGetParameter
#define VlmGetParameterA  NlmGetParameterA
#define VlmGetParameterW  NlmGetParameterW
#define VlmSetParameter   NlmSetParameter
#define VlmSetParameterA  NlmSetParameterA
#define VlmSetParameterW  NlmSetParameterW
#define VlmCreate         NlmCreate
#define VlmReset          NlmReset
#define VlmFree           NlmFree
#define VlmVerify         NlmVerify
#define VlmIdentifyStart  NlmIdentifyStart
#define VlmIdentifyNext   NlmIdentifyNext
#define VlmIdentifyEnd    NlmIdentifyEnd

#ifdef N_CPP
}
#endif

#endif // !VL_MATCHER_H_INCLUDED
