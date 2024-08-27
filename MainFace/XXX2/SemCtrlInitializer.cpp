#include "stdafx.h"
#include <afxwin.h>
#include "SemCtrlInitializer.h"
#include "SemDataType.h"
#include "MCIO.H"
#include "SemCtrl.h"
#include "SemCtrlPanelManager.h"
#include "CommSpellman.h"
#include "DebugMessageDispatcher.h"
#include "ControlLayer.h"
#include "BoardComm.h"
#include "XXX2.h"
#include "ScaleManager.h"
#include "SpellmanStatusDetector.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "HVPowerDataManager.h"
#include "MainFrm.h"
#include "DlgImpPanelDebugR.h"

extern CXXX2App theApp;

CSemCtrlInitializer& CSemCtrlInitializer::Instance()
{
	static CSemCtrlInitializer Inst;
	return Inst;
}


CSemCtrlInitializer::CSemCtrlInitializer()
{

}

//======2004-6-2_自动保存并更新控制量==================
// 修改标示：2004-6-2_自动保存并更新控制量
// 修改日期：2004年6月2日
// 修改人：罗鸣华
// 修改原因：
//	自动保存并更新物镜、聚光镜、电对中、消像散等控制量。
// 本次修改前代码副本
/*----------------------------------------------------- 
-----------------------------------------------------*/
// 本次修改后代码
//-----------------------------------------------------


void CSemCtrlInitializer::AutoInitSemCtrl_CPS()
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	// HV
	{
//		ClassID.lVal = SemControlPanelClass_CPS_HV;
		ClassID.lVal = SemControlPanelClass_HV;
		Flag.lVal = 1;
		Xpos.lVal = 0;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CSemCtrlInitializer::AutoInitSemCtrl_MCI()
{
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
	CSemCtrl::Instance().m_SemScan.USB_SetParams( 32, CControlLayer::Instance().m_BrdMCI.nDetPair );
	for(int i=0; i<3; i++ )
		CSemCtrl::Instance().m_SemScan.USB_SetParams( 33+i, CControlLayer::Instance().m_BrdMCI.nFilter[i] );

	VARIANT ClassID, SerialNumber, Flag;
	SerialNumber.lVal = -1;
	CSemCtrl::Instance().USB_WriteToBoard( USB_DetPairSelection, TRUE, CControlLayer::Instance().m_BrdMCI.nDetPair );
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_DetPair, CControlLayer::Instance().m_BrdMCI.nDetPair );
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_Det2nd, CControlLayer::Instance().m_BrdMCI.bDet2nd );
	CSemCoreAssemble::Instance().m_Signal2Brightness.SetParams( SET_PARAMS_DetPair, CControlLayer::Instance().m_BrdMCI.nDetPair );

	VARIANT		Brightness;
	Brightness.lVal		= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nBrightness, 0 );
	// Brightness
	{
		ClassID.lVal	= SemControlPanelClass_Brightness;
		Flag.lVal		= 1;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Brightness, Brightness, Flag );
	}
	VARIANT		Rotate;
	Rotate.lVal			= 0;
	// Rotate
	{
		ClassID.lVal	= SemControlPanelClass_ScanRotate;
		Flag.lVal		= 1;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Rotate, Rotate, Flag );
	}

	// Gain
	for(int i=0; i<6; i++ )
	{
		wValue = (WORD)( (i << 8) | (BYTE)(CControlLayer::Instance().m_BrdMCI.nGain[i]) );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );
	}

	// Filter
	for(int i=0; i<3; i++ )
	{
		wValue = (WORD)( (i << 8) | (BYTE)(CControlLayer::Instance().m_BrdMCI.nFilter[i]) );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
	}

	CControlLayer::Instance().CheckDet2nd( CControlLayer::Instance().m_BrdMCI.bDet2nd );
	CSemVirtualMCICard::Instance().SetParams( 11, CControlLayer::Instance().m_BrdMCI.nChnIndex[0] );
	CSemVirtualMCICard::Instance().SetParams( 12, CControlLayer::Instance().m_BrdMCI.nChnIndex[1] );
}

void CSemCtrlInitializer::AutoInitSemCtrl_SPR()
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag, Enabled;
	SerialNumber.lVal = -1;

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		Flag.lVal = 1;
		Xpos.lVal = 0;
		Ypos= Xpos;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		Sleep(100);
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	}
	else
	{
		ClassID.lVal = SemControlPanelClass_SP_PMT;
		Flag.lVal = 1;
		Xpos.lVal = 0;
		Ypos = Xpos;
		CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
		if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				pMainFrame->m_wndImpPanelAll2.ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
			else
				pMainFrame->m_wndImpPanelAll2.ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
		}

		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	}
	//18.03.06 让HVon B始终为开
