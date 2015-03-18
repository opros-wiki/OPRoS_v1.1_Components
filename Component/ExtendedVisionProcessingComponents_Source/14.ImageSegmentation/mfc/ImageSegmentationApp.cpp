
// ImageSegmentation.cpp : 
//
#include "stdafx.h"
#include "ImageSegmentationApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CImageSegmentationApp, CWinApp)
END_MESSAGE_MAP()

// CImageSegmentationApp creation

CImageSegmentationApp::CImageSegmentationApp()
{
	// 
	// 
}


// unique CImageSegmentationApp object.

CImageSegmentationApp theApp;


// initialize CImageSegmentationApp 

BOOL CImageSegmentationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	