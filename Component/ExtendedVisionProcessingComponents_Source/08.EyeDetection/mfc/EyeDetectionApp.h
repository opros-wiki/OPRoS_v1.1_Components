
// EyeDetection.h : EyeDetection DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CEyeDetectionApp
// 
//

class CEyeDetectionApp : public CWinApp
{
public:
	CEyeDetectionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	