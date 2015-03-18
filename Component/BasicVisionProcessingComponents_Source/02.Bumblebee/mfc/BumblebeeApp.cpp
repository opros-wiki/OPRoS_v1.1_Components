
// Bumblebee.cpp : 
//
#include "stdafx.h"
#include "BumblebeeApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CBumblebeeApp, CWinApp)
END_MESSAGE_MAP()

// CBumblebeeApp creation

CBumblebeeApp::CBumblebeeApp()
{
	// 
	// 
}


// unique CBumblebeeApp object.

CBumblebeeApp theApp;


// initialize CBumblebeeApp 

BOOL CBumblebeeApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	