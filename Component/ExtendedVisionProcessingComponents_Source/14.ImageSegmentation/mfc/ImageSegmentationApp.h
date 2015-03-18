
// ImageSegmentation.h : ImageSegmentation DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// CImageSegmentationApp
// 
//

class CImageSegmentationApp : public CWinApp
{
public:
	CImageSegmentationApp();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	