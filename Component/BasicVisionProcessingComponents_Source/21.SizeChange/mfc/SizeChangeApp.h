
// SizeChange.h : SizeChange DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CSizeChangeApp
// 
//

class CSizeChangeApp : public CWinApp
{
public:
	CSizeChangeApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	