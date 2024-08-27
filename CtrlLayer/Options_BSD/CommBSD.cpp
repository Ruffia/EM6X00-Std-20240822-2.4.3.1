// CommBSD.cpp : implementation file
//

#include "stdafx.h"
#include "Options_BSD.h"
#include "CommBSD.h"
#include "Serial.h"
#include <afxtempl.h>		// MFC support for Template
#include <cfifo.h>
#include <afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD WINAPI CommThreadBSD(LPVOID lpParam);
#define	nBSD_Sleep			20
/*
HANDLE	g_hThreadBSD;
DWORD	g_dwThreadBSD;
HANDLE	g_hEventWorkBSD		= CreateEvent(NULL,FALSE, FALSE, _T("WORKbsd"));
HANDLE	g_hEventTeminateBSD	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEbsd"));
char	g_szBSDSaved[255];
*/
HWND	g_hMain;
CSerial	g_BSD;

HANDLE	g_hFileBSD = NULL;
int		g_nLoopsBSD;

BOOL	g_bBSDOpen = FALSE;
BOOL	g_bNeedCommTest = FALSE;	// 默认是老用户，不支持通讯测试命令
int		g_nBSDGradeChn, g_nBSDGradeC;

/////////////////////////////////////////////////////////////////////////////
const BYTE BSD_SendToDevice_DataSize	= 5;
const BYTE byte_BSDhead = 0xF3;
const BYTE byte_BSDtail = 0x3F;

typedef struct tagBSD_MESSAGE
{
	BYTE	bCtrlMSGLength;			// Avialable bData Length
	BYTE	bData[ BSD_SendToDevice_DataSize ];
}BSD_Msg;
/////////////////////////////////////////////////////////////////////////////

static BOOL m_bEndThreads = FALSE;
CRITICAL_SECTION	csFifoBSD;
CFIFO<BSD_Msg, BSD_Msg&>	m_fifoHostMsg(200);

/////////////////////////////////////////////////////////////////////////////
// CCommBSD

CCommBSD::CCommBSD()
{
	g_nBSDGradeChn	= -1;
	g_nBSDGradeC	= -1;
}

CCommBSD::~CCommBSD()
{
}


