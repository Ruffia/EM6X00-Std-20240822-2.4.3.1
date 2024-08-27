// USB_TransceiverNew.cpp : implementation file
//

#include "stdafx.h"
#include "USB_TransceiverNew.h"
#include "UsbDI16A.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSB_TransceiverNew

IMPLEMENT_DYNCREATE(CUSB_TransceiverNew, CWinThread)

CUSB_TransceiverNew::CUSB_TransceiverNew(MicrocomputerID mcID)
{
	InitParameter( mcID );

	m_bAutoDelete	= FALSE;
	m_blClose		= FALSE;
	m_blPause		= FALSE;
	m_bLastIoRead	= 0;
	m_bNeedReadFIFO	= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CUSB_TransceiverNew::~CUSB_TransceiverNew()
{
	m_blClose = TRUE;

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

void CUSB_TransceiverNew::InitParameter( MicrocomputerID mcID )
{
	m_MicrocomputerID = mcID;
}

BOOL CUSB_TransceiverNew::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	// loop but check for kill notification
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
	{
		MainLoop();
		Sleep(1);
	}

	// avoid entering standard message loop by returning FALSE
	return FALSE;  // TRUE;
}

int CUSB_TransceiverNew::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUSB_TransceiverNew, CWinThread)
	//{{AFX_MSG_MAP(CUSB_TransceiverNew)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSB_TransceiverNew message handlers

void CUSB_TransceiverNew::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CUSB_TransceiverNew::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));
	m_blClose = TRUE;

	// allow thread to run at higher priority during kill process
//	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, 2000);
	WaitForSingleObject(m_hThread, 2000);

	// now delete CWinThread object since no longer necessary
	delete this;
}


/////////////////////////////////////////////////////////////////////////////
// CUSB_TransceiverNew state machine

BOOL CUSB_TransceiverNew::SetCoadjustParam( double dHV )
{
	return TRUE;
}

USB_TransceiverState CUSB_TransceiverNew::Listen()
{
	USB_TransceiverState NextState = USB_Transceiver_Error;	

	if ( isPause() )
	{
		Sleep(1);
		return USB_Transceiver_Pause;
	}

	if ( !m_fifoHostMSG.IsFIFOEmpty() )	// 先看是否有命令消息需要发送
		return USB_Transceiver_Send;

	// 13.01.25 测试中断时，该线程中先不读小端口，主程序中OnIdle读小端口；
	//			正常情况下，该线程中读，主程序中不读
	Sleep(1);
	return NextState;
	// 13.01.25
}

USB_TransceiverState CUSB_TransceiverNew::MainLoop()
{
	USB_TransceiverState State = USB_Transceiver_Error;

	if ( !m_fifoHostMSG.IsFIFOEmpty() )	// 先看是否有命令消息需要发送
	{
		USB_SEM_ControlMessage	hostMSG;
		
		if ( !CopyMSGfromHost( hostMSG ) )
			return State;
		//发送hostMSG
		if( hostMSG.bType == 0 )
		{
			// to MCI
			CUsbDI16A::Instance().USB_WriteToBoardDirect( hostMSG.bData[0], hostMSG.bData[1], hostMSG.bData[2], hostMSG.bData[3] );
		}
		else
		{
			// to other boards via MCI
			CUsbDI16A::Instance().USB_CommToBoardsDirect( hostMSG.bCtlMSGLength, hostMSG.bData );
		}

		// Delete the sent MSG from Host.
		// 结束处理
		CTimer				myTimer;
		myTimer.Reset( dwZ80COMMTimer_WaitEnd );
		while( !m_blClose )
		{
			if ( GetMSGfromHost( hostMSG ) )
			{
//				State = USB_Transceiver_Listen;
				break;
			}
			else if ( myTimer.IsTimeout() )
			{
#ifdef _DEBUG_ADD_ERRINFO_
				CString	csError;
				csError.Format( "GetMSGfromHost() Error! msgID = %4LX", hostMSG.bMessageID );
				PostErrorInformation( csError );
#endif
				break;
			}
			else
			{
				Sleep(1);
			}
		};
	}

/*	// 接收数据
	unsigned char rx[1024];
	DWORD dwBytesReturned;
	if( CUsbDI16A::Instance().USB_IoPortRead( rx, dwBytesReturned ) )
	{
		// 处理读回数据
	}
	Sleep(1);
*/	
	return State;
}


