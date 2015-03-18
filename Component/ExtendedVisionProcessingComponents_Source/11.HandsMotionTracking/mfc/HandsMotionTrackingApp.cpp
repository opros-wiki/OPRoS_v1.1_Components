
// HandsMotionTracking.cpp : 
//
#include "stdafx.h"
#include "HandsMotionTrackingApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CHandsMotionTrackingApp, CWinApp)
END_MESSAGE_MAP()

// CHandsMotionTrackingApp creation

CHandsMotionTrackingApp::CHandsMotionTrackingApp()
{
	// 
	// 
}


// unique CHandsMotionTrackingApp object.

CHandsMotionTrackingApp theApp;


// initialize CHandsMotionTrackingApp 

BOOL CHandsMotionTrackingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	