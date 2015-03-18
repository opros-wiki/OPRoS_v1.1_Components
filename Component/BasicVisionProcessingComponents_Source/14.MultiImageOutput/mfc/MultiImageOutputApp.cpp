
// MultiImageOutput.cpp : 
//
#include "stdafx.h"
#include "MultiImageOutputApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CMultiImageOutputApp, CWinApp)
END_MESSAGE_MAP()

// CMultiImageOutputApp creation

CMultiImageOutputApp::CMultiImageOutputApp()
{
	// 
	// 
}


// unique CMultiImageOutputApp object.

CMultiImageOutputApp theApp;


// initialize CMultiImageOutputApp 

BOOL CMultiImageOutputApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	