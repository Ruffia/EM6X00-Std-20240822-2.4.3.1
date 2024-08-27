#include "stdafx.h"
#include <math.h>
#include "VACDataManager.h"
#include "SMvac.h"
#include "SemCtrl.h"
#include "CFGFileManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "SemCtrlPanelManager.h"
#include "CommonFunctionManager.h"
#include "DebugMessageDispatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CVACDataManager& CVACDataManager::Instance()
{
	static CVACDataManager Inst;
	return Inst;
}

CVACDataManager::CVACDataManager()
{
	// 各真空参数Reset
	m_VacParamMain.bPower	= FALSE;
	m_VacParamMain.bChamb	= FALSE;
	m_VacParamMain.bGun		= FALSE;

	m_VacParamMain.dwTCs	= 0xFFFF;
	m_VacParamMain.dwExtSwi	= 0x0000;

	m_VacParamMain.bTCf		= FALSE;
	m_VacParamMain.bTCc		= FALSE;

	m_VacParamMain.bChambEn	= FALSE;
	m_VacParamMain.bGunEn	= FALSE;

	m_VacParamMain.bVf		= FALSE;
	m_VacParamMain.bVc		= FALSE;
	m_VacParamMain.bVa		= FALSE;
	m_VacParamMain.bV1		= FALSE;

	m_VacParamMain.bTMP		= FALSE;
	m_VacParamMain.bMP		= FALSE;
	m_VacParamMain.bHVInt	= FALSE;

	m_VacTCAmp.dTCfAmp		= 0;
	m_VacTCAmp.dTCcAmp		= 0;
	m_VacTCAmp.nState		= 0;
	m_VacTCAmp.dTCfMax		= 1.70;
	m_VacTCAmp.dTCfMax		= 1.65;
	m_VacTCAmp.dTCfMin		= 0.0;
	m_VacTCAmp.dTCcMin		= 0.0;
}


BOOL  CVACDataManager::GetHVInt()
{
	// 什么时候可以加高压
	BOOL bCheckVAC = CheckVACAndCCG();
	if( bCheckVAC )
	{
		m_VacParamMain.bHVInt	= TRUE;
		return TRUE;
	}
	else
	{
		m_VacParamMain.bHVInt	= FALSE;
		return FALSE;
	}
}


