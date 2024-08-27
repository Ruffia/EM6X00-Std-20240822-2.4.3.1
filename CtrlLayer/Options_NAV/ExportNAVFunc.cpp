//---------------------------
// ExportNAVFunc.cpp
// ��������ĺ���
//---------------------------

/////////////////////////////////////////////////
// ���ܣ�DLL���������
//		������Ӧ�ó�����ظ�DLL�󣬵����������
//		������ʾ��DLL�ڽ��ĶԻ���
// �������1��HWND hMainWnd���Ի��򸸴��ڵľ��
/////////////////////////////////////////////////


/////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Options_NAV.h"

#include "PanelNAV.h"

void NAV_ShowWnd(HWND hMainWnd, BOOL bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pPanelNAV != NULL && theApp.m_pPanelNAV->m_hWnd != NULL )
	{
		theApp.m_pPanelNAV->ShowWindow(bShow);
	}
	else
	{
		if( theApp.m_pPanelNAV != NULL )
		{
			delete theApp.m_pPanelNAV;
			theApp.m_pPanelNAV = NULL;
		}
		theApp.m_pPanelNAV = new CPanelNAV;
		CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
		ASSERT(pMainWnd);
		BOOL ret = theApp.m_pPanelNAV->Create(IDD_PANEL_NAV, pMainWnd);
		if( !ret )
		{
			MessageBox(pMainWnd->m_hWnd, _T("Create dialog in DLL failed"),_T( "Stage"), MB_OK);
		}
		else
		{
//			theApp.m_pPanelNAV->Connect();
			theApp.m_pPanelNAV->ShowWindow(bShow);
		}
	}
}

BOOL NAV_IsReady()
{
	if( theApp.m_pPanelNAV != NULL && theApp.m_pPanelNAV->m_hWnd != NULL )
	{
		return theApp.m_pPanelNAV->m_bInitCamera;
	}
	else
		return FALSE;
}

void NAV_SetLanguageID( WORD wID )
{
	if( theApp.m_pPanelNAV != NULL && theApp.m_pPanelNAV->m_hWnd != NULL )
		theApp.m_pPanelNAV->SetLanguageID( wID );
}

void NAV_UpdateXYPos( float fX, float fY )
{
	if( theApp.m_pPanelNAV != NULL && theApp.m_pPanelNAV->m_hWnd != NULL )
		theApp.m_pPanelNAV->UpdatePos( fX, fY );
}

void NAV_SetV1Status( BOOL bStatus)
{
	if( theApp.m_pPanelNAV != NULL )
		theApp.m_pPanelNAV->m_bV1Status = bStatus;
}

void NAV_UpdateStatus( int nIndex, BOOL bStatus )
{
	if( theApp.m_pPanelNAV != NULL )
	{
		switch( nIndex )
		{
		case	0:
			theApp.m_pPanelNAV->UpdateNAVSwitch( bStatus, FALSE );
			break;
		case	1:
			theApp.m_pPanelNAV->m_bStageIsMoving = bStatus;
			break;
		case    2:
			theApp.m_pPanelNAV->CallCameraInit();
			break;
		}
	}
}