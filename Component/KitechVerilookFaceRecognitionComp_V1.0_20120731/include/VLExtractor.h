/******************************************************************************\
|*                                                                            *|
|*                          VeriLook Extractor 3.2                            *|
|*                                                                            *|
|* VLExtractor.h                                                              *|
|* Obsolete, use NLExtractor.h instead                                        *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef VL_EXTRACTOR_H_INCLUDED
#define VL_EXTRACTOR_H_INCLUDED

#include <NCore.h>
#include <NImages.h>
#include <NGeometry.h>

#include "VLExtractorParams.h"
#include "NLExtractor.h"

#ifdef N_CPP
extern "C"
{
#endif

#define HVLExtractor HNLExtractor

#define VleFace_             NleFace_
#define VleFace              NleFace
#define VleEyes_             NleEyes_
#define VleEyes              NleEyes
#define VleDetectionDetails_ NleDetectionDetails_
#define VleDetectionDetails  NleDetectionDetails

#define VleGetInfo            NleGetInfo
#define VleGetInfoA           NleGetInfoA
#define VleGetInfoW           NleGetInfoW
#define VleIsRegistered       NleIsRegistered
#define VleCreate             NleCreate
#define VleReset              NleReset
#define VleFree               NleFree
#define VleGetParameter       NleGetParameter
#define VleGetParameterA      NleGetParameterA
#define VleGetParameterW      NleGetParameterW
#define VleSetParameter       NleSetParameter
#define VleSetParameterA      NleSetParameterA
#define VleSetParameterW      NleSetParameterW
#define VleCopyParameters     NleCopyParameters
#define VleGetMaxTemplateSize NleGetMaxTemplateSize
#define VleDetectFace         NleDetectFace
#define VleDetectFaces        NleDetectFaces
#define VleDetectEyes         NleDetectEyes
#define VleExtract            NleExtractOld
#define VleExtractStart       NleExtractStart
#define VleExtractNext        NleExtractNextOld
#define VleExtractUsingEyes   NleExtractUsingEyes
#define VleCompress           NleCompressOld
#define VleGeneralize         NleGeneralizeOld

#ifdef N_CPP
}
#endif

#endif // !VL_EXTRACTOR_H_INCLUDED
