/******************************************************************************\
|*                                                                            *|
|*                           VeriLook Matcher 3.2                             *|
|*                                                                            *|
|* VLMatcherParams.h                                                          *|
|* Obsolete, use NLMatcherParams.h instead                                    *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef VL_MATCHER_PARAMS_H_INCLUDED
#define VL_MATCHER_PARAMS_H_INCLUDED

#include <NParameters.h>
#include "NLMatcherParams.h"

#ifdef N_CPP
extern "C"
{
#endif

#define VLMP_MATCHING_THRESHOLD NLMP_MATCHING_THRESHOLD

#ifdef N_CPP
}
#endif

#endif // !VL_MATCHER_PARAMS_H_INCLUDED
