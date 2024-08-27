//---------------------------
// ExportStageTMCMFunc.cpp
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
#include "Options_StageTMCMWnd.h"

#include "PanelStage.h"

void StageTMCM_ShowWnd(HWND hMainWnd)
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
		BOOL ret = theApp.m_pPanelStage->Create(IDD_DIALOG_Stage, pMainWnd);
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

BOOL StageTMCM_IsReady()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
	{
		return theApp.m_pPanelStage->m_bIsTMCMReady;
	}
	else
		return FALSE;
}

BOOL StageTMCM_IsMoving()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		return theApp.m_pPanelStage->QueryMoving();
	else
		return FALSE;
}

void StageTMCM_SetLanguageID( WORD wID )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetLanguageID( wID );
}

void StageTMCM_SetPixelsPerMM( double dX, double dY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetPixelsPerMM( dX, dY );
}

void StageTMCM_SetMag( double dMag )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetMag( dMag );
}

void StageTMCM_SetResolution( CSize szReso )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetResolution( szReso );
}

void StageTMCM_MoveToCenter(int nCenterX, int nCenterY, int nX, int nY)
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->MoveToCenter( nCenterX, nCenterY, nX, nY  );
}

void StageTMCM_GoHome()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Home();
}

void StageTMCM_GoOrigin()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Zero();
}

void StageTMCM_Move( short nAxis, float fStep )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->MoveAxis_FromThird(nAxis, fStep);
}

void StageTMCM_Move5Axis(float fX, float fY, float fZ, float fR, float fT )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move5Axis_FromThird(fX ,fY, fZ, fR, fT);
}

void StageTMCM_Move2Axis(float fX, float fY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Move2Axis_FromThird(fX, fY);
}

void StageTMCM_StopMove()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Stop();
}

void StageTMCM_OnVentExit()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->OnVentExit();
}

void StageTMCM_GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->GetParam( nIndex, f1, f2, f3, f4, f5 );
}

void StageTMCM_SetTrackParam( int nSpeedX, int nSpeedY )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTrackParam( nSpeedX, nSpeedY );
}

void StageTMCM_StopTrack()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->StopTrack();
}

void StageTMCM_SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedZ )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTracklocus1( nSpeedX, nSpeedY,nSpeedZ );
}

void StageTMCM_SetTracklocus2( int nSpeedT, int nSpeedZ )
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->SetTracklocus2( nSpeedT, nSpeedZ);
}

void StageTMCM_StopTracklocus()
{
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->StopTracklocus();
}

