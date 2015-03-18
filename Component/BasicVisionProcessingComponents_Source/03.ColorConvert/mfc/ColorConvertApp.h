
// ColorConvert.h : ColorConvert DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CColorConvertApp
// 
//

class CColorConvertApp : public CWinApp
{
public:
	CColorConvertApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	