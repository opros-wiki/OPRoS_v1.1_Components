/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NfmMatchDetails.h                                                          *|
|* Header file for NfmMatchDetails module                                     *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NFM_MATCH_DETAILS_H_INCLUDED
#define NFM_MATCH_DETAILS_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

typedef struct NfmMatchDetails_
{
	NInt Score;
	NInt Reserved;
	NInt CenterX;
	NInt CenterY;
	NByte Rotation;
	NInt TranslationX;
	NInt TranslationY;
	NInt MatedMinutiaCount;
	NIndexPair * MatedMinutiae;
} NfmMatchDetails;

NResult N_API NfmMatchDetailsDeserialize(const void * buffer, NSizeType bufferLength, NfmMatchDetails * * ppMatchDetails);
void N_API NfmMatchDetailsFree(NfmMatchDetails * pMatchDetails);
NResult N_API NfmMatchDetailsSerialize(NfmMatchDetails * pMatchDetails, void * * pBuffer, NSizeType * pBufferLength);

#ifdef N_CPP
}
#endif

#endif // !NFM_MATCH_DETAILS_H_INCLUDED
