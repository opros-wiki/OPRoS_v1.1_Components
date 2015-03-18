
// NumberRecognition.h : NumberRecognition DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CNumberRecognitionApp
// 
//

class CNumberRecognitionApp : public CWinApp
{
public:
	CNumberRecognitionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	