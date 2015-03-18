
// VideoWrite.h : VideoWrite DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CVideoWriteApp
// 
//

class CVideoWriteApp : public CWinApp
{
public:
	CVideoWriteApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	