// 	wData		= 0x8C;		// Strobe IOX output HVonB high
// 	nWsize		= 8;
// 	nModID		= USB_MID_SPR;
// 	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
//	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	CBoardComm::Instance().BoardWrite0816( 8, USB_MID_SPR, USB_CID_SPR_ExpanderChip, 0x8C );
	Sleep(10);
	CBoardComm::Instance().BoardWrite0816( 8, USB_MID_SPR, USB_CID_SPR_ExpanderChip, 0x8C );
	Sleep(10);
	CBoardComm::Instance().BoardWrite0816( 8, USB_MID_SPR, USB_CID_SPR_ExpanderChip, 0x8C );
	theApp.DisplayMsg( "Strobe IOX output HVonB high 3 times, 10ms elapse" );

	//2020.10 实验
	// 初始化时将光电倍增管PMT的电压设置为0V, pos = 0xFFFF
	CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
	if( pMainFrame && pMainFrame->m_dlgDebugR.m_hWnd != NULL )
	{
		pMainFrame->m_dlgDebugR.m_scrollSPRDACsVal.SetScrollPos( 32767 );
		pMainFrame->m_dlgDebugR.SPRDACsValUpdae( 32767 );
		pMainFrame->m_dlgDebugR.SPRDACupdate();
	}
}

void CSemCtrlInitializer::AutoInitSemCtrl_LNS()
{
	VARIANT		BeemAlignmentX;
	VARIANT		BeemAlignmentY;
	BeemAlignmentX.lVal	= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nBeemAlignmentX, 0 );
	BeemAlignmentY.lVal	= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nBeemAlignmentY, 0 );

	VARIANT		CondenserLens;
	VARIANT		ObjectiveLens;
	CondenserLens.lVal	= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nCondenserLens, 0x8000 );
	ObjectiveLens.lVal	= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nObjectiveLens, 0x8000 );

	/////////////////////////////////////////////////////////////////
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	// BeemAlignment
	{
		ClassID.lVal = SemControlPanelClass_BeemAlignment;
		Flag.lVal = 3;
		Xpos = BeemAlignmentX;
		Ypos = BeemAlignmentY;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}

	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_CondenserLens;
		Flag.lVal = 1;
		Xpos = CondenserLens;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}

	// ObjectiveLens
	{
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Flag.lVal = 1;
		Xpos.dblVal = ObjectiveLens.lVal;
		Xpos.lVal = ObjectiveLens.lVal;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	//	Ypos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	//	SetupDFocus( Ypos.dblVal );
	theApp.SetupDFocus( 10000 );

	// Wobbler设置中间值0x0800
	CSemCtrl::Instance().USB_CommToBoards2(USB_CID_LNS_WobblerSetting,
											USB_TM_LNS_Write_16,
											0x01,
											0x08 );

	theApp.DisplayMsg( "Wobbler compensation ON" );
	// Wobbler补偿开关始终打开,是否增加本语句需要测试？
	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x2A as low byte (L2 compensation disable ctl.)
	CSemCtrl::Instance().USB_CommToBoards2(USB_CID_LNS_ObjectiveLens_Wobbler, // Serial Comm operand low byte (L2 compensation disable ctl.)
											USB_TM_LNS_Write_08,			// Serial Comm operand high byte (8 bit write to LNS board)
											0x01,							// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
											0x01 );							// Hi byte operand is what is written during 8 bit write
}



