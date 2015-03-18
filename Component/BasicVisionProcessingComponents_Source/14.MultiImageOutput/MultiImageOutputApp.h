
// MultiImageOutput.h : MultiImageOutput DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CMultiImageOutputApp
// 
//

class CMultiImageOutputApp : public CWinApp
{
public:
	CMultiImageOutputApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	