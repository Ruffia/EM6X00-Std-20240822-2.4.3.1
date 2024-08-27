#include "StdAfx.h"
#include "SpellmanStatusDetector.h"
#include <math.h>

extern	CString	HID_FindHIDInterface( int nVID, int nPID, int nUsagePage );
extern	HANDLE	HID_OpenForRead( CString strFileName );
extern	HANDLE	HID_OpenForWrite( CString strFileName );

#define minmax(x,l,r)   (((x)<(l))?(l):((x)>(r))?(r):(x))

CSpellmanStatusDetector& CSpellmanStatusDetector::Instance()
{
	static CSpellmanStatusDetector Inst;
	return Inst;
}


CSpellmanStatusDetector::CSpellmanStatusDetector(void)
{
	m_hWnd = NULL;
	g_AutoState = AS2_NULL;

	g_dblAutoStep_EHT1		= 0.5;	// 500;
	g_dblAutoStep_EHT2		= 1.0;	// 1000;
	g_dblAutoRated_EHT1		= 10.0;	// 10000;
	g_dblAutoRated_EHT2		= 30.0;	// 30000;
	g_dblAutoStep_Beam		= 10;
	g_dblAutoRated_Beam		= 100;
	g_dblAutoStep_Heater1	= 0.1;   // 100mA
	g_dblAutoStep_Heater2	= 0.01;  // 10mA
	g_dblAutoRated_Heater	= 1.9;	 // 1.9A

	g_hFile	= NULL;
	g_nLoops = 0;

	g_bPauseComm	= FALSE;	// 是否暂停与设备通讯
	g_bPausePost	= FALSE;	// 是否暂停上送监测值（但不停止与设备通讯）
	g_bPauseInquire	= FALSE;

	g_strDeviceName	= _T("");				// The system name for the remote button device
	g_bOpenned		= FALSE;				// Is the ButtonsAndLights device operational?

	g_hWrite		= NULL;					// Needed to write to a system device
	g_hRead			= NULL;					// Needed to read a system device

	ZeroMemory(g_bufferRead,BUFFER_LEN);
	ZeroMemory(g_bufferWrite,BUFFER_LEN);

	g_dConstStep_PMT	= 1350.0 /4095;
	g_dConstStep_II		= 411.11 /4095;
	g_dConstStep_Heater	= 4200.0 /4095 /1000;
	g_dConstStep_EHT	= 33000.0/1023;
	g_dConstStep_Col	= (471.06 +433.37)/1023;
	g_dConstStep_Scint	= 13660.0 /1023;
	g_dConstStep_PMT3	= 1350.0 /1023;

	g_strSpellman		= "";
	g_strSpellmanSaved	= "";

	g_hEvent = NULL;

	g_bStepAcc = FALSE;
	g_bStepBias = FALSE;
	g_bStepFila = FALSE;
	g_lastStep = 0;

	g_dblGoalAcc = 0.0;
	g_dblGoalBias = 0.0;
	g_dblGoalFila = 0.0;

	m_bNotify2FilamentCurrentContorl = FALSE;   //通知灯丝电流控件UI
	m_bNotify2HVContorl = FALSE;                //通知高压控件UI

	m_dwSleep = 20;
}


void CSpellmanStatusDetector::Init( HWND hWnd )
{
	char sz[512] = {0};
	sprintf_s(sz,512,"SPM771: Init( HWND hWnd )\r\n" );
	OutputDebugString(sz);

	m_hWnd = hWnd;
}


