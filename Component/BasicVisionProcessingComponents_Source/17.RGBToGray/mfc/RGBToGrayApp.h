
// RGBToGray.h : RGBToGray DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CRGBToGrayApp
// 
//

class CRGBToGrayApp : public CWinApp
{
public:
	CRGBToGrayApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	