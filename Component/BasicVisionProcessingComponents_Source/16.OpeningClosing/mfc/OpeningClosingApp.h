
// OpeningClosing.h : OpeningClosing DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// COpeningClosingApp
// 
//

class COpeningClosingApp : public CWinApp
{
public:
	COpeningClosingApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	