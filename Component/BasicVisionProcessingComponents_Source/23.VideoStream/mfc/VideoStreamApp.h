
// VideoStream.h : VideoStream DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CVideoStreamApp
// 
//

class CVideoStreamApp : public CWinApp
{
public:
	CVideoStreamApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	