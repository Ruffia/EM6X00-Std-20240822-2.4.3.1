#include "stdafx.h"
#include "HVPowerDataManager.h"
#include "DebugMessageDispatcher.h"
#include "CCPS3603tatusDetector.h"
#include "SpellmanStatusDetector.h"
#include "SemCtrl.h"
#include "GlobalParamSetting.h"
#include "SemDataType.h"
#include "SemCtrlPanelManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "VACDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CHVPowerDataManager& CHVPowerDataManager::Instance()
{
	static CHVPowerDataManager Inst;
	return Inst;
}

CHVPowerDataManager::CHVPowerDataManager()
{
	_InitSpellmanParamater();
}

void CHVPowerDataManager::_InitSpellmanParamater()
{
	m_Spellmans.status.bstatusSwitch	= 0x00;
	m_Spellmans.status.dwstatusRegister	= 0xFFFFFFFF;
	m_Spellmans.status.bReady		= FALSE;
	m_Spellmans.status.bCollector	= FALSE;
	m_Spellmans.status.bScint		= FALSE;
	m_Spellmans.status.bEHT		= FALSE;
	m_Spellmans.status.bHeater	= FALSE;
	m_Spellmans.status.bPMT		= FALSE;

	m_Spellmans.pS.dVC			= 0.0;
	m_Spellmans.pS.dVE			= 0.0;
	m_Spellmans.pS.dIB			= 0.0;
	m_Spellmans.pS.dIH			= 0.0;
	m_Spellmans.pS.dVP			= 0.0;

	m_Spellmans.pM.dVC			= 0.0;
	m_Spellmans.pM.dVE			= 0.0;
	m_Spellmans.pM.dIB			= 0.0;
	m_Spellmans.pM.dIH			= 0.0;
	m_Spellmans.pM.dVP			= 0.0;
	m_Spellmans.pM.dII			= 0.0;

	m_Spellmans.pM2.dVC			= 0.0;
	m_Spellmans.pM2.dVE			= 0.0;
	m_Spellmans.pM2.dIB			= 0.0;
	m_Spellmans.pM2.dIH			= 0.0;
	m_Spellmans.pM2.dVP			= 0.0;
}

void  CHVPowerDataManager::ResetSpellmanDebugParam()
{
	m_Spellmans.status.bReady		= FALSE;
	m_Spellmans.status.bCollector	= FALSE;
	m_Spellmans.status.bScint		= FALSE;
	m_Spellmans.status.bEHT			= FALSE;
	m_Spellmans.status.bHeater		= FALSE;
	m_Spellmans.status.bPMT			= FALSE;

	m_Spellmans.pS.dVC				= 0.0;
	m_Spellmans.pS.dVE				= 0.0;
	m_Spellmans.pS.dIB				= 0.0;
	m_Spellmans.pS.dIH				= 0.0;
	m_Spellmans.pS.dVP				= 0.0;

	m_Spellmans.pM.dVC				= 0.0;
	m_Spellmans.pM.dVE				= 0.0;
	m_Spellmans.pM.dIB				= 0.0;
	m_Spellmans.pM.dIH				= 0.0;
	m_Spellmans.pM.dVP				= 0.0;
	m_Spellmans.pM.dII				= 0.0;

	m_Spellmans.pM2.dVC			= 0.0;
	m_Spellmans.pM2.dVE			= 0.0;
	m_Spellmans.pM2.dIB			= 0.0;
	m_Spellmans.pM2.dIH			= 0.0;
	m_Spellmans.pM2.dVP			= 0.0;
}


void CHVPowerDataManager::ResetCPS3603Param()
{
	m_CPS3603.dVoutA = m_CPS3603.dIoutA = 0.0;
	m_CPS3603.dVoutE = m_CPS3603.dIoutE = 0.0;
	m_CPS3603.dVoutF = m_CPS3603.dIoutF = 0.0;
	m_CPS3603.dRf = m_CPS3603.dPf = m_CPS3603.dTf = 0.0;
	m_CPS3603.dTa = m_CPS3603.dThv = 0.0;
	m_CPS3603.dCoronalevel = 0.0;
}


