#include "stdafx.h"
#include <random>
#include "AppLogManager.h"
#include "GlobalParamSetting.h"
#include "VACDataManager.h"
#include "Semctrl.h"
#include "FormatConvertor.h"
#include <algorithm>
#include "Util.h"
#include "FieldMeta.h"
#include "DBDataManager.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppLogManager& CAppLogManager::Instance()
{
	static CAppLogManager Inst;
	return Inst;
}

CAppLogManager::CAppLogManager()
{
	m_hThreadLogAll = NULL;
	m_dwThreadLogAll = 0;
	m_hEventLogAll			= CreateEvent(NULL,FALSE, FALSE, _T("WORKlogall"));
	m_hEventTeminateLogAll	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATElogall"));
	m_hFileHVVAC	= NULL;
	m_nLoopsHVVAC	= 0;
	m_bLogHVVAC		= FALSE;
	m_hWnd = NULL;
}

BOOL CAppLogManager::InitLogFile()
{
	// 设置日志
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString str, strTemp = timeCurrent.Format("%Y%m%d%H%M");
	str.Format( "%s\\Log\\HVVAC-%s.csv", exeFullPath, strTemp );
	if( this->m_hFileHVVAC != NULL )
		CloseHandle( this->m_hFileHVVAC );
	this->m_hFileHVVAC	= NULL;
	this->m_hFileHVVAC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	this->m_nLoopsHVVAC = 1;
	// 写HVVAC日志文件头
	if( this->m_hFileHVVAC != NULL )
	{
		this->m_bLogHVVAC = TRUE;
		unsigned long cbRet;
		char Buf[512] = {0};
		str.Format( "\t%s\r\n", "Log HV and VAC params" );
		sprintf_s( Buf, "%s\r\n", str );
		WriteFile( this->m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );
		if(this->m_dwHVtype < 1 )
			str.Format( "HH:MM:SS,CCG,Acc set,Acc V mon,Acc I mon,Corona,Fila set,Fila I mon,Bias set,Bias V mon,Power,Chamb,Gun,Vf,Vc,TMP,Va,TCf,TCc,TCs,ExtSwi,TCfAmp,TCcAmp");
		else if(this->m_dwHVtype < 2)
			str.Format( "HH:MM:SS,CCG,Switch,Status,Col s,Col m,Col m2,EHT s,EHT m,EHT m2,Beam s,Beam m,EMI,Heat s,Heat m,Heat m2,PMT s,PMT m,PMT m2,Scint m2,Power,Chamb,Gun,Vf,Vc,TMP,Va,TCf,TCc,TCs,ExtSwi,TCfAmp,TCcAmp" );
		else if(this->m_dwHVtype > 2)
			str.Format("HH:MM:SS,sOutput,sProtectB,sProtectV,sProtectI,sProtectA,status1,status2,status3,SCI set,SCI out,SCI monv,SCI moni,COL set,COL out,COL monv,COL moni,PMT set,PMT out,PMT monv,PMT moni,ACL Vset,ACL Vout,ACL Vmonv,ACL Iset,ACL Iout,ACL Imoni,FIL set,FIL out,FIL moni,BIAS set,BIAS out,BIAS monv,P 3V3,P 5V,P 15V,N 15V,P 24V,T");
		sprintf_s( Buf, "%s\r\n", str );
		WriteFile( this->m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );
		return TRUE;
	}
	return FALSE;
}

