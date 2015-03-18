/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NlsMatcherParams.h                                                         *|
|* NlsMatcher parameters definition                                           *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NLS_MATCHER_PARAMS_H_INCLUDED
#define NLS_MATCHER_PARAMS_H_INCLUDED

#include <NParameters.h>

#ifdef N_CPP
extern "C"
{
#endif

// Part ids
#define NLSM_PART_NONE 0
#define NLSM_PART_NLM  1

#define NLSMP_MATCHING_THRESHOLD  100 // Double | [0, 1]

#ifdef N_CPP
}
#endif

#endif // !NLS_MATCHER_PARAMS_H_INCLUDED
