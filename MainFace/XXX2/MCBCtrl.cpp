// MCBCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "MCBCtrl.h"
#include "MCBControler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	CString	MCB_FindHIDInterface( int nVID, int nPID, int nUsagePage );
extern	HANDLE	MCB_OpenForRead( CString strFileName );
extern	HANDLE	MCB_OpenForWrite( CString strFileName );
extern	void	MCB_ErrorExit( CString strReason );

/////////////////////////////////////////////////////////////////////////////
// CMCBCtrl

IMPLEMENT_DYNCREATE(CMCBCtrl, CWinThread)

CMCBCtrl::CMCBCtrl()
{
	m_bAutoDelete	= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_strMCB_DeviceName	= _T("");
	m_bMCB_Openned		= FALSE;
	
	m_hMCB_Write		= NULL;
	m_hMCB_Read			= NULL;
	m_hMCB_ReadEvent	= NULL;
	m_MCB_overlapRead;
}

CMCBCtrl::~CMCBCtrl()
{
	MCB_CloseHIDDevice();

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CMCBCtrl::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;  // TRUE;
}

int CMCBCtrl::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMCBCtrl, CWinThread)
	//{{AFX_MSG_MAP(CMCBCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCBCtrl message handlers
BOOL CMCBCtrl::InitData()
{
	m_strMCB_DeviceName = MCB_OpenHIDDevice();
	MCB_InitHIDDevice( m_strMCB_DeviceName );

	m_lastTickCount = GetTickCount();
	return TRUE;
}


void CMCBCtrl::Delete()
{
	// calling the base here won't do anything but it is a good habit
//	DWORD	thisTickCount, lastTickCount;
	
//	lastTickCount = GetTickCount();
	CWinThread::Delete();
	
	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
//	thisTickCount = GetTickCount();
//	TRACE( "CBasisScan::Delete time = %ld.\r\n", thisTickCount - lastTickCount );
}

void CMCBCtrl::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.
	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));

	// allow thread to run at higher priority during kill process
	//	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}

