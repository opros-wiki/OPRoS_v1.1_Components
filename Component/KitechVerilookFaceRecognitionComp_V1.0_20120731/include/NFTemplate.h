/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Template 1.3                        *|
|*                                                                            *|
|* NFTemplate.h                                                               *|
|* Header file for NFTemplate module                                          *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NF_TEMPLATE_H_INCLUDED
#define NF_TEMPLATE_H_INCLUDED

#include "NFRecord.h"

#ifdef N_CPP
extern "C"
{
#endif

#define NFT_MAX_RECORD_COUNT 255

N_DECLARE_HANDLE(HNFTemplate)

NResult N_API NFTemplateCalculateSize(NInt recordCount, NSizeType * arRecordSizes, NSizeType * pSize);
NResult N_API NFTemplatePack(NInt recordCount, const void * * arRecords, NSizeType * arRecordSizes, void * buffer, NSizeType bufferSize, NSizeType * pSize);
NResult N_API NFTemplateUnpack(const void * buffer, NSizeType bufferSize,
	NByte * pMajorVersion, NByte * pMinorVersion, NUInt * pSize, NByte * pHeaderSize,
	NInt * pRecordCount, const void * * arRecords, NSizeType * arRecordSizes);
NResult N_API NFTemplateCheck(const void * buffer, NSizeType bufferSize);

NResult N_API NFTemplateGetRecordCountMem(const void * buffer, NSizeType bufferSize, NInt * pValue);

typedef struct NFTemplateInfo_
{
	NByte MajorVersion;
	NByte MinorVersion;
	NUInt Size;
	NByte HeaderSize;
	NInt RecordCount;
	NFRecordInfo * RecordInfos;
} NFTemplateInfo;

void N_API NFTemplateInfoDispose(NFTemplateInfo * pInfo);

#define NFT_PROCESS_FIRST_RECORD_ONLY 0x00000100 // Load/Save

NResult N_API NFTemplateCreate(HNFTemplate * pHTemplate);
NResult N_API NFTemplateCreateFromMemory(const void * buffer, NSizeType bufferSize,
	NUInt flags, NFTemplateInfo * pInfo, HNFTemplate * pHTemplate);

void N_API NFTemplateFree(HNFTemplate hTemplate);

NResult N_API NFTemplateGetRecordCount(HNFTemplate hTemplate, NInt * pValue);
NResult N_API NFTemplateGetRecord(HNFTemplate hTemplate, NInt index, HNFRecord * pValue);
NResult N_API NFTemplateGetRecordCapacity(HNFTemplate hTemplate, NInt * pValue);
NResult N_API NFTemplateSetRecordCapacity(HNFTemplate hTemplate, NInt value);
NResult N_API NFTemplateAddRecord(HNFTemplate hTemplate, NUShort width, NUShort height,
	NUShort horzResolution, NUShort vertResolution, NUInt flags, HNFRecord * pHRecord);
NResult N_API NFTemplateAddRecordFromMemory(HNFTemplate hTemplate, const void * buffer, NSizeType bufferSize,
	NUInt flags, HNFRecord * pHRecord);
NResult N_API NFTemplateAddRecordCopy(HNFTemplate hTemplate, HNFRecord hSrcRecord, HNFRecord * pHRecord);
NResult N_API NFTemplateRemoveRecord(HNFTemplate hTemplate, NInt index);
NResult N_API NFTemplateClearRecords(HNFTemplate hTemplate);

NResult N_API NFTemplateClone(HNFTemplate hTemplate, HNFTemplate * pHClonedTemplate);
NResult N_API NFTemplateGetSize(HNFTemplate hTemplate, NUInt flags, NSizeType * pSize);
NResult N_API NFTemplateSaveToMemory(HNFTemplate hTemplate, void * buffer, NSizeType bufferSize, NUInt flags, NSizeType * pSize);

#ifdef N_CPP
}
#endif

#endif // !NF_TEMPLATE_H_INCLUDED
