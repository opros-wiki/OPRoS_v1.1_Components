/******************************************************************************\
|*                                                                            *|
|*                         Neurotechnology Licensing                          *|
|*                                                                            *|
|* NLicensing.h                                                               *|
|* Header file for NLicensing module                                          *|
|*                                                                            *|
|* Copyright (C) 2007-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_LICENSING_H_INCLUDED
#define N_LICENSING_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

#ifndef N_NO_ANSI_FUNC
NResult N_API NLicensingGetInfoA(NLibraryInfoA * pValue);
#endif
#ifndef N_NO_UNICODE
NResult N_API NLicensingGetInfoW(NLibraryInfoW * pValue);
#endif
#define NLicensingGetInfo N_FUNC_AW(NLicensingGetInfo)

#ifndef N_NO_ANSI_FUNC
NResult N_API NLicenseObtainA(const NAChar * szAddress, const NAChar * szPort, const NAChar * szProducts, NBool * pAvailable);
NResult N_API NLicenseReleaseA(const NAChar * szProducts);
#endif
#ifndef N_NO_UNICODE
NResult N_API NLicenseObtainW(const NWChar * szAddress, const NWChar * szPort, const NWChar * szProducts, NBool * pAvailable);
NResult N_API NLicenseReleaseW(const NWChar * szProducts);
#endif
#define NLicenseObtain N_FUNC_AW(NLicenseObtain)
#define NLicenseRelease N_FUNC_AW(NLicenseRelease)

#ifdef N_CPP
}
#endif

#endif // !N_LICENSING_H_INCLUDED