BOOL CSpellmanStatusDetector::Start()
{
	char sz[512] = {0};
	sprintf_s(sz,512,"SPM771: Start() -----Begin \r\n" );
	OutputDebugString(sz);

	g_strDeviceName = HID_OpenHIDDevice();
	if( g_strDeviceName == "" )
	{
		g_Spellmans.status.bReady = FALSE;
		return 0;
	}


	ZeroMemory(sz,512);
	sprintf_s(sz,512,"SPM771: Start() ----- g_strDeviceName = %s \r\n", (LPSTR)(LPCTSTR)g_strDeviceName );
	OutputDebugString(sz);

	if( HID_InitHIDDevice( g_strDeviceName ) )
		g_Spellmans.status.bReady = TRUE;

	ZeroMemory(sz,512);
	sprintf_s(sz,512,"SPM771: Start() -----  g_Spellmans.status.bReady = %d;\r\n", g_Spellmans.status.bReady );
	OutputDebugString(sz);

	if( g_Spellmans.status.bReady )
	{
		/////////////////////////////////////////////////////////
		// Request for identification
		// Gun supply identification - "?Gc"
		// Command:		?G	(return code for Type identification)
		// Response:	EBM30N12
		m_cs.Lock();
		g_bufferWrite[1] = (BYTE)('?');
		g_bufferWrite[2] = (BYTE)('G');
		g_bufferWrite[3] = 0x0D;

		DWORD nTimes = 0;
		CString strAck = "GEBM30N12";
		do{
			for(int i=0; i<10; i++ )
				g_bufferRead[i] = 0;
			WriteToDevice();

			Sleep(100);
			CString strData = ReadFromDevice();
//			if( strData == strAck )
			if( strData.Find(strAck) > -1 )
			{
				ConvertReceiveData( strData );
				break;
			}
			nTimes++;
		} while( nTimes < 10 );
		m_cs.Unlock();

		if( nTimes == 10 )
			g_Spellmans.status.bReady	= FALSE;
		else
			g_Spellmans.status.bReady	= TRUE;

		/////////////////////////////////////////////////////////
		// Request for identification
		// Software Version identification - "?Vc"
		// Command:		?V	(return code for software Version identification)
		// Response:	V01.0c = A particular Version
		m_cs.Lock();
		// 发送查询命令
		g_bufferWrite[0] = 0;
		g_bufferWrite[1] = (BYTE)('?');
		g_bufferWrite[2] = (BYTE)('V');
		g_bufferWrite[3] = 0x0D;
		for(int i=0; i<10; i++ )
			g_bufferRead[i] = 0;
		WriteToDevice();

		Sleep( 10 );
		// 接收回复
		CString strData = ReadFromDevice();
		// 分析数据
		ConvertReceiveData( strData );
		m_cs.Unlock();
		Reset771();
	}
	PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 1, g_Spellmans.status.bReady );

	ZeroMemory(sz,512);
	sprintf_s(sz,512,"SPM771: Start() ----- PostMessage, g_Spellmans.status.bReady = %d;\r\n", g_Spellmans.status.bReady );
	OutputDebugString(sz);

	g_AutoState	= AS2_NULL;
	if( g_Spellmans.status.bReady )
	{
		// 日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;

		CString str;
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString strTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\Spell771-%s.csv", exeFullPath, strTime );
		if( g_hFile != NULL )
			CloseHandle( g_hFile );
		g_hFile	= NULL;
		g_hFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		g_nLoops = 1;
		// 写日志文件头
		if( g_hFile != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s\r\n", "EBM30N12/771" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		// 先查询一次所有加速度
		int nSleep = 20;
		ReadRampRates( nSleep );
		///////////////////////////////////////////////////
		// For test
// 		SetRampRates( 0, 640 );	// Collector 800, 160V/s
// 		SetRampRates( 1, 480 );	// EHT 600, 6000V/s
// 		SetRampRates( 2, 640 );	// Beam 800, 80uA/s
// 		SetRampRates( 3, 640 );	// Heater 800, 0.8A/s
// 		SetRampRates( 4, 400 );	// PMT 788, 260V/s
// 		PostMessage( m_hWnd, WM_USER_SPELLMAN_Param2, 1, (LPARAM)(&(g_Spellmans.pM2)) );
// 		Message2();
// 		ReadRampRates( nSleep );
		///////////////////////////////////////////////////
		if( g_hFile != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
//			str.Format( "\tSwitch\tStatus\tCollector s(V)\tCollector m(V)\tEHT s(KV)\tEHT m(KV)\tBeam s(uA)\tBeam m(uA)\tHeat s(A)\tHeat m(A)\tPMT s(V)\tPMT m(V)\tBeam Current(uA)" );
//			str.Format( ",Switch,Status,Collector s(V),Collector m(V),EHT s(KV),EHT m(KV),Beam s(uA),Beam m(uA),Heat s(A),Heat m(A),PMT s(V),PMT m(V),Beam Current(uA)" );
			str.Format( "Time, Switch,Status, C s(V),C m(V),C m2(V), E s(V),E m(V),E m2(V), B s(uA),B m(uA),S m(V), H s(A),H m(A),H m2(A), P s(V),P m(V),P m2(V), Beam Current(uA)" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		// 同步一次所有参数值
		if( SetEnquiryStatus( nSleep ) < 1 )
			//			PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, 0 );
				PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
		SetEnquiry( 0, nSleep );
		SetEnquiry( 1, nSleep );
		SetEnquiry( 2, nSleep );
		SetEnquiry( 3, nSleep );
		SetEnquiry( 4, nSleep );
		SetEnquiry( 5, nSleep );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_Param, 1, (LPARAM)(&(g_Spellmans.pM)) );

		ZeroMemory(sz,512);
		sprintf_s(sz,512,"SPM771: Start() ----- PostMessage (g_Spellmans.pM);\r\n");
		OutputDebugString(sz);

		Message();
		g_lastMon	= GetTickCount();
		g_lastAuto	= GetTickCount();
	}


	BOOL bStart = __super::Start();
	return bStart;
}

void CSpellmanStatusDetector::Release()
{
	g_Spellmans.status.bReady	= FALSE;
	if( g_hFile != NULL )
		CloseHandle( g_hFile );
	g_hFile	= NULL;
	HID_CloseHIDDevice();

	__super::Release();
}

DWORD CSpellmanStatusDetector::DoJob()
{
	if( !g_bPauseComm )
	{
		if( g_AutoState < AS2_NULL )
		{
			AutoProc();
//			Sleep(1);
		}

		DWORD thisTickCount = GetTickCount();
		if( g_bStepAcc )
		{
			// 启动分步加高压
			if( thisTickCount -g_lastStep > 400 )
			{
				StepAcc();
				g_lastStep = GetTickCount();
			}
		}

		if( g_bStepFila )
		{
			// 启动分步加灯丝
			if( thisTickCount -g_lastStep > 400 )
			{
				if( g_Spellmans.pS.dIH >= 2.0 )
					StepFila2();
				else
					StepFila1();
				g_lastStep = GetTickCount();
			}
		}

		// 500ms通讯一次
		CommWithSpellman(20);
	}
	Sleep( 1 );

	return 0L;
}

void CSpellmanStatusDetector::StepAcc()
{
	if (g_Spellmans.pS.dVE > g_dblGoalAcc)
	{
		g_Spellmans.pS.dVE -= g_dblAutoStep_EHT2;
		if( g_Spellmans.pS.dVE < g_dblGoalAcc )
		{
			g_Spellmans.pS.dVE = g_dblGoalAcc;
			SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepAcc() -----g_Spellmans.pS.dVE = %f", g_Spellmans.pS.dVE );
			OutputDebugString(sz);

			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_EHT2, (LPARAM)(g_Spellmans.pS.dVE*10) );
				m_bNotify2HVContorl = FALSE;
			}	
			g_bStepAcc = FALSE;
			g_lastStep = 0;
		}
		else
		{
			SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepAcc() -----g_Spellmans.pS.dVE = %f", g_Spellmans.pS.dVE );
			OutputDebugString(sz);

			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_EHT2, (LPARAM)(g_Spellmans.pS.dVE*10) );
			}
		}
	}
	else
	{
		g_Spellmans.pS.dVE += g_dblAutoStep_EHT2;
		if( g_Spellmans.pS.dVE > g_dblGoalAcc )
		{
			g_Spellmans.pS.dVE = g_dblGoalAcc;
			SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepAcc() -----g_Spellmans.pS.dVE = %f", g_Spellmans.pS.dVE );
			OutputDebugString(sz);

			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_EHT2, (LPARAM)(g_Spellmans.pS.dVE*10) );
				m_bNotify2HVContorl = FALSE;
			}	
			g_bStepAcc = FALSE;
			g_lastStep = 0;
		}
		else
		{
			SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepAcc() -----g_Spellmans.pS.dVE = %f", g_Spellmans.pS.dVE );
			OutputDebugString(sz);
			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_EHT2, (LPARAM)(g_Spellmans.pS.dVE*10) );
			}
		}
	}
}


