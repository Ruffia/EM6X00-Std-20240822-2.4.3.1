// DiagLogging.cpp : implementation file
//
// Last Modify : 2013.01.05

#include "stdafx.h"
#include "XXX2.h"
#include "ScanView.h"
#include "DlgDiagLog.h"
#include "DlgDiagLogGraph.h"
#include "MainFrm.h"
#include "ControlLayer.h"
#include "BoardComm.h"
#include "DebugMessageDispatcher.h"
#include "GraphBuffManager.h"
#include "ScaleManager.h"
#include "SpellmanStatusDetector.h"
#include "VACDataManager.h"
#include "HVPowerDataManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScanView
/*
void CScanView::OnBUTTONDiagnose() 
void CScanView::OnUpdateBUTTONDiagnose(CCmdUI* pCmdUI) 
void CScanView::OnBUTTONDiagnoseSetup() 
void CScanView::OnBUTTONDiagnoseSetup() 
void CScanView::OnUpdateBUTTONDiagnoseSetup(CCmdUI* pCmdUI) 
void CScanView::Diagnose( BOOL bCheck )
CString CScanView::Diag_Logging()
CString CScanView::Diag_Logging2()
*/
void CScanView::OnBUTTONDiagnose() 
{
	// TODO: Add your command handler code here
	if( ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd == NULL )
		return;
	theApp.m_bDiagLogging = !theApp.m_bDiagLogging;
	((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.ShowWindow( SW_SHOW );
	Diagnose( theApp.m_bDiagLogging );
}

void CScanView::OnUpdateBUTTONDiagnose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(theApp.m_bDiagLogging);
}

void CScanView::OnBUTTONDiagnoseSetup() 
{
	// TODO: Add your command handler code here
	if( ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd == NULL )
		return;
	CDlgDiagLog dlg;
	dlg.DoModal();
}

void CScanView::OnUpdateBUTTONDiagnoseSetup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !theApp.m_bDiagLogging );
}