DWORD CAppLogManager::CommThreadLogAll(LPVOID lpParam)
{
	CAppLogManager* pThis = (CAppLogManager*)lpParam;
/*	CString str, strTemp;
	// 设置日志
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");

	// 设置HVVAC日志文件
	str.Format( "%s\\Log\\HVVAC-%s.csv", exeFullPath, strTemp );
	if( pThis->m_hFileHVVAC != NULL )
		CloseHandle( pThis->m_hFileHVVAC );
	pThis->m_hFileHVVAC	= NULL;
	pThis->m_hFileHVVAC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	pThis->m_nLoopsHVVAC = 1;
	// 写HVVAC日志文件头
	if( pThis->m_hFileHVVAC != NULL )
	{
		pThis->m_bLogHVVAC = TRUE;
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "Log HV and VAC params" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( pThis->m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );
		if(pThis->m_dwHVtype < 1 )
			str.Format( "HH:MM:SS,CCG,Acc set,Acc V mon,Acc I mon,Corona,Fila set,Fila I mon,Bias set,Bias V mon,Power,Chamb,Gun,Vf,Vc,TMP,Va,TCf,TCc,TCs,ExtSwi,TCfAmp,TCcAmp");
		else if(pThis->m_dwHVtype < 2)
			str.Format( "HH:MM:SS,CCG,Switch,Status,Col s,Col m,Col m2,EHT s,EHT m,EHT m2,Beam s,Beam m,EMI,Heat s,Heat m,Heat m2,PMT s,PMT m,PMT m2,Scint m2,Power,Chamb,Gun,Vf,Vc,TMP,Va,TCf,TCc,TCs,ExtSwi,TCfAmp,TCcAmp" );
		else if(pThis->m_dwHVtype > 2)
			str.Format("HH:MM:SS,sOutput,sProtectB,sProtectV,sProtectI,sProtectA,status1,status2,status3,SCI set,SCI out,SCI monv,SCI moni,COL set,COL out,COL monv,COL moni,PMT set,PMT out,PMT monv,PMT moni,ACL Vset,ACL Vout,ACL Vmonv,ACL Iset,ACL Iout,ACL Imoni,FIL set,FIL out,FIL moni,BIAS set,BIAS out,BIAS monv,P 3V3,P 5V,P 15V,N 15V,P 24V,T");
		sprintf( Buf, "%s\r\n", str );
		WriteFile( pThis->m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );
*/
	BOOL bRet = pThis->InitLogFile();
	if( bRet )
	{
		while( WaitForSingleObject(pThis->m_hEventTeminateLogAll,250) != WAIT_OBJECT_0 )
		{
//			if( !g_bPauseComm )
			{
				// 500ms通讯一次
				pThis->LogHVVAC(CSemCtrl::Instance().m_nVACBoardType);
			}
			Sleep( 1 );
		}
	}

	if( pThis->m_hFileHVVAC != NULL )
		CloseHandle( pThis->m_hFileHVVAC );
	pThis->m_hFileHVVAC	= NULL;

	SetEvent( pThis->m_hEventLogAll );
	return 0;
}

BOOL CAppLogManager::LogHVVACInit(HWND hWnd,DWORD dwHVtype)
{
	m_hWnd = hWnd;
	m_dwHVtype = dwHVtype;
	/////////////////////////////////////////////////////////////////
	ResetEvent( m_hEventLogAll );
	ResetEvent( m_hEventTeminateLogAll );

	m_hThreadLogAll	= ::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CommThreadLogAll, (LPVOID)this, 0, &m_dwThreadLogAll );
	if( m_hThreadLogAll == NULL )
		return FALSE;

	CloseHandle(m_hThreadLogAll);
	/////////////////////////////////////////////////////////////////
	return TRUE;
}

void CAppLogManager::LogHVVACClose()
{
	if( m_bLogHVVAC )
	{
		SetEvent( m_hEventTeminateLogAll );
		WaitForSingleObject( m_hEventLogAll, INFINITE );
		m_bLogHVVAC = FALSE;
	}
}


