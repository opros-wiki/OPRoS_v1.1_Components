/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Template 1.3                        *|
|*                                                                            *|
|* NTemplate.h                                                                *|
|* Header file for NTemplate module                                           *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_TEMPLATE_H_INCLUDED
#define N_TEMPLATE_H_INCLUDED

#include "NFTemplate.h"
#include "NLTemplate.h"

#ifdef N_CPP
extern "C"
{
#endif

N_DECLARE_HANDLE(HNTemplate)

#ifndef N_NO_ANSI_FUNC
NResult N_API NTemplateGetInfoA(NLibraryInfoA * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NTemplateGetInfoW(NLibraryInfoW * pValue);
#endif
#define NTemplateGetInfo N_FUNC_AW(NTemplateGetInfo)

NResult N_API NTemplateCalculateSize(NSizeType fingersTemplateSize, NSizeType facesTemplateSize, NSizeType * pSize);
NResult N_API NTemplatePack(
	const void * fingersTemplate, NSizeType fingersTemplateSize,
	const void * facesTemplate, NSizeType facesTemplateSize,
	void * buffer, NSizeType bufferSize, NSizeType * pSize);
NResult N_API NTemplateUnpack(const void * buffer, NSizeType bufferSize,
	NByte * pMajorVersion, NByte * pMinorVersion, NUInt * pSize, NByte * pHeaderSize,
	const void * * pFingersTemplate, NSizeType * pFingersTemplateSize,
	const void * * pFacesTemplate, NSizeType * pFacesTemplateSize);
NResult N_API NTemplateCheck(const void * buffer, NSizeType bufferSize);

typedef struct NTemplateInfo_
{
	NByte MajorVersion;
	NByte MinorVersion;
	NUInt Size;
	NByte HeaderSize;
	NFTemplateInfo * FingersInfo;
	NLTemplateInfo * FacesInfo;
} NTemplateInfo;

void N_API NTemplateInfoDispose(NTemplateInfo * pInfo);

NResult N_API NTemplateCreate(HNTemplate * pHTemplate);
NResult N_API NTemplateCreateFromMemory(const void * buffer, NSizeType bufferSize,
	NUInt flags, NTemplateInfo * pInfo, HNTemplate * pHTemplate);

void N_API NTemplateFree(HNTemplate hTemplate);

NResult N_API NTemplateGetFingers(HNTemplate hTemplate, HNFTemplate * pValue);
NResult N_API NTemplateAddFingers(HNTemplate hTemplate, HNFTemplate * pHFingers);
NResult N_API NTemplateAddFingersFromMemory(HNTemplate hTemplate, const void * buffer, NSizeType bufferSize,
	NUInt flags, HNFTemplate * pHFingers);
NResult N_API NTemplateAddFingersCopy(HNTemplate hTemplate, HNFTemplate hSrcFingers, HNFTemplate * pHFingers);
NResult N_API NTemplateRemoveFingers(HNTemplate hTemplate);

NResult N_API NTemplateGetFaces(HNTemplate hTemplate, HNLTemplate * pValue);
NResult N_API NTemplateAddFaces(HNTemplate hTemplate, HNLTemplate * pHFaces);
NResult N_API NTemplateAddFacesFromMemory(HNTemplate hTemplate, const void * buffer, NSizeType bufferSize,
	NUInt flags, HNLTemplate * pHFaces);
NResult N_API NTemplateAddFacesCopy(HNTemplate hTemplate, HNLTemplate hSrcFaces, HNLTemplate * pHFaces);
NResult N_API NTemplateRemoveFaces(HNTemplate hTemplate);

NResult N_API NTemplateClear(HNTemplate hTemplate);
NResult N_API NTemplateClone(HNTemplate hTemplate, HNTemplate * pHClonedTemplate);
NResult N_API NTemplateGetSize(HNTemplate hTemplate, NUInt flags, NSizeType * pSize);
NResult N_API NTemplateSaveToMemory(HNTemplate hTemplate, void * buffer, NSizeType bufferSize, NUInt flags, NSizeType * pSize);

#ifdef N_CPP
}
#endif

#endif // !N_TEMPLATE_H_INCLUDED
