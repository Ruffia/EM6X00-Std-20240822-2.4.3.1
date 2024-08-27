#include "StdAfx.h"
#include "IPCStatusDetector.h"
#include "CommonVACFunction.h"

CIPCStatusDetector& CIPCStatusDetector::Instance()
{
	static CIPCStatusDetector Inst;
	return Inst;
}


CIPCStatusDetector::CIPCStatusDetector(void)
{
	m_hWnd = NULL;

	ZeroMemory( &m_cfgIPC, sizeof(CfgVAC_IP_CCG) );
	m_cfgIPC.nType = 2;

	m_strIPC = "";
	m_strIPCSaved = "";
	m_strWinIPC = "";

	m_hFileIPC	= NULL;
	m_nLoopsIPC = 0;

	m_nIPtype		= 1;		// 离子泵控制器是老式还是新式（影响通讯协议），默认为新式
	m_bPauseComm	= FALSE;	// 是否暂停与设备通讯
	g_bPausePost	= FALSE;	// 是否暂停上送监测值（但不停止与设备通讯）

	m_dwSleep = 250;
}

void CIPCStatusDetector::SetChannel(int nChn,CString strIPGChn)
{
	m_strWinIPC.Format( "8%d2", nChn );
	if( strIPGChn == m_strWinIPC )
		m_bIsSameChn = TRUE;
}

void CIPCStatusDetector::Init( HWND hWnd,int nPort, BOOL nIPtype)
{
	m_hWnd = hWnd;
	m_cfgIPC.nPort = nPort;
	m_nIPtype = nIPtype;
}

BOOL CIPCStatusDetector::Start()
{
	// 打开端口
	BOOL bCommOpen	= m_IPC.Open( m_cfgIPC.nPort );
	if (!bCommOpen)
	{
		OutputDebugString("CIPCStatusDetector---------- BOOL bCommOpen = m_IPC.Open( m_cfgCCG.nPort ); Failed.\r\n");
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 2, bCommOpen );
		return FALSE;
	}


	if( CommWithIPC( 200 ) > 1 )
		m_cfgIPC.bReady = FALSE;
	else
	{
		/////////////////////////////////////////////
		// 设置日志
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;

		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d%H%M");

		// 设置IPC日志文件
		str.Format( "%s\\Log\\IPC%s.csv", exeFullPath, strTemp );
		if( m_hFileIPC != NULL )
			CloseHandle( m_hFileIPC );
		m_hFileIPC	= NULL;
		m_hFileIPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsIPC = 1;
		// 写IPC日志文件头
		if( m_hFileIPC != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s\r\n", "IPC" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( m_hFileIPC, Buf, strlen(Buf), &cbRet, NULL );
		}
	}

	PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 2, m_cfgIPC.bReady );


	BOOL bStart = __super::Start();
	return bStart;
}


void CIPCStatusDetector::Release()
{
	m_IPC.Close();
	m_cfgIPC.bReady	= m_IPC.IsOpened();
	if( m_hFileIPC != NULL )
		CloseHandle( m_hFileIPC );
	m_hFileIPC	= NULL;

	__super::Release();
}


DWORD CIPCStatusDetector::DoJob()
{
	if( !m_bPauseComm )
	{
		// 1秒钟通讯一次，读一次真空值
		CommWithIPC(100);
	}
	Sleep( 1 );

	return 0L;
}


