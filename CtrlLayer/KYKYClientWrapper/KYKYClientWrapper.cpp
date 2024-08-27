// KYKYClientWrapper.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "KYKYClientWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CKYKYClientWrapperApp

BEGIN_MESSAGE_MAP(CKYKYClientWrapperApp, CWinApp)
	//{{AFX_MSG_MAP(CKYKYClientWrapperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKYKYClientWrapperApp construction

CKYKYClientWrapperApp::CKYKYClientWrapperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CKYKYClientWrapperApp::KYKYSEM_Init(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SEMCommClient.Init(hWnd);
}

BOOL CKYKYClientWrapperApp::KYKYSEM_Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return SEMCommClient.Connect(IP1, IP2, IP3, IP4);
}

void CKYKYClientWrapperApp::KYKYSEM_Disconnect(BOOL bActive)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SEMCommClient.Disconnect(bActive);
}

BOOL CKYKYClientWrapperApp::KYKYSEM_QueryMagnification()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return SEMCommClient.QueryMagnification();
}

BOOL CKYKYClientWrapperApp::KYKYSEM_QueryWorkDistance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return SEMCommClient.QueryWorkDistance();
}

BOOL CKYKYClientWrapperApp::KYKYSEM_QueryScanRotAngle()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return SEMCommClient.QueryScanRotAngle();
}

BOOL CKYKYClientWrapperApp::KYKYSEM_QueryHighVoltage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return SEMCommClient.QueryHighVoltage();
}

BOOL CKYKYClientWrapperApp::KYKYSEM_QueryEMI()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return SEMCommClient.QueryEMI();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKYKYClientWrapperApp object

CKYKYClientWrapperApp theApp;
