
// HandsMotionTracking.h : HandsMotionTracking DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CHandsMotionTrackingApp
// 
//

class CHandsMotionTrackingApp : public CWinApp
{
public:
	CHandsMotionTrackingApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	