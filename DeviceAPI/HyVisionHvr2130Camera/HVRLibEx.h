#ifndef __HVRLIBEX_H__
#define __HVRLIBEX_H__

////////////////////////////////////////////////////////////////////////////
//
// Camera Control Parts...
//
////////////////////////////////////////////////////////////////////////////
#define MAX_DEVICE	16

//Product Number
#define HVR2030R	0
#define HVR2300R	1
#define HVR2130P	2
#define HVR2200P	3
#define HVR2300P	4

//Bayer type - Output mode
#define	BGGR				1	
#define RGGB				2	
#define GBRG				3	
#define GRBG				4	
#define BlackWhite			5

//YUV type - Output mode
#define YCbYCr				1
#define YCrYCb				2
#define CbYCrY				3
#define CrYCbY				4

//Resolution
#define AOI					99
#define QXGA				100
#define UXGA				101
#define SXGA				103
#define XGA					105
#define VGA					107
#define QVGA				109

//USB Status Code
#define USB_DATA_PASS		1
#define USB_DATA_FAIL		-1
#define USB_BAND_HALT		-2

extern "C" __declspec(dllexport) int	__stdcall HVR_camDrvInit(int *DrvInfo);
extern "C" __declspec(dllexport) int	__stdcall HVR_camDrvInit_Single(int DrvNum, int *DrvInfo);
extern "C" __declspec(dllexport) BOOL	__stdcall HVR_camSensorInit(int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) BOOL	__stdcall HVR_camRegWrite(int ProductNo, int DeviceNo, WORD wRegAddr, WORD wRegData);
extern "C" __declspec(dllexport) WORD	__stdcall HVR_camRegRead(int ProductNo, int DeviceNo, WORD wRegAddr);
extern "C" __declspec(dllexport) int	__stdcall HVR_camGetImageData(int iImageLen, LPBYTE pRawBuffer, int DeviceNo);

extern "C" __declspec(dllexport) BOOL	__stdcall HVR_camTrigerRead(int DeviceNo);
extern "C" __declspec(dllexport) BOOL	__stdcall HVR_camLEDonoff(WORD bOnOff, int DeviceNo);

////////////////////////////////////////////////////////////////////////////
//
// ISP Control Parts...
//
////////////////////////////////////////////////////////////////////////////

//Auto Exposure Control
#define AEC_DISABLE		100
#define AEC_ENABLE		101

//Auto Gain Control
#define AGC_DISABLE		100
#define AGC_ENABLE		101

//Auto WhiteBalance Control
#define AWB_DISABLE		100
#define AWB_ENABLE		101

//Gain selection set Control
#define GAIN_RED		100
#define GAIN_GREEN		101
#define GAIN_BLUE		102

//H-Mirror Control
#define MIR_DISABLE		100
#define MIR_ENABLE		101

//V-Flip Control
#define FLP_DISABLE		100
#define FLP_ENABLE		101

//Banding Filter Select
#define FLT_50HZ		100
#define FLT_60HZ		101

extern "C" __declspec(dllexport) int __stdcall HVR_ispResolutionCmd(int Resolution, int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispResolutionCmdforAOI(int ProductNo, int DeviceNo, int startX, int startY, int width, int height);
extern "C" __declspec(dllexport) int __stdcall HVR_ispMoveStartPosOnAOI(int ProductNo, int DeviceNo, int *currX, int *currY, int moveX, int moveY);

extern "C" __declspec(dllexport) int __stdcall HVR_ispAllCmdSetDefault(int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispVBlankAdjustment(int blank, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispVBlankAdjustmentRange(int *pMin, int *pMax, int *pValue, int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispSetExposureTime(int Value, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispGetExposureRange(int *pMin, int *pMax, int *pValue, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispAutoExposureEnable(int *pValue, int Value, int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispSetAWBGainControl(int iMode, int iGain, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispGetAWBGainControlRange(int iMode, int *pMin, int *pMax, int *pValue, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispAutoWhiteBalanceEnable(int *pValue, int Value, int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispTotalGainControl(int iAGain, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispTotalGainControlRange(int *pMin, int *pMax, int *pValue, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispAutoGainControlEnable(int *pValue, int Value, int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispMirrorEnable(int *pValue, int Value, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispVFlipEnable(int *pValue, int Value, int ProductNo, int DeviceNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispBandingFilterSelect(int *pValue, int Value, int ProductNo, int DeviceNo);

extern "C" __declspec(dllexport) int __stdcall HVR_ispYCbCrToBmp(int iOutMode, int width, int height, BYTE *pYUVBuffer, BYTE *pBMPBuffer, int ProductNo);
extern "C" __declspec(dllexport) int __stdcall HVR_ispRawToBmp(int iOutMode, int width, int height, BYTE *pRAWBuffer, BYTE *pBMPBuffer, int ProductNo);

#endif  // __HVRLIBEX_H__