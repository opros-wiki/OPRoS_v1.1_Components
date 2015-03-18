
// OpeningClosing.cpp : 
//
#include "stdafx.h"
#include "OpeningClosingApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(COpeningClosingApp, CWinApp)
END_MESSAGE_MAP()

// COpeningClosingApp creation

COpeningClosingApp::COpeningClosingApp()
{
	// 
	// 
}


// unique COpeningClosingApp object.

COpeningClosingApp theApp;


// initialize COpeningClosingApp 

BOOL COpeningClosingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	