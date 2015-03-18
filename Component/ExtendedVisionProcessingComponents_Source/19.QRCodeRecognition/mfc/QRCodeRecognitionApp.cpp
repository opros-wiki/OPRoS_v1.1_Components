
// QRCodeRecognition.cpp : 
//
#include "stdafx.h"
#include "QRCodeRecognitionApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CQRCodeRecognitionApp, CWinApp)
END_MESSAGE_MAP()

// CQRCodeRecognitionApp creation

CQRCodeRecognitionApp::CQRCodeRecognitionApp()
{
	// 
	// 
}


// unique CQRCodeRecognitionApp object.

CQRCodeRecognitionApp theApp;


// initialize CQRCodeRecognitionApp 

BOOL CQRCodeRecognitionApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	