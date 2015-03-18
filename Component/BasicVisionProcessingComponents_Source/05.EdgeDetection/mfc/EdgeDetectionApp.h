
// EdgeDetection.h : EdgeDetection DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CEdgeDetectionApp
// 
//

class CEdgeDetectionApp : public CWinApp
{
public:
	CEdgeDetectionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	