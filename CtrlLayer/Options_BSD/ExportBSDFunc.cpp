//---------------------------
// ExportBSDFunc.cpp
// ��������ĺ���
//---------------------------

/////////////////////////////////////////////////
// ���ܣ�DLL���������
//		������Ӧ�ó�����ظ�DLL�󣬵����������
//		������ʾ��DLL�ڽ��ĶԻ���
// �������1��HWND hMainWnd���Ի��򸸴��ڵľ��
/////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Options_BSD.h"

#include "PanelBSD.h"

void BSD_ShowWnd(HWND hMainWnd, BOOL bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pPanelBSD != NULL && theApp.m_pPanelBSD->m_hWnd != NULL )
	{
		if( bShow )
			theApp.m_pPanelBSD->ShowWindow(SW_SHOW);
		else
			theApp.m_pPanelBSD->ShowWindow(SW_HIDE);
	}
	else
	{
		if( theApp.m_pPanelBSD != NULL )
		{
			delete theApp.m_pPanelBSD;
			theApp.m_pPanelBSD = NULL;
		}
		theApp.m_pPanelBSD = new CPanelBSD;
		CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
		ASSERT(pMainWnd);
		BOOL ret = theApp.m_pPanelBSD->Create(IDD_PANEL_BSD, pMainWnd);
		if( !ret )
		{
			MessageBox(pMainWnd->m_hWnd, "Create dialog in DLL failed", "BSD", MB_OK);
		}
		else
		{
			theApp.m_pPanelBSD->InitHWND(hMainWnd);
			if( bShow )
				theApp.m_pPanelBSD->ShowWindow(SW_SHOW);
			else
				theApp.m_pPanelBSD->ShowWindow(SW_HIDE);
		}
	}
}

BOOL BSD_IsReady()
{
	if( theApp.m_pPanelBSD != NULL && theApp.m_pPanelBSD->m_hWnd != NULL )
		return theApp.m_pPanelBSD->m_bIsReady;
	else
		return FALSE;
}

void BSD_SetLanguageID( WORD wID )
{
	if( theApp.m_pPanelBSD != NULL && theApp.m_pPanelBSD->m_hWnd != NULL )
		theApp.m_pPanelBSD->SetLanguageID( wID );
}
