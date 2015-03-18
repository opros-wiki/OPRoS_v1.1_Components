
// ZoomInOut.h : ZoomInOut DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CZoomInOutApp
// 
//

class CZoomInOutApp : public CWinApp
{
public:
	CZoomInOutApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	