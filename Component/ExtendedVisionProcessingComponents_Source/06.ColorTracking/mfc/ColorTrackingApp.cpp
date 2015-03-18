
// ColorTracking.cpp : 
//
#include "stdafx.h"
#include "ColorTrackingApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CColorTrackingApp, CWinApp)
END_MESSAGE_MAP()

// CColorTrackingApp creation

CColorTrackingApp::CColorTrackingApp()
{
	// 
	// 
}


// unique CColorTrackingApp object.

CColorTrackingApp theApp;


// initialize CColorTrackingApp 

BOOL CColorTrackingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	