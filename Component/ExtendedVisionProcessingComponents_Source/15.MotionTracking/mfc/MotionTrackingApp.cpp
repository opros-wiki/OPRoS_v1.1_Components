
// MotionTracking.cpp : 
//
#include "stdafx.h"
#include "MotionTrackingApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CMotionTrackingApp, CWinApp)
END_MESSAGE_MAP()

// CMotionTrackingApp creation

CMotionTrackingApp::CMotionTrackingApp()
{
	// 
	// 
}


// unique CMotionTrackingApp object.

CMotionTrackingApp theApp;


// initialize CMotionTrackingApp 

BOOL CMotionTrackingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	