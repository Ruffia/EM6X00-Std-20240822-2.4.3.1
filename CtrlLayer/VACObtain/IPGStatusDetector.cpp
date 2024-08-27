#include "StdAfx.h"
#include "IPGStatusDetector.h"
#include "CommonVACFunction.h"

CIPGStatusDetector& CIPGStatusDetector::Instance()
{
	static CIPGStatusDetector Inst;
	return Inst;
}


CIPGStatusDetector::CIPGStatusDetector(void)
{
	m_hWnd = NULL;

	ZeroMemory( &m_cfgIPG, sizeof(CfgVAC_IP_CCG) );
	m_cfgIPG.nType = 2;

	m_strIPG = "";
	m_strIPGSaved = "";
	m_strWinIPG = "";

	m_hFileIPG	= NULL;
	m_nLoopsIPG = 0;

	m_nIPtype		= 1;		// 离子泵控制器是老式还是新式（影响通讯协议），默认为新式
	m_bPauseComm	= FALSE;	// 是否暂停与设备通讯

	m_bIsDual		= FALSE;	// 离子泵控制器是否是双通道的

	m_bIsSameChn    = FALSE;
	m_bPausePost    = FALSE;

	m_dwSleep = 250;
}


void CIPGStatusDetector::Init( HWND hWnd,int nPort, BOOL bIntype, BOOL bIsDual)
{
	m_hWnd = hWnd;
	m_cfgIPG.nPort = nPort;
	m_nIPtype = bIntype;
	m_bIsDual = bIsDual;
}

void CIPGStatusDetector::SetChannel(int nChn,CString strIPCChn)
{
	m_strWinIPG.Format( "8%d2", nChn );
	if( strIPCChn == m_strWinIPG )
		m_bIsSameChn = TRUE;
}

BOOL CIPGStatusDetector::Start()
{
	// 打开端口
	BOOL bCommOpen	= m_IPG.Open( m_cfgIPG.nPort );
	if (!bCommOpen)
	{
		OutputDebugString("CIPGStatusDetector---------- BOOL bCommOpen = m_IPG.Open( m_cfgIPG.nPort ); Failed.\r\n");
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 1, bCommOpen );
		return FALSE;
	}


	if( CommWithIPG( 200 ) > 1 )
		m_cfgIPG.bReady = FALSE;
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

		// 设置IPG日志文件
		str.Format( "%s\\Log\\IPG%s.csv", exeFullPath, strTemp );
		if( m_hFileIPG != NULL )
			CloseHandle( m_hFileIPG );
		m_hFileIPG	= NULL;
		m_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsIPG = 1;
		// 写IPG日志文件头
		if( m_hFileIPG != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s\r\n", "IPG" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( m_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
		}

		if( m_bIsDual )
		{
			// 设置IPG日志文件
			str.Format( "%s\\Log\\IPG%s.csv", exeFullPath, strTemp );
			if( m_hFileIPG != NULL )
				CloseHandle( m_hFileIPG );
			m_hFileIPG	= NULL;
			m_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			m_nLoopsIPG = 1;
			// 写IPG日志文件头
			if( m_hFileIPG != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "IPG" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( m_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}

	PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 1, m_cfgIPG.bReady );
	if( m_bIsDual )
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 2, m_cfgIPG.bReady );

	BOOL bStart = __super::Start();
	return bStart;
}


void CIPGStatusDetector::Release()
{
	m_IPG.Close();
	m_cfgIPG.bReady	= m_IPG.IsOpened();
	if( m_hFileIPG != NULL )
		CloseHandle( m_hFileIPG );
	m_hFileIPG	= NULL;

	__super::Release();
}


DWORD CIPGStatusDetector::DoJob()
{
	if( !m_bPauseComm )
	{
		// 1秒钟通讯一次，读一次真空值
		CommWithIPG(100);
	}
	Sleep( 1 );

	return 0L;
}


