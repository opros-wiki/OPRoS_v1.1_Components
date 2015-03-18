/******************************************************************************\
|*                                                                            *|
|*                          VeriLook Extractor 3.2                            *|
|*                                                                            *|
|* VLExtractorParams.h                                                        *|
|* Obsolete, use NLExtractorParams.h instead                                  *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef VL_EXTRACTOR_PARAMS_H_INCLUDED
#define VL_EXTRACTOR_PARAMS_H_INCLUDED

#include "NParameters.h"
//#include "NLExtractionParams.h"
#include "NLExtractorParams.h"

#ifdef N_CPP
extern "C"
{
#endif

#define VLEP_MIN_IOD                   NLEP_MIN_IOD
#define VLEP_MAX_IOD                   NLEP_MAX_IOD
#define VLEP_FACE_CONFIDENCE_THRESHOLD NLEP_FACE_CONFIDENCE_THRESHOLD
#define VLEP_FAVOR_LARGEST_FACE        NLEP_FAVOR_LARGEST_FACE
#define VLEP_FACE_QUALITY_THRESHOLD    NLEP_FACE_QUALITY_THRESHOLD
#define VLEP_GENERALIZATION_THRESHOLD  NLEP_GENERALIZATION_THRESHOLD
#define VLEP_MODE                      NLEP_MODE
	#define VLE_MODE_GENERAL                         NLE_MODE_GENERAL
	#define VLE_MODE_VERILOOK_30_TEMPLATE_COMPATIBLE NLE_MODE_VERILOOK_30_TEMPLATE_COMPATIBLE

#ifdef N_CPP
}
#endif

#endif // !VL_EXTRACTOR_PARAMS_H_INCLUDED
