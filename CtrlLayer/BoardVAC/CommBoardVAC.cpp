// CommBoardVAC.cpp : implementation file
//

#include "stdafx.h"
#include "CommBoardVAC.h"

#include "Serial.h"
#include <BoardVAC.h>

#include <math.h>
#include <afxmt.h>
#include <mmsystem.h>		// system timer support

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	CCriticalSection		m_mutexPortBoard;

#define	nBoard_Sleep			20

DWORD WINAPI CommThreadRVC(LPVOID lpParam);
DWORD WINAPI CommThreadBoard(LPVOID lpParam);

HANDLE	g_hThreadBoard;
DWORD	g_dwThreadBoard;
HANDLE	g_hEventWorkBoard		= CreateEvent(NULL,FALSE, FALSE, _T("WORKboard"));
HANDLE	g_hEventTeminateBoard	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEboard"));

HWND	g_hMain;
CSerial	g_board;
CfgVAC_Ports	g_cfgBoard;
VACstatus6K_Board	g_boardStatusServer, g_boardStatusMain;
VACvalue6K_CCG		g_valueCCG;
VACvalue6K_TC		g_valueTC;

char	g_szBoardSaved[255];

HANDLE	g_hFileBoard= NULL;
HANDLE	g_hFileTest	= NULL;
int		g_nLoopsBoard, g_nLoopsTest;
DWORD	g_lastMon	= 0;

BOOL	g_bReady		= FALSE;	// 是否至少有一个设备联通
BOOL	g_bPauseComm	= FALSE;	// 是否暂停与设备通讯
BOOL	g_bPausePost	= FALSE;	// 是否暂停上送监测值（但不停止与设备通讯）
BOOL	g_bPauseInquire	= FALSE;
BOOL	g_bInit			= TRUE;
int		g_nGaugeType	= 3;		// 真空计类型，3为INFICON，4为WPC
									// 需选用不同的电压真空度转换公式

/////////////////////////////////////////////////////////////////////////////
// CCommBoardVAC

CCommBoardVAC::CCommBoardVAC()
{
	g_hMain	= NULL;
	ZeroMemory( &g_cfgBoard, sizeof(CfgVAC_Ports) );
	ZeroMemory( &g_boardStatusServer, sizeof(VACstatus6K_Board) );
	ZeroMemory( &g_boardStatusMain, sizeof(VACstatus6K_Board) );
}

CCommBoardVAC::~CCommBoardVAC()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCommBoardVAC message handlers