void CSemCtrlInitializer::AutoInitSemCtrl_SCN( BOOL bPC9Reset )
{
	VARIANT		StigmatorX;
	VARIANT		StigmatorY;
	VARIANT		Magnifier;

	StigmatorX.lVal		= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nStigmatorX, 0 );
	StigmatorY.lVal		= ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nStigmatorY, 0 );

	Magnifier.dblVal		= atof( theApp.GetProfileString( g_csSettingSection, g_csRegEntry_nMagnifier, "150" ) );

	// 上次退出程序时在最低放大倍数档位的一律设置为500倍
	double dRateCtrl2	= 0.0;
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 11, dRateCtrl2 );
	if( Magnifier.dblVal < dRateCtrl2 )
		Magnifier.dblVal = 500;

	/////////////////////////////////////////////////////////////////
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	// Stig Ctrl switch open
	BYTE bytePos = 0x80 | 0x13;		// 19
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	Sleep( 100 );
	bytePos = 0x80 | 0x1A;			// 26
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	Sleep( 100 );
	bytePos = 0x80 | 0x17;			// 23
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	Sleep( 100 );
	bytePos = 0x80 | 0x1E;			// 30
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	Sleep( 100 );
	bytePos = 0x40 | 0x12;			// 18
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	Sleep( 100 );
	bytePos = 0x40 | 0x19;			// 25
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	Sleep( 1000 );
	// Stigmator
	{
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal = 3;
		Xpos = StigmatorX;
		Ypos = StigmatorY;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	Sleep( 100 );

	// Magnifier
	{
		ClassID.lVal = SemControlPanelClass_Magnifier;
		Flag.lVal = 1;
		Xpos = Magnifier;
//		if( !bPC9Reset && (Xpos.dblVal != 150) )	// 05.06.22 PC9复位时不做x10处理
//			Xpos.dblVal *= 10;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	Sleep( 100 );
	CControlLayer::Instance().SyncFineMag();

	// ScanShift
//	SetupScanShift( m_Magnifier.dblVal );
	{
		ClassID.lVal = SemControlPanelClass_ScanShift;
		Flag.lVal		= 3;
		Xpos.lVal		= 0;
		Ypos.lVal		= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	Sleep( 5 );
	/////////////////////////////////////////////////////////////////
}

void CSemCtrlInitializer::AutoSaveSemCtrl()
{
	VARIANT		BeemAlignmentX;
	VARIANT		BeemAlignmentY;
	VARIANT		StigmatorX;
	VARIANT		StigmatorY;
	VARIANT		CondenserLens;
	VARIANT		ObjectiveLens;
	VARIANT		Magnifier;
	VARIANT		Brightness;

	/////////////////////////////////////////////////////////////////
	BeemAlignmentX	= CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
	BeemAlignmentY	= CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();

	StigmatorX		= CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
	StigmatorY		= CSemCoreAssemble::Instance().m_StigmatorY.GetPos();

	CondenserLens	= CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
	ObjectiveLens	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();

	Magnifier		= CSemCoreAssemble::Instance().m_Magnifier.GetPos();

	Brightness		= CSemCoreAssemble::Instance().m_Brightness.GetPos();
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nBeemAlignmentX, BeemAlignmentX.lVal );
	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nBeemAlignmentY, BeemAlignmentY.lVal );

	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nStigmatorX, StigmatorX.lVal );
	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nStigmatorY, StigmatorY.lVal );

	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nCondenserLens, (UINT)(CondenserLens.lVal) );
	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nObjectiveLens, (UINT)(ObjectiveLens.lVal) );

	//	WriteProfileInt( g_csSettingSection, 
	//		g_csRegEntry_nMagnifier, (UINT)(m_Magnifier.dblVal) );
	CString str;
	str.Format( "%f", Magnifier.dblVal );
	theApp.WriteProfileString( g_csSettingSection,
		g_csRegEntry_nMagnifier, str );

	theApp.WriteProfileInt( g_csSettingSection,
		g_csRegEntry_nBrightness, (UINT)(Brightness.lVal) );
	/////////////////////////////////////////////////////////////////
}


DWORD CSemCtrlInitializer::ReadSettingFromRegistry(const CString& sSettingSection, const CString& sRegEntry, const DWORD dDefaultValue)
{
	DWORD dwValue = 0;
	DWORD dwSize = sizeof(DWORD);
	CString strTitle, strSubKey;
	strTitle.LoadString( AFX_IDS_APP_TITLE );
	strSubKey.Format( "Software\\KYKY\\%s\\%s", strTitle, sSettingSection );
	HKEY hk;
	if( RegOpenKeyEx( HKEY_CURRENT_USER, (LPCTSTR)strSubKey,0, KEY_ALL_ACCESS, &hk ) != ERROR_SUCCESS )
	{
		return 0;
	}

	if( RegQueryValueEx( hk, (LPCTSTR)sRegEntry, NULL, NULL, (LPBYTE)&dwValue, &dwSize) != ERROR_SUCCESS )
	{
		return 0;
	}

	return dwValue;
}
//-----------------------------------------------------
//======2004-6-2_自动保存并更新控制量==================

