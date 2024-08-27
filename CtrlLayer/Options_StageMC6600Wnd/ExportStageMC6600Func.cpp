//---------------------------
// ExportStageMC6600Func.cpp
// 定义输出的函数
//---------------------------

/////////////////////////////////////////////////
// 功能：DLL的输出函数
//		当其它应用程序加载该DLL后，调用这个函数
//		可以显示该DLL内建的对话框
// 输入参数1：HWND hMainWnd，对话框父窗口的句柄
/////////////////////////////////////////////////


/////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Options_StageMC6600Wnd.h"

#include "PanelStage.h"


void StageMC6600_ShowWnd(HWND hMainWnd)
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
		BOOL ret = theApp.m_pPanelStage->Create(IDD_PANEL_STAGE, pMainWnd);
		if( !ret )
		{
			MessageBox(pMainWnd->m_hWnd, _T("Create dialog in DLL failed"),_T( "Stage"), MB_OK);
		}
		else
		{
			theApp.m_pPanelStage->Connect();
			theApp.m_pPanelStage->ShowWindow(SW_SHOW);
		}
	}
}

BOOL StageMC6600_IsReady()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
	{
		return theApp.m_pPanelStage->m_bIsMC6600Ready;
	}
	else
		return FALSE;
}

BOOL StageMC6600_IsMoving()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		return theApp.m_pPanelStage->QueryMoving();
	else
		return FALSE;
}

void StageMC6600_SetLanguageID( WORD wID )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetLanguageID( wID );
}

void StageMC6600_SetPixelsPerMM( double dX, double dY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetPixelsPerMM( dX, dY );
}

void StageMC6600_SetMag( double dMag )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetMag( dMag );
}

void StageMC6600_SetResolution( CSize szReso )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetResolution( szReso );
}

void StageMC6600_MoveToCenter(int nCenterX, int nCenterY, int nX, int nY)
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->MoveToCenter( nCenterX, nCenterY, nX, nY  );
}

void StageMC6600_GoHome()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Home();
}

void StageMC6600_GoOrigin()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Zero();
}

void StageMC6600_Move( short nAxis, float fStep )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->MoveAxis_FromThird(nAxis, fStep);
}

void StageMC6600_Move5Axis(float fX, float fY, float fZ, float fR, float fT )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move5Axis_FromThird(fX, fY, fZ, fR, fT);
}

void StageMC6600_Move2Axis(float fX, float fY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move2Axis_FromThird(fX, fY);
}

void StageMC6600_StopMove()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Stop();
}

void StageMC6600_OnVentExit()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->OnVentExit();
}

void StageMC6600_GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->GetParam( nIndex, f1, f2, f3, f4, f5 );
}

void StageMC6600_SetTrackParam( int nSpeedX, int nSpeedY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTrackParam( nSpeedX, nSpeedY );
}

void StageMC6600_StopTrack()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->StopTrack();
}

void StageMC6600_SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedR )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTracklocus1( nSpeedX, nSpeedY,nSpeedR );
}

void StageMC6600_SetTracklocus2( int nSpeedT, int nSpeedZ )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTracklocus2( nSpeedT, nSpeedZ);
}

void StageMC6600_StopTracklocus()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->StopTracklocus();
}

