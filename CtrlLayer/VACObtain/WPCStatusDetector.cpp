#include "StdAfx.h"
#include "WPCStatusDetector.h"
#include "CommonVACFunction.h"
#include "CRC_Calc.h"
#include <math.h>

CWPCStatusDetector& CWPCStatusDetector::Instance()
{
	static CWPCStatusDetector Inst;
	return Inst;
}


CWPCStatusDetector::CWPCStatusDetector(void)
{
	m_hWnd = NULL;

	ZeroMemory( &m_cfgWPC, sizeof(CfgVAC_IP_CCG) );
	m_cfgWPC.nType = 4;

	m_strWPC = "";
	m_strWPCSaved = "";
	m_strWinWPC = "";

	m_hFileWPC	= NULL;
	m_nLoopsWPC = 0;

	m_bPausePost = FALSE;

	ZeroMemory( &m_boardStatusServer, sizeof(StatusFEGBrdVAC_board) );
	ZeroMemory( &m_boardStatusMain, sizeof(StatusFEGBrdVAC_board) );

	m_dwSleep = 100;
}


void CWPCStatusDetector::Init( HWND hWnd,int nPort)
{
	m_hWnd = hWnd;
	m_cfgWPC.nPort = nPort;
}

void CWPCStatusDetector::InitThreshold(double dThreshhold1,double dThreshhold2)
{
	m_cfgWPC.dThreshold1 = dThreshhold1;
	m_cfgWPC.dThreshold2 = dThreshhold2;
}

int CWPCStatusDetector::WPC_WriteToDevice( int nSleep, BYTE* data, int nLength )
{
	if( !m_cfgWPC.bReady )
		return -1;
	int nLenSend = m_WPC.SendDataEx(data, nLength);
	Sleep(nSleep);
	return nLenSend;
}

int CWPCStatusDetector::WPC_ReadFromDevice( int nSleep, BYTE* data )
{
	if( !m_cfgWPC.bReady )
		return -1;

	int nLenRecv = 0;
	int nLenTotal = 0;
	int nCount = 0;
	do{
//		Sleep(10);
		nLenRecv = m_WPC.ReadData( &(data[nLenTotal]), 255 );
		if( nLenRecv < byte_LenReqRecv )
		{
			// 数据不完整
			Sleep(nSleep);
		}
		else
		{
			nLenTotal += nLenRecv;
			break;
		}
		nLenTotal += nLenRecv;
		nCount++;
	} while( (nLenTotal < 1) && (nCount < 5) );
	return nLenTotal;
}

void CWPCStatusDetector::WPC_ConvertSendReqData( BYTE Addr, BYTE* data )
{
	data[0] = byte_Sn;
	data[1]	= byte_FuncCodeR;
	data[2]	= byte_AddrH;
	data[3]	= Addr;
	data[4]	= byte_ReadLenH;
	data[5] = byte_ReadLenL;
	unsigned short crc = CRC16_MODBUS2( data, 6 );
	data[6] = (BYTE)(crc & 0x00FF);
	data[7] = (BYTE)((crc & 0xFF00) >> 8);
}

void CWPCStatusDetector::WPC_ConvertSendSetData( BYTE Addr, DWORD dwValue, BYTE* data )
{
	data[0] = byte_Sn;
	data[1]	= byte_FuncCodeW;
	data[2]	= byte_AddrH;
	data[3]	= Addr;
	data[4]	= byte_ReadLenH;
	data[5]	= byte_ReadLenL;
	data[6]	= byte_ReadLen;
	data[7]	= (BYTE)((dwValue & 0xFF000000) >> 24);
	data[8]	= (BYTE)((dwValue & 0x00FF0000) >> 16);
	data[9]	= (BYTE)((dwValue & 0x0000FF00) >> 8);
	data[10]= (BYTE)(dwValue & 0x000000FF);
	unsigned short crc = CRC16_MODBUS2( data, 11 );
	data[11] = (BYTE)(crc & 0x00FF);
	data[12] = (BYTE)((crc & 0xFF00) >> 8);
}

