//---------------------------
// ExportCameraFunc.cpp
// 定义输出的函数
//---------------------------

/////////////////////////////////////////////////
// 功能：DLL的输出函数
//		当其它应用程序加载该DLL后，调用这个函数
//		可以显示该DLL内建的对话框
// 输入参数1：HWND hMainWnd，对话框父窗口的句柄
/////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "Options_Camera.h"

#include "PanelCamera.h"

void ShowCameraDlg(HWND hMainWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pPanelCamera != NULL && theApp.m_pPanelCamera->m_hWnd != NULL )
	{
		if( theApp.m_pPanelCamera->m_nCapture < 1 )
			theApp.m_pPanelCamera->CamInit();
		theApp.m_pPanelCamera->ShowWindow(SW_SHOW);
	}
	else
	{
		if( theApp.m_pPanelCamera != NULL )
		{
			delete theApp.m_pPanelCamera;
			theApp.m_pPanelCamera = NULL;
		}
		theApp.m_pPanelCamera = new CPanelCamera;
		CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
		ASSERT(pMainWnd);
		BOOL ret = theApp.m_pPanelCamera->Create(IDD_PANEL_Camera, pMainWnd);
		if( !ret )
		{
			MessageBox(pMainWnd->m_hWnd, "Create dialog in DLL failed", "Camera", MB_OK);
		}
		else
		{
			theApp.m_pPanelCamera->CamInit();
			theApp.m_pPanelCamera->ShowWindow(SW_SHOW);
		}
	}
}

void ShowCameraWnd(HWND hMainWnd, BOOL bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pCameraWnd != NULL && theApp.m_pCameraWnd->m_hWnd != NULL )
	{
		if( bShow )
		{
			if( theApp.m_pCameraWnd->m_nCapture < 1 )
				theApp.m_pCameraWnd->CamInit();
			theApp.m_pCameraWnd->ShowWindow(SW_SHOW);
		}
		else
			theApp.m_pCameraWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		if( !bShow )
			return;
		if( theApp.m_pCameraWnd != NULL )
		{
			delete theApp.m_pCameraWnd;
			theApp.m_pCameraWnd = NULL;
		}
		theApp.m_pCameraWnd = new CCameraWnd;
		CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
		ASSERT(pMainWnd);
		
		//////////////////////////////////////////////////////////////////////
		// Create camera
		//////////////////////////////////////////////////////////////////////
		CRect rectScreen(CPoint(100,100), CSize(320,240));
		DWORD dwExStyle		= WS_EX_CLIENTEDGE;
		DWORD dwStyle		= WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME
			| WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX ;
		
		LPCTSTR pstrOwnerClass = AfxRegisterWndClass(1);
//		if ( !theApp.m_pCameraWnd->CreateEx( dwExStyle, pstrOwnerClass, "CCD - 使用能谱时请关闭本窗口", dwStyle,
		if ( !theApp.m_pCameraWnd->CreateEx( dwExStyle, pstrOwnerClass, "CCD - Close when using EDS", dwStyle,
			(RECT)rectScreen, pMainWnd, 0 ) )
		{
			MessageBox(pMainWnd->m_hWnd, "Create dialog in DLL failed", "CCD", MB_OK);
		}
		else
		{
			theApp.m_pCameraWnd->CamInit();
			theApp.m_pCameraWnd->ShowWindow(SW_SHOW);
		}
	}
}

void CCD_SetLanguageID( WORD wID )
{
	if( theApp.m_pCameraWnd != NULL )
		theApp.m_pCameraWnd->SetLanguageID( wID );
}
