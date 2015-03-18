/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NStream.h                                                                  *|
|* Header file for NStream module                                             *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_STREAM_H_INCLUDED
#define N_STREAM_H_INCLUDED

#include "NCore.h"

#ifdef N_CPP
extern "C"
{
#endif

/// Handle to the stream.
N_DECLARE_HANDLE(HNStream)

typedef enum NSeekOrigin_
{
	nsoBegin = 0,
	nsoCurrent = 1,
	nsoEnd = 2
} NSeekOrigin;

NResult N_API NStreamGetNull(HNStream * pValue);

NResult N_API NStreamIsOpened(HNStream hStream, NBool * pValue);
NResult N_API NStreamCanRead(HNStream hStream, NBool * pValue);
NResult N_API NStreamCanWrite(HNStream hStream, NBool * pValue);
NResult N_API NStreamCanSeek(HNStream hStream, NBool * pValue);
NResult N_API NStreamClose(HNStream hStream);
NResult N_API NStreamFlush(HNStream hStream);
NResult N_API NStreamGetLength(HNStream hStream, NPosType * pValue);
NResult N_API NStreamSetLength(HNStream hStream, NPosType value);
NResult N_API NStreamGetPosition(HNStream hStream, NPosType * pValue);
NResult N_API NStreamSetPosition(HNStream hStream, NPosType value);
NResult N_API NStreamSeek(HNStream hStream, NPosType offset, NSeekOrigin origin);
NResult N_API NStreamReadByte(HNStream hStream, NInt * pValue);
NResult N_API NStreamRead(HNStream hStream, void * buffer, NSizeType count, NSizeType * pCountRead);
NResult N_API NStreamWriteByte(HNStream hStream, NByte value);
NResult N_API NStreamWrite(HNStream hStream, const void * buffer, NSizeType count);

#ifdef N_CPP
}
#endif

#endif // !N_STREAM_H_INCLUDED
