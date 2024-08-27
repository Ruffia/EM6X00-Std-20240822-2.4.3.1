// USB_Recv.cpp : implementation file
//

#include "stdafx.h"
#include "USB_Recv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSB_Recv

IMPLEMENT_DYNCREATE(CUSB_Recv, CWinThread)

CUSB_Recv::CUSB_Recv()
{
	m_bAutoDelete	= FALSE;
	m_blClose		= FALSE;
	m_blPause		= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_p232Device	= NULL;
	m_hMain			= NULL;
	m_nListIndex	= 0;
}

CUSB_Recv::~CUSB_Recv()
{
	m_blClose = TRUE;

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CUSB_Recv::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	// loop but check for kill notification
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;  // TRUE;
}

int CUSB_Recv::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUSB_Recv, CWinThread)
	//{{AFX_MSG_MAP(CUSB_Recv)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSB_Recv message handlers

void CUSB_Recv::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CUSB_Recv::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));
//	m_blClose = TRUE;

	// allow thread to run at higher priority during kill process
	//	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}

void CUSB_Recv::MainLoop()
{
	// 接收数据
	BYTE bBuf[1024];
//	unsigned char bBuf[1024];
	DWORD dwBytesReturned = 0;
	if( m_p232Device->Serial_Read( bBuf, dwBytesReturned ) == 0 )
	{
		// 处理读回数据
		if( dwBytesReturned < 1 )
		{
			Sleep(1);
			return;
		}
		USB_SEM_ControlMessage	usb_hostMSG;
		usb_hostMSG.bType			= 0x00;
		usb_hostMSG.bCtlMSGLength	= (BYTE)dwBytesReturned;
		for( DWORD i=0; i<dwBytesReturned; i++ )
			usb_hostMSG.bData[i]	= bBuf[i];
		PostMessage( m_hMain, WM_USB_SERIAL_RECV, (WPARAM)(&usb_hostMSG), 0 );
	}

	Sleep(1);
}

void CUSB_Recv::AnalyzeReadbackData( BYTE* bBuf, DWORD dwLen )
{
	if( dwLen % frameLen2 == 0 )
	{
		// BD端口状态 或者 MCI电源回读
		if( (bBuf[fHead] == byte_HeadRec)
			&& (bBuf[fLen] == byte_LenSnd1)
			&& (bBuf[frameLen2-1] == byte_TailRec) )
		{
			//
		}
	}
	else if( (dwLen % 4 == 0) || (dwLen % 6 == 0) )
	{
		// 0xD22D开头的子卡回读信息
	}
}
/////////////////////////////////////////////////////////////////////////////
// CUSB_Recv  down-interface
BOOL CUSB_Recv::NotifyMSGInfo( USB_SEM_SIO_Package& sspTemp, UINT nFlag = 0 )
{
/*	////////////////////////////////////////////////
	// Filter Inquirer MSG
	////////////////////////////////////////////////
	if( sspTemp.flag.mc == SEM_Z80ID_Z80B )
	if( (sspTemp.msg.bMessageID == (BYTE)Z80B_Inquirer) ||
		(sspTemp.msg.bMessageID == (BYTE)Z80B_HeatingFilamentElectricity) )
	{
		return TRUE;
	}

	if ( (sspTemp.flag.mc == SEM_Z80ID_Z80A)
		&& (sspTemp.msg.bMessageID == (BYTE)Z80A_Inquirer) )
	{
		return TRUE;
	}
	////////////////////////////////////////////////
*/
	CString csInfo;
	csInfo.Format("\tL=%2d ID=%2X, [%2X,%2X,%2X,%2X,%2X]",
		sspTemp.msg.bCtlMSGLength,
		sspTemp.msg.bMessageID,
		sspTemp.msg.bData[1],sspTemp.msg.bData[2],
		sspTemp.msg.bData[3],sspTemp.msg.bData[4],
		sspTemp.msg.bData[5] );

	if( sspTemp.msg.bType == 0 )
		csInfo = _T("MCI:") + csInfo;
	else
		csInfo = _T("SUB:") + csInfo;

	if ( nFlag == 0 )
		csInfo = _T("P-") + csInfo;
	else 
		csInfo = _T("S-") + csInfo;

	PostErrorInformation( csInfo );

	return TRUE;
}

BOOL CUSB_Recv::PostMSGtoHost( USB_SEM_ControlMessage& mcMSG )
{
	if ( !m_fifoRecv.IsFIFOFull() )
	{
		USB_SEM_SIO_Package		sspTemp;

		sspTemp.msg				= mcMSG;
		sspTemp.flag.nType		= m_pSPlist[m_nListIndex].nType;
		sspTemp.flag.nNum		= m_pSPlist[m_nListIndex].nNum;
		sspTemp.flag.nMID		= m_pSPlist[m_nListIndex].nMID;
		sspTemp.flag.nChn		= m_pSPlist[m_nListIndex].nNum;
		sspTemp.flag.bAnalyze	= m_pSPlist[m_nListIndex].bAnalyze;
		m_nListIndex++;
		if( m_nListIndex > 127 )
			m_nListIndex = 0;

		POSITION posRet = m_fifoRecv.Input( sspTemp );
		if( posRet != (POSITION)(-1) )
		{
			m_ReceiveRate.CountLoopRate();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_Recv::PostErrorInformation( CString& csError )
{
	if ( m_fifoError.IsFIFOFull() )
	{
		CString		sspTemp;
		m_fifoError.Output( sspTemp );
	}

	CString	csTickCount;
	CString csTime = CTime::GetCurrentTime().Format("[%H:%M:%S]");
	const DWORD thisTickCount = timeGetTime();
	//csTickCount.Format( "(0x%08X) ", thisTickCount );
	csTickCount.Format( "[%010Ld] ", thisTickCount );
	csTime	+= csTickCount;
	csError = csTime + csError;

// 	if ( m_MicrocomputerID == SEM_Z80ID_Z80A )
// 		csError = _T("PC1-MP(A)->") + csError;
// 	else if ( m_MicrocomputerID == SEM_Z80ID_Z80B )
// 		csError = _T("PC9-MP(B)->") + csError;

	return m_fifoError.Input( csError ) != (POSITION)(-1);
}

BOOL CUSB_Recv::GetMSGfromMicrocomputer( USB_SEM_ControlMessage& mcMSG )
{
	if ( !m_fifoRecv.IsFIFOEmpty() )
	{
		USB_SEM_SIO_Package		sspTemp;

		if ( m_fifoRecv.Output( sspTemp ) )
		{
			mcMSG = sspTemp.msg;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_Recv::GetLastErrorInformation( CString& csError )
{
	if ( !m_fifoError.IsFIFOEmpty() )
	{
		CString		sspTemp;

		if ( m_fifoError.Output( sspTemp ) )
		{
			csError = sspTemp;

			return TRUE;
		}
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CUSB_Recv::SetSharedParams( int nIndex, void* pParams )
{
	switch( nIndex )
	{
	case	0:
		m_pSPcfgADCs	= (SharedParams_Cfg_ADCs*)pParams;
		break;
	case	1:
		m_pSPbackADCs	= (SharedParams_Back_ADCs*)pParams;
		break;
	case	2:
		m_pSPbackOthers	= (SharedParams_Back_Others*)pParams;
		break;
	case	3:
		m_pSPlist		= (SharedParams_Record_List*)pParams;
		break;
	default:
		break;
	}
}
