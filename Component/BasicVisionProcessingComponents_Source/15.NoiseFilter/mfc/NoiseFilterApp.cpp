
// NoiseFilter.cpp : 
//
#include "stdafx.h"
#include "NoiseFilterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CNoiseFilterApp, CWinApp)
END_MESSAGE_MAP()

// CNoiseFilterApp creation

CNoiseFilterApp::CNoiseFilterApp()
{
	// 
	// 
}


// unique CNoiseFilterApp object.

CNoiseFilterApp theApp;


// initialize CNoiseFilterApp 

BOOL CNoiseFilterApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	