void Record( CString strData )
{
	if( g_hFileBSD != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s],%s", csCurrentTime, strData );

		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFileBSD, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoopsBSD++;
		if( g_nLoopsBSD > 10000 )
		{
			CloseHandle( g_hFileBSD );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\BSD%s.txt", exeFullPath, csCurrentTime );
			g_hFileBSD = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBSD = 1;

			str.Format( "%s\r\n", "BSD: send data" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileBSD, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
int BSD_WriteToDevice( int nSleep, BYTE* data, int nLength )
{
	if( !g_bBSDOpen )
		return -1;
	int nLenSend = g_BSD.SendDataEx(data, nLength);
	Sleep(nSleep);
	return nLenSend;
}

BOOL BSD_SendCmd( BYTE byteOpc )
{
	if( !g_bBSDOpen )
		return -1;

	BSD_Msg msg;
	msg.bData[0]	= byte_BSDhead;
	msg.bData[1]	= byteOpc;
	msg.bData[2]	= byteOpc;
	msg.bData[3]	= msg.bData[1] ^ msg.bData[2];
	msg.bData[4]	= byte_BSDtail;
	msg.bCtrlMSGLength = BSD_SendToDevice_DataSize;
	m_fifoHostMsg.Input(msg);
	return TRUE;
}

BOOL BSD_SendCmd2( BYTE byteOpc, WORD wOpr )
{
	if( !g_bBSDOpen )
		return -1;

	BSD_Msg msg;
	msg.bData[0]	= byte_BSDhead;
	msg.bData[1]	= byteOpc;
	msg.bData[2]	= (BYTE)((wOpr & 0xFF00 ) >> 8);
	msg.bData[3]	= (BYTE)(wOpr & 0x00FF);
//	msg.bData[4]	= data[1] ^ data[2] ^ data[3];
	msg.bData[4]	= byte_BSDtail;
	msg.bCtrlMSGLength = BSD_SendToDevice_DataSize;
	m_fifoHostMsg.Input(msg);
	return TRUE;
}

int CommWithBSD( int nSleep )
{
	if( !g_bBSDOpen )
		return 1;
	return 0;
}

int BSD_CommTest()
{
// 	2024.06 增加探测器测试命令：
// 	上位机发送：F3 08 08 00 3F
// 	下位机回应：F3 21 21 00 3F
	BYTE bDataSend[5] = {0};
	bDataSend[0] = byte_BSDhead;
	bDataSend[1] = 0x08;
	bDataSend[2] = 0x08;
	bDataSend[3] = 0x00;
	bDataSend[4] = byte_BSDtail;
	int nLenSend = g_BSD.SendDataEx(bDataSend, BSD_SendToDevice_DataSize);
	Sleep( nBSD_Sleep );

	BYTE bDataRecv[255] = {0};
	int nLenTotal = 0;
	int nLenRecv = 0;
	int nTimes = 0;
	do{
		nLenRecv = g_BSD.ReadData( bDataRecv +nLenTotal, 5 );
		if( nLenRecv > 4 )
			break;
		nLenTotal += nLenRecv;
		nTimes++;
		Sleep( nBSD_Sleep );
	} while( nTimes < 10 );

	char szBuf[255] = {0}, szTemp[10] = {0};
	sprintf( szBuf, "BSD: CommTest send %d bytes: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n", nLenSend, bDataSend[0], bDataSend[1], bDataSend[2], bDataSend[3], bDataSend[4] );
	OutputDebugString( szBuf );
	sprintf( szBuf, "BSD: CommTest recv %d bytes (Times = %d): ", nLenRecv, nTimes );
	for( int i=0; i<nLenRecv; i++ )
	{
		sprintf( szTemp, "0x%02X, ", bDataRecv[i] );
		strcat( szBuf, szTemp );
	}
	strcat( szBuf, "\r\n" );
	OutputDebugString( szBuf );

	if( nLenRecv < BSD_SendToDevice_DataSize )
	{
		OutputDebugString( "BSD: in CommTest, nLenRecv < BSD_SendToDevice_DataSize, return -1.\r\n" );
		return -1;
	}
	else
	{
		if( (bDataRecv[0] != byte_BSDhead) || (bDataRecv[1] != 0x21) || (bDataRecv[2] != 0x21) || (bDataRecv[3] != 0x00) || (bDataRecv[4] != byte_BSDtail) )
		{
			OutputDebugString( "BSD: in CommTest, bytes not match, return -2.\r\n" );
			return -2;
		}
	}
	return 0;
}

DWORD WINAPI CommThreadBSD(LPVOID lpParam)
{
/*	// 打开端口
	g_bBSDOpen = g_BSD.Open( (short)lpParam );
	if( g_bBSDOpen )
	{
		WORD wSend = BSD_SendCmd( nLVBSD_Sleep, byte_opcStatus1, byte_opcStatus1 );		// 常规查询
		if( wSend < 0x0105 )	// 发送5字节，回读大于1字节
			g_bBSDOpen = FALSE;
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
			// 设置BSD日志文件
			str.Format( "%s\\Log\\BSD%s.csv", exeFullPath, strTemp );
			if( g_hFileBSD != NULL )
				CloseHandle( g_hFileBSD );
			g_hFileBSD	= NULL;
			g_hFileBSD	= CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBSD	= 1;
			// 写BSD日志文件头
			if( g_hFileBSD != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "%s\r\n", "BSD: status1, status2, CCG, TC1, TC2, TC3" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileBSD, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	PostMessage( g_hMain, WM_USER_BSDVAC_UpdateStatus, 4, g_bBSDOpen );

	while( g_bBSDOpen && WaitForSingleObject(g_hEventTeminateBSD,500) != WAIT_OBJECT_0 )
	{
		if( !g_bPauseComm )
		{
			// 0.5秒钟通讯一次
			CommWithBSD(100);
			if( !g_bPauseInquire )
			{
			}
		}
		Sleep( 1 );
	}

	g_BSD.Close();
	g_bBSDOpen	= g_BSD.IsOpened();
	if( g_hFileBSD != NULL )
		CloseHandle( g_hFileBSD );
	g_hFileBSD	= NULL;

	SetEvent( g_hEventWorkBSD );
*/
	// 打开端口
	g_bBSDOpen = g_BSD.Open( (short)lpParam );
	if( g_bBSDOpen )
	{
		char szBuf[255] = {0};
		sprintf( szBuf, "BSD: open COM%d successful.\r\n", (short)lpParam );
		OutputDebugString( szBuf );
		if( g_bNeedCommTest )
		{
			// 老用户没有进行硬件程序更新，不支持通讯测试命令
			if( BSD_CommTest() < 0 )
				g_bBSDOpen = FALSE;
		}
	}
	PostMessage( g_hMain, WM_USER_BSD_Update, 0, g_bBSDOpen );
	if( g_bBSDOpen )
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
		// 设置BSD日志文件
		str.Format( "%s\\Log\\BSD%s.txt", exeFullPath, strTemp );
		if( g_hFileBSD != NULL )
			CloseHandle( g_hFileBSD );
		g_hFileBSD	= NULL;
		g_hFileBSD	= CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		g_nLoopsBSD	= 1;
		// 写BSD日志文件头
		if( g_hFileBSD != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "%s\r\n", "BSD: send data" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileBSD, Buf, strlen(Buf), &cbRet, NULL );
		}

		// 默认选择A通道，对比度粗调为X10，对比度细调为07FF
		g_nBSDGradeChn = 0;
		BSD_SendCmd( 0x01 );
		g_nBSDGradeC = 0;
		BSD_SendCmd( 0x05 );
		BSD_SendCmd2( 0x11, 0x07FF );

		while (TRUE)
		{
			if( m_bEndThreads )
			{
// 				LeaveCriticalSection(&csPort);
//	 			LeaveCriticalSection(&csFifoBSD);
				break;
			}

// 			EnterCriticalSection(&csPort);
//			EnterCriticalSection(&csFifoBSD);

			if( !m_fifoHostMsg.IsFIFOEmpty() )
			{
				BSD_Msg msg;
				m_fifoHostMsg.Output(msg);
				str.Format( "Send: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X",
					msg.bData[0], msg.bData[1], msg.bData[2], msg.bData[3], msg.bData[4] );
				Record(str);
				g_BSD.SendDataEx(msg.bData, msg.bCtrlMSGLength);
			}

// 			LeaveCriticalSection(&csPort);
//			LeaveCriticalSection(&csFifoBSD);

			Sleep(nBSD_Sleep);
		}
	}
	g_BSD.Close();
	g_bBSDOpen	= g_BSD.IsOpened();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCommBSD message handlers
void CCommBSD::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}

BOOL CCommBSD::Open( short port )
{
/*	g_szBSDSaved[0] = '\0';

	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEventWorkBSD );
	ResetEvent( g_hEventTeminateBSD );

	g_hThreadBSD	= CreateThread( NULL, 0, CommThreadBSD, (LPVOID)(port), 0, &g_dwThreadBSD );
	if( g_hThreadBSD == NULL )
		return FALSE;

	CloseHandle(g_hThreadBSD);
*/
	m_bEndThreads = FALSE;
	InitializeCriticalSection(&csFifoBSD);
	HANDLE hThread = CreateThread(NULL, 0, CommThreadBSD, (LPVOID)(port), 0, NULL);
	if( hThread == NULL )
		return FALSE;
	CloseHandle(hThread);
	return TRUE;
}

BOOL CCommBSD::Close()
{
/*	if( g_bBSDOpen )
	{
		SetEvent( g_hEventTeminateBSD );
		WaitForSingleObject( g_hEventWorkBSD, INFINITE );
		g_bBSDOpen	= FALSE;
	}
*/
	m_bEndThreads = TRUE;
	if( g_hFileBSD != NULL )
		CloseHandle( g_hFileBSD );
	g_hFileBSD	= NULL;
	Sleep(2000);
	DeleteCriticalSection(&csFifoBSD);
	return TRUE;
}

void CCommBSD::SetCommTest( BOOL bNeed )
{
	g_bNeedCommTest = bNeed;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 通道选择：（默认为高电平，选中的通道为高电平）
// A	F3 01 01 00 3F
// B	F3 02 02 00 3F
// A+B	F3 03 03 00 3F
// A-B	F3 04 04 00 3F
BOOL CCommBSD::ChangeChannel( int nIndex )
{
	if( g_nBSDGradeChn != nIndex )
	{
		g_nBSDGradeChn = nIndex;
		CString str;
		switch( nIndex )
		{
		case	0:
			str.Format( "Change channel to A.");
			break;
		case	1:
			str.Format( "Change channel to B.");
			break;
		case	2:
			str.Format( "Change channel to A+B.");
			break;
		case	3:
			str.Format( "Change channel to A-B.");
			break;
		}
		Record(str);
		BSD_SendCmd( (BYTE)(nIndex +1) );
	}
	return TRUE;
}

// 对比度粗调：
// X10	F3 05 05 00 3F
// X50	F3 06 06 00 3F
// X100	F3 07 07 00 3F
BOOL CCommBSD::ChangeCoarse( int nIndex )
{
	if( g_nBSDGradeC != nIndex )
	{
		g_nBSDGradeC = nIndex;
		CString str;
		switch( nIndex )
		{
		case	0:
			str.Format( "Change coarse to x10.");
			break;
		case	1:
			str.Format( "Change coarse to x50.");
			break;
		case	2:
			str.Format( "Change coarse to x100.");
			break;
		}
		Record(str);
		BSD_SendCmd( (BYTE)(nIndex +5) );
	}
	return TRUE;
}

// 对比度细调：F3 11 XX XX 3F （其中XX XX 为0000-FFFF）
BOOL CCommBSD::ChangeFine( WORD wValue )
{
	CString str;
	str.Format( "Change fine to 0x03X, %.3f", wValue, 100.0 *wValue /0xFFFF );
	BSD_SendCmd2( 0x11, wValue );
	return TRUE;
}

// 亮度调节：F3 44 XX XX 3F （其中XX XX 为0000-0FFF）
BOOL CCommBSD::ChangeBrt( WORD wValue )
{
	CString str;
	str.Format( "Change brt to 0x03X, %.2f", wValue, 100.0 *wValue /0xFFF );
	BSD_SendCmd2( 0x44, wValue );
	return TRUE;
}
