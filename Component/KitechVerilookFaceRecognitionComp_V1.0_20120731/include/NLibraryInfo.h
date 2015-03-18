/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NLibraryInfo.h                                                             *|
|* Header file for NLibraryInfo module                                        *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_LIBRARY_INFO_H_INCLUDED
#define N_LIBRARY_INFO_H_INCLUDED

#include "NTypes.h"
#include "NErrors.h"

#ifdef N_CPP
extern "C"
{
#endif

#define N_LI_TITLE_MAX_LENGTH     64
#define N_LI_PRODUCT_MAX_LENGTH   64
#define N_LI_COMPANY_MAX_LENGTH   64
#define N_LI_COPYRIGHT_MAX_LENGTH 64

#ifndef N_NO_ANSI_FUNC
typedef struct NLibraryInfoA_
{
	NAChar Title[N_LI_TITLE_MAX_LENGTH];
	NAChar Product[N_LI_PRODUCT_MAX_LENGTH];
	NAChar Company[N_LI_COMPANY_MAX_LENGTH];
	NAChar Copyright[N_LI_COPYRIGHT_MAX_LENGTH];
	NInt VersionMajor;
	NInt VersionMinor;
	NInt VersionBuild;
	NInt VersionRevision;
	NInt DistributorId;
	NInt SerialNumber;
} NLibraryInfoA;
#endif
#ifndef N_NO_UNICODE
typedef struct NLibraryInfoW_
{
	NWChar Title[N_LI_TITLE_MAX_LENGTH];
	NWChar Product[N_LI_PRODUCT_MAX_LENGTH];
	NWChar Company[N_LI_COMPANY_MAX_LENGTH];
	NWChar Copyright[N_LI_COPYRIGHT_MAX_LENGTH];
	NInt VersionMajor;
	NInt VersionMinor;
	NInt VersionBuild;
	NInt VersionRevision;
	NInt DistributorId;
	NInt SerialNumber;
} NLibraryInfoW;
#endif
#define NLibraryInfo N_STRUCT_AW(NLibraryInfo)

#ifdef N_CPP
}
#endif

#endif // !N_LIBRARY_INFO_H_INCLUDED
