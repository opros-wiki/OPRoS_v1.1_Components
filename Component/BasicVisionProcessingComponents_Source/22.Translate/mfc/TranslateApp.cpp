
// Translate.cpp : 
//
#include "stdafx.h"
#include "TranslateApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(CTranslateApp, CWinApp)
END_MESSAGE_MAP()

// CTranslateApp creation

CTranslateApp::CTranslateApp()
{
	// 
	// 
}


// unique CTranslateApp object.

CTranslateApp theApp;


// initialize CTranslateApp 

BOOL CTranslateApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	