void CMCBCtrl::MainLoop()
{
/*	if ( m_boolFinished )
	{
		Sleep( 13-5 );
		return;
	}
*/
/*	if ( m_dwExpectantInterval < 13 )
		Sleep( m_dwExpectantInterval );
	else
		Sleep( 13-5 );
	Sleep( m_dwExpectantInterval );

	if ( !InputData() )
		return;

	OutputData();*/
	Sleep( 1 );
	DWORD thisTickCount = GetTickCount();
    if( thisTickCount - m_lastTickCount > 10000 )	// 10秒钟查一次
	{
		// I check for the IO device attach or removal every 3 seconds
		// I do this by closing the device and reopenning it
		g_nMCB_PollCounter = 0;
		if( m_bMCB_Openned )
			MCB_CloseHIDDevice();
		m_strMCB_DeviceName = MCB_OpenHIDDevice();
		if( m_strMCB_DeviceName != "" )
		{
			m_hMCB_Read		= MCB_OpenForRead( m_strMCB_DeviceName );
			m_hMCB_Write	= MCB_OpenForWrite( m_strMCB_DeviceName );
			m_bMCB_Openned	= TRUE;
		}
		m_lastTickCount = GetTickCount();
	}

	if( m_hMCB_Read )
	{
		// Start an asynchronous read, only wait up to 80msec
		DWORD dwBytesRead = MCB_StartRead( &g_byteMCB_ReadReport[0], 2 );	// Read = Data from device TO host
		long lResult = WaitForSingleObject( m_hMCB_ReadEvent, 5 );	// 80 = 80mS wait for non-response
		ResetEvent( m_hMCB_ReadEvent );
		if( lResult == WAIT_TIMEOUT )
			lResult = CancelIo( m_hMCB_Read );				// Read was not successful, so cancel the request
		else
		{
			// Read was successful and a change of button state was reported
			g_byteMCB_Port2RBdata = g_byteMCB_ReadReport[1];				// As of 6-21-10, ONLY element 1 has data, which is 2nd byte of 2.

			if( g_byteMCB_Port2RBdata >= 0 )
			{
				// TEMP!!!!  SUSPEND REPORT IF DATA if it's zero
				// Change this condition to something smarter, and segregate status report info vs. encoder+sw data
				// Map data from MCB PLD to corresponding form components:
				// First, identify whether data is status or a change of switch or encoder position(s):
				CString str;
				str.Format( "MCB Message: 0x%02X", g_byteMCB_Port2RBdata );
				theApp.DisplayMsg( str );

				if( !m_bMCB_ExpectHighByte && !m_bMCB_ExpectLowByte )
				{
					// Time Stamp ( count stamp )
					int nRet = MCB_ReceiveCountStamp( g_byteMCB_Port2RBdata );
					if( nRet < 0 )
					{
						// not count stamp
						theApp.DisplayMsg( "EER: not count stamp" );
						// try high byte
						nRet = MCB_ReceiveHighByte( g_byteMCB_Port2RBdata );
						if( nRet == 0 )
						{
							theApp.DisplayMsg( "and not hi, abandon" );
							theApp.DisplayMsg( " " );
							m_bMCB_ExpectHighByte	= FALSE;
							m_bMCB_ExpectLowByte	= FALSE;
							return;
						}
						theApp.DisplayMsg( "EER: it is high" );
					}
					else
						g_byteMCB_Port2RBdataBak = g_byteMCB_Port2RBdata;	// if it is stamp, backup it
				}
				else if( !m_bMCB_ExpectLowByte )
				{
					BOOL bRet = MCB_ReceiveHighByte( g_byteMCB_Port2RBdata );
					if( !bRet )
					{
						theApp.DisplayMsg( "EER: not high byte" );
						// not high byte, maybe it is low byte
						// so the backup data maybe high byte
						bRet = MCB_ReceiveHighByte( g_byteMCB_Port2RBdataBak );
						if( bRet )
						{
							if( MCB_ReceiveLowByte( g_byteMCB_Port2RBdata ) )
							{
								CMCBControler::Instance().MCB_Ctrl_Flush_WP( g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS, theApp.m_blUnlock_HV, theApp.m_blOverhaulState );
								theApp.DisplayMsg( "EER: the backup is hi, HI & LO are both OK" );
								theApp.DisplayMsg( " " );
							}
							else
							{
								// if the backup data is not high byte, abandon
								theApp.DisplayMsg( "EER: Neither backup or now is high byte, abandon" );
								theApp.DisplayMsg( " " );
								m_bMCB_ExpectHighByte	= FALSE;
								m_bMCB_ExpectLowByte	= FALSE;
							}
						}
					}
					return;
				}	// (If NOT expecting low byte)
				else
				{
					if( MCB_ReceiveLowByte( g_byteMCB_Port2RBdata ) )
					{
						CMCBControler::Instance().MCB_Ctrl_Flush_WP( g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS, theApp.m_blUnlock_HV, theApp.m_blOverhaulState );
						theApp.DisplayMsg( " " );
					}
					else
					{
						theApp.DisplayMsg( "EER: not low byte, try count stamp" );
						theApp.DisplayMsg( " " );
						MCB_ReceiveCountStamp( g_byteMCB_Port2RBdata );
					}
				}
			}	// End of "If(Port2RBdata >= 0)" condition
		}	// End of "If USB Read Was Successful"
	}
}

CString CMCBCtrl::MCB_OpenHIDDevice()
{
	// I know this is a HID device with a specific VID and PID.
	// I also search for the UsagePage that defines the data reports
	// I return the system name for the device or NULL if the device is not plugged in
	CString strSystemName = _T("");
	// check for examples 1 and 4
	strSystemName = MCB_FindHIDInterface( 0x4242, 0xEE01, 0xFF00 );
	if( strSystemName == "" )
		strSystemName = MCB_FindHIDInterface( 0x4242, 0xEE04, 0xFF00 );		// Set in USB_descr.asm

	return strSystemName;
}