BOOL  CVACDataManager::CheckVAC()
{
	if(m_VacParamMain.bMP &&m_VacParamMain.bV1
		&&m_VacParamMain.bTCf &&m_VacParamMain.bTCc
		&&m_VacParamMain.bVf &&m_VacParamMain.bVc &&m_VacParamMain.bTMP
		)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL  CVACDataManager::CheckVACAndCCG()
{
	if(m_VacParamMain.bMP &&m_VacParamMain.bV1
		&&m_VacParamMain.bTCf &&m_VacParamMain.bTCc
		&&m_VacParamMain.bVf &&m_VacParamMain.bVc &&m_VacParamMain.bTMP
		&& ( !m_cfgVacCCG.bInInterlock ||
		(m_cfgVacCCG.bInInterlock && (m_cfgVacCCG.dVac < m_cfgVacCCG.dThreshold1) ) ) )
	{
		return TRUE;
	}

	return FALSE;
}


void CVACDataManager::CHECKVacuumTCc( int nCheckf, int nCheckc ) 
{
	if( CSMvac::Instance().isReady() && m_VacParamMain.bPower )
	{
		if( nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9FAF );
		else if( nCheckf && !nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F6F );
		else if( !nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F9F );
		else
			CSMvac::Instance().SetParams( 12, 0x9F5F );
	}
	else
	{
		m_VacParamMain.bTCc = (BOOL)nCheckc;
	}
}


void CVACDataManager::CHECKVacuumTCf( int nCheckf, int nCheckc ) 
{
	if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
	{
		if( nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9FAF );
		else if( nCheckf && !nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F6F );
		else if( !nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F9F );
		else
			CSMvac::Instance().SetParams( 12, 0x9F5F );
	}
	else
	{
		m_VacParamMain.bTCf = (BOOL)nCheckf;
	}
}


void CVACDataManager::CHECKFPGArelayEn4( int nCheck ) 
{
	// 放气阀
	if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
	{
		if( nCheck )
			CSMvac::Instance().SetParams( VACIndex_Va, 1 );
		else
			CSMvac::Instance().SetParams( VACIndex_Va, 0 );
	}
	else
	{
		m_VacParamMain.bVa = (BOOL)nCheck;
	}
}


void CVACDataManager::CHECKFPGArelayEn56( int nCheck ) 
{
	// 分子泵
	if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
	{
		if( nCheck )
			CSMvac::Instance().SetParams( VACIndex_TMP, 1 );
		else
			CSMvac::Instance().SetParams( VACIndex_TMP, 0 );
	}
	else
	{
		m_VacParamMain.bTMP = (BOOL)nCheck;
	}
}


void CVACDataManager::CHECKFPGArelayEn2( int nCheck ) 
{
	// 枪阀
	if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
	{
		if( nCheck )
			CSMvac::Instance().SetParams( VACIndex_Vc, 1 );
		else
			CSMvac::Instance().SetParams( VACIndex_Vc, 0 );
	}
	else
	{
		m_VacParamMain.bVc = (BOOL)nCheck;
	}
}


void CVACDataManager::CHECKFPGArelayEn1( int nCheck ) 
{
	// 前级阀
	if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
	{
		if( nCheck )
			CSMvac::Instance().SetParams( VACIndex_Vf, 1 );
		else
			CSMvac::Instance().SetParams( VACIndex_Vf, 0 );
	}
	else
	{
		m_VacParamMain.bVf = (BOOL)nCheck;
	}
}


void CVACDataManager::CHECKV1status( BOOL bV1 ) 
{
	if( bV1 )
	{
		if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
			CSMvac::Instance().SetParams( 13, 1 );
		else
		{
			m_VacParamMain.bV1 = TRUE;
		}
	}
	else
	{
		if( CSMvac::Instance().isReady() &&m_VacParamMain.bPower )
			CSMvac::Instance().SetParams( 13, 0 );
		else
		{
			m_VacParamMain.bV1 = FALSE;
		}
	}
}


void CVACDataManager::SetParam_Power() 
{
	///////////////////////////////////////////////////////
	// 通过记录看是否上次退出时真空电源是打开的
	if(m_VacParamMain.bPower )
	{
		CSMvac::Instance().SetParams( VACIndex_Power, 1 );
		CSMvac::Instance().SetParams( VACIndex_MP, 1 );
	}
	else
	{
		CSMvac::Instance().SetParams( VACIndex_Power, 0 );
	}
}


void CVACDataManager::SetParam_Chamb() 
{
	if(m_VacParamMain.bChamb )
	{
		CSMvac::Instance().SetParams( VACIndex_Chamb, 1 );
	}
	else
	{
		CSMvac::Instance().SetParams( VACIndex_Chamb, 0 );
	}
}


void CVACDataManager::SetParam_Gun() 
{
	if(m_VacParamMain.bGun )
	{
		CSMvac::Instance().SetParams( VACIndex_Gun, 1 );
	}
	else
	{
		CSMvac::Instance().SetParams( VACIndex_Gun, 0 );
	}
}


BOOL CVACDataManager::CheckVACConnection(CWnd* pWnd) 
{
	BOOL bRet = FALSE;
	// 真正连接下位机时，V1的状态要靠回读
	BOOL bUSBReady = CSemCtrl::Instance().USB_isReady();
	if( bUSBReady )
	{
		// 使用Read Board ID功能来判断VAC板是否联通
		unsigned short nRegister = 0;
		int nCount = 0;
		CString str;
		do
		{
			CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_ReadBoardID, &nRegister );
			nCount++;
			if( nRegister == 0x0 )
				CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		} while( nRegister == 0x0 && nCount < 50 );
		//str.Format( GetResString(IDF_SD_PROMPT_VAC_29), nCount, nRegister );
		//theApp.DisplayMsg( str );
		if( nRegister == 0xA3 )
		{
			// 真实情况，非调试模式
			CSMvac::Instance().Reset( pWnd );
			CSMvac::Instance().SetParams(20, 0);
			CSMvac::Instance().SetParams(22, 0);
			CSMvac::Instance().SetParams(23, m_dwV1CtrlMode > 0);
			bRet = TRUE;
		}
	}

	if( !bRet )
	{
		// 调试模式/未连下位机
		CSMvac::Instance().SetParams(20, 1);
		CSMvac::Instance().SetParams(23, m_dwV1CtrlMode > 0);
	}

	return bRet;
}


