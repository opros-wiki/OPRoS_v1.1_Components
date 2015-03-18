/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NMMatchDetails.h                                                           *|
|* Header file for NMMatchDetails module                                      *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NM_MATCH_DETAILS_H_INCLUDED
#define NM_MATCH_DETAILS_H_INCLUDED

#include "NfsmMatchDetails.h"
#include "NlsmMatchDetails.h"

#ifdef N_CPP
extern "C"
{
#endif

typedef struct NMMatchDetails_
{
	NInt Score;
	NfsmMatchDetails * FingersMatchDetails;
	NlsmMatchDetails * FacesMatchDetails;
} NMMatchDetails;

NResult N_API NMMatchDetailsDeserialize(const void * buffer, NSizeType bufferLength, NMMatchDetails * * ppMatchDetails);
void N_API NMMatchDetailsFree(NMMatchDetails * pMatchDetails);
NResult N_API NMMatchDetailsSerialize(NMMatchDetails * pMatchDetails, void * * pBuffer, NSizeType * pBufferLength);

#ifdef N_CPP
}
#endif

#endif // !NM_MATCH_DETAILS_H_INCLUDED