int CIPGStatusDetector::CommWithIPG( int nSleep )
{
	int nLenSend, nLenRecv, nRet = 0;
	char dataSend[255], dataRecv[255];
	CString str, strOut, strTemp = "";
	double dValue = 0.0;
	int i, n, nExp = 0;

	if( m_nIPtype < 1 )
	{
		nLenSend = CCommonVACFunction::Instance().ConvertComData22( "U01?", dataSend );
		m_IPG.SendData(dataSend, nLenSend);	//发送数据
	}
	else
	{
		nLenSend = CCommonVACFunction::Instance().ConvertCommData_Win2( m_strWinIPG, 'R', "", dataSend );
		m_IPG.SendData( dataSend, nLenSend );
	}

	Sleep(nSleep);

	nLenRecv = m_IPG.ReadData( dataRecv, 19 );	// nLenSend要根据实际情况改
	if( nLenRecv < 2 )
	{
		Sleep( 1 );
		nRet = 2;
	}

	if( nRet < 2 )
	{
		for( i=0; i<nLenRecv; i++ )
		{
			str.Format( "%c", dataRecv[i] );
			strTemp += str;
		}
		m_strIPG = m_strIPGSaved + strTemp;

		// 分析数据
		n = CCommonVACFunction::Instance().AnalyzeRecvData( m_strIPG,m_bIsSameChn, strOut, dValue, nExp );
		if( n < 1 )
		{
			m_strIPG		= "";
			m_strIPGSaved	= "";
			nRet = 0;
			if( m_bIsSameChn )
			{
				RecordVacuum( strOut );
				m_cfgIPG.dVac = dValue;
				if( !m_bPausePost )
					PostMessage( m_hWnd, WM_USER_VAC_UpdateIPGValue, 0, (LPARAM)( &m_cfgIPG ) );
			}
		}
		else
		{
			if( n == 2 )
				m_strIPGSaved = m_strIPG;
			//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPGValue, 72, -10 );
			m_strIPGSaved = m_strIPG;
			if( m_strIPG.GetLength() > 6 )
				m_strIPGSaved = "";
			nRet = 3;
		}
	}

	if( !m_bIsDual )
		return nRet;

	nLenSend = CCommonVACFunction::Instance().ConvertCommData_Win2( m_strWinIPG, 'R', "", dataSend );
	m_IPG.SendData( dataSend, nLenSend );

	Sleep(nSleep);

	nLenRecv = m_IPG.ReadData( dataRecv, 19 );	// nLenSend要根据实际情况改
	if( nLenRecv < 2 )
	{
		Sleep( 1 );
		nRet = 2;
	}

	if( nRet < 2 )
	{
		strTemp = "";
		for( i=0; i<nLenRecv; i++ )
		{
			str.Format( "%c", dataRecv[i] );
			strTemp += str;
		}
		m_strIPG = m_strIPGSaved + strTemp;

		// 分析数据
		n = CCommonVACFunction::Instance().AnalyzeRecvData( m_strIPG, m_bIsSameChn,strOut, dValue, nExp );
		if( n < 1 )
		{
			m_strIPG		= "";
			m_strIPGSaved	= "";
			nRet = 0;
		}
		else
		{
			if( n == 2 )
				m_strIPGSaved = m_strIPG;
			//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPGValue, 72, -10 );
			m_strIPGSaved = m_strIPG;
			if( m_strIPG.GetLength() > 6 )
				m_strIPGSaved = "";
			nRet = 3;
		}
	}
	return nRet;
}


void CIPGStatusDetector::RecordVacuum( CString strVacuum )
{
	unsigned long cbRet;
	char Buf[255];
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, strVacuum );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileIPG, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsIPG++;
	if( m_nLoopsIPG > 10000 )
	{
		CloseHandle( m_hFileIPG );
		// 设置日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		CTime	timeCurrent = CTime::GetCurrentTime();
		csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\IPG%s.csv", exeFullPath, csCurrentTime );
		m_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsIPG = 1;

		str.Format( "\t%s\r\n", "IPG" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
	}
}


int CIPGStatusDetector::AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp )
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

			if( strCmd == m_strWinIPG )
			{
				RecordVacuum( strOut );
				//if( !g_bPausePost )
				PostMessage( m_hWnd, WM_USER_VAC_UpdateIPGValue, 0,(LPARAM)(&m_cfgIPG) );
			}
			return 0;
		}
	}

	return 5;	// 不是真空值
}