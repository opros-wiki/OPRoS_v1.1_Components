
// QRCodeRecognition.h : QRCodeRecognition DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CQRCodeRecognitionApp
// 
//

class CQRCodeRecognitionApp : public CWinApp
{
public:
	CQRCodeRecognitionApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	