void CHVPowerDataManager::ResetCPS3603Status()
{
	m_CPS3603s.nConnectA = m_CPS3603s.nConnectE = m_CPS3603s.nConnectF = 0;
	m_CPS3603s.nVerA = m_CPS3603s.nVerE = m_CPS3603s.nVerF = 0;
	m_CPS3603s.nPassiveBias		= 0;
	m_CPS3603s.nVoltageControlF	= 0;
	m_CPS3603s.nExtraParams		= 0;
	m_CPS3603s.nResistorControl	= 0;
	m_CPS3603s.nSeparatedFeedback	= 0;
}


void CHVPowerDataManager::HVPower_SetEnable_HVType( int nCheck, int nID, USB_ScanType eScanType, BOOL blOverhaulState ) 
{
	VARIANT Xpos, Ypos, Enable, Flag, ClassID, SerialNumber;
	CString strX, strY;
	Xpos.lVal = 0;
	Enable.lVal	= nCheck;

	if( m_dwHVtype < 1 )
	{
		// CPS3603
		switch( nID )
		{
		case	5:
			if( nCheck )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set HV Enable" );
			}
			else
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set HV to 0 and disable" );
				CCPS3603tatusDetector::Instance().SetParam( 10, 0 );
				Flag.ulVal	= (DWORD)0x01;

				ClassID.lVal		= SemControlPanelClass_CPS_HV;
				SerialNumber.lVal	= -1;
				Flag.lVal			= 1;
				Xpos.lVal			= 0;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
				if( eScanType != USB_ST_Stop )
					CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
				m_dHV = 0.0;
				CGlobalParamSetting::Instance().g_dHV = 0.0;

				strX = "0.0 KV";
			}
			Flag.lVal	= 3;
			break;

		case	6:
			if( nCheck )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Bias Enable" );
			}
			else
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Bias to 0" );
				CCPS3603tatusDetector::Instance().SetParam( 11, 0 );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0.0 V";
				CGlobalParamSetting::Instance().g_dHV2 = 0.0;
			}
			Flag.lVal	= 3;
			break;

		case	7:
			if( nCheck )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Fila Enable" );
			}
			else
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Fila to 0" );
				CCPS3603tatusDetector::Instance().SetParam( 12, 0 );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0.00 A";
				CGlobalParamSetting::Instance().g_dHV3 = 0.0;
			}
			Flag.lVal	= 3;
			break;

		default:
			break;
		}

		if( blOverhaulState )
			CCPS3603tatusDetector::Instance().SetParam( nID, 0 );
		else
			CCPS3603tatusDetector::Instance().SetParam( nID, nCheck );
	}
	else
	{
		// Spellman EBM30N12/771
		switch( nID )
		{
		case	5:
			if( nCheck )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set HV Enable" );
			}
			else
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set HV to 0 and disable" );
				CSpellmanStatusDetector::Instance().SetParam( 32, 0 );
				Flag.ulVal	= (DWORD)0x01;

				ClassID.lVal		= SemControlPanelClass_SP_HV;
				SerialNumber.lVal	= -1;
				Flag.lVal			= 1;
				Xpos.lVal			= 0;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
				if( eScanType != USB_ST_Stop )
					CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
				m_Spellmans.pS.dVE = 0.0;
				CGlobalParamSetting::Instance().g_Spellmans.pS.dVE = 0.0;
				m_dHV = 0.0;

				strX = "0.0 KV";
			}
			Flag.lVal	= 3;
			if( blOverhaulState )
				CSpellmanStatusDetector::Instance().SetParam( 23, 0 );
			else
				CSpellmanStatusDetector::Instance().SetParam( 23, nCheck );
			break;

		case	6:
			if( nCheck )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Bias Enable" );
			}
			else
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Bias to 0" );
				CSpellmanStatusDetector::Instance().SetParam( 33, 0 );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0 uA";
				m_Spellmans.pS.dIB = 0.0;
				CGlobalParamSetting::Instance().g_Spellmans.pS.dIB = 0.0;
			}
			Flag.lVal	= 3;
			break;

		case	7:
			if( nCheck )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Fila Enable" );
			}
			else
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "Set Fila to 0" );
				CSpellmanStatusDetector::Instance().SetParam( 34, 0 );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0.00 A";
				m_Spellmans.pS.dIH = 0.0;
				CGlobalParamSetting::Instance().g_Spellmans.pS.dIH = 0.0;
			}
			Flag.lVal	= 3;
			if( blOverhaulState )
				CSpellmanStatusDetector::Instance().SetParam( 24, 0 );
			else
				CSpellmanStatusDetector::Instance().SetParam( 24, nCheck );

			break;

		default:
			break;
		}
	}
}


