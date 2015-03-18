
// Sharpening.cpp : 
//
#include "stdafx.h"
#include "SharpeningApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CSharpeningApp, CWinApp)
END_MESSAGE_MAP()

// CSharpeningApp creation

CSharpeningApp::CSharpeningApp()
{
	// 
	// 
}


// unique CSharpeningApp object.

CSharpeningApp theApp;


// initialize CSharpeningApp 

BOOL CSharpeningApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	