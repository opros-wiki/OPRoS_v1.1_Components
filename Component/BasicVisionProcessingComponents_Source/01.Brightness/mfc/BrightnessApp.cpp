
// Brightness.cpp : 
//
#include "stdafx.h"
#include "BrightnessApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CBrightnessApp, CWinApp)
END_MESSAGE_MAP()

// CBrightnessApp creation

CBrightnessApp::CBrightnessApp()
{
	// 
	// 
}


// unique CBrightnessApp object.

CBrightnessApp theApp;


// initialize CBrightnessApp 

BOOL CBrightnessApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	