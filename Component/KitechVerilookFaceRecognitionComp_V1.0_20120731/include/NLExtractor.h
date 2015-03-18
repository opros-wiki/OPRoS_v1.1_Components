/******************************************************************************\
|*                                                                            *|
|*                Neurotechnology Face Features Extractor 1.2                 *|
|*                                                                            *|
|* NLExtractor.h                                                              *|
|* Header file for NLExtractor module                                         *|
|*                                                                            *|
|* Copyright (C) 2006-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NL_EXTRACTOR_H_INCLUDED
#define NL_EXTRACTOR_H_INCLUDED

#include <NCore.h>
#include <NImages.h>
#include <NGeometry.h>
#include <NLTemplate.h>

#include "NLExtractorParams.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNLExtractor);

#ifndef N_NO_ANSI_FUNC
NResult N_API NleGetInfoA(NLibraryInfoA * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NleGetInfoW(NLibraryInfoW * pValue);
#endif
#define NleGetInfo N_FUNC_AW(NleGetInfo)

NBool N_API NleIsRegistered(void);

NResult N_API NleCreate(HNLExtractor *pHExtractor);
NResult N_API NleReset(HNLExtractor hExtractor);
void N_API NleFree(HNLExtractor hExtractor);

#ifndef N_NO_ANSI_FUNC
NResult N_API NleGetParameterA(HNLExtractor hExtractor, NUInt parameterId, void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NleGetParameterW(HNLExtractor hExtractor, NUInt parameterId, void * pValue);
#endif
#define NleGetParameter N_FUNC_AW(NleGetParameter)

#ifndef N_NO_ANSI_FUNC
NResult N_API NleSetParameterA(HNLExtractor hExtractor, NUInt parameterId, const void * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NleSetParameterW(HNLExtractor hExtractor, NUInt parameterId, const void * pValue);
#endif
#define NleSetParameter N_FUNC_AW(NleSetParameter)

NResult N_API NleCopyParameters(HNLExtractor hDstExtractor, HNLExtractor hSrcExtractor);

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete")
#endif
NResult N_API NleGetMaxTemplateSize(HNLExtractor hExtractor, NSizeType *pSize);

// structures

typedef struct NleFace_
{
	NRect Rectangle;
	NDouble Confidence;
} NleFace;

typedef struct NleEyes_
{
	NPoint First;
	NDouble FirstConfidence;
	NPoint Second;
	NDouble SecondConfidence;
} NleEyes;

typedef struct NleDetectionDetails_
{
	NBool FaceAvailable;
	NleFace Face;
	NBool EyesAvailable;
	NleEyes Eyes;
} NleDetectionDetails;

typedef enum NleExtractionStatus_
{
	nleesNone = 0,
	nleesTemplateCreated = 1,
	nleesFaceNotDetected = 2,
	nleesEyesNotDetected = 3,
	nleesFaceTooCloseToImageBorder = 4,
	nleesQualityCheckGrayscaleDensityFailed = 100,
	nleesQualityCheckExposureFailed = 101,
	nleesQualityCheckSharpnessFailed = 102,
	nleesLivenessCheckFailed = 200,
} NleExtractionStatus;

// face detection

NResult N_API NleDetectFace(HNLExtractor hExtractor,
	HNImage hImage,
	NBool * pDetected, NleFace * pFace);
NResult N_API NleDetectFaces(HNLExtractor hExtractor,
	HNImage hImage,
	NInt * pFaceCount, NleFace * * pArFaces);

// facial features detection

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete, use NleDetectFacialFeatures instead")
#else
N_DEPRECATED("function is obsolete, turn NLExtractor 1.1 compatibility mode off and use NleDetectFacialFeatures instead")
#endif
NResult N_API NleDetectEyes(HNLExtractor hExtractor,
	HNImage hImage, NleFace *pFace, NleEyes *pEyes);
#ifndef NL_EXTRACTOR_1_1_COMPAT
NResult N_API NleDetectFacialFeatures(HNLExtractor hExtractor,
	HNImage hImage, const NleFace * pFace, NleDetectionDetails * pDetectionDetails);
#endif

// feature extraction

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete, use NleExtract instead")
#else
N_DEPRECATED("function is obsolete, turn NLExtractor 1.1 compatibility mode off and use NleExtract instead")
#endif
NResult N_API NleExtractOld(HNLExtractor hExtractor,
	HNImage hImage,
	NleDetectionDetails *pDetectionDetails,
	void *pBuffer, NSizeType bufferSize, NSizeType *pSize);
#ifndef NL_EXTRACTOR_1_1_COMPAT
NResult N_API NleExtract(HNLExtractor hExtractor,
	HNImage hImage,
	NleDetectionDetails * pDetectionDetails,
	NleExtractionStatus * pStatus, HNLTemplate * pHTemplate);
#else
#define NleExtract NleExtractOld
#endif

NResult N_API NleExtractStart(HNLExtractor hExtractor,
	NInt durationInFrames);

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete, use NleExtractNext instead")
#else
N_DEPRECATED("function is obsolete, turn NLExtractor 1.1 compatibility mode off and use NleExtractNext instead")
#endif
NResult N_API NleExtractNextOld(HNLExtractor hExtractor,
	HNImage hImage,
	NleDetectionDetails *pDetectionDetails,
	void *pBuffer, NSizeType bufferSize, NSizeType *pSize, NBool *pStopped);
#ifndef NL_EXTRACTOR_1_1_COMPAT
NResult N_API NleExtractNext(HNLExtractor hExtractor,
	HNImage hImage, NleDetectionDetails * pDetectionDetails,
	HNLTemplate * pHTemplate, NleExtractionStatus * pStatus);
#else
#define NleExtractNext NleExtractNextOld
#endif

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete, use NleExtractUsingDetails instead")
#else
N_DEPRECATED("function is obsolete, turn NLExtractor 1.1 compatibility mode off and use NleExtractUsingDetails instead")
#endif
NResult N_API NleExtractUsingEyes(HNLExtractor hExtractor,
	HNImage hImage, NleEyes *pEyes,
	void *pBuffer, NSizeType bufferSize, NSizeType *pSize);
#ifndef NL_EXTRACTOR_1_1_COMPAT
NResult N_API NleExtractUsingDetails(HNLExtractor hExtractor,
	HNImage hImage, NleDetectionDetails * pDetectionDetails,
	NleExtractionStatus * pStatus, HNLTemplate * pHTemplate);
#endif

// features compression

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete, use NleCompress instead")
#else
N_DEPRECATED("function is obsolete, turn NLExtractor 1.1 compatibility mode off and use NleCompress instead")
#endif
NResult N_API NleCompressOld(HNLExtractor hExtractor,
	void *pBufferSrc, NSizeType bufferSrcSize,
	void *pBufferDst, NSizeType bufferDstSize, NSizeType *pSize);
#ifndef NL_EXTRACTOR_1_1_COMPAT
NResult N_API NleCompress(HNLExtractor hExtractor,
	HNLTemplate hTemplate,
	HNLTemplate * pHCompressedTemplate);
#else
#define NleCompress NleCompressOld
#endif

// generalization

#ifndef NL_EXTRACTOR_1_1_COMPAT
N_DEPRECATED("function is obsolete, use NleGeneralize instead")
#else
N_DEPRECATED("function is obsolete, turn NLExtractor 1.1 compatibility mode off and use NleGeneralize instead")
#endif
NResult N_API NleGeneralizeOld(HNLExtractor hExtractor,
	NInt templateCount, const void **arTemplates, const NSizeType *arTemplateSizes,
	void *pBuffer, NSizeType bufferSize, NSizeType *pSize);
#ifndef NL_EXTRACTOR_1_1_COMPAT
NResult N_API NleGeneralize(HNLExtractor hExtractor,
	NInt templateCount, const HNLTemplate * arHTemplates,
	HNLTemplate * pHTemplate);
#else
#define NleGeneralize NleGeneralizeOld
#endif

#ifdef N_CPP
}
#endif

#endif // !NL_EXTRACTOR_H_INCLUDED
