
// FastFourierTransform.h : FastFourierTransform DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CFastFourierTransformApp
// 
//

class CFastFourierTransformApp : public CWinApp
{
public:
	CFastFourierTransformApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	