void CScanView::Diagnose( BOOL bCheck )
{
	if( bCheck )
	{
		///////////////////////////////////////////////////
		// 准备记录文件
		char appfilepath[255];
		::GetModuleFileName( NULL, appfilepath, 255 );
		char *ptemp = strrchr( appfilepath, '\\');
		ptemp[1] = 0;

		CString str, strTemp, strParam, strFirst;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%s\\Log\\zLog%s.txt", appfilepath, strTemp );
		CControlLayer::Instance().m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		///////////////////////////////////////////////////

		///////////////////////////////////////////////////
		// 更新参数设置
		str			= "\t";
		strParam	= "Min,Max,Step\t";
		strFirst	= "0000\t";

		///////////////////////////////////////////////////
		// Logging Params
		unsigned long cbRet;
		char Buf[1024];
		
		CControlLayer::Instance().m_DL.other.lTimes	= 0;
		CControlLayer::Instance().m_DL.other.lLoggingDataCounts = 0;
		if( CControlLayer::Instance().m_DL.other.bGaugeCompr )
		{
			strTemp.Format( "Gauge compr\t" );
			str += strTemp;
		}
		CString strTemp2;
		for(int i=0; i<9; i++ )
		{
			if( CControlLayer::Instance().m_DL.log.bLogParam[i] )
			{
				strTemp2.LoadString(IDS_CHECK_PARAM_LNS_L1DAC +i);
				strTemp.Format( "%s\t", strTemp2 );
				str += strTemp;
			}
		}
		for(int i=0; i<96; i++ )	// 6*16
		{
			if( CControlLayer::Instance().m_DL.log.bLog[i] )
			{
				strTemp.Format( "%s\t\t\t\t\t", CControlLayer::Instance().m_DL.adc.chn[i].strLabel );
				str += strTemp;
			}
		}
		sprintf( Buf, "%s\n", str );
		WriteFile( CControlLayer::Instance().m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		sprintf( Buf, "%s\n", strParam );
		WriteFile( CControlLayer::Instance().m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		sprintf( Buf, "%s\n", strFirst );
		WriteFile( CControlLayer::Instance().m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
//		m_pPageVS->SetTimers( Diag_Total, TRUE );
		///////////////////////////////////////////////////

		// SCN
		for(int i=Diag_Stig_X; i<Diag_CalI_TC0; i++ )
		{
			CControlLayer::Instance().m_DL.diag[i].nIntv	= CControlLayer::Instance().m_DL.other.nElapseChanging;
			CControlLayer::Instance().m_DL.diag[i].nFinish	= -1;
			if( CControlLayer::Instance().m_DL.diag[i].nCheck )
			{
				strTemp.Format( "%s\t", CControlLayer::Instance().m_DL.diag[i].strLabel );
				str += strTemp;
				strTemp.Format( "%04X,%04X,%04X\t", CControlLayer::Instance().m_DL.diag[i].nMin, CControlLayer::Instance().m_DL.diag[i].nMax, CControlLayer::Instance().m_DL.diag[i].nStep );
				strParam += strTemp;
				strTemp.Format( "%04X\t", CControlLayer::Instance().m_DL.diag[i].nMin );
				strFirst += strTemp;
			}
		}
		// VAC & SPR
		for(int i=Diag_CalI_TC0; i<Diag_Offset_0; i++ )
		{
			CControlLayer::Instance().m_DL.diag[i].nIntv	= CControlLayer::Instance().m_DL.other.nElapseChanging;
			CControlLayer::Instance().m_DL.diag[i].nFinish	= -1;
			if( CControlLayer::Instance().m_DL.diag[i].nCheck )
			{
				strTemp.Format( "%s\t\t", CControlLayer::Instance().m_DL.diag[i].strLabel );
				str += strTemp;
				strTemp.Format( "%04X,%04X,%04X\t\t", CControlLayer::Instance().m_DL.diag[i].nMin, CControlLayer::Instance().m_DL.diag[i].nMax, CControlLayer::Instance().m_DL.diag[i].nStep );
				strParam += strTemp;
				strTemp.Format( "%04X\t\t", CControlLayer::Instance().m_DL.diag[i].nMin );
				strFirst += strTemp;
			}
		}
		// MCI
		for(int i=Diag_Offset_0; i<Diag_Total; i++ )
		{
			CControlLayer::Instance().m_DL.diag[i].nIntv	= CControlLayer::Instance().m_DL.other.nElapseChanging;
			CControlLayer::Instance().m_DL.diag[i].nFinish	= -1;
			if( CControlLayer::Instance().m_DL.diag[i].nCheck )
			{
				strTemp.Format( "%s\t", CControlLayer::Instance().m_DL.diag[i].strLabel );
				str += strTemp;
				strTemp.Format( "%04X,%04X,%04X\t", CControlLayer::Instance().m_DL.diag[i].nMin, CControlLayer::Instance().m_DL.diag[i].nMax, CControlLayer::Instance().m_DL.diag[i].nStep );
				strParam += strTemp;
				strTemp.Format( "%04X\t", CControlLayer::Instance().m_DL.diag[i].nMin );
				strFirst += strTemp;
			}
		}
		///////////////////////////////////////////////////

		// 准备工作
		///////////////////////////////////////////////////
//		KillTimer( tmrReadPort );
//		KillTimer( tmrReadFIFO );
//		KillTimer( tmrClear );
//		KillTimer( tmrWaitToInit );
		// 暂停真空自动流程的工作
		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
			CSMvac::Instance().SetParams( 22, 1 );

		// 保存现场
		CScanParameterSaver::Instance().m_lBrightnessSaved		= CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal;
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			CScanParameterSaver::Instance().m_lContrastSaved	= CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal;
		else
			CScanParameterSaver::Instance().m_lContrastSaved	= CSemCoreAssemble::Instance().m_SP_PMT.GetPos().lVal;
		CScanParameterSaver::Instance().m_lStigmatorXSaved		= CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
		CScanParameterSaver::Instance().m_lStigmatorYSaved		= CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
		CScanParameterSaver::Instance().m_lCondensorLensSaved	= CSemCoreAssemble::Instance().m_CondenserLens.GetPos().lVal;
		CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
		CScanParameterSaver::Instance().m_lAlignXSaved			= CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos().lVal;
		CScanParameterSaver::Instance().m_lAlignYSaved			= CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos().lVal;

		// Changing Params 初始化
		for(int i=Diag_Stig_X; i<Diag_Total; i++ )
			if( CControlLayer::Instance().m_DL.diag[i].nCheck )
				CControlLayer::Instance().Diag_Init( i, TRUE );
		///////////////////////////////////////////////////

		((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.m_nTotal = -1;
		((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.InitGraphData();

		///////////////////////////////////////////////////
		// 停止采集、设置USB-PIXEL RATE、清空FIFO
		// set pix_clock to 2
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, 2 );
		CSemCtrl::Instance().USB_ClearFIFO();

		SetTimer( Diag_Total, CControlLayer::Instance().m_DL.other.nElapseLogging, NULL );
	}
	else
	{
		KillTimer( Diag_Total );
		CloseHandle(CControlLayer::Instance().m_DL.other.hLogFile);
		// 恢复真空自动流程的工作
		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
			CSMvac::Instance().SetParams( 22, 0 );

		///////////////////////////////////////////////////
		// 恢复现场
		// Brightness
		WORD wInit	= 0x0800;
		WORD wValue = wInit;
		for(int i=0; i<4; i++ )
		{
			wValue = wInit & 0x00FF;
			wValue |= ( ((wInit >> 8) | (i << 6)) << 8 );
			CSemCtrl::Instance().USB_WriteToBoard( USB_Det03StoreDAC, TRUE, wValue );
		}
		for(int i=0; i<2; i++ )
		{
			wValue = wInit & 0x00FF;
			wValue |= ( ((wInit >> 8) | (i << 6)) << 8 );
			CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		}
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		SerialNumber.lVal = -1;
		CSemCtrl::Instance().USB_WriteToBoard( USB_DetPairSelection, TRUE, CControlLayer::Instance().m_BrdMCI.nDetPair );
		CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_DetPair, CControlLayer::Instance().m_BrdMCI.nDetPair );
		CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_Det2nd, CControlLayer::Instance().m_BrdMCI.bDet2nd );
		CSemCoreAssemble::Instance().m_Signal2Brightness.SetParams( SET_PARAMS_DetPair, CControlLayer::Instance().m_BrdMCI.nDetPair );
		CSemCtrl::Instance().m_SemScan.USB_SetParams( 32, CControlLayer::Instance().m_BrdMCI.nDetPair );

		// Brightness
		{
			ClassID.lVal	= SemControlPanelClass_Brightness;
			Flag.lVal		= 1;
			Xpos.lVal		= CScanParameterSaver::Instance().m_lBrightnessSaved;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		// Contrast
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			ClassID.lVal = SemControlPanelClass_Contrast;
			Flag.lVal = 1;
			Xpos.lVal = 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		else
		{
			ClassID.lVal = SemControlPanelClass_SP_PMT;
			Flag.lVal = 1;
			Xpos.lVal = 0;
			CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
			if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					pMainFrame->m_wndImpPanelAll2.ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
				else
					pMainFrame->m_wndImpPanelAll2.ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
			}
		}
		// Stigmator
		{
			ClassID.lVal = SemControlPanelClass_Stigmator;
			Flag.lVal	= 3;
			Xpos.lVal	= CScanParameterSaver::Instance().m_lStigmatorXSaved;
			Ypos.lVal	= CScanParameterSaver::Instance().m_lStigmatorYSaved;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		// CondenserLens
		{
			ClassID.lVal = SemControlPanelClass_CondenserLens;
			Flag.lVal	= 1;
			Xpos.lVal	= CScanParameterSaver::Instance().m_lCondensorLensSaved;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		// ObjectiveLens
		{
			ClassID.lVal = SemControlPanelClass_ObjectiveLens;
			Flag.lVal	= 1;
			Xpos.dblVal = CScanParameterSaver::Instance().m_lObjectiveLensSaved;
			Xpos.lVal	= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		// BeemAlignment
		{
			ClassID.lVal = SemControlPanelClass_BeemAlignment;
			Flag.lVal	= 3;
			Xpos.lVal	= CScanParameterSaver::Instance().m_lAlignXSaved;
			Ypos.lVal	= CScanParameterSaver::Instance().m_lAlignYSaved;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		///////////////////////////////////////////////////

		///////////////////////////////////////////////////
		// 准备记录文件
		char appfilepath[255];
		::GetModuleFileName( NULL, appfilepath, 255 );
		char *ptemp = strrchr( appfilepath, '\\');
		ptemp[1] = 0;
		
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		// 存曲线图
		str.Format( "%s\\Log\\zLog%s-Graph.png", appfilepath, strTemp );
		((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.SaveGraph( str );
		// 存日志
		str.Format( "%s\\Log\\zLog%s-Graph.txt", appfilepath, strTemp );
		CControlLayer::Instance().m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		unsigned long cbRet;
		char Buf[100];

		for(int i=0; i<CControlLayer::Instance().m_DL.other.lTimes-1; i++ )
		{
			str = "";
			for( int j=0; j<GraphDataBuffGroups; j++ )
			{
				if( CControlLayer::Instance().m_DL.graph[j].bGraph )
				{
					strTemp.Format( "%.8f\t", CGraphBuffMgr::Instance().m_pGraphDataBuff[j][i] );
					str += strTemp;
				}
			}
			sprintf( Buf, "%s\n", str );
			WriteFile( CControlLayer::Instance().m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		CloseHandle(CControlLayer::Instance().m_DL.other.hLogFile);
	}
}



CString CScanView::Diag_Logging()
{
	CString strRes = CControlLayer::Instance().Diag_Logging();
	if( CControlLayer::Instance().m_DL.other.bDebug )
	{
		// 13.05.09
		((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.UpdateGraphData( CControlLayer::Instance().m_DL.other.lLoggingDataCounts );
		CControlLayer::Instance().m_DL.other.lLoggingDataCounts += 3;
	}
	else
		((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.UpdateGraphData( CControlLayer::Instance().m_DL.other.lTimes );
	return strRes;
}

CString CScanView::Diag_Logging2()
{
	CString strRes = CControlLayer::Instance().Diag_Logging2();
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( CControlLayer::Instance().m_DL.other.bDebug )
	{
		// 13.05.09
		if( pfrm != NULL )
			pfrm->m_dlgGraph.UpdateGraphData( CControlLayer::Instance().m_DL.other.lLoggingDataCounts );
		CControlLayer::Instance().m_DL.other.lLoggingDataCounts += 3;
	}
	else
	{
		if( pfrm != NULL )
		{
			pfrm->m_dlgGraph.UpdateGraphData( CControlLayer::Instance().m_DL.other.lTimes );
//			pfrm->m_dlgDiffGraph.UpdateGraphData( CControlLayer::Instance().m_DL.other.lTimes );
		}
	}
	return strRes;
}

CString CScanView::Diag_VAC()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();

	if( CControlLayer::Instance().m_DL.diag[Diag_CalI_TC0].nCheck )
	{
		nIndex = Diag_CalI_TC0;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_CalI_TC1].nCheck )
	{
		nIndex = Diag_CalI_TC1;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_CalI_TC2].nCheck )
	{
		nIndex = Diag_CalI_TC2;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_CalI_TC3].nCheck )
	{
		nIndex = Diag_CalI_TC3;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 3;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_CalI_TC4].nCheck )
	{
		nIndex = Diag_CalI_TC4;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_CalI_TC5].nCheck )
	{
		nIndex = Diag_CalI_TC5;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	if( CControlLayer::Instance().m_DL.diag[Diag_SetPoint_TC0].nCheck )
	{
		nIndex = Diag_SetPoint_TC0;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SetPoint_TC1].nCheck )
	{
		nIndex = Diag_SetPoint_TC1;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SetPoint_TC2].nCheck )
	{
		nIndex = Diag_SetPoint_TC2;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SetPoint_TC3].nCheck )
	{
		nIndex = Diag_SetPoint_TC3;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 3;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SetPoint_TC4].nCheck )
	{
		nIndex = Diag_SetPoint_TC4;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SetPoint_TC5].nCheck )
	{
		nIndex = Diag_SetPoint_TC5;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACselected	= 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nVACDACchannel	= 3;
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACgroupOptionChanged();
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACvalUpdate( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_scrollVACDACsVal.SetScrollPos( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}

	return strRes;
}

CString CScanView::Diag_SPR()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();

	if( CControlLayer::Instance().m_DL.diag[Diag_SPRDAC_ChA].nCheck )
	{
		nIndex = Diag_SPRDAC_ChA;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t%.6f\t", nPos+32768, 2.5 *(nPos+32768) /65536 );
		strRes += str;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nSPRDACchannel = 0;
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin -32768;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACsValUpdae( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACupdate();
		}
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SPRDAC_ChB].nCheck )
	{
		nIndex = Diag_SPRDAC_ChB;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t%.6f\t", nPos+32768, 2.5 *(nPos+32768) /65536 );
		strRes += str;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nSPRDACchannel = 1;
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin -32768;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACsValUpdae( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACupdate();
		}
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SPRDAC_ChC].nCheck )
	{
		nIndex = Diag_SPRDAC_ChC;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t%.6f\t", nPos+32768, 2.5 *(nPos+32768) /65536 );
		strRes += str;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nSPRDACchannel = 2;
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin -32768;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACsValUpdae( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACupdate();
		}
	}
	if( CControlLayer::Instance().m_DL.diag[Diag_SPRDAC_ChD].nCheck )
	{
		nIndex = Diag_SPRDAC_ChD;
		int nPos = CControlLayer::Instance().m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t%.6f\t", nPos+32768, 2.5 *(nPos+32768) /65536 );
		strRes += str;
		if( CControlLayer::Instance().m_DL.diag[nIndex].nFinish < 0 )
		{
			pfrm->m_MainPanelDebug.m_PagePanelVS.m_nSPRDACchannel = 3;
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACselOptionChanged();
			nPos += CControlLayer::Instance().m_DL.diag[nIndex].nStep;
			if( nPos > CControlLayer::Instance().m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = CControlLayer::Instance().m_DL.diag[nIndex].nMin -32768;
//				CControlLayer::Instance().m_DL.diag[nIndex].nFinish = 1;
			}
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACsValUpdae( nPos );
			pfrm->m_MainPanelDebug.m_PagePanelVS.SPRDACupdate();
		}
	}

	return strRes;
}

CString CScanView::Diag_MCI()
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( CControlLayer::Instance().m_DL.diag[Diag_Offset_0].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet03Chn2 = 0;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_Offset_1].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet03Chn2 = 1;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_Offset_2].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet03Chn2 = 2;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_Offset_3].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet03Chn2 = 3;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_Offset_4].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet45Chn2 = 0;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_Offset_5].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet45Chn2 = 1;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_FineMag_X].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet45Chn2 = 2;
	}

	if( CControlLayer::Instance().m_DL.diag[Diag_FineMag_Y].nCheck )
	{
		pfrm->m_MainPanelDebug.m_PagePanelMCI.m_nDet45Chn2 = 3;
	}

	CString strRes = CControlLayer::Instance().Diag_MCI();
	return strRes;
}