void RecordTest( CString strRecord )
{
	if( g_hFileTest != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s],%s", csCurrentTime, strRecord );

		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFileTest, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoopsTest++;
		if( g_nLoopsTest > 10000 )
		{
			CloseHandle( g_hFileTest );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\Test%s.csv", exeFullPath, csCurrentTime );
			g_hFileTest = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsTest = 1;

			str.Format( ",%s\r\n", "Comm Test" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileTest, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

void RecordVacuum( CString strVacuum )
{
	if( g_hFileBoard != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s],%s", csCurrentTime, strVacuum );

		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFileBoard, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoopsBoard++;
		if( g_nLoopsBoard > 10000 )
		{
			CloseHandle( g_hFileBoard );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\Board%s.csv", exeFullPath, csCurrentTime );
			g_hFileBoard = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBoard = 1;

			str.Format( "%s\r\n", "Board: status, CCG, TC3, TC4" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileBoard, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
int Board_WriteToDevice( int nSleep, char* data, int nLength )
{
	if( !g_cfgBoard.bReady )
		return -1;
	int nLenSend = g_board.SendData(data, nLength);
	Sleep(nSleep);
	return nLenSend;
}

int Board_ReadFromDevice( int nSleep, BYTE* data )
{
	if( !g_cfgBoard.bReady )
		return -1;
/*	
	//	data[0] = '\0';
	strcpy( data, g_szBoardSaved );
	g_szBoardSaved[0] = '\0';
	
	char buf[255];
	int nLen, nLenRecv = 0;
	int nIndex = 0;
	do{
		nLen = g_board.ReadData( buf, 6 );
		buf[nLen] = '\0';
		strcat( data, buf );
		nLenRecv += nLen;
		nIndex++;
	}while(	(strrchr(buf, '2F') == NULL) && nIndex < 10 );
	CString str;
	str.Format( "Merge From Device [%d]: %s", nIndex, data );
	RecordTest( str );
	return nLenRecv;
*/
	int nLenRecv = 0;
	int nCount = 0;
	do{
		Sleep(10);
		nLenRecv = g_board.ReadData( data, 255 );
		if( nLenRecv < byte_BrdLenRec )
		{
			// 数据不完整
			Sleep(nSleep);
		}
		else
			break;
		nCount++;
	} while( (nLenRecv < 1) && (nCount < 5) );
// 	if( nCount == 5 )
// 		return -2;
	return nLenRecv;
}

void Board_ConvertSendData( BYTE byteOpc, BYTE byteOpr, char* data )
{
	data[0] = byte_BrdHead;
	data[1]	= byteOpc;
	data[2]	= byteOpr;
	data[3]	= data[1] ^ data[2];
	data[4]	= byte_BrdTail;
}

void Board_ConvertSendData2( BYTE byteOpc, WORD wOpr, char* data )
{
	data[0] = byte_BrdHead;
	data[1]	= byteOpc;
	data[2]	= (BYTE)((wOpr & 0xFF00 ) >> 8);
	data[3] = (BYTE)(wOpr & 0x00FF);
	data[4]	= data[1] ^ data[2] ^ data[3];
	data[5]	= byte_BrdTail;
}
	
int Board_ConvertReceiveData( int nLen, BYTE* data )
{
	if( nLen < byte_BrdLenRec )
		return 1;	// 不够一帧数据

	int nCount = nLen /byte_BrdLenRec;	// 一次收到多少帧数据
	int i = 0;
	CString str;
	for( int j=0; j<nCount; j++ )
	{
// 		str.Format( "recv = %02X, %02X, %02X, %02X, %02X, %02X",
// 			data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5] );
// 		RecordTest(str);
		i = j*byte_BrdLenRec;
		if( data[i] != byte_BrdHead )
		{
			if( j < nCount )
				continue;
			else
				return 2;	// 帧头不对
		}
		if( data[i+5] != byte_BrdTail )
		{
			if( j < nCount )
				continue;
			else
				return 3;	// 帧尾不对
		}

		BYTE chk = data[i+1] ^ data[i+2];
		chk ^= data[i+3];
		if( chk != data[i+4] )
		{
			if( j < nCount )
				continue;
			else
				return 4;	// 校验码不对
		}

		BYTE bOpc = data[i+1];
		if( bOpc == byte_opcStatus1 )	// 真空状态
		{
//			WORD wStatus = (data[i+3] << 8) | data[i+2];
			BYTE byteStatus = data[i+2];
//			if( byteStatus != g_boardStatusServer.sVac.wStatus )
			{
				g_boardStatusServer.Vac.bStatus = byteStatus;
				/////////////////////////////////////////////////////
				// 分析状态
				// const	BYTE	byte_opcStatus	= 0x11;		// 返回真空状态
				// 参数1：0xXY
				//	X：	0 初始化状态；1 样品室大气状态；2 电子枪大气状态
				//		3 样品室抽真空状态；4 电子枪抽真空状态
				//		5 样品室粗真空状态；6 电子枪粗真空状态；
				//		7 TMP运行状态；8 TMP运行错误状态
				//	Y：	0bMKQP
				//		M：1 样品室真空错误状态；0 样品室真空正常状态
				//		KQP 保留
				//	参数2：0x00
				/////////////////////////////////////////////////////
				PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcStatus1, (LPARAM)( &g_boardStatusServer.Vac ) );
			}
		}
		else if( bOpc == byte_opcStatus2 )	// 阀门状态
		{
			WORD wStatus = (data[i+3] << 8) | data[i+2];
//			if( wStatus != g_boardStatusServer.sValve.wStatus )
			{
				g_boardStatusServer.Valve.wStatus = wStatus;
				/////////////////////////////////////////////////////
				// 分析状态
				//const	BYTE	byte_opcStatus2	= 0x21;		// 返回阀门状态
				//	参数1：0b ABCD EFGH （1为开，0为关）
				//		A:CCD；B:TMP；C:CCG；D:V5；E:V4；F:V3：G:V2；H:V1
				//	参数2：0b ABCD EFGH （1为开，0为关）
				//		H:V1回读
				//		ABCDEFG保留
				/////////////////////////////////////////////////////
				PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcStatus2, (LPARAM)( &g_boardStatusServer.Valve ) );
			}
		}
		else if( bOpc == byte_opcCCG )	// CCG
		{
			if( !(g_boardStatusServer.Valve.status.CCG) )
				return 0;
			WORD wCCG = (data[i+2] << 8) | data[i+3];
			double dU = 1.0 * wCCG /0xFFFF *10;						// U = D /65535 *10 (V)
			if( g_nGaugeType > 3 )
			{
				// 宜准真空计
				// Page 4 of WPC400 真空计操作手册_宜准CNV3.pdf
				g_valueCCG.dVac = pow( 10, 1.222 *(dU -7.647) );		// U = 7.647 +0.818 *log10(P), P = 10 ^ 1.222*(U -7.647) (Torr)
			}
			else
			{
				// Inficon真空计
				// Page 29 of MPG400-TINA48E1_B_Web.pdf, Appendix A
				g_valueCCG.dVac = pow( 10, (dU *1.667 -11.46) );		// U = 6.875 +0.6 *log10(P), P = 10 ^ (1.667 *U -11.46) (Torr)
			}
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcCCG, (LPARAM)(&g_valueCCG) );
		}
		else if( bOpc == byte_opcTC3 )	// TC3
		{
			WORD wIPG = (data[i+2] << 8) | data[i+3];
			g_valueTC.fTC3 = 1.0 * wIPG /0xFFFF *10;				// U = D /65535 *10 (V)
//			g_boardStatusServer.dP = pow( 10, dU -5.625 );			// U = log10(P) +5.625, P = 10 ^ (U-5.625) (Torr)
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcTC3, (LPARAM)( &g_valueTC ) );
		}
		else if( bOpc == byte_opcTC4 )	// TC4
		{
			WORD wIPG = (data[i+2] << 8) | data[i+3];
			g_valueTC.fTC4 = 1.0 * wIPG /0xFFFF *10;				// U = D /65535 *10 (V)
//			g_boardStatusServer.dPipg = pow( 10, (dU -18.5) /2 );	// U = 2log10(P) +18.5, P = 10 ^ ((U-18.5) /2) (Torr)
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcTC4, (LPARAM)( &g_valueTC ) );
		}
		else if( bOpc == byte_opcWarning )
		{
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, byte_opcWarning, (LPARAM)(data[i+3]) );
			RecordVacuum( "接收到warning" );
		}
	}
	return 0;
}

