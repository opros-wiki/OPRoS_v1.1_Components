
// ObjectTracking.cpp : 
//
#include "stdafx.h"
#include "ObjectTrackingApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CObjectTrackingApp, CWinApp)
END_MESSAGE_MAP()

// CObjectTrackingApp creation

CObjectTrackingApp::CObjectTrackingApp()
{
	// 
	// 
}


// unique CObjectTrackingApp object.

CObjectTrackingApp theApp;


// initialize CObjectTrackingApp 

BOOL CObjectTrackingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	