//---------------------------
// ExportStageMC600Func.cpp
// ��������ĺ���
//---------------------------

/////////////////////////////////////////////////
// ���ܣ�DLL���������
//		������Ӧ�ó�����ظ�DLL�󣬵����������
//		������ʾ��DLL�ڽ��ĶԻ���
// �������1��HWND hMainWnd���Ի��򸸴��ڵľ��
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Version
// 2020.04.16 1.0.0.1
// ��� MC600 4�����̨
//
// 2020.07.02 1.0.1.1
// MC600��翪����Ĭ����Ч��ΪĬ����Ч�����Ӳ���Ķ���
// MC600��һЩ����������������H�����ܺ�رչ�翪�أ�
// ���ָ�Ĭ�ϲ�����������pitch���������ʱ�Ա�֤������Ч
//
// 2020.11.03 1.1.0.1
// ����Vince��T�ᣩ����Ϊ5��̨
/////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Options_StageMC600Wnd.h"

#include "PanelStage.h"

int	g_nStageType = 2;	// 4��

void StageMC600_ShowWnd(HWND hMainWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
	{
// 		if( !(theApp.m_pPanelStage->m_Stage.xy.bReady) )
// 			theApp.m_pPanelStage->Stage_XY_Init();
		theApp.m_pPanelStage->ShowWindow(SW_SHOW);
	}
	else
	{
		if( theApp.m_pPanelStage != NULL )
		{
			delete theApp.m_pPanelStage;
			theApp.m_pPanelStage = NULL;
		}
		theApp.m_pPanelStage = new CPanelStage;
		CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
		ASSERT(pMainWnd);
		BOOL ret = theApp.m_pPanelStage->Create(IDD_PANEL_Stage, pMainWnd);
		if( !ret )
		{
			MessageBox(pMainWnd->m_hWnd, "Create dialog in DLL failed", "Stage", MB_OK);
		}
		else
		{
			theApp.m_pPanelStage->m_nStageType = g_nStageType;
			theApp.m_pPanelStage->Connect();
			theApp.m_pPanelStage->ShowWindow(SW_SHOW);
		}
	}
}

BOOL StageMC600_IsReady()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
	{
// 		if( g_nStageType > 2 )	// 5��
// 			return ( theApp.m_pPanelStage->m_bIsMC600Ready && theApp.m_pPanelStage->m_bIsVinceReady );
// 		else
			return theApp.m_pPanelStage->m_bIsMC600Ready;
	}
	else
		return FALSE;
}

void StageMC600_SetLanguageID( WORD wID )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetLanguageID( wID );
}

void StageMC600_SetPixelsPerMM( double dX, double dY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetPixelsPerMM( dX, dY );
}

void StageMC600_SetResolution( CSize szReso )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetResolution( szReso );
}

void StageMC600_SetMag( double dMag )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetMag( dMag );
}

BOOL StageMC600_IsMoving()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		return theApp.m_pPanelStage->QueryMoving();
	else
		return FALSE;
}

void StageMC600_StopMove()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Stop();
}

void StageMC600_MoveToCenter( int nCenterX, int nCenterY, int nX, int nY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->MoveToCenter( nCenterX, nCenterY, nX, nY );
}

void StageMC600_Move( short nAxis, float fPos )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move( nAxis, fPos );
}

void StageMC600_Move4Axis( float fX, float fY, float fZ, float fR )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move4Axis( 3, fX, fY, fZ, fR );
}

void StageMC600_Move5Axis( float fX, float fY, float fZ, float fR, float fT )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move5Axis( 3, fX, fY, fZ, fR, fT );
}

void StageMC600_GoOrigin()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Zero();
}

void StageMC600_GoHome()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Home(1);
}

void StageMC600_OnVentExit()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->OnVentExit();
}

void StageMC600_SetTrackParam( int nSpeedX, int nSpeedY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTrackParam( nSpeedX, nSpeedY );
}

void StageMC600_StopTrack()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->StopTrack();
}

void StageMC600_SetParam( int nIndex, double dParam )
{
	if( nIndex > 0 )
	{
		if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
			theApp.m_pPanelStage->SetParam( nIndex, dParam );
	}
	else
		g_nStageType = (int)dParam;
}

void StageMC600_GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->GetParam( nIndex, f1, f2, f3, f4, f5 );
}
