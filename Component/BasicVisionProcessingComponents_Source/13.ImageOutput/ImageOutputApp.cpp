
// ImageOutput.cpp : 
//
#include "stdafx.h"
#include "ImageOutputApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CImageOutputApp, CWinApp)
END_MESSAGE_MAP()

// CImageOutputApp creation

CImageOutputApp::CImageOutputApp()
{
	// 
	// 
}


// unique CImageOutputApp object.

CImageOutputApp theApp;


// initialize CImageOutputApp 

BOOL CImageOutputApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	