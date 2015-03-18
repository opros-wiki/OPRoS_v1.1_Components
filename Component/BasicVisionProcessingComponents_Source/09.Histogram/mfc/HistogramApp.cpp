
// Histogram.cpp : 
//
#include "stdafx.h"
#include "HistogramApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CHistogramApp, CWinApp)
END_MESSAGE_MAP()

// CHistogramApp creation

CHistogramApp::CHistogramApp()
{
	// 
	// 
}


// unique CHistogramApp object.

CHistogramApp theApp;


// initialize CHistogramApp 

BOOL CHistogramApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	