void CVACDataManager::VAC_Reset(const USB_ScanType scanType ) 
{
	// 关高压、灯丝，对比度退0
	// 对比度
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
	ClassID.lVal		= SemControlPanelClass_Contrast;
	SerialNumber.lVal	= -1;
	Flag.lVal			= 1;
	Xpos.lVal			= 0;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
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
	if( scanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
}

void CVACDataManager::CFGfile_InitVacParams( char* path ) 
{
	char		szBuf[20], szBuf1[20];
	char		stabuff[20], staitembuff[20];

	int			nTotal	= 7;
	int*		pCheck	= new int[nTotal];
	sprintf(stabuff, "VACParam");
	// 阀状态
	sprintf(staitembuff, "VStates");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck) == nTotal )
	{
		//		for( int i=0; i<nTotal; i++ )
		//			m_BrdVAC.bStates[i] = pCheck[i];
		m_VacParamMain.bPower	= pCheck[0];
		m_VacParamMain.bChamb	= pCheck[1];
		m_VacParamMain.bGun		= pCheck[2];
		m_VacParamMain.bVf		= pCheck[3];
		m_VacParamMain.bVc		= pCheck[4];
		m_VacParamMain.bTMP		= pCheck[5];
		m_VacParamMain.bVa		= pCheck[6];
	}
	delete [] pCheck;

	// TC Amp的最大值
	if(GetPrivateProfileString(stabuff, "Max", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是当前探测器组的第二通道的增益值
			*strchr(szBuf, ',') = '\0';				// szBuf中是当前探测器组的第一通道的增益值
			m_VacTCAmp.dTCfMax	= atof( szBuf );
			m_VacTCAmp.dTCcMax	= atof( szBuf1 );
		}
	}
	// TC Amp的最小值
	if(GetPrivateProfileString(stabuff, "Min", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是当前探测器组的第二通道的增益值
			*strchr(szBuf, ',') = '\0';				// szBuf中是当前探测器组的第一通道的增益值
			m_VacTCAmp.dTCfMin	= atof( szBuf );
			m_VacTCAmp.dTCcMin	= atof( szBuf1 );
		}
	}

}


void CVACDataManager::CFGfile_InitCCGParams( char* path ) 
{
	char		szBuf[20], szBuf1[20];
	char		stabuff[20] = {0};
	sprintf(stabuff, "VACParam");

	// CCG端口
	//	m_VacCCG.SetParam( 3, m_cfgVacCCG.nPort );	// COM 3 (nPort = 2)
	m_nCCGconnects = 0;

	// CCG阈值
	if(GetPrivateProfileString(stabuff, "Thresholds", "", szBuf, 18, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是阈值2
			*strchr(szBuf, ',') = '\0';				// szBuf中是阈值1
			m_cfgVacCCG.dThreshold1 = atof( szBuf );	//ScientificToDouble(str);
			m_cfgVacCCG.dThreshold2 = atof( szBuf1 );	//ScientificToDouble(str);
			CString str;
			str.Format( "Read from ini: Threshold Upper = %.2e, Threshold Lower = %.2e", m_cfgVacCCG.dThreshold1, m_cfgVacCCG.dThreshold2 );
			CDebugMessageDispatcher::Instance().DisplayMsg( str );
		}
	}
	m_cfgVacCCG.bInInterlock = (BOOL)(GetPrivateProfileInt( stabuff, "InInterlock", 0, path ));
	// 真空计类型：0为INFICON，1为WPC
	if(GetPrivateProfileString(stabuff, "GaugeType", "", szBuf, 10, path) != 0)
		m_cfgVacCCG.nType = 3 +atoi(szBuf);	// 3为INFICON，4为WPC
}


void CVACDataManager::LoadVACFactor( char * path ) 
{
	const int nTotal = 4;
	double* pFactors = new double[nTotal];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "VACParam", "Factors", DOUBLESTYLE, nTotal, pFactors) == nTotal )
	{
		// 电压阈值（即为真空度阈值）
		double* pThresholds	= new double[nTotal];
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, "VACParam", "Thresholds2", DOUBLESTYLE, nTotal, pThresholds) == nTotal )
		{
			// Thresholds_AmpVoltage = A * (Vacuum ^ B)
			CSMvac::Instance().SetParams2( 1, pFactors[0] * pow(pThresholds[0], pFactors[1]) );
			CSMvac::Instance().SetParams2( 2, pFactors[0] * pow(pThresholds[1], pFactors[1]) );
			CSMvac::Instance().SetParams2( 3, pFactors[2] * pow(pThresholds[2], pFactors[3]) );
			CSMvac::Instance().SetParams2( 4, pFactors[2] * pow(pThresholds[3], pFactors[3]) );
		}
		delete [] pThresholds;
	}
	delete [] pFactors;
	// 以何种方式开启TMP：0为TC翻转值；1为电压值（对应即为真空度）
	CSMvac::Instance().SetParams2( 5, GetPrivateProfileInt( "VACParam", "VacSrc", 0, path ) );
}