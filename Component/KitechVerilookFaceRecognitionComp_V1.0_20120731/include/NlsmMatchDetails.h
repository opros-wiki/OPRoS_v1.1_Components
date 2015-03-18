/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NlsmMatchDetails.h                                                         *|
|* Header file for NlsmMatchDetails module                                    *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NLSM_MATCH_DETAILS_H_INCLUDED
#define NLSM_MATCH_DETAILS_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

typedef struct NlsmFaceMatchDetails_
{
	NDouble Score;
	NInt MatchedIndex;
} NlsmFaceMatchDetails;

typedef struct NlsmMatchDetails_
{
	NDouble Score;
	NInt MatchedIndex;
	NInt FaceCount;
	NlsmFaceMatchDetails * Faces;
} NlsmMatchDetails;

NResult N_API NlsmMatchDetailsDeserialize(const void * buffer, NSizeType bufferLength, NlsmMatchDetails * * ppMatchDetails);
void N_API NlsmMatchDetailsFree(NlsmMatchDetails * pMatchDetails);
NResult N_API NlsmMatchDetailsSerialize(NlsmMatchDetails * pMatchDetails, void * * pBuffer, NSizeType * pBufferLength);

#ifdef N_CPP
}
#endif

#endif // !NLSM_MATCH_DETAILS_H_INCLUDED
