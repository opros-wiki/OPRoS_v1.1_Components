
// BumblebeeExpansion.h : BumblebeeExpansion DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CBumblebeeExpansionApp
// 
//

class CBumblebeeExpansionApp : public CWinApp
{
public:
	CBumblebeeExpansionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	