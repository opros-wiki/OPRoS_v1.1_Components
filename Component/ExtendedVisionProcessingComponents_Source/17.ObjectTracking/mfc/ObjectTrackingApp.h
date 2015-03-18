
// ObjectTracking.h : ObjectTracking DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CObjectTrackingApp
// 
//

class CObjectTrackingApp : public CWinApp
{
public:
	CObjectTrackingApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	