WORD Board_SendCmd( int nSleep, BYTE byteOpc, BYTE byteOpr )
{
	if( !g_cfgBoard.bReady )
		return -1;
	m_mutexPortBoard.Lock();
	char dataSend[5];
	Board_ConvertSendData( byteOpc, byteOpr, dataSend );
	BYTE nLenSend = (BYTE)Board_WriteToDevice(nSleep, dataSend, byte_BrdLenSnd1);

// 	CString str;
// 	str.Format( "Send %2X bytes = %02X, %02X, %02X, %02X, %02X", nLenSend,
// 			(BYTE)(dataSend[0]),dataSend[1],dataSend[2],dataSend[3],dataSend[4]);
// 	RecordTest( str );

	if( byteOpc < byte_opcStatus1 )	// 放气、抽气、V1等命令无应答
	{
		m_mutexPortBoard.Unlock();
		return nLenSend;
	}

	BYTE dataRecv[255];
	BYTE nLenRecv = Board_ReadFromDevice( nSleep, dataRecv );
	if( nLenRecv > 0 )
		Board_ConvertReceiveData( nLenRecv, dataRecv );
	m_mutexPortBoard.Unlock();
	return ((nLenRecv << 8) | nLenSend);
}

int Board_SendCmd2( int nSleep, BYTE byteOpc, WORD wOpr )
{
	if( !g_cfgBoard.bReady )
		return -1;
	m_mutexPortBoard.Lock();
	char dataSend[6];
	Board_ConvertSendData2( byteOpc, wOpr, dataSend );
	int nSend = g_board.SendData(dataSend, byte_BrdLenSnd2);
	Sleep( nSleep );

// 	CString str;
// 	str.Format( "Send %d bytes = %02X, %02X, %02X, %02X, %02X, %02X", nSend,
// 		dataSend[0],dataSend[1],dataSend[2],dataSend[3],dataSend[4],dataSend[5]);
// 	RecordTest( str );
	m_mutexPortBoard.Unlock();
	return nSend;
}