/////////////////////////////////////////////////////////////////////////////
// CUSB_TransceiverNew  down-interface
BOOL CUSB_TransceiverNew::NotifyMSGInfo( USB_SEM_SIO_Package& sspTemp, UINT nFlag = 0 )
{
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

BOOL CUSB_TransceiverNew::PostMSGtoHost( USB_SEM_ControlMessage& mcMSG )
{
	if ( !m_fifoZ80MSG.IsFIFOFull() )
	{
		USB_SEM_SIO_Package		sspTemp;
		
		sspTemp.msg = mcMSG;
		sspTemp.flag.mc = SEM_MCI;

		POSITION posRet = m_fifoZ80MSG.Input( sspTemp );
		if( posRet != (POSITION)(-1) )
		{
			m_Z80MsgReceiveRate.CountLoopRate();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_TransceiverNew::CopyMSGfromHost( USB_SEM_ControlMessage& hostMSG )
{
	if ( !m_fifoHostMSG.IsFIFOEmpty() )
	{
		USB_SEM_SIO_Package		sspTemp;

		if ( m_fifoHostMSG.CopyHead( sspTemp ) )
		{
			hostMSG = sspTemp.msg;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_TransceiverNew::GetMSGfromHost( USB_SEM_ControlMessage& hostMSG )
{
	if ( !m_fifoHostMSG.IsFIFOEmpty() )
	{
		USB_SEM_SIO_Package		sspTemp;

		if ( m_fifoHostMSG.Output( sspTemp ) )
		{
			hostMSG = sspTemp.msg;
			m_HostMsgSendRate.CountLoopRate();
			
			NotifyMSGInfo( sspTemp, 1 );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_TransceiverNew::PostErrorInformation( CString& csError )
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

	if ( m_MicrocomputerID == SEM_Z80ID_Z80A )
		csError = _T("PC1-MP(A)->") + csError;
	else if ( m_MicrocomputerID == SEM_Z80ID_Z80B )
		csError = _T("PC9-MP(B)->") + csError;

	return m_fifoError.Input( csError ) != (POSITION)(-1);
}


/////////////////////////////////////////////////////////////////////////////
// CUSB_TransceiverNew  up-interface

BOOL CUSB_TransceiverNew::PostMSGtoMicrocomputer( USB_SEM_ControlMessage& hostMSG )
{
	static BOOL		IsFIFOFullDisplaied = FALSE;
	// debug
	m_HostMsgPostRate.CountLoopRate();

	if ( !m_fifoHostMSG.IsFIFOFull() )
	{
		////////////////////////////////////////////////
		if ( IsFIFOFullDisplaied )
		{
			CString	csError;
			csError.Format(" Overflow Relieve: Host->Z80 MSG-FIFO free!");
			PostErrorInformation( csError );

			IsFIFOFullDisplaied = FALSE;
		}
		////////////////////////////////////////////////
		
		////////////////////////////////////////////////
		// If busy, pause Inquirer
		////////////////////////////////////////////////
		if ( m_fifoHostMSG.GetFIFOCount() > 0 )
		{
/*			if( m_MicrocomputerID == SEM_Z80ID_Z80B )
			if( (hostMSG.bMessageID == (BYTE)Z80B_Inquirer) ||
				(hostMSG.bMessageID == (BYTE)Z80B_HeatingFilamentElectricity) )
			{
				return TRUE;
			}

			if ( (m_MicrocomputerID == SEM_Z80ID_Z80A)
				&& (hostMSG.bMessageID == (BYTE)Z80A_Inquirer) )
			{
				return TRUE;
			}
*/		}
		////////////////////////////////////////////////

		////////////////////////////////////////////////
		// Add message package to MSG FIFO
		////////////////////////////////////////////////
		USB_SEM_SIO_Package		sspTemp;
		
		sspTemp.msg = hostMSG;
//		sspTemp.flag.mc = m_MicrocomputerID;

		POSITION posRet = m_fifoHostMSG.Input( sspTemp );
		if( posRet != (POSITION)(-1) )
		{
			// m_HostMsgPostRate.CountLoopRate();
			NotifyMSGInfo( sspTemp );
			return TRUE;
		}
		////////////////////////////////////////////////
	}
#ifdef _DEBUG_ADD_ERRINFO_
	else
	{
		BOOL boolNotify = TRUE;

		if ( (m_MicrocomputerID == SEM_Z80ID_Z80B)
			&& (hostMSG.bMessageID == (BYTE)Z80B_Inquirer) )
		{
			boolNotify = FALSE;
		}

		if ( (m_MicrocomputerID == SEM_Z80ID_Z80A)
			&& (hostMSG.bMessageID == (BYTE)Z80A_Inquirer) )
		{
			boolNotify = FALSE;
		}

		////////////////////////////////////////////////
		if ( boolNotify && !IsFIFOFullDisplaied )
		{
			CString	csError;
			csError.Format(" Overflow Error: Host->Z80 MSG-FIFO Full!");
			PostErrorInformation( csError );

			IsFIFOFullDisplaied = TRUE;
		}
		////////////////////////////////////////////////
	}
#endif

	m_HostMsgFifoOverflowRate.CountLoopRate();

	return FALSE;
}

BOOL CUSB_TransceiverNew::PostMSGtoFPGA( USB_SEM_ControlMessage& hostMSG )
{
	static BOOL		IsFIFOFullDisplaied = FALSE;
	// debug
	m_HostMsgPostRate.CountLoopRate();

	if ( !m_fifoHostMSG.IsFIFOFull() )
	{
		////////////////////////////////////////////////
		if ( IsFIFOFullDisplaied )
		{
			CString	csError;
			csError.Format(" Overflow Relieve: Host->MCI MSG-FIFO free!");
			PostErrorInformation( csError );

			IsFIFOFullDisplaied = FALSE;
		}
		////////////////////////////////////////////////
		
		////////////////////////////////////////////////
		// If busy, pause Inquirer
		////////////////////////////////////////////////
		if ( m_fifoHostMSG.GetFIFOCount() > 0 )
		{
/*			if( m_MicrocomputerID == SEM_Z80ID_Z80B )
			if( (hostMSG.bMessageID == (BYTE)Z80B_Inquirer) ||
				(hostMSG.bMessageID == (BYTE)Z80B_HeatingFilamentElectricity) )
			{
				return TRUE;
			}

			if ( (m_MicrocomputerID == SEM_Z80ID_Z80A)
				&& (hostMSG.bMessageID == (BYTE)Z80A_Inquirer) )
			{
				return TRUE;
			}
*/		}
		////////////////////////////////////////////////

		////////////////////////////////////////////////
		// Add message package to MSG FIFO
		////////////////////////////////////////////////
		USB_SEM_SIO_Package		sspTemp;
		
		sspTemp.msg = hostMSG;
//		sspTemp.flag.mc = m_MicrocomputerID;

		POSITION posRet = m_fifoHostMSG.Input( sspTemp );
		if( posRet != (POSITION)(-1) )
		{
			// m_HostMsgPostRate.CountLoopRate();
			NotifyMSGInfo( sspTemp );
			return TRUE;
		}
		////////////////////////////////////////////////
	}
#ifdef _DEBUG_ADD_ERRINFO_
	else
	{
		BOOL boolNotify = TRUE;

/*		if ( (m_MicrocomputerID == SEM_Z80ID_Z80B)
			&& (hostMSG.bMessageID == (BYTE)Z80B_Inquirer) )
		{
			boolNotify = FALSE;
		}

		if ( (m_MicrocomputerID == SEM_Z80ID_Z80A)
			&& (hostMSG.bMessageID == (BYTE)Z80A_Inquirer) )
		{
			boolNotify = FALSE;
		}
*/
		////////////////////////////////////////////////
		if ( boolNotify && !IsFIFOFullDisplaied )
		{
			CString	csError;
			csError.Format(" Overflow Error: Host->MCI MSG-FIFO Full!");
			PostErrorInformation( csError );

			IsFIFOFullDisplaied = TRUE;
		}
		////////////////////////////////////////////////
	}
#endif

	m_HostMsgFifoOverflowRate.CountLoopRate();

	return FALSE;
}

BOOL CUSB_TransceiverNew::GetMSGfromMicrocomputer( USB_SEM_ControlMessage& mcMSG )
{
	if ( !m_fifoZ80MSG.IsFIFOEmpty() )
	{
		USB_SEM_SIO_Package		sspTemp;

		if ( m_fifoZ80MSG.Output( sspTemp ) )
		{
			mcMSG = sspTemp.msg;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_TransceiverNew::GetLastErrorInformation( CString& csError )
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
