
// FrequencyFilter.h : FrequencyFilter DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CFrequencyFilterApp
// 
//

class CFrequencyFilterApp : public CWinApp
{
public:
	CFrequencyFilterApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	