short CWPCStatusDetector::WPC_ConvertReceiveData( int nLen, BYTE* data )
{
	if( nLen < byte_LenReqRecv )
		return -1;	// 不够一帧数据

	int nCount = nLen /byte_LenReqRecv;	// 一次收到多少帧数据
	int i = 0;
	unsigned short crc1, crc2;
	CString str;
	for( int j=0; j<nCount; j++ )
	{
// 		str.Format( "recv = %02X, %02X, %02X, %02X, %02X, %02X",
// 			data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5] );
// 		RecordTest(str);
		i = j*byte_LenReqRecv;
		if( data[i] != byte_Sn )
		{
			if( j < nCount )
				continue;
			else
				return -2;	// 仪表编号不对
		}
		if( data[i+1] != byte_FuncCodeR )
		{
			if( j < nCount )
				continue;
			else
				return -3;	// 功能代码不对
		}
		if( data[i+2] != byte_ReadLen )
		{
			if( j < nCount )
				continue;
			else
				return -4;	// 读取字节数不对
		}
		crc1 = CRC16_MODBUS2( &(data[i]), 7 );
		crc2 = (data[i+8] << 8) | (BYTE)(data[i+7]);
// 		crc2 <<= 8;
// 		crc2 |= (BYTE)(data[i+5]);
		if( crc1 != crc2 )
			return -5;		// CRC不对

		str.Format( "%c.%cE%c%c", data[i+3], data[i+4], data[i+5], data[i+6] );
		RecordVacuum( str );
		m_cfgWPC.dVac = atof( str );// /133.33;	// 上传为Pa值，要转换为Torr
		if( !m_bPausePost )
			PostMessage( m_hWnd, WM_USER_VAC_UpdateCCGValue, 0, (LPARAM)(&m_cfgWPC) );
		m_strWPC = "";
		return 0;

/*		BYTE bOpc = data[i+1];
		if( bOpc == byte_opcStatus1 )	// 状态1
		{
			BYTE byteStatus = data[i+2];
//			if( byteStatus != g_boardStatusServer.sVac )
			{
				g_boardStatusServer.Vac.bStatus = byteStatus;

				/////////////////////////////////////////////////////
				// 分析状态
				//const	BYTE	byte_opcStatus1	= 0x11;		// 返回真空状态
				// 参数1：0xXY
				//	X：	0 初始化状态；1 样品室大气状态；2 电子枪大气状态
				//		3 抽高真空状态；4 抽低真空状态；
				//		5 高真空状态；6 低真空状态；7 TMP运行错误状态
				//	Y：	0bMKQP
				//		M：1 枪真空错误状态；0 枪真空正常状态
				//		KQP 保留
				//	参数2：0x00
				// 分析参数1
//				g_boardStatusServer.bX = (byteStatus & 0x70) >> 4;		// 高4位中的低3位表示X
//				g_boardStatusServer.bY = byteStatus & 0x0F;				// 低4位表示Y
				/////////////////////////////////////////////////////

				PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcStatus1, (LPARAM)( &g_boardStatusServer.Vac ) );
			}
		}
		else if( bOpc == byte_opcStatus2 )	// 状态2
		{
			WORD wStatus = (data[i+3] << 8) | data[i+2];
//			if( wStatus != g_boardStatusServer.sValve.wStatus )
			{
				g_boardStatusServer.Valve.wStatus = wStatus;
				/////////////////////////////////////////////////////
				// 分析状态
				//const	BYTE	byte_opcStatus2	= 0x21;		// 返回阀门状态
				//	参数1：0b ABCD EFGH （1为开，0为关）
				//		A:V8；B:V7；C:V6；D:V5；E:V4；F:V3：G:V2；H:V1
				//	参数2：0b ABCD EFGH （1为开，0为关）
				//		A:保留；B:保留；C:保留；D:TMP；E:CCG；F:MP2：G:MP1；H:V9
				/////////////////////////////////////////////////////
				PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcStatus2, (LPARAM)( &g_boardStatusServer.Valve ) );
			}
		}
*/	}
	return 0;
}

WORD CWPCStatusDetector::WPC_SendReq( int nSleep, BYTE Addr )
{
	if( !m_cfgWPC.bReady )
		return -1;
	CSingleLock lock(&m_cs);
	BYTE dataSend[8];
	WPC_ConvertSendReqData( Addr, dataSend );
	BYTE nLenSend = (BYTE)WPC_WriteToDevice(nSleep, dataSend, byte_LenReqSend);

	BYTE dataRecv[255];
	BYTE nLenRecv = WPC_ReadFromDevice( nSleep, dataRecv );
	if( nLenRecv > 0 )
	{
		short ret = WPC_ConvertReceiveData( nLenRecv, dataRecv );
	}
	return ((nLenRecv << 8) | nLenSend);
}