int CommWithBoard( int nSleep )
{
	if( !g_cfgBoard.bReady )
		return 1;

	CString str;
	if( g_boardStatusServer.byteV1 != g_boardStatusMain.byteV1 )
	{
		BYTE nSend = (BYTE)Board_SendCmd( nBoard_Sleep, byte_opcV1, g_boardStatusMain.byteV1 );
		str.Format( "V1状态改变: %2X 字节", nSend );
		RecordVacuum( str );
		g_boardStatusServer.byteV1 = g_boardStatusMain.byteV1;
	}
	if( g_boardStatusServer.byteVent != g_boardStatusMain.byteVent )
	{
		if( g_boardStatusMain.byteVent == byte_opcVentChamb )	// 样品室放气
		{
			BYTE nSend = (BYTE)Board_SendCmd( nBoard_Sleep, byte_opcVentChamb, byte_opcVentChamb );
			str.Format( "发送样品室放气命令: %2X 字节", nSend );
			RecordVacuum( str );
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, 150 +byte_opcVentChamb, nSend );
		}
		if( g_boardStatusMain.byteVent == byte_opcVentGun )		// 电子枪放气
		{
			BYTE nSend = (BYTE)Board_SendCmd( nBoard_Sleep, byte_opcVentGun, byte_opcVentGun );
			str.Format( "发送电子枪放气命令: %2X 字节", nSend );
			RecordVacuum( str );
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, 150 +byte_opcVentGun, nSend );
		}
		g_boardStatusServer.byteVent = g_boardStatusMain.byteVent;
	}
	if( g_boardStatusServer.bytePump != g_boardStatusMain.bytePump )
	{
		CString str;
		if( g_boardStatusMain.bytePump == byte_opcPumpChamb )	// 样品室抽真空
		{
			BYTE nSend = (BYTE)Board_SendCmd( nBoard_Sleep, byte_opcPumpChamb, byte_opcPumpChamb );
			str.Format( "发送抽样品室真空命令: %2X 字节", nSend );
			RecordVacuum( str );
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, 150 +byte_opcPumpChamb, nSend );
		}
		if( g_boardStatusMain.bytePump == byte_opcPumpGun )		// 电子枪抽真空
		{
			BYTE nSend = (BYTE)Board_SendCmd( nBoard_Sleep, byte_opcPumpGun, byte_opcPumpGun );
			str.Format( "发送抽电子枪真空命令: %2X 字节", nSend );
			RecordVacuum( str );
			PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, 150 +byte_opcPumpGun, nSend );
		}
		g_boardStatusServer.bytePump = g_boardStatusMain.bytePump;
	}
