
// HandsTracking.cpp : 
//
#include "stdafx.h"
#include "HandsTrackingApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CHandsTrackingApp, CWinApp)
END_MESSAGE_MAP()

// CHandsTrackingApp creation

CHandsTrackingApp::CHandsTrackingApp()
{
	// 
	// 
}


// unique CHandsTrackingApp object.

CHandsTrackingApp theApp;


// initialize CHandsTrackingApp 

BOOL CHandsTrackingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	