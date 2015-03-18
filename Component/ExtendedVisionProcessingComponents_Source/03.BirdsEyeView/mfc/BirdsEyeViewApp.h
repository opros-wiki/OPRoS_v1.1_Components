
// BirdsEyeView.h : BirdsEyeView DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CBirdsEyeViewApp
// 
//

class CBirdsEyeViewApp : public CWinApp
{
public:
	CBirdsEyeViewApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	