void CAppLogManager::LogHVVAC( int nVACBoardType )
{
	unsigned long cbRet;
	char Buf[512] = {0};
	CString str;
	if( m_dwHVtype < 1 )
	{
		LogHVVAC_CPS3603(nVACBoardType, str);
	}
	else if(m_dwHVtype < 2)
	{
		LogHVVAC_SPM771(nVACBoardType, str);
	}
	else if(m_dwHVtype > 2)
	{
		LogHVVAC_WSM30KV2(nVACBoardType, str);
	}

	sprintf_s( Buf, "%s\r\n", str );
	WriteFile( m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );

	m_nLoopsHVVAC++;
	if( m_nLoopsHVVAC > 5000 )
	{
/*		CloseHandle( m_hFileHVVAC );
		// 设置日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\HVVAC-%s.csv", exeFullPath, csCurrentTime );
		m_hFileHVVAC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsHVVAC = 1;

		str.Format( "\t%s", "Log HV and VAC params" );
		sprintf( Buf, "%s\n", str );
		WriteFile( m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );
		if( m_dwHVtype < 1 )
			str.Format( "HH:MM:SS,CCG,Acc set,Acc V mon,Acc I mon,Corona,Fila set,Fila I mon,Bias set,Bias V mon,Power,Chamb,Gun,Vf,Vc,TMP,Va,TCf,TCc,TCs,ExtSwi,TCfAmp,TCcAmp");
		else if(m_dwHVtype < 2)
			str.Format( "HH:MM:SS,CCG,Switch,Status,Col s,Col m,Col m2,EHT s,EHT m,EHT m2,Beam s,Beam m,EMI,Heat s,Heat m,Heat m2,PMT s,PMT m,PMT m2,Scint m2,Power,Chamb,Gun,Vf,Vc,TMP,Va,TCf,TCc,TCs,ExtSwi,TCfAmp,TCcAmp" );	
		else if(m_dwHVtype > 2)
			str.Format("HH:MM:SS,sOutput,sProtectB,sProtectV,sProtectI,sProtectA,status1,status2,status3,SCI set,SCI out,SCI monv,SCI moni,COL set,COL out,COL monv,COL moni,PMT set,PMT out,PMT monv,PMT moni,ACL Vset,ACL Vout,ACL Vmonv,ACL Iset,ACL Iout,ACL Imoni,FIL set,FIL out,FIL moni,BIAS set,BIAS out,BIAS monv,P 3V3,P 5V,P 15V,N 15V,P 24V,T");
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileHVVAC, Buf, strlen(Buf), &cbRet, NULL );*/
		InitLogFile();
	}
}

// 11.07.19 设置所有日志
void CAppLogManager::SetupLogs( BOOL bVal )
{
	m_boolHVLog						= bVal;
	m_boolScanShiftLog				= bVal;
	m_boolBeemAlignmentLog			= bVal;
	m_boolBrightnessLog				= bVal;
	m_boolContrastLog				= bVal;
	m_boolStigmatorLog				= bVal;
	m_boolObjectiveLensLog			= bVal;
	m_boolCondenserLensLog			= bVal;
	m_boolFilamentCurrentLog		= bVal;
	m_boolMagnifierLog				= bVal;
	m_boolWorkDistanceLog			= bVal;
	m_boolObjectiveLensReverseLog	= bVal;
	m_boolCondenserLensReverseLog	= bVal;
	m_boolAuxiliaryStigmationLog	= bVal;
	m_boolDCondenserLensLog			= bVal;
	m_boolScanRotateLog				= bVal;
	m_boolSACPLog					= bVal;
	m_boolDFocusLog					= bVal;
	m_boolDFocusSwitchLog			= bVal;
	m_boolDObjectiveLensLog			= bVal;
	m_boolDObjectiveLensSwitchLog	= bVal;
	m_boolRobinsonDetectorSwitchLog	= bVal;
	m_boolSignal2BrightnessLog		= bVal;
	m_boolSignal2ContrastLog		= bVal;
	m_boolYAdjustLog				= bVal;
	m_boolImageReverseLog			= bVal;
	m_boolGunCurrentLog				= FALSE;
	m_boolGunCurrentUpdate			= FALSE;

	m_boolMsgFromMicroPCLog			= FALSE;	// 11.07.19
	///////////////////////////////////////////////////////////
	m_boolLogMsgPcMc				= FALSE;
	m_boolLogMsgRemoteGet			= TRUE;		// 22.03.11
}

