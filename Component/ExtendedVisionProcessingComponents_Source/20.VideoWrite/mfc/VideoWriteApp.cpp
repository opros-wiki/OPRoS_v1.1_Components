
// VideoWrite.cpp : 
//
#include "stdafx.h"
#include "VideoWriteApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CVideoWriteApp, CWinApp)
END_MESSAGE_MAP()

// CVideoWriteApp creation

CVideoWriteApp::CVideoWriteApp()
{
	// 
	// 
}


// unique CVideoWriteApp object.

CVideoWriteApp theApp;


// initialize CVideoWriteApp 

BOOL CVideoWriteApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	