BOOL CMCBCtrl::MCB_InitHIDDevice( CString strDeviceName )
{
	///////////////////////////////////////////////////////////////////////////
	// I do asyncnronous reads so I need an object to wait on
	m_hMCB_ReadEvent = CreateEvent(0, FALSE, FALSE, 0);
	if( m_hMCB_ReadEvent == NULL )
		MCB_ErrorExit( "Could not create ReadEvent" );

	m_MCB_overlapRead.Offset		= 0;
	m_MCB_overlapRead.OffsetHigh	= 0;
	m_MCB_overlapRead.hEvent		= m_hMCB_ReadEvent;
	///////////////////////////////////////////////////////////////////////////

	g_nMCB_PollCounter				= 100;	// Force an initial search for the IO device
	// 设置时间戳初始值
	g_nMCB_CountStamp				= 0;

    if (strDeviceName == "")
        return FALSE;
    else
	{
		m_hMCB_Read		= MCB_OpenForRead(strDeviceName);
		m_hMCB_Write	= MCB_OpenForWrite(strDeviceName);
		m_bMCB_Openned	= TRUE;
	}

	// 接收数据缓冲区清零
	memset( g_byteMCB_ReadReport, 0, sizeof(g_byteMCB_ReadReport) );

	// 最先接收时间戳，设置标志位
	m_bMCB_ExpectLowByte	= FALSE;
	m_bMCB_ExpectHighByte	= FALSE;

	return TRUE;
}

void CMCBCtrl::MCB_CloseHIDDevice()
{
	if( m_hMCB_Read != NULL )
	{
		CloseHandle( m_hMCB_Read );
		m_hMCB_Read = NULL;
	}
	if( m_hMCB_Write != NULL )
	{
		CloseHandle( m_hMCB_Write );
		m_hMCB_Write = NULL;
	}

	ResetEvent( m_hMCB_ReadEvent );

	m_bMCB_Openned = FALSE;
}

DWORD CMCBCtrl::MCB_StartRead( BYTE* pBuf, int nCount )
{
	// Also called by (n)mS polling loop (via timer)
	// Note that this HID report has a 0 byte as the first byte that is not sent on the bus
	// Windows uses this as a ReportID
	// 在每次读/写数据前都要先接收/发送1字节的PID标志，所以每次读/写数据的时候都要多一个字节。
	// 例如：每次读/写的是64字节数据，但是在这64字节之前必须放1字节的PID数据，所以就是65字节。
	// 一般这个PID字节的值为0.
	DWORD dwBytesRead = 0;
	// Data arrives TO HOST via USB HERE
	BOOL bSuccess = ReadFile( m_hMCB_Read, pBuf, nCount, &dwBytesRead, &m_MCB_overlapRead );

	return dwBytesRead;
}

void CMCBCtrl::MCB_SendReport( BYTE byteBuf, int nCount )
{
	// Note that this HID report has a 0 byte as the first byte that is not sent on the bus
	// Windows uses this as a ReportID
	DWORD dwBytesWritten;
	// Rubber meets the road HERE
	BOOL bSuccess = WriteFile( m_hMCB_Write, &byteBuf, nCount, &dwBytesWritten, NULL );
	if( !bSuccess || (dwBytesWritten != (DWORD)nCount) )
		MCB_ErrorExit( "Error in SendReport" );
}

int CMCBCtrl::MCB_ReceiveCountStamp( BYTE bData )
{
	int nRet = 1;
	if( (bData & 0x01) != 0x01 )
	{
		nRet = -1;
		return nRet;
	}

	int nCountStamp = bData >> 1;		// 右移1位
	if( nCountStamp != g_nMCB_CountStamp )
	{
		g_nMCB_CountStamp = nCountStamp;
		nRet = 0;
	}

	g_nMCB_CountStamp += 1;
	if( g_nMCB_CountStamp == 0x80 )
		g_nMCB_CountStamp = 0;

	m_bMCB_ExpectHighByte = TRUE;
	return nRet;
}

