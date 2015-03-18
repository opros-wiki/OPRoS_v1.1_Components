/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Template 1.3                        *|
|*                                                                            *|
|* NLTemplate.h                                                               *|
|* Header file for NLTemplate module                                          *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NL_TEMPLATE_H_INCLUDED
#define NL_TEMPLATE_H_INCLUDED

#include "NLRecord.h"

#ifdef N_CPP
extern "C"
{
#endif

#define NLT_MAX_RECORD_COUNT 255

N_DECLARE_HANDLE(HNLTemplate)

NResult N_API NLTemplateCalculateSize(NInt recordCount, NSizeType * arRecordSizes, NSizeType * pSize);
NResult N_API NLTemplatePack(NInt recordCount, const void * * arRecords, NSizeType * arRecordSizes, void * buffer, NSizeType bufferSize, NSizeType * pSize);
NResult N_API NLTemplateUnpack(const void * buffer, NSizeType bufferSize,
	NByte * pMajorVersion, NByte * pMinorVersion, NUInt * pSize, NByte * pHeaderSize,
	NInt * pRecordCount, const void * * arRecords, NSizeType * arRecordSizes);
NResult N_API NLTemplateCheck(const void * buffer, NSizeType bufferSize);

NResult N_API NLTemplateGetRecordCountMem(const void * buffer, NSizeType bufferSize, NInt * pValue);

typedef struct NLTemplateInfo_
{
	NByte MajorVersion;
	NByte MinorVersion;
	NUInt Size;
	NByte HeaderSize;
	NInt RecordCount;
	NLRecordInfo * RecordInfos;
} NLTemplateInfo;

void N_API NLTemplateInfoDispose(NLTemplateInfo * pInfo);

#define NLT_PROCESS_FIRST_RECORD_ONLY 0x00000100 // Load/Save

NResult N_API NLTemplateCreate(HNLTemplate * pHTemplate);
NResult N_API NLTemplateCreateFromMemory(const void * buffer, NSizeType bufferSize,
	NUInt flags, NLTemplateInfo * pInfo, HNLTemplate * pHTemplate);

void N_API NLTemplateFree(HNLTemplate hTemplate);

NResult N_API NLTemplateGetRecordCount(HNLTemplate hTemplate, NInt * pValue);
NResult N_API NLTemplateGetRecord(HNLTemplate hTemplate, NInt index, HNLRecord * pValue);
NResult N_API NLTemplateGetRecordCapacity(HNLTemplate hTemplate, NInt * pValue);
NResult N_API NLTemplateSetRecordCapacity(HNLTemplate hTemplate, NInt value);
NResult N_API NLTemplateAddRecord(HNLTemplate hTemplate, NUInt flags, HNLRecord * pHRecord);
NResult N_API NLTemplateAddRecordFromMemory(HNLTemplate hTemplate, const void * buffer, NSizeType bufferSize,
	NUInt flags, HNLRecord * pHRecord);
NResult N_API NLTemplateAddRecordCopy(HNLTemplate hTemplate, HNLRecord hSrcRecord, HNLRecord * pHRecord);
NResult N_API NLTemplateRemoveRecord(HNLTemplate hTemplate, NInt index);
NResult N_API NLTemplateClearRecords(HNLTemplate hTemplate);

NResult N_API NLTemplateClone(HNLTemplate hTemplate, HNLTemplate * pHClonedTemplate);
NResult N_API NLTemplateGetSize(HNLTemplate hTemplate, NUInt flags, NSizeType * pSize);
NResult N_API NLTemplateSaveToMemory(HNLTemplate hTemplate, void * buffer, NSizeType bufferSize, NUInt flags, NSizeType * pSize);

#ifdef N_CPP
}
#endif

#endif // !NL_TEMPLATE_H_INCLUDED
