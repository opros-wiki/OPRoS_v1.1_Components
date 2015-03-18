
// Sharpening.h : Sharpening DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CSharpeningApp
// 
//

class CSharpeningApp : public CWinApp
{
public:
	CSharpeningApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	