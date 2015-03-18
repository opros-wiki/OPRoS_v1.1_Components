/******************************************************************************\
|*                                                                            *|
|*                Neurotechnology Face Features Extractor 1.2                 *|
|*                                                                            *|
|* NLExtractorParams.h                                                        *|
|* NLExtractor parameters definition                                          *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NL_EXTRACTOR_PARAMS_H_INCLUDED
#define NL_EXTRACTOR_PARAMS_H_INCLUDED

#include "NParameters.h"

#ifdef N_CPP
extern "C"
{
#endif

// Main parameters

#define NLEP_MIN_IOD                    10101 // Int
#define NLEP_MAX_IOD                    10102 // Int
#define NLEP_FACE_CONFIDENCE_THRESHOLD  10103 // Double
#define NLEP_FAVOR_LARGEST_FACE         10104 // Bool

#define NLEP_FACE_QUALITY_THRESHOLD  10350 // Byte [0..255]

// obsolete
#define NLEP_GENERALIZATION_THRESHOLD  10501 // Double | [0, 1]

#define NLEP_USE_LIVENESS_CHECK 10402 // Bool, Default = False
#define NLEP_LIVENESS_THRESHOLD 10403 // Double [0, 100], Default = 50

#define NLEP_MAX_RECORDS_PER_TEMPLATE      10408 // NInt [2..16], default = 5

#ifdef N_CPP
}
#endif

#endif // !NL_EXTRACTOR_PARAMS_H_INCLUDED
