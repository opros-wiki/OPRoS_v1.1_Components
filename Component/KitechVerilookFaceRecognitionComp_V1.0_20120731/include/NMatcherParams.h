/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NMatcherParams.h                                                           *|
|* NMatcher parameters definition                                             *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_MATCHER_PARAMS_H_INCLUDED
#define N_MATCHER_PARAMS_H_INCLUDED

#include <NParameters.h>

#ifdef N_CPP
extern "C"
{
#endif

// Part ids
#define NM_PART_NONE     0
#define NM_PART_FINGERS 10
#define NM_PART_FACES   20

#define NMP_MATCHING_THRESHOLD       100 // Int | >= 0

#define NMP_FUSION_TYPE              200 // Int | One of NMFusionType
#define NMP_FACES_MATCHING_THRESHOLD 210 // Double | [0, 1], Matters only if NMP_FUSION_TYPE is nmftSelectByFaceThenFuse

typedef enum NMFusionType_
{
	nmftFuseAlways = 0,
	nmftSelectByFaceThenFuse = 1 // NMP_FACES_MATCHING_THRESHOLD is used
} NMFusionType;

#ifdef N_CPP
}
#endif

#endif // !N_MATCHER_PARAMS_H_INCLUDED
