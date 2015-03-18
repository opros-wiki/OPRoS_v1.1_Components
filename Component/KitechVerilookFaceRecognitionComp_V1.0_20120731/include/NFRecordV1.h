/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Template 1.3                        *|
|*                                                                            *|
|* NFRecord.h                                                                 *|
|* Header file for NFRecord module, V1 functionality                          *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NF_RECORD_V1_H_INCLUDED
#define NF_RECORD_V1_H_INCLUDED

#include "NFRecord.h"

#ifdef N_CPP
extern "C"
{
#endif

NResult N_API NFRecordGetMaxSizeV1(NFMinutiaFormat minutiaFormat, NInt minutiaCount,
	NInt coreCount, NInt deltaCount, NInt doubleCoreCount, NInt boWidth, NInt boHeight, NSizeType * pSize);

NResult N_API NFRecordGetSizeV1(HNFRecord hRecord, NUInt flags, NSizeType * pSize);
NResult N_API NFRecordSaveToMemoryV1(HNFRecord hRecord, void * buffer, NSizeType bufferSize, NUInt flags, NSizeType * pSize);

#ifdef N_CPP
}
#endif

#endif // !NF_RECORD_V1_H_INCLUDED
