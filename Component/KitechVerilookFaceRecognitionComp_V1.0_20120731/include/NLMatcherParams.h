/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NLMatcherParams.h                                                          *|
|* NLMatcher parameters definition                                            *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NL_MATCHER_PARAMS_H_INCLUDED
#define NL_MATCHER_PARAMS_H_INCLUDED

#include <NParameters.h>

#ifdef N_CPP
extern "C"
{
#endif

// main parameters

#define NLMP_MATCHING_THRESHOLD  10000 // Double | [0, 1]
#define NLMP_MATCHING_SPEED      10001 // Int | Can be one of NlmSpeed

typedef enum NlmSpeed_
{
	nlmsLow    =   0,
	nlmsHigh   = 256
} NlmSpeed;

#ifdef N_CPP
}
#endif

#endif // !NL_MATCHER_PARAMS_H_INCLUDED