void CSpellmanStatusDetector::StepFila2()
{
	if (g_Spellmans.pS.dIH > g_dblGoalFila)
	{
		g_Spellmans.pS.dIH -= g_dblAutoStep_Heater2;
		if (g_Spellmans.pS.dIH < g_dblGoalFila)
		{
			g_Spellmans.pS.dIH = g_dblGoalFila;
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila2() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
				m_bNotify2FilamentCurrentContorl = FALSE;
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila2() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
			}
		}
	}
	else
	{
		g_Spellmans.pS.dIH += g_dblAutoStep_Heater2;
		if( g_Spellmans.pS.dIH > g_dblGoalFila )
		{
			g_Spellmans.pS.dIH = g_dblGoalFila;
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila2() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
				m_bNotify2FilamentCurrentContorl = FALSE;
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila2() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
			}
		}
	}
}


void CSpellmanStatusDetector::StepFila1()
{
	if (g_Spellmans.pS.dIH > g_dblGoalFila)
	{
		g_Spellmans.pS.dIH -= g_dblAutoStep_Heater1;
		if( g_Spellmans.pS.dIH < g_dblGoalFila )
		{
			g_Spellmans.pS.dIH = g_dblGoalFila;
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila1() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila1() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
			}
		}
	}
	else
	{
		g_Spellmans.pS.dIH += g_dblAutoStep_Heater1;
		if( g_Spellmans.pS.dIH > g_dblGoalFila )
		{
			g_Spellmans.pS.dIH = g_dblGoalFila;
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila1() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );

			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"SPM771: StepFila1() -----g_Spellmans.pS.dIH = %f, g_dblGoalFila = %f", g_Spellmans.pS.dIH, g_dblGoalFila );
			OutputDebugString(sz);

			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, AS2_Rise_Heater, (LPARAM)(g_Spellmans.pS.dIH*1000) );
			}
		}
	}
}

void CSpellmanStatusDetector::SetParam( int nIndex, int nValue )
{
	char szBuf[255] = {0};
	CSingleLock Lock(&m_cs,TRUE);
	switch( nIndex )
	{
	case	11:
		g_bPauseComm	= (BOOL)nValue;
		break;
	case	12:
		g_bPausePost	= (BOOL)nValue;
		break;
	case	13:
		g_bPauseInquire	= (BOOL)nValue;
		break;

	case	16:
		g_AutoState		= (Spellman_AutoState)nValue;
		if( g_AutoState < AS2_NULL )
		{
			if( (g_AutoState == AS2_Rise_EHT1) && g_Spellmans.pS.dVE > g_dblAutoRated_EHT1 )
			{
				g_AutoState = AS2_Rise_EHT2;
				sprintf( szBuf, "SPM771: current VE (%.3f KV) > 10000V, start to rise EHT2", g_Spellmans.pS.dVE );
				OutputDebugString( szBuf );
			}
			else if( (g_AutoState == AS2_Rise_EHT1) && ((int)g_Spellmans.pS.dVE == (int)g_dblAutoRated_EHT2) )
			{
				g_AutoState = AS2_Rise_Heater;
				sprintf_s( szBuf, "SPM771: current VE (%.3f KV) == 30000V, start to rise Heater", g_Spellmans.pS.dVE );
				OutputDebugString( szBuf );
			}
			else
			{
				sprintf( szBuf, "SPM771: current VE = %.3f KV, start to rise EHT1", g_Spellmans.pS.dVE );
				OutputDebugString( szBuf );
			}
		}
		break;

	case	17:
		g_dblAutoRated_EHT2		= nValue /1000.0;	// KV
		sprintf_s( szBuf, "SPM771: set AutoRated EHT2 = %.3f KV", g_dblAutoRated_EHT2 );
		OutputDebugString( szBuf );
		break;
	case	18:
		g_dblAutoRated_Beam		= nValue;
		sprintf_s( szBuf, "SPM771: set AutoRated Beam = %.3f", g_dblAutoRated_Beam );
		OutputDebugString( szBuf );
		break;
	case	19:
		g_dblAutoRated_Heater	= nValue /100.0;
		sprintf_s( szBuf, "SPM771: set AutoRated Heater = %.3f", g_dblAutoRated_Heater );
		OutputDebugString( szBuf );
		break;

	case	21:	// Collector
	case	22:	// Scint
	case	23:	// EHT
	case	24:	// Heater
	case	25:	// PMT
		SetSwitch( nIndex -21, nValue );
		break;

	case	31:	// Collector
		g_Spellmans.pS.dVC	= nValue *0.2;
		SetValue( nIndex -31, nValue );
		break;
	case	32:	// EHT
		g_Spellmans.pS.dVE	= nValue *10.0 /100;
		SetValue( nIndex -31, nValue );
		break;
	case	33:	// Beam
		g_Spellmans.pS.dIB	= nValue *0.1;
		SetValue( nIndex -31, nValue );
		break;
	case	34:	// Heater
		g_Spellmans.pS.dIH	= nValue *0.001;
		SetValue( nIndex -31, nValue );
		break;
	case	35:	// PMT
		g_Spellmans.pS.dVP	= nValue *g_dConstStep_PMT;
		SetValue( nIndex -31, nValue );
		break;

	case	41:	// Collector
	case	42:	// EHT
	case	43:	// Beam
	case	44:	// Heater
	case	45:	// PMT
		SetRampRates( nIndex -41, nValue );
		break;

	case	46:
	case	47:
	case	48:
	case	49:
		SetSteps( nIndex -46, nValue );
		break;

	case	51:
		SetEnquiryID();
		break;
	case	52:
		SetEnquiryVer();
		break;
	case	53:
		if( SetEnquiryStatus( 10 ) < 1 )
//			PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, 0 );
			PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
		break;
	case	54:
		ReadRampRates( 10 );
		break;

	case	55:
		Unlock5KV();
		break;
	case	56:
		Reset771();
		break;

	case	61:
		ClearSwitches();
		break;
	case	62:
		OpenSwitches();
		break;

	case    72:
		g_dblGoalAcc	= nValue *10.0 /100;
		g_bStepAcc		= TRUE;
		m_bNotify2HVContorl = TRUE;
		g_lastStep		= GetTickCount();	
		break;
	case    74:
		g_dblGoalFila	= nValue *0.001;
		g_bStepFila		= TRUE;
		m_bNotify2FilamentCurrentContorl = TRUE;
		g_lastStep		= GetTickCount();	
		break;
	}
}


