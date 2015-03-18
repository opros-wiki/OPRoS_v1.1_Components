
// Brightness.h : Brightness DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CBrightnessApp
// 
//

class CBrightnessApp : public CWinApp
{
public:
	CBrightnessApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	