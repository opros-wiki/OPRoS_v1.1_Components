
// Chromakey.cpp : 
//
#include "stdafx.h"
#include "ChromakeyApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CChromakeyApp, CWinApp)
END_MESSAGE_MAP()

// CChromakeyApp creation

CChromakeyApp::CChromakeyApp()
{
	// 
	// 
}


// unique CChromakeyApp object.

CChromakeyApp theApp;


// initialize CChromakeyApp 

BOOL CChromakeyApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	