CString CSpellmanStatusDetector::HID_OpenHIDDevice()
{
	// I know this is a HID device with a specific VID and PID.
	// I also search for the UsagePage that defines the data reports
	// I return the system name for the device or NULL if the device is not plugged in
	CString strSystemName = _T("");
	// check for examples 1 and 4
	strSystemName = HID_FindHIDInterface( 0x04D8, 0x003F, 0xFF00 );
	if( strSystemName == "" )
		strSystemName = HID_FindHIDInterface( 0x04D8, 0x003F, 0xFF00 );		// Set in USB_descr.asm

	return strSystemName;
}

BOOL CSpellmanStatusDetector::HID_InitHIDDevice( CString strDeviceName )
{
	g_hEvent				= CreateEvent(NULL,FALSE, FALSE, _T("WORKspellman"));
	///////////////////////////////////////////////////////////////////////////
	g_overlapRead.Offset		= 0;
	g_overlapRead.OffsetHigh	= 0;
	g_overlapRead.hEvent		= g_hEvent;
	///////////////////////////////////////////////////////////////////////////

	if (strDeviceName == "")
		return FALSE;
	else
	{
		g_hRead		= HID_OpenForRead(strDeviceName);
		g_hWrite	= HID_OpenForWrite(strDeviceName);
		g_bOpenned	= TRUE;
	}

	// 接收数据缓冲区清零
	memset( g_bufferRead, 0, sizeof(g_bufferRead) );
	memset( g_bufferWrite, 0, sizeof(g_bufferWrite) );

	return TRUE;
}

void CSpellmanStatusDetector::HID_CloseHIDDevice()
{
	if( g_hRead != NULL )
	{
		CloseHandle( g_hRead );
		g_hRead = NULL;
	}
	if( g_hWrite != NULL )
	{
		CloseHandle( g_hWrite );
		g_hWrite = NULL;
	}

	if (g_hEvent != NULL)
	{
		CloseHandle(g_hEvent);
		g_hEvent = NULL;
	}

	g_bOpenned = FALSE;
}


void CSpellmanStatusDetector::Message()
{
	if( g_hFile != NULL )
	{
		unsigned long cbRet;
		char Buf[255];

		CString str, strTemp;
//		str.Format( "0x%02X\t0x%03X\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.3f\t%.3f\t%.2f\t%.2f\t%.2f",
		str.Format( "0x%02X,0x%05X, %7.3lf,%7.3lf,%7.3lf, %9.3lf,%9.3lf,%9.3lf, %7.3lf,%7.3lf,%9.3lf, %5.3lf,%5.3lf,%5.3lf, %7.3lf,%7.3lf,%7.3lf, %6.2lf",
					g_Spellmans.status.bstatusSwitch, g_Spellmans.status.dwstatusRegister,
					g_Spellmans.pS.dVC, g_Spellmans.pM.dVC, g_Spellmans.pM3.dVC,
					g_Spellmans.pS.dVE, g_Spellmans.pM.dVE, g_Spellmans.pM3.dVE,
					g_Spellmans.pS.dIB, g_Spellmans.pM.dIB, g_Spellmans.pM3.dIB,
					g_Spellmans.pS.dIH, g_Spellmans.pM.dIH, g_Spellmans.pM3.dIH,
					g_Spellmans.pS.dVP, g_Spellmans.pM.dVP, g_Spellmans.pM3.dVP,
					g_Spellmans.pM.dII );

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
		strTemp.Format( "[%s], %s", csCurrentTime, str );
		sprintf(Buf, "%s\r\n", strTemp );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoops++;
		if( g_nLoops > 10000 )
		{
			CloseHandle( g_hFile );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\Spell771-%s.csv", exeFullPath, csCurrentTime );
			g_hFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoops = 1;

			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s\r\n", "EBM30N12/771" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
//			str.Format( "\tSwitch\tStatus\tCollector s(V)\tCollector m(V)\tEHT s(KV)\tEHT m(KV)\tBeam s(uA)\tBeam m(uA)\tHeat s(A)\tHeat m(A)\tPMT s(V)\tPMT m(V)\tBeam Current(uA)" );
//			str.Format( ",Switch,Status,Collector s(V),Collector m(V),EHT s(KV),EHT m(KV),Beam s(uA),Beam m(uA),Heat s(A),Heat m(A),PMT s(V),PMT m(V),Beam Current(uA)" );
			str.Format( "Time, Switch,Status, C s(V),C m(V),C m2(V), E s(V),E m(V),E m2(V), B s(uA),B m(uA),S m(V), H s(A),H m(A),H m2(A), P s(V),P m(V),P m2(V), Beam Current(uA)" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

void CSpellmanStatusDetector::Message2()
{
	if( g_hFile != NULL )
	{
		unsigned long cbRet;
		char Buf[255];

		CString str;
		str.Format( "Collector Ramp rates = %.2f V/s", g_Spellmans.pM2.dVC );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "EHT Ramp rates = %.2f V/s", g_Spellmans.pM2.dVE );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "Beam Ramp rates = %.2f uA/s", g_Spellmans.pM2.dIB );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "Heater Ramp rates = %.2f A/s", g_Spellmans.pM2.dIH );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "PMT Ramp rates = %.2f V/s", g_Spellmans.pM2.dVP );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str = " ";
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
	}
}