void CAppLogManager::LogHVVAC_CPS3603(int nVACBoardType, CString& str)
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
//           Time  CCG Acc                  Corona Fila          Bias          Power/Chamb/Gun/Vf/Vc/TMP/Va/TCf/TCc/TCs/ExtSwi/TCfAmp/TCcAmp
	str.Format( "[%s],%.2e,%9.3lf,%9.3lf,%9.3lf,%9.3lf,%6.3lf,%6.3lf,%8.3lf,%8.3lf,%d,%d,%d,%d,%d,%d,%d,%d,%d,0x%04X,0x%04X,%.3f,%.3f",
	csCurrentTime, nVACBoardType < 1 ? CGlobalParamSetting::Instance().g_cfgVacCCG.dVac : CVACDataManager::Instance().m_valueCCG.dVac,// CCG
	CGlobalParamSetting::Instance().g_dHV*100,						// Acc set
	CGlobalParamSetting::Instance().g_CPS3603.dVoutA,				// Acc V mon
	CGlobalParamSetting::Instance().g_CPS3603.dIoutA * 1e6,			// Acc I mon
	CGlobalParamSetting::Instance().g_CPS3603.dCoronalevel,
	CGlobalParamSetting::Instance().g_dHV3,							// Fila set
	CGlobalParamSetting::Instance().g_CPS3603.dIoutF,				// Fila I mon
	CGlobalParamSetting::Instance().g_dHV2,							// Bias set
	CGlobalParamSetting::Instance().g_CPS3603.dVoutE,				// Bias V mon
	CGlobalParamSetting::Instance().g_VacParamMain.bPower,
	CGlobalParamSetting::Instance().g_VacParamMain.bChamb,
	CGlobalParamSetting::Instance().g_VacParamMain.bGun,
	CGlobalParamSetting::Instance().g_VacParamMain.bVf,
	CGlobalParamSetting::Instance().g_VacParamMain.bVc,
	CGlobalParamSetting::Instance().g_VacParamMain.bTMP,
	CGlobalParamSetting::Instance().g_VacParamMain.bVa,
	CGlobalParamSetting::Instance().g_VacParamMain.bTCf,
	CGlobalParamSetting::Instance().g_VacParamMain.bTCc,
	CGlobalParamSetting::Instance().g_VacParamMain.dwTCs,
	CGlobalParamSetting::Instance().g_VacParamMain.dwExtSwi,
	CGlobalParamSetting::Instance().g_VacTCAmp.dTCfAmp,
	CGlobalParamSetting::Instance().g_VacTCAmp.dTCcAmp );

	string sSQL = "";
	string sTbaleName = CDBDataManager::Instance().GetLogTableName();
	sSQL += "insert into ";
	sSQL += sTbaleName;
	sSQL += "(";
	__int64 timestamp = GetCurrentTimeStampMS();

	int i = 0;
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(strTableName);

	vector<pair<string,CFieldDesc*>> metaV(meta.begin(),meta.end());//用map对该容器初始化
	sort(metaV.begin(),metaV.end(),CompareBySequenceID);//用sort排序

	::sort(metaV.begin(), metaV.end(), CompareBySequenceID); 

	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;

		if(i != metaV.size() - 1)
		{
			sSQL += pFieldDesc->m_strFieldID;
			sSQL += ",";
		}
		else
		{
			sSQL += pFieldDesc->m_strFieldID;
		}
		i++;
	}
	sSQL += ")";
	sSQL += " values(";

	string sFormat = "";
	CFormatConvertor FC;
	i = 0;
	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;
		if(i != metaV.size() - 1)
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
			sFormat += ",";
		}
		else
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
		}
		i++;
	}

	double dCCG = nVACBoardType < 1 ? CGlobalParamSetting::Instance().g_cfgVacCCG.dVac : CVACDataManager::Instance().m_valueCCG.dVac;
	char szValue[512] = {0};
	sprintf_s(szValue,512,sFormat.c_str(),timestamp, dCCG, // CCG
		CGlobalParamSetting::Instance().g_dHV*100,						// Acc set
		CGlobalParamSetting::Instance().g_CPS3603.dVoutA,				// Acc V mon
		CGlobalParamSetting::Instance().g_CPS3603.dIoutA * 1e6,			// Acc I mon
		CGlobalParamSetting::Instance().g_CPS3603.dCoronalevel,
		CGlobalParamSetting::Instance().g_dHV3,							// Fila set
		CGlobalParamSetting::Instance().g_CPS3603.dIoutF,				// Fila I mon
		CGlobalParamSetting::Instance().g_dHV2,							// Bias set
		CGlobalParamSetting::Instance().g_CPS3603.dVoutE,				// Bias V mon
		CGlobalParamSetting::Instance().g_VacParamMain.bPower,
		CGlobalParamSetting::Instance().g_VacParamMain.bChamb,
		CGlobalParamSetting::Instance().g_VacParamMain.bGun,
		CGlobalParamSetting::Instance().g_VacParamMain.bVf,
		CGlobalParamSetting::Instance().g_VacParamMain.bVc,
		CGlobalParamSetting::Instance().g_VacParamMain.bTMP,
		CGlobalParamSetting::Instance().g_VacParamMain.bVa,
		CGlobalParamSetting::Instance().g_VacParamMain.bTCf,
		CGlobalParamSetting::Instance().g_VacParamMain.bTCc,
		CGlobalParamSetting::Instance().g_VacParamMain.dwTCs,
		CGlobalParamSetting::Instance().g_VacParamMain.dwExtSwi,
		CGlobalParamSetting::Instance().g_VacTCAmp.dTCfAmp,
		CGlobalParamSetting::Instance().g_VacTCAmp.dTCcAmp);

	sSQL += szValue;
	sSQL += ")";

	static int nExec = 0;
	if (nExec)
	{
		OutputDebugString(sSQL.c_str());
		nExec++;
	}
	CDBDataManager::Instance().Exec(sSQL);

	double dHV = CGlobalParamSetting::Instance().g_CPS3603.dVoutA;
	double dFila = CGlobalParamSetting::Instance().g_CPS3603.dIoutA * 1e6;
	double dEMI = CGlobalParamSetting::Instance().g_CPS3603.dVoutE;

	int nSec = timestamp/1000;   //毫秒时间戳---> 秒时间戳

