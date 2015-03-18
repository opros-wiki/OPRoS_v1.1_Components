
// ColorTracking.h : ColorTracking DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CColorTrackingApp
// 
//

class CColorTrackingApp : public CWinApp
{
public:
	CColorTrackingApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	