int CSpellmanStatusDetector::ConvertReceiveData( CString strData )
{
	if( strData.IsEmpty() )
		return -1;

	CString str, strC, strType, strValue;
	int nPos, nRet = 0;
	str = strData;
/*	strC.Format( "\012" );
	nPos = str.Find( strC );
	int nLength = str.GetLength();
	if( nPos < nLength -2 )
	{
		// 回车在字符串中间
		g_strSpellmanSaved = str.Right( nLength -nPos -2 );
		str = str.Left( nPos );
	}
	else
		g_strSpellmanSaved = "";
	if( str.GetLength() < 5 )
		return 1;	// 数据量不足
*/
	strType = str.Left( 1 );	// G,V, O,P, C/E/B/H/J/I, c/e/b/h/j

	strValue = str.Mid(1,3);
	char *bufNum = strValue.GetBuffer(4);
	sscanf(bufNum, "%x", &nPos);

	if( strType == "G" )		// Gun supply identification
	{
		g_Spellmans.status.strID = str.Right( str.GetLength() -1 );
//		PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 2, 0 );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 2, (LPARAM)(&(g_Spellmans.status)) );
		nRet = 1;
	}
	else if( strType == "V" )	// Software Version identification
	{
		g_Spellmans.status.strVer = str.Right( str.GetLength() -1 );
//		PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 3, 0 );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 3, (LPARAM)(&(g_Spellmans.status)) );
		nRet = 2;
	}
	else if( strType == "O" )	// On/Off/Status
	{
		strValue = str.Mid( 1, str.GetLength() -1 );
		int nLength = strValue.GetLength();
		char *bufStatus = strValue.GetBuffer( nLength +1 );
		sscanf(bufStatus, "%X", &nPos );
//		if( nLength > 20 )
		{
			g_Spellmans.status.dwstatusRegister	= nPos & 0xFFFFF;
			g_Spellmans.status.bstatusSwitch	= nPos >> 20;
		}
/*		else
		{
			g_Spellmans.status.dwstatusRegister	= nPos & 0xFFF;
			g_Spellmans.status.bstatusSwitch	= nPos >> 12;
		}
*/		g_Spellmans.status.bPMT			= (g_Spellmans.status.bstatusSwitch & 0x10) == 0x10;
		g_Spellmans.status.bHeater		= (g_Spellmans.status.bstatusSwitch & 0x08) == 0x08;
		g_Spellmans.status.bEHT			= (g_Spellmans.status.bstatusSwitch & 0x04) == 0x04;
		g_Spellmans.status.bScint		= (g_Spellmans.status.bstatusSwitch & 0x02) == 0x02;
		g_Spellmans.status.bCollector	= (g_Spellmans.status.bstatusSwitch & 0x01) == 0x01;
		nRet = 3;
	}
	else if( strType == "P" )
		nRet = 4;
	else if( strType == "C" )	// Collector voltage program
	{
		g_Spellmans.pM.dVC	= (nPos -2048) *0.2;
		nRet = 11;
	}
	else if( strType == "E" )	// EHT voltage program
	{
		g_Spellmans.pM.dVE	= nPos *10.0;
		nRet = 12;
	}
	else if( strType == "B" )	// Beam current program
	{
		g_Spellmans.pM.dIB	= nPos *0.1;
		nRet = 13;
	}
	else if( strType == "H" )	// Heater current program
	{
		g_Spellmans.pM.dIH	= nPos *0.001;
		nRet = 14;
	}
	else if( strType == "J" )	// PMT voltage program
	{
		g_Spellmans.pM.dVP	= nPos *g_dConstStep_PMT;
		nRet = 15;
	}
	else if( strType == "I" )	// Beam current monitor
	{
		g_Spellmans.pM.dII	= nPos *g_dConstStep_II;
		nRet = 16;
	}
	/////////////////////////////////////////////
	else if( strType == "c" )	// Collector ramp rates
	{
		g_Spellmans.pM2.dVC	= nPos *0.2;
		nRet = 21;
	}
	else if( strType == "e" )	// EHT ramp rates
	{
		g_Spellmans.pM2.dVE	= nPos *10;
		nRet = 22;
	}
	else if( strType == "b" )	// Beam ramp rates
	{
		g_Spellmans.pM2.dIB	= nPos *0.1;
		nRet = 23;
	}
	else if( strType == "h" )	// Heater ramp rates
	{
		g_Spellmans.pM2.dIH	= nPos *0.001;
		nRet = 24;
	}
	else if( strType == "j" )	// PMT ramp rates
	{
		g_Spellmans.pM2.dVP	= nPos *0.33;
		nRet = 25;
	}
	/////////////////////////////////////////////
	else if( strType == "f" )	// Heater current monitor
	{
		g_Spellmans.pM3.dIH	= nPos *g_dConstStep_Heater;
		nRet = 31;
	}
	else if( strType == "a" )	// EHT voltage monitor
	{
		g_Spellmans.pM3.dVE	= nPos *g_dConstStep_EHT;
		nRet = 32;
	}
	else if( strType == "k" )	// Collector voltage monitor
	{
		g_Spellmans.pM3.dVC	= 471.06 -nPos *g_dConstStep_Col;	// 0.88;
		nRet = 33;
	}
	else if( strType == "s" )	// Scinitillator voltage monitor
	{
		g_Spellmans.pM3.dIB	= nPos *g_dConstStep_Scint;
		nRet = 34;
	}
	else if( strType == "p" )	// PMT voltage monitor
	{
		g_Spellmans.pM3.dVP	= nPos *g_dConstStep_PMT3;
		nRet = 35;
	}
	return nRet;
}

BOOL CSpellmanStatusDetector::WriteToDevice()
{
	DWORD dwBytesWritten, dwErr;
	BOOL bRetWrite = WriteFile( g_hWrite, g_bufferWrite, BUFFER_LEN, &dwBytesWritten, NULL );
	if( !bRetWrite )
		dwErr = GetLastError();
	return bRetWrite;
}

CString CSpellmanStatusDetector::ReadFromDevice()
{
	CString strData = "";
	DWORD dwErr, dwBytesRead = 0;

	BOOL bRetRead = ReadFile(g_hRead, g_bufferRead, BUFFER_LEN, &dwBytesRead, &g_overlapRead);
	if( !bRetRead )
		dwErr = GetLastError();

	if( bRetRead )
	{
		CString str;
		for( DWORD i=1; i<dwBytesRead; i++ )
		{
			if( g_bufferRead[i] == 0x0D )
				break;
			str.Format( "%c", g_bufferRead[i] );
			strData += str;
		}
	}
	return strData;
}

void CSpellmanStatusDetector::ReadRampRate( int nIndex, int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_cs.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	switch( nIndex )
	{
	case	0:	// Collector
		g_bufferWrite[2] = (BYTE)('c');
		break;
	case	1:	// EHT
		g_bufferWrite[2] = (BYTE)('e');
		break;
	case	2:	// Beam
		g_bufferWrite[2] = (BYTE)('b');
		break;
	case	3:	// Heater
		g_bufferWrite[2] = (BYTE)('h');
		break;
	case	4:	// PMT
		g_bufferWrite[2] = (BYTE)('j');
		break;
	}
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( nSleep );
	// 接收回复
	// *hhh: where * is either C for Collector, E for EHT, B for Beam or H for Heater
	//		J for PMT or I for the Beam current monitor
	//		hhh is a 12 bit number representing the raw value to be sent to the DAC
	// Example: - Beam Current Monitor "?Ic"
	// Reply:	I7D0c = Beam current is 200uA
	CString strData = ReadFromDevice();
	// 分析数据
	ConvertReceiveData( strData );
	m_cs.Unlock();
}

