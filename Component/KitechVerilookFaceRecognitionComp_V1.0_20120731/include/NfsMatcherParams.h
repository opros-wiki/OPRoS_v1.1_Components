/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NfsMatcherParams.h                                                         *|
|* NfsMatcher parameters definition                                           *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NFS_MATCHER_PARAMS_H_INCLUDED
#define NFS_MATCHER_PARAMS_H_INCLUDED

#include <NParameters.h>

#ifdef N_CPP
extern "C"
{
#endif

// Part ids
#define NFSM_PART_NONE 0
#define NFSM_PART_NFM  1

#define NFSMP_MATCHING_THRESHOLD 100 // Int | >= 0

#define NFSMP_MIN_MATCHED_FINGER_COUNT           201 // Int | >= 1 & <= NFT_MAX_RECORD_COUNT
#define NFSMP_MIN_MATCHED_FINGER_COUNT_THRESHOLD 202 // Int | >= 0

#ifdef N_CPP
}
#endif

#endif // !NFS_MATCHER_PARAMS_H_INCLUDED
