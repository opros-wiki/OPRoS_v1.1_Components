
// HumanDetection.h : HumanDetection DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CHumanDetectionApp
// 
//

class CHumanDetectionApp : public CWinApp
{
public:
	CHumanDetectionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	