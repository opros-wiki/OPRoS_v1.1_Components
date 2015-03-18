
// FaceDetectionASM.h : FaceDetectionASM DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CFaceDetectionASMApp
// 
//

class CFaceDetectionASMApp : public CWinApp
{
public:
	CFaceDetectionASMApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	