void CHVPowerDataManager::VAC_Reset_ByHVType( const USB_ScanType scanType ) 
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
	if( m_dwHVtype < 1 )
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		SerialNumber.lVal	= -1;
		Flag.lVal = 1;
		Xpos.lVal = 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		Sleep(100);
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );

		ClassID.lVal		= SemControlPanelClass_Contrast;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

		// HV to 0
		ClassID.lVal		= SemControlPanelClass_CPS_HV;
		Flag.lVal			= 1;
		Xpos.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		ClassID.lVal		= SemControlPanelClass_CPS_HV;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	}
	else
	{
		ClassID.lVal = SemControlPanelClass_SP_PMT;
		SerialNumber.lVal	= -1;
		Flag.lVal = 1;
		Xpos.lVal = 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		CString strX = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();

		CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal );

		ClassID.lVal		= SemControlPanelClass_SP_PMT;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

		// HV to 0
		ClassID.lVal		= SemControlPanelClass_SP_HV;
		Flag.lVal			= 1;
		Xpos.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		ClassID.lVal		= SemControlPanelClass_SP_HV;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	}

	if( scanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
	m_dHV = CGlobalParamSetting::Instance().g_dHV = CGlobalParamSetting::Instance().g_dHV2 = CGlobalParamSetting::Instance().g_dHV3 = 0.0;
}


void CHVPowerDataManager::HVPower_Init(CWnd* pWnd) 
{
	if( CCPS3603tatusDetector::Instance().IsReady() )
	{
		CCPS3603tatusDetector::Instance().Release();
	}

	ResetCPS3603Status();

	CCPS3603tatusDetector::Instance().Init( pWnd->m_hWnd, m_CPS3603s.nPort );
	CCPS3603tatusDetector::Instance().Start();

	ResetCPS3603Param();
}


void  CHVPowerDataManager::HVPower_Init_ByHVType(CWnd* pWnd)
{
	char sz[512] = {0};
	sprintf_s(sz,512,"void CHVPowerDataManager::HVPower_Init_ByHVType(CWnd* pWnd),m_dwHVtype = %d  \r\n",m_dwHVtype );
	OutputDebugString(sz);

	if( m_dwHVtype < 1 )
	{
		CCPS3603tatusDetector::Instance().Release();
	}
	else
	{
		CSpellmanStatusDetector::Instance().Release();
	}

	if( m_dwHVtype < 1 )
	{
		ResetCPS3603Status();
		CCPS3603tatusDetector::Instance().Init(pWnd->m_hWnd,m_CPS3603s.nPort);
		CCPS3603tatusDetector::Instance().Start();
		ResetCPS3603Param();
		CGlobalParamSetting::Instance().ResetCPS3603Param();
	}
	else
	{
		ResetSpellmanDebugParam();
		CSpellmanStatusDetector::Instance().Init( pWnd->m_hWnd );
		CSpellmanStatusDetector::Instance().Start();
		CGlobalParamSetting::Instance().ResetSpellmanDebugParam();
	}
}

CString CHVPowerDataManager::HVPower_UpdateHVdisp()
{
	CString csHV;
	csHV.Format( "%.1f KV", m_dHV );
	return csHV;
}

void CHVPowerDataManager::HVPower_SetEnable( int nID, int nCheck, const USB_ScanType scanType, BOOL blOverhaulState ) 
{
	VARIANT Xpos, Ypos, Flag, ClassID, SerialNumber;
	CString strX, strY;

	Xpos.lVal = 0;

	switch( nID )
	{
	case	5:
		if( nCheck )
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "Set HV Enable" );
		}
		else
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "Set HV to 0 and disable" );
			CCPS3603tatusDetector::Instance().SetParam( 10, 0 );
			Flag.ulVal	= (DWORD)0x01;

			ClassID.lVal		= SemControlPanelClass_CPS_HV;
			SerialNumber.lVal	= -1;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

			if( scanType != USB_ST_Stop )
				CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );

			strX = "0.0 KV";
		}
		Flag.lVal	= 3;
		break;

	case	6:
		if( nCheck )
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "Set Bias Enable" );
		}
		else
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "Set Bias to 0" );
			CCPS3603tatusDetector::Instance().SetParam( 11, 0 );
			Flag.ulVal	= (DWORD)0x01;
			strX = "0.0 V";
		}
		Flag.lVal	= 3;
		break;

	case	7:
		if( nCheck )
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "Set Fila Enable" );
		}
		else
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "Set Fila to 0" );
			CCPS3603tatusDetector::Instance().SetParam( 12, 0 );
			Flag.ulVal	= (DWORD)0x01;
			strX = "0.000 A";
		}
		Flag.lVal	= 3;
		break;

	default:
		break;
	}

	if( blOverhaulState )
		CCPS3603tatusDetector::Instance().SetParam( nID, 0 );
	else
		CCPS3603tatusDetector::Instance().SetParam( nID, nCheck );
}



