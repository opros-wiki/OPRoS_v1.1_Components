
// Contrast.cpp : 
//
#include "stdafx.h"
#include "ContrastApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CContrastApp, CWinApp)
END_MESSAGE_MAP()

// CContrastApp creation

CContrastApp::CContrastApp()
{
	// 
	// 
}


// unique CContrastApp object.

CContrastApp theApp;


// initialize CContrastApp 

BOOL CContrastApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	