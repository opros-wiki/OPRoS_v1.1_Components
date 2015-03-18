
// AreaDetection.h : AreaDetection DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CAreaDetectionApp
// 
//

class CAreaDetectionApp : public CWinApp
{
public:
	CAreaDetectionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	