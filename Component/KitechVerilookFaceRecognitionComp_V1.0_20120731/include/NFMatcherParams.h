/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Matcher 2.1                         *|
|*                                                                            *|
|* NFMatcherParams.h                                                          *|
|* NFMatcher parameters definition                                            *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NF_MATCHER_PARAMS_H_INCLUDED
#define NF_MATCHER_PARAMS_H_INCLUDED

#include <NParameters.h>

#ifdef N_CPP
extern "C"
{
#endif

// Main parameters

#define NFMP_MATCHING_THRESHOLD 200 // Int | >= 0
#define NFMP_MAXIMAL_ROTATION   201 // Byte | 0..128
#define NFMP_MATCHING_SPEED     220 // Int | Can be one of NfmSpeed

#define NFMP_MODE 1000 // Int | Can be one of the following
	#define NFM_MODE_GENERAL                           0
	#define NFM_MODE_DIGITALPERSONA_UAREU            100
	#define NFM_MODE_BIOMETRIKA_FX2000               200
	#define NFM_MODE_BIOMETRIKA_FX3000               201
	#define NFM_MODE_KEYTRONIC_SECUREDESKTOP         300
	#define NFM_MODE_IDENTIX_TOUCHVIEW               400
	#define NFM_MODE_IDENTIX_DFR2090                 401
	#define NFM_MODE_PRECISEBIOMETRICS_100CS         500
	#define NFM_MODE_UPEK_TOUCHCHIP                  600
	#define NFM_MODE_IDENTICATORTECHNOLOGY_DF90      700
	#define NFM_MODE_AUTHENTEC_AFS2                  800
	#define NFM_MODE_AUTHENTEC_AES4000               810
	#define NFM_MODE_AUTHENTEC_AES2501B              811
	#define NFM_MODE_ATMEL_FINGERCHIP                900
	#define NFM_MODE_BMF_BLP100                     1000
	#define NFM_MODE_SECUGEN_HAMSTER                1100
	#define NFM_MODE_ETHENTICA                      1200
	#define NFM_MODE_CROSSMATCH_VERIFIER300         1300
	#define NFM_MODE_NITGEN_FINGKEY_HAMSTER         1400
	#define NFM_MODE_TESTECH_BIOI                   1500
	#define NFM_MODE_DIGENT_IZZIX                   1600
	#define NFM_MODE_STARTEK_FM200                  1700
	#define NFM_MODE_FUJITSU_MBF200                 1800
	#define NFM_MODE_FUTRONIC_FS80                  1900
	#define NFM_MODE_LIGHTUNING_LTTC500             2000
	#define NFM_MODE_TACOMA_CMOS                    2100

typedef enum NfmSpeed_
{
	nfmsLow    =   0,
	nfmsMedium = 128,
	nfmsHigh   = 256
} NfmSpeed;

#ifdef N_CPP
}
#endif

#endif // !NF_MATCHER_PARAMS_H_INCLUDED