/*	double* pDouble = new double[4];
	pDouble[0] = dCCG;
	pDouble[1] = dHV;
	pDouble[2] = dFila;
	pDouble[3] = dEMI;
	PostMessage(m_hWnd,WM_MsgCheckVACAlarm,WPARAM(timestamp),LPARAM(pDouble));*/

	m_logHVVACdata.dCCG = dCCG;
	m_logHVVACdata.dHV	= dHV;
	m_logHVVACdata.dFil	= dFila;
	m_logHVVACdata.dEMI	= dEMI;
	PostMessage(m_hWnd,WM_MsgCheckVACAlarm,WPARAM(nSec),LPARAM(&m_logHVVACdata));
}

void CAppLogManager::LogHVVAC_SPM771(int nVACBoardType, CString& str)
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
	//           Time  CCG Switch Status Collector         EHT               Beam              Heater            PMT               Scint  Power/Chamb/Gun/Vf/Vc/TMP/Va/TCf/TCc/TCs/ExtSwi/TCfAmp/TCcAmp
	str.Format( "[%s],%.2e,0x%02X,0x%05X,%7.3f,%7.3f,%7.3f,%9.3f,%9.3f,%9.3f,%5.1f,%5.1f,%5.1f,%5.3f,%5.3f,%5.3f,%7.2f,%7.2f,%7.2f,%9.3f,%d,%d,%d,%d,%d,%d,%d,%d,%d,0x%04X,0x%04X,%.3f,%.3f",
	csCurrentTime, nVACBoardType < 1 ? CGlobalParamSetting::Instance().g_cfgVacCCG.dVac : CVACDataManager::Instance().m_valueCCG.dVac,// CCG
	CGlobalParamSetting::Instance().g_Spellmans.status.bstatusSwitch, CGlobalParamSetting::Instance().g_Spellmans.status.dwstatusRegister,
	CGlobalParamSetting::Instance().g_Spellmans.pS.dVC, CGlobalParamSetting::Instance().g_Spellmans.pM.dVC, CGlobalParamSetting::Instance().g_Spellmans.pM3.dVC,
	CGlobalParamSetting::Instance().g_Spellmans.pS.dVE,CGlobalParamSetting::Instance().g_Spellmans.pM.dVE, CGlobalParamSetting::Instance().g_Spellmans.pM3.dVE,
	CGlobalParamSetting::Instance().g_Spellmans.pS.dIB, CGlobalParamSetting::Instance().g_Spellmans.pM.dIB, CGlobalParamSetting::Instance().g_Spellmans.pM.dII,	// Current monitor
	CGlobalParamSetting::Instance().g_Spellmans.pS.dIH, CGlobalParamSetting::Instance().g_Spellmans.pM.dIH, CGlobalParamSetting::Instance().g_Spellmans.pM3.dIH,
	CGlobalParamSetting::Instance().g_Spellmans.pS.dVP, CGlobalParamSetting::Instance().g_Spellmans.pM.dVP, CGlobalParamSetting::Instance().g_Spellmans.pM3.dVP,
	CGlobalParamSetting::Instance().g_Spellmans.pM3.dII,	// Scint monitor
	CGlobalParamSetting::Instance().g_VacParamMain.bPower,
	CGlobalParamSetting::Instance().g_VacParamMain.bChamb,
	CGlobalParamSetting::Instance().g_VacParamMain.bGun,
	CGlobalParamSetting::Instance().g_VacParamMain.bVf,
	CGlobalParamSetting::Instance().g_VacParamMain.bVc,
	CGlobalParamSetting::Instance().g_VacParamMain.bTMP,
	CGlobalParamSetting::Instance().g_VacParamMain.bVa,
	CGlobalParamSetting::Instance().g_VacParamMain.bTCf,
	CGlobalParamSetting::Instance().g_VacParamMain.bTCc,
	CGlobalParamSetting::Instance().g_VacParamMain.dwTCs,
	CGlobalParamSetting::Instance().g_VacParamMain.dwExtSwi,
	CGlobalParamSetting::Instance().g_VacTCAmp.dTCfAmp,
	CGlobalParamSetting::Instance().g_VacTCAmp.dTCcAmp );

	string sSQL = "";
	string sTbaleName = CDBDataManager::Instance().GetLogTableName();
	sSQL += "insert into ";
	sSQL += sTbaleName;
	sSQL += "(";
	__int64 timestamp = GetCurrentTimeStampMS();

	int i = 0;
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(strTableName);

	vector<pair<string,CFieldDesc*>> metaV(meta.begin(),meta.end());//用map对该容器初始化
	sort(metaV.begin(),metaV.end(),CompareBySequenceID);//用sort排序

	::sort(metaV.begin(), metaV.end(), CompareBySequenceID); 

	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;

		if(i != metaV.size() - 1)
		{
			sSQL += pFieldDesc->m_strFieldID;
			sSQL += ",";
		}
		else
		{
			sSQL += pFieldDesc->m_strFieldID;
		}
		i++;
	}
	sSQL += ")";
	sSQL += " values(";

	string sFormat = "";
	CFormatConvertor FC;
	i = 0;
	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;
		if(i != metaV.size() - 1)
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
			sFormat += ",";
		}
		else
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
		}
		i++;
	}

	double dCCG = nVACBoardType < 1 ? CGlobalParamSetting::Instance().g_cfgVacCCG.dVac : CVACDataManager::Instance().m_valueCCG.dVac;
	char szValue[512] = {0};
	sprintf_s(szValue,512,sFormat.c_str(),timestamp, dCCG, // CCG
		CGlobalParamSetting::Instance().g_Spellmans.status.bstatusSwitch, CGlobalParamSetting::Instance().g_Spellmans.status.dwstatusRegister,
		CGlobalParamSetting::Instance().g_Spellmans.pS.dVC, CGlobalParamSetting::Instance().g_Spellmans.pM.dVC, CGlobalParamSetting::Instance().g_Spellmans.pM3.dVC,
		CGlobalParamSetting::Instance().g_Spellmans.pS.dVE,CGlobalParamSetting::Instance().g_Spellmans.pM.dVE, CGlobalParamSetting::Instance().g_Spellmans.pM3.dVE,
		CGlobalParamSetting::Instance().g_Spellmans.pS.dIB, CGlobalParamSetting::Instance().g_Spellmans.pM.dIB, CGlobalParamSetting::Instance().g_Spellmans.pM.dII,	// Current monitor
		CGlobalParamSetting::Instance().g_Spellmans.pS.dIH, CGlobalParamSetting::Instance().g_Spellmans.pM.dIH, CGlobalParamSetting::Instance().g_Spellmans.pM3.dIH,
		CGlobalParamSetting::Instance().g_Spellmans.pS.dVP, CGlobalParamSetting::Instance().g_Spellmans.pM.dVP, CGlobalParamSetting::Instance().g_Spellmans.pM3.dVP,
		CGlobalParamSetting::Instance().g_Spellmans.pM3.dII,	// Scint monitor
		CGlobalParamSetting::Instance().g_VacParamMain.bPower,
		CGlobalParamSetting::Instance().g_VacParamMain.bChamb,
		CGlobalParamSetting::Instance().g_VacParamMain.bGun,
		CGlobalParamSetting::Instance().g_VacParamMain.bVf,
		CGlobalParamSetting::Instance().g_VacParamMain.bVc,
		CGlobalParamSetting::Instance().g_VacParamMain.bTMP,
		CGlobalParamSetting::Instance().g_VacParamMain.bVa,
		CGlobalParamSetting::Instance().g_VacParamMain.bTCf,
		CGlobalParamSetting::Instance().g_VacParamMain.bTCc,
		CGlobalParamSetting::Instance().g_VacParamMain.dwTCs,
		CGlobalParamSetting::Instance().g_VacParamMain.dwExtSwi,
		CGlobalParamSetting::Instance().g_VacTCAmp.dTCfAmp,
		CGlobalParamSetting::Instance().g_VacTCAmp.dTCcAmp);
	sSQL += szValue;
	sSQL += ")";

	static int nExec = 0;
	if (nExec)
	{
		OutputDebugString(sSQL.c_str());
		nExec++;
	}

	CDBDataManager::Instance().Exec(sSQL);

	double dHV = CGlobalParamSetting::Instance().g_Spellmans.pM.dVE;
	double dFila = CGlobalParamSetting::Instance().g_Spellmans.pM.dIH;
	double dEMI = CGlobalParamSetting::Instance().g_Spellmans.pM.dII;

	//std::random_device rd;  //将用于获取随机数引擎的种子
	//std::mt19937 gen(rd()); //以rd（）作为种子的标准mersenne_twister_engine
	//std::uniform_int_distribution<> distrib(10000, 30000);
	//std::uniform_int_distribution<> distrib2(1, 100);

	//dCCG = distrib(gen)/(pow(10,8)) * distrib2(gen);
	//dHV = distrib(gen)/(pow(10,3));;
	//dFila = distrib(gen)/(pow(10,4));
	//dEMI = distrib(gen)/(pow(10,2));

	int nSec = timestamp/1000;   //毫秒时间戳---> 秒时间戳

