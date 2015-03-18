/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NfsmMatchDetails.h                                                         *|
|* Header file for NfsmMatchDetails module                                    *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NFSM_MATCH_DETAILS_H_INCLUDED
#define NFSM_MATCH_DETAILS_H_INCLUDED

#include "NfmMatchDetails.h"

#ifdef N_CPP
extern "C"
{
#endif

typedef struct NfsmFingerMatchDetails_
{
	NInt Score;
	NInt MatchedIndex;
#ifdef N_CPP
	::NfmMatchDetails * NfmMatchDetails;
#else
	NfmMatchDetails * NfmMatchDetails;
#endif
} NfsmFingerMatchDetails;

typedef struct NfsmMatchDetails_
{
	NInt Score;
	NInt FingerCount;
	NfsmFingerMatchDetails * Fingers;
} NfsmMatchDetails;

NResult N_API NfsmMatchDetailsDeserialize(const void * buffer, NSizeType bufferLength, NfsmMatchDetails * * ppMatchDetails);
void N_API NfsmMatchDetailsFree(NfsmMatchDetails * pMatchDetails);
NResult N_API NfsmMatchDetailsSerialize(NfsmMatchDetails * pMatchDetails, void * * pBuffer, NSizeType * pBufferLength);

#ifdef N_CPP
}
#endif

#endif // !NFSM_MATCH_DETAILS_H_INCLUDED
