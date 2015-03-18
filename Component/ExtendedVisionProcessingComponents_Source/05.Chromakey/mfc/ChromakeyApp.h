
// Chromakey.h : Chromakey DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CChromakeyApp
// 
//

class CChromakeyApp : public CWinApp
{
public:
	CChromakeyApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	