
// Bumblebee.h : Bumblebee DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CBumblebeeApp
// 
//

class CBumblebeeApp : public CWinApp
{
public:
	CBumblebeeApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	