void CSpellmanStatusDetector::ReadRampRates( int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	ReadRampRate( 0, nSleep );
	ReadRampRate( 1, nSleep );
	ReadRampRate( 2, nSleep );
	ReadRampRate( 3, nSleep );
	ReadRampRate( 4, nSleep );
	PostMessage( m_hWnd, WM_USER_SPELLMAN_Param2, 1, (LPARAM)(&(g_Spellmans.pM2)) );
	Message2();
}

void CSpellmanStatusDetector::SetValue( int nIndex, int nValue )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_cs.Lock();
	CString str;
	switch( nIndex )
	{
	case	0:	// Collector
		str.Format( "C%03X", nValue +2048 );
		break;
	case	1:	// EHT
		str.Format( "E%03X", nValue *10 );	// E000 - EBB8 : 0 - 3000, --> 0 - 30000 V (Step = 10 V)
		break;
	case	2:	// Beam
		str.Format( "B%03X", nValue );
		break;
	case	3:	// Heater
		str.Format( "H%03X", nValue );
		break;
	case	4:	// PMT
		str.Format( "J%03X", nValue );
		break;
	}
	g_bufferWrite[0] = 0;
	for( int i=0; i<4; i++ )
		g_bufferWrite[i+1] = (BYTE)(str[i]);
	g_bufferWrite[5] = 0x0D;
	WriteToDevice();
	m_cs.Unlock();
}

void CSpellmanStatusDetector::SetRampRates( int nIndex, int nValue )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_cs.Lock();
	CString str;
	switch( nIndex )
	{
	case	0:	// Collector
		str.Format( "c%03X", nValue );
		break;
	case	1:	// EHT
		str.Format( "e%03X", nValue );
		break;
	case	2:	// Beam
		str.Format( "b%03X", nValue );
		break;
	case	3:	// Heater
		str.Format( "h%03X", nValue );
		break;
	case	4:	// PMT
		str.Format( "j%03X", nValue );
		break;
	}
	g_bufferWrite[0] = 0;
	for( int i=0; i<4; i++ )
		g_bufferWrite[i+1] = (BYTE)(str[i]);
	g_bufferWrite[5] = 0x0D;
	WriteToDevice();
	m_cs.Unlock();
	ReadRampRate( nIndex, 10 );
}

void CSpellmanStatusDetector::SetEnquiry( int nIndex, int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_cs.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	switch( nIndex )
	{
	case	0:	// Collector voltage program
		g_bufferWrite[2] = (BYTE)('C');
		break;
	case	1:	// EHT voltage program
		g_bufferWrite[2] = (BYTE)('E');
		break;
	case	2:	// Beam current program
		g_bufferWrite[2] = (BYTE)('B');
		break;
	case	3:	// Heater current program
		g_bufferWrite[2] = (BYTE)('H');
		break;
	case	4:	// PMT voltage program
		g_bufferWrite[2] = (BYTE)('J');
		break;
	case	5:	// Beam current monitor
		g_bufferWrite[2] = (BYTE)('I');
		break;

	case	6:	// Heater current monitor
		g_bufferWrite[2] = (BYTE)('f');
		break;
	case	7:	// EHT voltage monitor
		g_bufferWrite[2] = (BYTE)('a');
		break;
	case	8:	// Collector voltage monitor
		g_bufferWrite[2] = (BYTE)('k');
		break;
	case	9:	// Scintillator voltage monitor
		g_bufferWrite[2] = (BYTE)('s');
		break;
	case	10:	// PMT voltage monitor
		g_bufferWrite[2] = (BYTE)('p');
		break;
	}
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( nSleep );
	// 接收回复
	// *hhh: where * is either C for Collector, E for EHT, B for Beam or H for Heater
	//		J for PMT or I for the Beam current monitor
	//		hhh is a 12 bit number representing the raw value to be sent to the DAC
	// Example: - Beam Current Monitor "?Ic"
	// Reply:	I7D0c = Beam current is 200uA
	CString strData = ReadFromDevice();
	// 分析数据
	ConvertReceiveData( strData );
	m_cs.Unlock();
}

int CSpellmanStatusDetector::SetEnquiryStatus( int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return -1;

	// On/Off/Status Enquiry - "?Oc"
	// Command:		?O
	// Response:	OhhHHHHH
	// Where	hh	represents the On/Off commands as Collector/Scintillator/EHT/Heater/PMT
	//			HH	represents the status register as below
	//				18:	Filament Timeout
	//				17:	Filament Regulation Trip
	//				16:	PMT Regulation Trip
	//				15:	Scintillator Regulation Trip
	//				14:	Collector Regulation Trip
	//				13:	EHT Regulation Trip
	//				12:	Internal Rail Fault
	//				11:	Scintillator interlock
	//				10:	Collector interlock
	//				9:	Accelerator Over Voltage Trip
	//				8:	Accelerator Over Current Trip
	//				7:	PMT interlock
	//				6:	Over Temperature Trip
	//				5:	Arc has occurred
	//				4:	Trip (EHT Tripped Off), (4 sen of over current or 16 Arc groups)
	//				3:	Heater Open Circuit
	//				2:	EHT interlock
	//				1:	Vacuum interlock
	//				0:	Power on reset
	m_cs.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	g_bufferWrite[2] = (BYTE)('O');
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( nSleep );

	// 接收回复
	CString strData = ReadFromDevice();
	// 分析数据
	int nRet;
	if( ConvertReceiveData( strData ) == 3 )
		nRet = 0;
	else
		nRet = 1;
	m_cs.Unlock();
	return nRet;
}

int CSpellmanStatusDetector::SetEnquiryID()
{
	if( !g_Spellmans.status.bReady )
		return -1;

	// Request for identification
	// Gun supply identification - "?Gc"
	// Command:		?G	(return code for Type identification)
	// Response:	GEBM30N12_771
	// 发送查询命令
	m_cs.Lock();
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	g_bufferWrite[2] = (BYTE)('G');
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( 10 );

	// 接收回复
	CString strData = ReadFromDevice();
	// 分析数据
	int nRet;
	if( ConvertReceiveData( strData ) == 1 )
	{
		if( g_Spellmans.status.strID == "EBM30N12" )
			nRet = 0;
		else
			nRet = 1;
	}
	else
		nRet = 2;
	m_cs.Unlock();
	return nRet;
}