void CHVPowerDataManager::HV_UpdateStatusSwitches(const BYTE bSSbak ) 
{
	BYTE bS = m_Spellmans.status.bstatusSwitch;
	if( (bS & 0x10) != (bSSbak & 0x10) )
	{
		if( bS & 0x10 )
		{
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
			SerialNumber.lVal	= -1;
			// Contrast to 0
			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );

			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			Enabled.lVal		= 1;
			Flag.lVal			= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
			CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal );	// PMT
		}
		else
		{
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
			SerialNumber.lVal	= -1;
			// Contrast to 0
			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );

			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			Enabled.lVal		= 0;
			Flag.lVal			= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
			CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal );	// PMT
		}
	}
}


void CHVPowerDataManager::ChangeHighVoltage_771( BOOL bNotify, VARIANT ClassID, VARIANT SerialNumber, VARIANT &Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSet, BOOL bSync, USB_ScanType eScanType ) 
{
	// bNotify: 通知所有控制面板
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	if( bNotify )
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	double dHV = 0;
	if( Xpos.lVal < 100 )
	{
		if( bSet )
			CSpellmanStatusDetector::Instance().SetParam( 32, Xpos.lVal );
		dHV = Xpos.lVal /10.0;
	}
	else
	{
		int nPos = Xpos.lVal -100 +10;
		if( bSet )
			CSpellmanStatusDetector::Instance().SetParam( 32, nPos *10 );
		dHV = nPos;
	}

	m_Spellmans.pS.dVE	= dHV;
	CGlobalParamSetting::Instance().g_Spellmans.pS.dVE	= dHV;
	m_dHV		= dHV;

	if( eScanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( dHV );
}


void CHVPowerDataManager::ChangeHighVoltage_3603( BOOL bNotify, VARIANT ClassID, VARIANT SerialNumber, VARIANT &Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSet, USB_ScanType eScanType ) 
{
	// bNotify: 通知所有控制面板
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	if( bNotify )
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	double dHV = 0;
	CString str;
	if( Xpos.lVal < 100 )
	{
		if( bSet )
			CCPS3603tatusDetector::Instance().SetParam( 10, Xpos.lVal *100 );
		dHV = Xpos.lVal /10.0;
	}
	else
	{
		int nPos = Xpos.lVal -100 +10;
		if( bSet )
			CCPS3603tatusDetector::Instance().SetParam( 10, nPos *1000 );
		dHV = nPos;
	}

	m_dHV = dHV;
	CGlobalParamSetting::Instance().g_dHV = dHV;

	if( eScanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( dHV );
}


void CHVPowerDataManager::CFGFile_InitCPS3603s( char * path ) 
{
	// 读CPS & CCG端口值
	m_CPS3603s.nPort	= 1;
	CVACDataManager::Instance().m_cfgVacCCG.nPort	= 3;
	CVACDataManager::Instance().m_cfgVacCCG.nType	= 3;
	CVACDataManager::Instance().m_portBoard.nPort	= 3;
	char szBuf[40], szBuf1[20];
	if(GetPrivateProfileString("Options", "CommPorts", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是CCG端口值
			*strchr(szBuf, ',') = '\0';				// szBuf中是CPS端口值
			m_CPS3603s.nPort	= atoi( szBuf ) -1;
			if( CSemCtrl::Instance().m_nVACBoardType < 1 )
				CVACDataManager::Instance().m_cfgVacCCG.nPort	= atoi( szBuf1 );
			else
				CVACDataManager::Instance().m_portBoard.nPort	= atoi( szBuf1 );
		}
	}
}