int CIPCStatusDetector::CommWithIPC( int nSleep )
{
	char dataSend[255] = {0};
	if( m_nIPtype < 1 )
	{
		int nLenSend = CCommonVACFunction::Instance().ConvertComData22( "U01?", dataSend );
		m_IPC.SendData(dataSend, nLenSend);	//发送数据
	}
	else
	{
		int nLenSend = CCommonVACFunction::Instance().ConvertCommData_Win2( m_strWinIPC, 'R', "", dataSend );
		m_IPC.SendData(dataSend, nLenSend);	//发送数据
	}

	Sleep(nSleep);

	char dataRecv[255] = {0};
	int nLenRecv = m_IPC.ReadData( dataRecv, 19 );	// nLenSend要根据实际情况改
	if( nLenRecv < 2 )
	{
		Sleep( 1 );
		return 2;
	}

	CString str, strOut, strTemp = "";
	for(int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	m_strIPC = m_strIPCSaved + strTemp;

	// 分析数据
	double dValue = 0.0;
	int nExp = 0;
	int n = AnalyzeRecvData( m_strIPC, strOut, dValue, nExp );
	if( n < 1 )
	{
		m_strIPC		= "";
		m_strIPCSaved	= "";
		if( m_bIsSameChn )
		{
			RecordVacuum(strOut );
			m_cfgIPC.dVac = dValue;
			if( !g_bPausePost )
				PostMessage( m_hWnd, WM_USER_VAC_UpdateIPCValue, 0, (LPARAM)( &m_cfgIPC ) );
		}
		return 0;
	}
	else
	{
		if( n == 2 )
			m_strIPCSaved = m_strIPC;
		//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPCValue, 13, -8 );
		m_strIPCSaved = m_strIPC;
		if( m_strIPC.GetLength() > 6 )
			m_strIPCSaved = "";
		return 3;
	}
}


void CIPCStatusDetector::RecordVacuum( CString strVacuum )
{
	unsigned long cbRet;
	char Buf[255] = {0};
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, strVacuum );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileIPC, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsIPC++;
	if( m_nLoopsIPC > 10000 )
	{
		CloseHandle( m_hFileIPC );
		// 设置日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		CTime	timeCurrent = CTime::GetCurrentTime();
		csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\IPC%s.csv", exeFullPath, csCurrentTime );
		m_hFileIPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsIPC = 1;

		str.Format( "\t%s\r\n", "IPC" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileIPC, Buf, strlen(Buf), &cbRet, NULL );
	}
}


int CIPCStatusDetector::AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp )
{
	CString str;
	strOut = "";

	BYTE bt = strIn[0];
	BOOL bIsRecvData = TRUE;
	int nRet = 0;
	switch( bt )
	{
	case	0x24:
		nRet	= CCommonVACFunction::Instance().ConvertReceiveData( strIn, str );
		break;
	case	0x01:
		nRet	= CCommonVACFunction::Instance().ConvertReceiveData2( strIn, str );
		break;
	case	0x02:
		nRet	= CCommonVACFunction::Instance().ConvertReceiveData_Win( strIn, str );
		break;
	default:
		bIsRecvData	= FALSE;
		break;
	}
	if( !bIsRecvData )
		return 1;	// 不是符合协议的数据

	if( nRet == 2 || nRet == 3 )
	{
		// 数据包不完整的时候先保存
		//		m_strDataSaved = strIn;
		return 2;
	}
	if( nRet == 4 )
	{
		// 发生check error
		return 3;
	}

	int nPosCmd		= str.Find( "Cmd=" );
	int nPosData	= str.Find( "Data=" );
	if( nPosCmd < 0 || nPosData < 0 )
		return 4;	// 没有找到相关标志

	if( bt != 0x02 )
	{
		CString strCmd = str.Mid(nPosCmd+4, 2);
		if( strCmd == "U0" )
		{
			// 真空值，用于监测、比较
			int nLen = str.GetLength();
			strOut = str.Right( nLen -nPosData -5 );
			CCommonVACFunction::Instance().ScientificToDouble( strOut, dValue, nExp );
			return 0;
		}
	}
	else
	{
		CString strCmd = str.Mid(nPosCmd+4, 3);
		if( (strCmd == "812") || (strCmd == "822")  || (strCmd == "832") || (strCmd == "842"))
		{
			// 真空值，用于监测、比较
			int nLen = str.GetLength();
			strOut = str.Right( nLen -nPosData -5 );
			CCommonVACFunction::Instance().ScientificToDouble( strOut, dValue, nExp );

			if( strCmd == m_strWinIPC )
			{
				RecordVacuum( strOut );
				//if( !g_bPausePost )
					PostMessage( m_hWnd, WM_USER_VAC_UpdateIPCValue, 0,(LPARAM)(&m_cfgIPC) );
			}
			return 0;
		}
	}

	return 5;	// 不是真空值
}