int CSpellmanStatusDetector::SetEnquiryVer()
{
	if( !g_Spellmans.status.bReady )
		return -1;

	// Request for identification
	// Software Version identification - "?Vc"
	// Command:		?V	(return code for software Version identification)
	// Response:	V01.0c = A particular Version

	m_cs.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	g_bufferWrite[2] = (BYTE)('V');
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( 10 );

	// 接收回复
	CString strData = ReadFromDevice();
	// 分析数据
	int nRet;
	if( ConvertReceiveData( strData ) == 2 )
		nRet = 0;
	else
		nRet = 1;
	m_cs.Unlock();
	return nRet;
}

void CSpellmanStatusDetector::SetSwitch( int nIndex, int nSwitch )
{
	if( !g_Spellmans.status.bReady )
		return;

	// On/Off Command - "Ohhc"
	// Command:		Ohh		Where h is the ASCII representation of a hexadecimal number
	// Response:	None
	// Where h is the ASCII representation of a hexadecimal number
	//				7:	X
	//				6:	X
	//				5:	X
	//				4:	PMT
	//				3:	Heater
	//				2:	EHT
	//				1:	Scintillator
	//				0:	Collector
	// 0 indicates that the supply is OFF
	// 1 indicates that the supply is ON
	BYTE bSwitchSet = (g_Spellmans.status.bPMT << 4) | (g_Spellmans.status.bHeater << 3) | (g_Spellmans.status.bEHT << 2) | (g_Spellmans.status.bScint << 1) | g_Spellmans.status.bCollector;
	BYTE bSwitch = g_Spellmans.status.bstatusSwitch;
	if( nSwitch > 0 )
	{
		// 打开开关
		bSwitch |= (1<<nIndex);
	}
	else
	{
		// 关闭开关
		bSwitch &= (~(1<<nIndex));
	}
	if( bSwitchSet == bSwitch )
		return;

	CString str;
	str.Format( "%02X", bSwitch );
	int nTimes = 0;
	BOOL bRet = FALSE;
	// 更新开关设置
	do{
		m_cs.Lock();
		g_bufferWrite[0] = 0;
		g_bufferWrite[1] = (BYTE)('O');
		g_bufferWrite[2] = (BYTE)(str[0]);
		g_bufferWrite[3] = (BYTE)(str[1]);
		g_bufferWrite[4] = 0x0D;
		bRet = WriteToDevice();
		m_cs.Unlock();
		if( bRet )
			break;
		nTimes++;
	} while( nTimes < 10 );
	if( nTimes == 10 )
		return;
	Sleep(50);
	// 再检测开关状态
	nTimes = 0;
	do{
		SetEnquiryStatus(20);
		if( g_Spellmans.status.bstatusSwitch == bSwitchSet )
			break;
		nTimes++;
	} while( nTimes < 20 );
//	if( nTimes != 20 )
//		PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, 0 );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
}

void CSpellmanStatusDetector::ClearSwitches()
{
	if( !g_Spellmans.status.bReady )
		return;

	if( g_Spellmans.status.bstatusSwitch == 0x00 )
		return;

	m_cs.Lock();
	// 开关清零
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('O');
	g_bufferWrite[2] = (BYTE)('0');
	g_bufferWrite[3] = (BYTE)('0');
	g_bufferWrite[4] = 0x0D;
	WriteToDevice();
	m_cs.Unlock();
}

void CSpellmanStatusDetector::OpenSwitches()
{
	if( !g_Spellmans.status.bReady )
		return;

	if( g_Spellmans.status.bstatusSwitch == 0x1F )
		return;

	BYTE bSwitchSet = 0x1F;
	int nTimes = 0;
	BOOL bRet = FALSE;
	// 更新开关设置
	do{
		m_cs.Lock();
		g_bufferWrite[0] = 0;
		g_bufferWrite[1] = (BYTE)('O');
		g_bufferWrite[2] = (BYTE)('1');
		g_bufferWrite[3] = (BYTE)('F');
		g_bufferWrite[4] = 0x0D;
		bRet = WriteToDevice();
		m_cs.Unlock();
		if( bRet )
			break;
		nTimes++;
	} while( nTimes < 10 );
	// 再检测开关状态
	nTimes = 0;
	do{
		SetEnquiryStatus(20);
		if( g_Spellmans.status.bstatusSwitch == bSwitchSet )
			break;
		nTimes++;
	} while( nTimes < 10 );
//	PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, 0 );
	PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
}

void CSpellmanStatusDetector::Unlock5KV()
{
	if( !g_Spellmans.status.bReady )
		return;

	m_cs.Lock();
	// 解除5KV的限制
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('G');
	g_bufferWrite[2] = (BYTE)('P');
	g_bufferWrite[3] = (BYTE)('0');
	g_bufferWrite[4] = 0x0D;
	WriteToDevice();
	m_cs.Unlock();
}

void CSpellmanStatusDetector::ConvertCommand( CString strCmd )
{
	if( !g_Spellmans.status.bReady )
		return;

	int i, nLen = strCmd.GetLength();
	if( nLen > BUFFER_LEN )
		return;
	m_cs.Lock();
	g_bufferWrite[0] = 0;
	for( i=0; i<nLen; i++ )
	{
		char ch = strCmd[i];
		g_bufferWrite[i+1] = (BYTE)ch;
	}
	g_bufferWrite[i+1] = 0x0D;
	WriteToDevice();
	m_cs.Unlock();
}

void CSpellmanStatusDetector::Reset771()
{
	if( !g_Spellmans.status.bReady )
		return;

	m_cs.Lock();
	// 复位设备
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('R');
	g_bufferWrite[2] = 0x0D;
	WriteToDevice();
	m_cs.Unlock();
}

