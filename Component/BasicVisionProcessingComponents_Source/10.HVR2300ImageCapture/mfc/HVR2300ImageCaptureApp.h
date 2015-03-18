
// HVR2300ImageCapture.h : HVR2300ImageCapture DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CHVR2300ImageCaptureApp
// 
//

class CHVR2300ImageCaptureApp : public CWinApp
{
public:
	CHVR2300ImageCaptureApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	