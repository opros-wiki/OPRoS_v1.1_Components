
// VideoStream.cpp : 
//
#include "stdafx.h"
#include "VideoStreamApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CVideoStreamApp, CWinApp)
END_MESSAGE_MAP()

// CVideoStreamApp creation

CVideoStreamApp::CVideoStreamApp()
{
	// 
	// 
}


// unique CVideoStreamApp object.

CVideoStreamApp theApp;


// initialize CVideoStreamApp 

BOOL CVideoStreamApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	