/*
	BYTE dataRecv[255];
	int nLenRecv = 0;
	int nCount = 0;
	int nRet = -1;
	do{
		Sleep(nSleep);
		nLenRecv = g_board.ReadData( dataRecv, 255 );
		if( nLenRecv < 1 )
		{
			// 数据不完整
			Sleep(nSleep);
		}
		else
		{
			nRet = Board_ConvertReceiveData( nLenRecv, dataRecv );
			if( nRet == 0 )
				break;
		}
		nCount++;
	} while( (nLenRecv < 1) && (nCount < 5) );
	if( nCount == 5 )
		return 2;
	if( nRet > 0 )
		return 3;

	CString strOut, strTemp = "";
	int i;
	for( i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	g_strBoard = g_strBoardSaved + strTemp;
*/	return 0;
}

DWORD WINAPI CommThreadBoard(LPVOID lpParam)
{
	// 打开端口
	g_cfgBoard.bReady = g_board.Open( g_cfgBoard.nPort );
	if( g_cfgBoard.bReady )
	{
		WORD wSend = Board_SendCmd( nBoard_Sleep, byte_opcStatus1, byte_opcStatus1 );		// 常规查询
		if( wSend < 0x0105 )	// 发送5字节，回读大于1字节
			g_cfgBoard.bReady = FALSE;
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
			// 设置Board日志文件
			str.Format( "%s\\Log\\Board%s.csv", exeFullPath, strTemp );
			if( g_hFileBoard != NULL )
				CloseHandle( g_hFileBoard );
			g_hFileBoard	= NULL;
			g_hFileBoard	= CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBoard	= 1;
			// 写Board日志文件头
			if( g_hFileBoard != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "%s\r\n", "Board: status, CCG, TC3, TC4" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileBoard, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	PostMessage( g_hMain, WM_USER_BoardVAC_UpdateStatus, 4, g_cfgBoard.bReady );

	while( g_cfgBoard.bReady && WaitForSingleObject(g_hEventTeminateBoard,500) != WAIT_OBJECT_0 )
	{
		if( !g_bPauseComm )
		{
			// 0.5秒钟通讯一次
			CommWithBoard(100);
			if( !g_bPauseInquire )
			{
				Board_SendCmd( nBoard_Sleep, byte_opcStatus1, byte_opcStatus1 );		// 常规查询状态
				Board_SendCmd( nBoard_Sleep, byte_opcStatus2, byte_opcStatus2 );		// 常规查询状态
				// 2s查看一次TC/CCG值
				DWORD thisTickCount = GetTickCount();
				if( thisTickCount -g_lastMon > 1000 )
				{
					if( g_boardStatusServer.Valve.status.CCG )
						Board_SendCmd( nBoard_Sleep, byte_opcCCG, byte_opcCCG );		// 查询CCG状态
					Board_SendCmd( nBoard_Sleep, byte_opcTC3, byte_opcTC3 );			// 查询TC3状态
					Board_SendCmd( nBoard_Sleep, byte_opcTC4, byte_opcTC4 );			// 查询TC4状态
					g_lastMon = GetTickCount();
				}
				CString str;
				str.Format( "0x%02X, 0x%04X, %.1e, %.3f, %.3f",
							g_boardStatusServer.Vac.bStatus, g_boardStatusServer.Valve.wStatus,
							g_valueCCG.dVac, g_valueTC.fTC3, g_valueTC.fTC4 );
				RecordVacuum( str );
			}
		}
		Sleep( 1 );
	}

	g_board.Close();
	g_cfgBoard.bReady	= g_board.IsOpened();
	if( g_hFileBoard != NULL )
		CloseHandle( g_hFileBoard );
	g_hFileBoard	= NULL;

	SetEvent( g_hEventWorkBoard );
	return 0;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void CCommBoardVAC::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
		/////////////////////////////////////////
	case	4:
		g_cfgBoard.nPort= nValue;
		break;
	case	11:
		g_bPauseComm	= (BOOL)nValue;
		break;
	case	12:
		g_bPausePost	= (BOOL)nValue;
		break;
	case	13:
		g_bPauseInquire	= (BOOL)nValue;
		break;
		/////////////////////////////////////////
	case	21:	// 真空计类型，需选用不同的电压真空度转换公式
		g_nGaugeType	= nValue;
		break;
		/////////////////////////////////////////
	case	50:
		{
			BYTE data[byte_BrdLenRec];
			data[0] = 0xF2;
			data[1] = 0x11;
			data[2] = 0x70;
			data[3] = 0x36;
			data[4] = 0x57;
			data[5] = 0x2F;
			Board_ConvertReceiveData(byte_BrdLenRec, data);
		}
		break;

		/////////////////////////////////////////
	case	(100 +byte_opcVentChamb):
	case	(100 +byte_opcVentGun):
		g_boardStatusMain.byteVent	= (BYTE)nValue;
		g_boardStatusMain.bytePump	= 0x00;
		break;

	case	(100 +byte_opcPumpChamb):
	case	(100 +byte_opcPumpGun):
		g_boardStatusMain.bytePump	= (BYTE)nValue;
		g_boardStatusMain.byteVent	= 0x00;
		break;

	case	(100 +byte_opcV1):
		g_boardStatusMain.byteV1	= (BYTE)nValue;
		break;
	case	(100 +byte_opcCCD):
		Board_SendCmd( nBoard_Sleep, (BYTE)(nIndex-100), (BYTE)nValue );
		break;

	case	(100 +byte_opcTsTC3):
	case	(100 +byte_opcTsTC4):
	case	(100 +byte_opcTsCCG1):
	case	(100 +byte_opcTsCCG2):
		{
			WORD wOpr = (WORD)(nValue /1000.0 /10 *0xFFFF);
			Board_SendCmd2( nBoard_Sleep, (BYTE)(nIndex-100), wOpr );
		}
		break;

	case	(100 +byte_opcInterval):
		if( nValue < 60 )
			nValue = 60;
		if( nValue > 90 )
			nValue = 90;
		Board_SendCmd2( nBoard_Sleep, (BYTE)(nIndex-100), (WORD)nValue );
		break;

	case	(100 +byte_opcStatus1):
	case	(100 +byte_opcStatus2):
	case	(100 +byte_opcCCG):
	case	(100 +byte_opcTC3):
	case	(100 +byte_opcTC4):
		Board_SendCmd( nBoard_Sleep, (BYTE)(nIndex-100), (BYTE)nValue );
		break;
		/////////////////////////////////////////
	}
}

void CCommBoardVAC::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
/*	/////////////////////////////////////////////
	// 设置日志
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	
	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");
	// 设置Test日志文件
	str.Format( "%s\\Log\\Test%s.csv", exeFullPath, strTemp );
	if( g_hFileBoard != NULL )
		CloseHandle( g_hFileBoard );
	g_hFileTest	= NULL;
	g_hFileTest	= CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	g_nLoopsTest	= 1;
	// 写Test日志文件头
	if( g_hFileTest != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "%s\r\n", "Comm Test" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( g_hFileTest, Buf, strlen(Buf), &cbRet, NULL );
	}*/
}

BOOL CCommBoardVAC::Init( int nIndex )
{
	g_szBoardSaved[0] = '\0';

	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEventWorkBoard );
	ResetEvent( g_hEventTeminateBoard );

	g_hThreadBoard	= CreateThread( NULL, 0, CommThreadBoard, NULL, 0, &g_dwThreadBoard );
	if( g_hThreadBoard == NULL )
		return FALSE;

	CloseHandle(g_hThreadBoard);
	return TRUE;
}

BOOL CCommBoardVAC::Close( int nIndex )
{
	if( g_cfgBoard.bReady )
	{
		SetEvent( g_hEventTeminateBoard );
		WaitForSingleObject( g_hEventWorkBoard, INFINITE );
		g_cfgBoard.bReady	= FALSE;
		g_cfgBoard.nPort	= -1;
		ZeroMemory( &g_boardStatusServer, sizeof(VACstatus6K_Board) );
		ZeroMemory( &g_boardStatusMain, sizeof(VACstatus6K_Board) );
	}

	if( g_hFileTest != NULL )
		CloseHandle( g_hFileTest );
	g_hFileTest	= NULL;

	return TRUE;
}
