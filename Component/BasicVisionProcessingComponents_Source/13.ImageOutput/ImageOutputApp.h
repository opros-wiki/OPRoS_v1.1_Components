
// ImageOutput.h : ImageOutput DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CImageOutputApp
// 
//

class CImageOutputApp : public CWinApp
{
public:
	CImageOutputApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	