BOOL CMCBCtrl::MCB_CheckHighByte( BYTE bData )
{
	BOOL bRet = FALSE;

/*	switch( bData )
	{
	case	0x04:
	case	0x07:
	case	0x14:
	case	0x17:
	case	0x28:
	case	0x2B:
	case	0x34:
	case	0x37:
	case	0x44:
	case	0x47:
	case	0x58:
	case	0x5B:
	case	0x68:
	case	0x6B:
	case	0x78:
	case	0x7B:
	case	0x84:
	case	0x87:
		bRet = TRUE;
		break;
	}
*/
	switch( bData )
	{
	case	0x04:
	case	0x07:

	case	0x08:
	case	0x0B:

	case	0x14:
	case	0x17:

	case	0x18:
	case	0x1B:

	case	0x24:
	case	0x27:

	case	0x28:
	case	0x2B:

	case	0x34:
	case	0x37:

	case	0x38:
	case	0x3B:

	case	0x44:
	case	0x47:

	case	0x48:
	case	0x4B:

	case	0x54:
	case	0x57:

	case	0x58:
	case	0x5B:

	case	0x64:
	case	0x67:

	case	0x68:
	case	0x6B:

	case	0x74:
	case	0x77:
	case	0x78:
	case	0x7B:

	case	0x84:
	case	0x87:

	case	0x88:
	case	0x8B:
		bRet = TRUE;
		break;
	}

	return bRet;
}

BOOL CMCBCtrl::MCB_ReceiveHighByte( BYTE bData )
{
	if( !MCB_CheckHighByte( bData ) )
		return FALSE;

	int nChanNow;
	BOOL bChanGood, bSwPosGood, bRotationGood, bHiByteGood;	// Sanity checks in case out of sync

	// Process current byte as a HIGH byte, then reject if not valid
	// Expect High byte first pass, then low byte 2nd pass:
	// 15 14 13 12 || 11 || 10 || 09 || 08 07 06 05 04 03 02 01 00
	// Knob ID	   || fA || fB || neg|| net pulses
	nChanNow = (bData & 0xF0) /16;
	if( nChanNow < 9 )					// 9 knobs total
		bChanGood = TRUE;
	else
		bChanGood = FALSE;

	g_nMCB_SwPos = (bData & 0x0C) /4;
	if( (g_nMCB_SwPos == 1) || (g_nMCB_SwPos == 2) )	// 1 means fB, 2 means fA
		bSwPosGood = TRUE;
	else
		bSwPosGood = FALSE;

	g_nMCB_TicksHi = ( bData & 0x03 );
	if( (g_nMCB_TicksHi == 3) || (g_nMCB_TicksHi == 0) )
		bRotationGood = TRUE;
	else
		bRotationGood = FALSE;

	if( bChanGood && bSwPosGood && bRotationGood )
		bHiByteGood = TRUE;
	else
		bHiByteGood = FALSE;

	if( bHiByteGood )
	{
		g_nMCB_Channel = nChanNow;

		m_bMCB_ExpectLowByte	= TRUE;		// Finished processed what appeared to be a high byte, so expect low byte next
		m_bMCB_ExpectHighByte	= FALSE;
	}

	return bHiByteGood;
}

BOOL CMCBCtrl::MCB_ReceiveLowByte( BYTE bData )
{
	BOOL bRet = TRUE;
	// Data byte is LOW, so append 8 bit TicksLo to 2 bit TicksHi value for new encoder position
	int	nMCB_TicksLo	= bData;
	int	nMCB_Ticks		= g_nMCB_TicksHi *256 +nMCB_TicksLo;
	if( nMCB_Ticks > 0x39F )	// > 0x39F, so CCW
	{
		g_nMCB_TicksABS	= 1024 - nMCB_Ticks;
		g_nMCB_RotDir	= -1;
	}
	else						// CW Rotation
	{
		g_nMCB_TicksABS	= nMCB_Ticks;
		g_nMCB_RotDir	= 1;
	}
	if( g_nMCB_TicksABS > 0x40 )		// so big value
		bRet = FALSE;

	m_bMCB_ExpectLowByte	= FALSE;		// Since low byte was just processed, expect count stamp next //high byte next
	m_bMCB_ExpectHighByte	= FALSE;

	return bRet;
}