void CSpellmanStatusDetector::SetSteps( int nIndex, int nValue )
{
	char szBuf[255] = {0};
	switch( nIndex )
	{
	case	0:
		g_dblAutoStep_EHT1	= nValue /1000.0;	// KV
		sprintf_s( szBuf, "SPM771: set step of EHT1 = %.3f KV", g_dblAutoStep_EHT1 );
		OutputDebugString( szBuf );
		break;
	case	1:
		g_dblAutoStep_EHT2	= nValue /1000.0;	// KV
		sprintf_s( szBuf, "SPM771: set step of EHT2 = %.3f KV", g_dblAutoStep_EHT2 );
		OutputDebugString( szBuf );
		break;
	case	2:
		g_dblAutoStep_Beam	= (double)nValue;
		sprintf_s( szBuf, "SPM771: set step of Beam = %.3f uA", g_dblAutoStep_Beam );
		OutputDebugString( szBuf );
		break;
	case	3:
		g_dblAutoStep_Heater1 = (double)(nValue /1000.0);
		sprintf_s( szBuf, "SPM771: set step of Heater = %.3f A", g_dblAutoStep_Heater1 );
		OutputDebugString( szBuf );
		break;
	}
}

void CSpellmanStatusDetector::CommWithSpellman( int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	SetEnquiry( 0, nSleep );
	SetEnquiry( 1, nSleep );
	SetEnquiry( 2, nSleep );
	SetEnquiry( 3, nSleep );
	SetEnquiry( 4, nSleep );
	SetEnquiry( 5, nSleep );
	SetEnquiry( 6, nSleep );
	SetEnquiry( 7, nSleep );
	SetEnquiry( 8, nSleep );
	SetEnquiry( 9, nSleep );
	SetEnquiry( 10, nSleep );
	PostMessage( m_hWnd, WM_USER_SPELLMAN_Param, 0, (LPARAM)(&(g_Spellmans.pM)) );
	PostMessage( m_hWnd, WM_USER_SPELLMAN_Param3, 0, (LPARAM)(&(g_Spellmans.pM3)) );

	// 2s查看一次状态值
	DWORD thisTickCount = GetTickCount();
//	if( thisTickCount -g_lastMon > 2000 )
	{
		// 备份
 		BYTE switchBak	 = g_Spellmans.status.bstatusSwitch;
 		DWORD registerBak = g_Spellmans.status.dwstatusRegister;
		if( SetEnquiryStatus( nSleep ) < 1 )
		{
			// 状态有变化才向主程序投递更新消息
	 		if( (switchBak != g_Spellmans.status.bstatusSwitch)
	 			|| (registerBak != g_Spellmans.status.dwstatusRegister) )
//	 			PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, 0 );
				PostMessage( m_hWnd, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
		}
		g_lastMon = GetTickCount();
	}
	Message();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Auto Functions
void CSpellmanStatusDetector::Auto_Rise_EHT1()
{
	g_Spellmans.pS.dVE += g_dblAutoStep_EHT1;
	if( g_Spellmans.pS.dVE >= g_dblAutoRated_EHT2 )
	{
		g_Spellmans.pS.dVE = g_dblAutoRated_EHT2;
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
		g_AutoState = AS2_Rise_Heater;
		OutputDebugString( "SPM771: start to rise Heater" );
	}
	else if( g_Spellmans.pS.dVE >= g_dblAutoRated_EHT1 )
	{
		g_Spellmans.pS.dVE = g_dblAutoRated_EHT1;
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
		g_AutoState = AS2_Rise_EHT2;
		OutputDebugString( "SPM771: start to rise EHT2" );
	}
	else
	{
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
	}
}

void CSpellmanStatusDetector::Auto_Rise_EHT2()
{
	g_Spellmans.pS.dVE += g_dblAutoStep_EHT2;
	if( g_Spellmans.pS.dVE >= g_dblAutoRated_EHT2 )
	{
		g_Spellmans.pS.dVE = g_dblAutoRated_EHT2;
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
		g_AutoState = AS2_Rise_Heater;
		OutputDebugString( "SPM771: start to rise Heater" );
	}
	else
	{
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
	}
}

void CSpellmanStatusDetector::Auto_Rise_Heater()
{
	g_Spellmans.pS.dIH += g_dblAutoStep_Heater1;
	if( g_Spellmans.pS.dIH >= g_dblAutoRated_Heater )
	{
		g_Spellmans.pS.dIH = g_dblAutoRated_Heater;
		SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dIH*1000) );
		g_AutoState = AS2_Rise_Beam;
		OutputDebugString( "SPM771: start to rise Beam" );
	}
	else
	{
		SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dIH*1000) );
	}
}

void CSpellmanStatusDetector::Auto_Rise_Beam()
{
	g_Spellmans.pS.dIB += g_dblAutoStep_Beam;
	if( g_Spellmans.pS.dIB >= g_dblAutoRated_Beam )
	{
		g_Spellmans.pS.dIB = g_dblAutoRated_Beam;
		SetValue( 2, (int)(g_Spellmans.pS.dIB *10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)g_Spellmans.pS.dIB );
		g_AutoState = AS2_Rise_WaitEMI;	// 等待束流回读值到位
		OutputDebugString( "SPM771: start to wait for EMI" );
	}
	else
	{
		SetValue( 2, (int)(g_Spellmans.pS.dIB *10) );
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)g_Spellmans.pS.dIB );
	}
}

void CSpellmanStatusDetector::Auto_Rise_WaitEMI()
{
	char szBuf[255] = {0};
	sprintf_s( szBuf, "SPM771: the current EMI = %.3f, aim is %.3f", g_Spellmans.pM.dII, g_dblAutoRated_Beam );
	OutputDebugString( szBuf );
	if( abs( g_dblAutoRated_Beam -g_Spellmans.pM.dII ) < 5.0 )	// 束流值误差+-5uA内
	{
		g_AutoState = AS2_NULL;	// 结束
		PostMessage( m_hWnd, WM_USER_SPELLMAN_AutoParam, g_AutoState, 0 );
		OutputDebugString( "SPM771: rise finished" );
	}
}

void CSpellmanStatusDetector::AutoProc()
{
	char sz[512] = {0};
	DWORD thisTickCount = GetTickCount();
	// 灯丝1
	switch( g_AutoState )
	{
		/////////////////////////////////////////
		// Auto Rise
		/////////////////////////////////////////
	case	AS2_Rise_EHT1:
		if( thisTickCount - g_lastAuto > 200 )
		{
			Auto_Rise_EHT1();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_EHT2:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_EHT2();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_Heater:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Heater();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_Beam:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Beam();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_WaitEMI:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_WaitEMI();
			g_lastAuto = GetTickCount();
		}
		break;
	}
}