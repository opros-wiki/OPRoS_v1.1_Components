
// ARToolkitOPRoS.h : ARToolkitOPRoS DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CARToolkitOPRoSApp
// 
//

class CARToolkitOPRoSApp : public CWinApp
{
public:
	CARToolkitOPRoSApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	