/*	double* pDouble = new double[4];
	pDouble[0] = dCCG;
	pDouble[1] = dHV;
	pDouble[2] = dFila;
	pDouble[3] = dEMI;
    PostMessage(m_hWnd,WM_MsgCheckVACAlarm,WPARAM(nSec),LPARAM(pDouble));*/

	m_logHVVACdata.dCCG = dCCG;
	m_logHVVACdata.dHV	= dHV;
	m_logHVVACdata.dFil	= dFila;
	m_logHVVACdata.dEMI	= dEMI;
	PostMessage(m_hWnd,WM_MsgCheckVACAlarm,WPARAM(nSec),LPARAM(&m_logHVVACdata));
}

void CAppLogManager::LogHVVAC_WSM30KV2(int nVACBoardType, CString& str)
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
	str.Format( "[%s],%d,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%5.3f,%5.3f,%5.3f,%5.3f,%5.3f,%5.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f",
		csCurrentTime,
		CGlobalParamSetting::Instance().g_WSM30KV2status.statusO.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.statusB.wStatus,
		CGlobalParamSetting::Instance().g_WSM30KV2status.statusV.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.statusI.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.statusA.wStatus,
		CGlobalParamSetting::Instance().g_WSM30KV2status.status1.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.status2.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.status3.wStatus,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fSCIv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fSCIv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fSCIv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fSCIi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fCOLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fCOLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fCOLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fCOLi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fPMTv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fPMTv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fPMTv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fPMTi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fACLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fACLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fACLv,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fACLi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fACLi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fACLi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fFILi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fFILi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fFILi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fBIASv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fBIASv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fBIASv,
		CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP3V3, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP5V, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP15V,
		CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fN15V, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP24V, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fTemp);

	string sSQL = "";
	string sTbaleName = CDBDataManager::Instance().GetLogTableName();
	sSQL += "insert into ";
	sSQL += sTbaleName;
	sSQL += "(";
	__int64 timestamp = GetCurrentTimeStampMS();

	int i = 0;
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(strTableName);

	vector<pair<string,CFieldDesc*>> metaV(meta.begin(),meta.end());//用map对该容器初始化
	sort(metaV.begin(),metaV.end(),CompareBySequenceID);//用sort排序

	::sort(metaV.begin(), metaV.end(), CompareBySequenceID); 

	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;

		if(i != metaV.size() - 1)
		{
			sSQL += pFieldDesc->m_strFieldID;
			sSQL += ",";
		}
		else
		{
			sSQL += pFieldDesc->m_strFieldID;
		}
		i++;
	}
	sSQL += ")";
	sSQL += " values(";

	string sFormat = "";
	CFormatConvertor FC;
	i = 0;
	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;
		if(i != metaV.size() - 1)
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
			sFormat += ",";
		}
		else
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
		}
		i++;
	}

	double dCCG;
	if(nVACBoardType <1)
		dCCG = CGlobalParamSetting::Instance().g_cfgVacCCG.dVac;
	else
		dCCG = CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg;

	char szValue[1024] = {0};
	sprintf_s(szValue,1024,sFormat.c_str(),timestamp, dCCG,
		CGlobalParamSetting::Instance().g_WSM30KV2status.statusO.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.statusB.wStatus,
		CGlobalParamSetting::Instance().g_WSM30KV2status.statusV.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.statusI.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.statusA.wStatus,
		CGlobalParamSetting::Instance().g_WSM30KV2status.status1.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.status2.wStatus, CGlobalParamSetting::Instance().g_WSM30KV2status.status3.wStatus,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fSCIv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fSCIv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fSCIv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fSCIi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fCOLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fCOLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fCOLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fCOLi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fPMTv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fPMTv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fPMTv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fPMTi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fACLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fACLv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fACLv,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fACLi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fACLi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fACLi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fFILi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fFILi, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fFILi,
		CGlobalParamSetting::Instance().g_WSM30KV2set.fBIASv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP.fBIASv, CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fBIASv,
		CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP3V3, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP5V, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP15V,
		CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fN15V, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fP24V, CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT.fTemp);
	sSQL += szValue;
	sSQL += ")";

	static int nExec = 0;
	if (nExec)
	{
		OutputDebugString(sSQL.c_str());
		nExec++;
	}

	CDBDataManager::Instance().Exec(sSQL);

	double dHV = CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fACLv/1000;
	double dFila = CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fFILi;
	double dEMI = CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB.fACLi;

	int nSec = timestamp/1000;   //毫秒时间戳---> 秒时间戳

/*	double* pDouble = new double[4];
	pDouble[0] = dCCG;
	pDouble[1] = dHV;
	pDouble[2] = dFila;
	pDouble[3] = dEMI;
	PostMessage(m_hWnd,WM_MsgCheckVACAlarm,WPARAM(nSec),LPARAM(pDouble));*/

	m_logHVVACdata.dCCG = dCCG;
	m_logHVVACdata.dHV	= dHV;
	m_logHVVACdata.dFil	= dFila;
	m_logHVVACdata.dEMI	= dEMI;
	PostMessage(m_hWnd,WM_MsgCheckVACAlarm,WPARAM(nSec),LPARAM(&m_logHVVACdata));
}
