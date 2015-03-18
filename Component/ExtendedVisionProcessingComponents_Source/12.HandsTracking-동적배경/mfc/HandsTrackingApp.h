
// HandsTracking.h : HandsTracking DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CHandsTrackingApp
// 
//

class CHandsTrackingApp : public CWinApp
{
public:
	CHandsTrackingApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	