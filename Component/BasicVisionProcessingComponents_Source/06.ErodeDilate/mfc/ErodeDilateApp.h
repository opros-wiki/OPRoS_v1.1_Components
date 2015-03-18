
// ErodeDilate.h : ErodeDilate DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CErodeDilateApp
// 
//

class CErodeDilateApp : public CWinApp
{
public:
	CErodeDilateApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	