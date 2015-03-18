
// Histogram.h : Histogram DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CHistogramApp
// 
//

class CHistogramApp : public CWinApp
{
public:
	CHistogramApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	