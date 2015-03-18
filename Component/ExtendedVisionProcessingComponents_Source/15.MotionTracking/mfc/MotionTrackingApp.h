
// MotionTracking.h : MotionTracking DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CMotionTrackingApp
// 
//

class CMotionTrackingApp : public CWinApp
{
public:
	CMotionTrackingApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	