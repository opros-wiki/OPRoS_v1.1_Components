
// NoiseFilter.h : NoiseFilter DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CNoiseFilterApp
// 
//

class CNoiseFilterApp : public CWinApp
{
public:
	CNoiseFilterApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	