int CWPCStatusDetector::WPC_SendSet( int nSleep, BYTE Addr, DWORD dwValue )
{
	if( !m_cfgWPC.bReady )
		return -1;

	CSingleLock lock(&m_cs);
	BYTE dataSend[13];
	WPC_ConvertSendSetData( Addr, dwValue, dataSend );
	int nSend = WPC_WriteToDevice(nSleep, dataSend, byte_LenSetSend);

	BYTE dataRecv[255];
	int nLenRecv = WPC_ReadFromDevice( nSleep, dataRecv );
	return nSend;
}

static DWORD Float2DWORD(const float f) 
{
	unsigned char *hex = (unsigned char *)&f;
	char* sHex = new char[2*8 + 1];
	memset(sHex,0,17);
	sprintf(sHex,"%02X%02X%02X%02X",hex[3],hex[2],hex[1],hex[0]);	// 高位在前
	DWORD dValue = (DWORD)strtoul(sHex, NULL, 16);
	delete sHex;
	sHex = NULL;
	return dValue;
}

BOOL CWPCStatusDetector::Start()
{
	// 打开端口
	m_cfgWPC.bReady = m_WPC.Open( m_cfgWPC.nPort );
	if( m_cfgWPC.bReady )
	{
		//		WPC_SendReq( nWPC_Sleep, byte_AddrReq1 );		// 常规查询
		if( CommWithWPC(100) > 0 )
			m_cfgWPC.bReady = FALSE;
		if( m_cfgWPC.bReady )
		{
			/////////////////////////////////////////////////////////////////
			// 2017.11.10 For Test
			// 设置日志文件
			unsigned long cbRet;
			char Buf[255];
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			CString str, csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\WPC%s.csv", exeFullPath, csCurrentTime );
			m_hFileWPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			m_nLoopsWPC = 1;
			str.Format( "\t%s\r\n", "WPC" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( m_hFileWPC, Buf, strlen(Buf), &cbRet, NULL );
			/////////////////////////////////////////////////////////////////
			DWORD dwValue = Float2DWORD( (float)(m_cfgWPC.dThreshold1) );
			WPC_SendSet( m_dwSleep, byte_AddrSet1, dwValue );
			WPC_SendSet( m_dwSleep, byte_AddrSet1, dwValue );
			m_cfgWPC.dThreshold2 = m_cfgWPC.dThreshold1 *1.1;	// 110% 设定值
		}
	}
	PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 5, m_cfgWPC.bReady );

	BOOL bStart = __super::Start();
	return bStart;
}


void CWPCStatusDetector::Release()
{
	m_WPC.Close();
	m_cfgWPC.bReady	= m_WPC.IsOpened();
	if( m_hFileWPC != NULL )
		CloseHandle( m_hFileWPC );
	m_hFileWPC	= NULL;

	__super::Release();
}


DWORD CWPCStatusDetector::DoJob()
{
	if( !m_bPauseComm )
	{
		CommWithWPC(m_dwSleep);
	}
	Sleep( 1 );

	return 0L;
}


int CWPCStatusDetector::CommWithWPC( int nSleep )
{
	if( !m_cfgWPC.bReady )
		return 1;
	WPC_SendReq( nSleep, byte_AddrReq1 );		// 常规查询
	return 0;
}


void CWPCStatusDetector::RecordVacuum( CString strVacuum )
{
	unsigned long cbRet;
	char Buf[255];
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, strVacuum );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileWPC, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsWPC++;
	if( m_nLoopsWPC > 10000 )
	{
		CloseHandle( m_hFileWPC );
		// 设置日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		CTime	timeCurrent = CTime::GetCurrentTime();
		csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\WPC%s.csv", exeFullPath, csCurrentTime );
		m_hFileWPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsWPC = 1;

		str.Format( "\t%s\r\n", "WPC" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileWPC, Buf, strlen(Buf), &cbRet, NULL );
	}
}